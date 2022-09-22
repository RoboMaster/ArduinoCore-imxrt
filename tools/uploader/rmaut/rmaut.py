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

import sys
import os
import time

import serial
import serial.tools.list_ports

from open_protocol import OpenProto
from upgrade import Upgrade, ModuleInfoStruct

import optparse
import logging

from cmd_parser import parserFunc

import serial

LOCAL_ADDR = 0x0103

class MainFunc():
    def __init__(self, options):

        self.port = options.port
        self.baud = options.baud
        self.fw_path = options.fw_path
        self.dst_addr = options.dst_addr
        self.erase_num = options.erase_bytes

        self.hwid = 0
        self.sn = 0

        self.upgrade_t = None
        self.upgrade_monitor_flag = 0
        self.upgrade = None

    def to_query(self):
        try:
            proto = OpenProto(self.port, self.baud, LOCAL_ADDR, logging)
            upgrade = Upgrade(proto, logging)
            modules = upgrade.query_ver()
            logging.debug("Upgrade: %d Module has been queried", len(modules))
            # 取第一个模块进行升级
            self.hwid = modules[0].hw_id
            self.sn = modules[0].sn
            self.dst_addr = modules[0].addr
            return True
        except Exception as e:
            logging.debug("Error", "Queried Faild")
            return False

    def to_upgrade(self):

        if not isinstance(self.dst_addr, int):
            logging.debug("dst_addr error")
        if self.dst_addr < 0 or self.dst_addr > 0xFFFF:
            logging.debug("dst_addr out range")

        if not os.path.exists(self.fw_path):
            logging.debug("no firmware")

        try:
            proto = OpenProto(self.port, self.baud, LOCAL_ADDR, logging)

            # 查版本
            self.upgrade = Upgrade(proto, logging)
            module = ModuleInfoStruct(0, 0, self.hwid, self.sn, self.dst_addr)
            # 下载固件
            self.upgrade.load_firmware(self.fw_path)
            self.upgrade.erase_num = self.erase_num
            self.upgrade_monitor_flag = 1
            ret = self.upgrade.download(module)

            # 重启
            proto.open()
            proto.send_pack(module.addr, 0x0001, None, need_ack=False)
            proto.close()

            if ret[0]:
                self.upgrade_monitor_flag = 255
            else:
                self.upgrade_monitor_flag = -1

        except Exception as e:
            self.upgrade_monitor_flag = -1
            return


def main(args):

    options = parserFunc(args)

    loglevel = logging.WARNING
    if (options.debug):
        loglevel = logging.DEBUG

    # logging.basicConfig(
    #     level=loglevel, format='%(asctime)-8s [%(levelname)s]: %(message)s', datefmt='%H:%M:%S')
    logging.basicConfig(level=loglevel, 
                        format='%(message)s', 
                        datefmt='%S')

    logging.debug("Options: %s", str(options))

    if (not options.port or not options.fw_path):
        logging.critical("Not enough arguments.")
        return 1

    # --------------------------------------------------------
    # 发送重启进入loader的指令，让目标开发板进入bootloader再进入升级
    if options.reset == True:
        ser = serial.Serial(options.port, options.baud, 8, 'N', 1)
        if ser.is_open:
            logging.debug("Reset board to bootloader ...")        
            ser.write(b'\xaa\x55')
            time.sleep( 0.5 )
            ser.close()
        else:
            logging.critical("Serial open error!!!")
        time.sleep( 0.1 )
    # --------------------------------------------------------

    main_func = MainFunc(options)
    # 查询版本
    ret = main_func.to_query()

    if ret == True:
        # 执行升级过程
        main_func.to_upgrade()
    else:
        logging.debug(
            "Failed to query the development board and is about to exit")

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))


"""
打包文件

    pyinstaller -F rmaut.py

    pyinstaller -n xxx -w -D main.py
    -F, –onefile 打包成一个exe文件。
    -D, –onedir 创建一个目录，包含exe文件，但会依赖很多文件（默认选项）。
    -c, –console, –nowindowed 使用控制台，无界面(默认)
    -w, –windowed, –noconsole 使用窗口，无控制台

下载download

    python rmaut.py -d -p "COM10" -f "D:/ai_hub_app.bin" 

    .\rmaut.exe -d -p "COM10" -f "D:/ai_hub/ai_hub_module_hw4/Debug/ai_hub_app.bin"

"""
