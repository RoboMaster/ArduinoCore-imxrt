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

import optparse

def parserFunc(unparsed_args):
    parser = optparse.OptionParser(
        usage="%prog [options]",
        description="upload tools"
    )

    group = optparse.OptionGroup(parser, "Destination")
    group.add_option("-p", "--port",
                     dest="port",
                     help="download port com",
                     default="COM10"
                     )
    group.add_option("-f", "--file",
                     dest="fw_path",
                     help="Image fw_path.",
                     metavar="FILE",
                     default=None
                     )
    group.add_option("-r", "--reset_to_bl",
                     dest="reset",
                    #  type="int",
                     help="Reset the board before upgrading",
                    #  action="store_true",
                     default=True
                     )
    group.add_option("-a", "--dst_addr",
                     dest="dst_addr",
                     help="dst_addr.",
                     default=None
                     )
    group.add_option("-e", "--erase_bytes",
                     dest="erase_bytes",
                     help="erase_bytes.",
                     type="int",
                     default=0
                     )
    group.add_option("-b", "--baud",
                     dest="baud",
                     type="int",
                     help="Serial port baud rate used when flashing/reading",
                     default=0
                     )
    parser.add_option_group(group)

    group = optparse.OptionGroup(parser, "Debug")
    group.add_option("-d", "--debug",
                     dest="debug",
                     help="Show debug output. And override loglevel with debug.",
                     action="store_true",
                     default=False
                     )
    parser.add_option_group(group)

    (options, args) = parser.parse_args(unparsed_args)

    return options
