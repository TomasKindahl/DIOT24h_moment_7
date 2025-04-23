# Random script example

import time
from machine import LED
ledr = LED("LED_RED")
ledg = LED("LED_GREEN")
ledb = LED("LED_BLUE")

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
