# -*-coding:utf-8-*-
# Copyright (c) 2022 DJI.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License in the file LICENSE.txt or at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from ctypes import *
import serial
import threading
import time
import dji_crc
import struct


class OpenProto:

    CRC8_INIT = 0x11
    CRC16_INIT = 0x4f19

    SOF = 0xAA

    ATTR_RE_ACK = 0x02
    ATTR_IS_ACK = 0x01

    def __init__(self, com_port, baudrate, local_addr, logging):
        self.local_addr = local_addr
        self.com_port = com_port
        self.baudrate = baudrate
        self.is_open = False

        self.ser = None     # type:serial.Serial
        self.unpack_thread_t = None

        self.seq_cnt = 0
        self.pack_list = []
        self.pack = dict()
        self.pack_buff = []
        self.unpack_cnt = 0
        self.unpack_step = 0

        self.logging = logging

    def is_open(self):
        return self.is_open()

    def open(self):
        if not self.is_open:
            self.is_open = True
            self.ser = serial.Serial(self.com_port, self.baudrate, timeout=1)
            self.unpack_thread_t = threading.Thread(target=self.__unpack_thread)
            self.unpack_thread_t.start()

    def close(self):
        if self.is_open:
            self.is_open = False
            self.unpack_thread_t.join()
            self.ser.close()

    def send_pack(self, dst, cmd, data, need_ack=True):
        if self.is_open:
            attr = 0
            if need_ack:
                attr = OpenProto.ATTR_RE_ACK
            send_data = self.__pack_data(dst, attr, self.seq_cnt, cmd, data)
            self.seq_cnt += 1
            try:
                self.ser.write(send_data)
            except serial.SerialException:
                self.close()
                self.is_open = False
                self.logging.debug("OpenProto: Serial port sending failed, close")

    def ack_pack(self, recv_pack, data):
        if self.is_open:
            if recv_pack['attr'] == OpenProto.ATTR_RE_ACK:
                send_data = self.__pack_data(recv_pack['src'], OpenProto.ATTR_IS_ACK, recv_pack['seq'],
                                             recv_pack['cmd'], data)
            try:
                self.ser.write(send_data)
            except serial.SerialException:
                self.close()
                self.is_open = False
                self.logging.debug("OpenProto: Serial port sending failed, close")

    def recv_pack(self, src=0xFFFF, cmd_id=0xFFFF):
        ret_pack = []
        for pack in self.pack_list:
            if pack["dst"] == self.local_addr or pack["dst"] == 0xFFFF:
                if pack["src"] == src or src == 0xFFFF:
                    if cmd_id == 0xFFFF:
                        ret_pack.append(pack)
                    else:
                        if cmd_id == pack["cmd"]:
                            ret_pack.append(pack)
        self.pack_list = []
        return ret_pack

    def send_and_recv_ack_pack(self, dst, cmd, data, wait_time=0.5, retry=3, dst_only_one=True):
        if not self.is_open:
            return []

        while retry > 0:
            retry -= 1
            self.send_pack(dst, cmd, data)
            t = time.time()
            packs = []
            while time.time() - t < wait_time:
                packs += self.recv_pack(dst, cmd)
                if dst_only_one and len(packs) > 0:
                    return packs
                time.sleep(0.01)
            if len(packs) > 0:
                return packs
            self.logging.debug('OpenProtocol: Wait for the receive timeout, resend the packet:0x%04x.' % cmd)
        return []

    def __pack_data(self, dst, attr, seq, cmd, data):
        if data is None:
            data = bytes([])
        pack_raw = struct.pack("<BH", OpenProto.SOF, len(data))
        crc8 = dji_crc.get_crc8_check(pack_raw, OpenProto.CRC8_INIT)
        pack_raw += struct.pack("<BHHBHH", crc8, self.local_addr, dst, attr, seq, cmd)
        pack_raw += bytes(data)
        crc16 = dji_crc.get_crc16_check(pack_raw, OpenProto.CRC16_INIT)
        pack_raw += struct.pack("<H", crc16)

        return pack_raw

    def __unpack_data(self, stream):
        self.pack_buff += stream
        while self.unpack_step < 2:
            if self.unpack_step == 0:  # SOF & Data Len & CRC8
                if len(self.pack_buff) >= 4:
                    if self.pack_buff[0] == 0xAA:
                        self.pack['data_len'] = self.pack_buff[1]
                        self.pack['data_len'] |= self.pack_buff[2] << 8
                        self.pack['crc8'] = self.pack_buff[3]
                        if dji_crc.get_crc8_check(self.pack_buff[0:3], OpenProto.CRC8_INIT) == self.pack['crc8']:
                            if self.pack['data_len'] > 512:
                                self.unpack_step = 0
                                self.pack_buff = self.pack_buff[1:]
                            self.unpack_step = 1
                        else:
                            self.unpack_step = 0
                            self.pack_buff = self.pack_buff[1:]
                    else:
                        self.unpack_step = 0
                        self.pack_buff = self.pack_buff[1:]
                else:
                    return

            if self.unpack_step == 1:
                pack_len = 13 + self.pack['data_len'] + 2
                if len(self.pack_buff) >= pack_len:
                    self.pack['crc16'] = struct.unpack("H", bytes(self.pack_buff[pack_len - 2:pack_len]))[0]
                    if dji_crc.get_crc16_check(self.pack_buff[0:pack_len-2], OpenProto.CRC16_INIT) == self.pack['crc16']:
                        header = struct.unpack("<HHBHH", bytes(self.pack_buff[4:13]))
                        self.pack['src'] = header[0]
                        self.pack['dst'] = header[1]
                        self.pack['attr'] = header[2]
                        self.pack['seq'] = header[3]
                        self.pack['cmd'] = header[4]
                        self.pack['data'] = self.pack_buff[13:13 + self.pack['data_len']]
                        self.pack_list.append(self.pack)

                        self.pack_buff = self.pack_buff[pack_len:]
                        self.pack = dict()
                        self.unpack_step = 0
                    else:
                        self.unpack_step == 0
                        self.pack_buff = self.pack_buff[1:]
                else:
                    return

    def __unpack_thread(self):
        while self.is_open:
            try:
                if self.ser.in_waiting:
                    stream = self.ser.read(self.ser.in_waiting)
                    self.__unpack_data(stream)
                time.sleep(0.01)
            except serial.SerialException:
                self.close()
                self.is_open = False
                self.logging.debug("OpenProto: Serial port read failed, close")
                return
