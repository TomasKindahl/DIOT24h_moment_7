# This work is licensed under the MIT license.
# Copyright (c) 2013-2023 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE
#
# Blinky example

import time
from machine import LED

ledr = LED("LED_RED")
print(ledr.__dict__)
ledg = LED("LED_GREEN")
print(ledg.__dict__)
ledb = LED("LED_BLUE")
print(ledb.__dict__)

while True:
    ledr.on()
    ledg.off()
    ledb.off()
    time.sleep_ms(500)
    ledr.off()
    ledg.on()
    ledb.off()
    time.sleep_ms(500)
    ledr.off()
    ledg.off()
    ledb.on()
    time.sleep_ms(500)
    ledr.off()
    ledg.off()
    ledb.off()
    time.sleep_ms(500)
