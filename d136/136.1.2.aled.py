import machine
import time

from machine import Pin, PWM

pwm = PWM(Pin("D2"))
pwm.freq(1000)
button = Pin("D3", Pin.IN, Pin.PULL_UP)

while True:
    for i in range(0, 2048, 10):
        if button.value() == 1:
            pwm.duty_u16(0)
        else:
            pwm.duty_u16(i)
        time.sleep_ms(20)
