import sys
import ftrobopy
import time

ftrob = ftrobopy.ftrobopy('auto')
red = ftrob.output(1)
yellow = ftrob.output(2)
green = ftrob.output(3)
while True:
    red.setLevel(512)
    time.sleep(3)
    yellow.setLevel(512)
    time.sleep(1)
    red.setLevel(0)
    yellow.setLevel(0)
    green.setLevel(512)
    time.sleep(3)
    green.setLevel(0)
    yellow.setLevel(512)
    time.sleep(1)
    yellow.setLevel(0)

