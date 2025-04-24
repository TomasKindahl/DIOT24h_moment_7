import machine
import time

from machine import Pin, PWM

pwm = PWM(Pin("D2"))
pwm.freq(1000)

while True:
    for i in range(0, 2048, 10):
        pwm.duty_u16(i)
        time.sleep_ms(20)
