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

import os, time

i = 0

while i < 10 :
    i += 1
    print("======================================================test count:", i )
    start = time.time()
    os.system('python rmaut.py -d -p "COM5" -f "C:\\Users\\flame.yu\\AppData\\Local\\Temp\\arduino_build_12558/HelloFreeRTOS.ino.bin" ')
    end = time.time()
    print("======================================================exe time:", (end-start), "(s)" )
    time.sleep( 2 )

