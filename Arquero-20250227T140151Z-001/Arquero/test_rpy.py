import RPI.GPIO as GPIO
import time

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

# PINS  
driverPUL = 7 # Cambiar
driverDIR = 6 

# Pin Setup
GPIO.setup(driverPUL, GPIO.OUT)
GPIO.setup(driverDIR, GPIO.OUT)

# Variables
pd = 300 # uS (Microseconds)
setDir = GPIO.HIGH

def move_forward():
    setDir = GPIO.HIGH
    GPIO.output(driverDIR, setDir)
    GPIO.output(driverPUL, GPIO.HIGH)
    time.sleep(pd/1000000) # Para pasarlo a uS
    GPIO.output(driverPUL, GPIO.LOW)
    time.sleep(pd/1000000)

def move_backward():
    setDir = GPIO.LOW
    GPIO.output(driverDIR, setDir)
    GPIO.output(driverPUL, GPIO.HIGH)
    time.sleep(pd/1000000) # Para pasarlo a uS
    GPIO.output(driverPUL, GPIO.LOW)
    time.sleep(pd/1000000)