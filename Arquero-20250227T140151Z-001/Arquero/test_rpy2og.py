from collections import deque
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import RPI.GPIO as GPIO
import imutils
import time
from math import sqrt, acos, degrees

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

# PINS  
driverPUL = 27 # Cambiar
driverDIR = 17 

# Pin Setup
GPIO.setup(driverPUL, GPIO.OUT)
GPIO.setup(driverDIR, GPIO.OUT)

pd = 300 
poiints_per_rev = 800
setDir = GPIO.HIGH
points = 0
cur = 0 
obj = 0

def move_to(ang):
	if ang >= cur:
		obj = (ang/360) * poiints_per_rev
		if points != obj:
			move_forward()
		else:
			cur = ang
	else:
		obj = (ang/360) * poiints_per_rev
		if points != obj:
			move_backward()
		else:
			cur = ang
		
	

def move_forward():
    setDir = GPIO.HIGH
    GPIO.output(driverDIR, setDir)
    GPIO.output(driverPUL, GPIO.HIGH)
    time.sleep(pd/1000000) # Para pasarlo a uS
    GPIO.output(driverPUL, GPIO.LOW)
    time.sleep(pd/1000000)
    points = points + 1
    if points > poiints_per_rev:
        points = 0
  

def move_backward():
    setDir = GPIO.LOW
    GPIO.output(driverDIR, setDir)
    GPIO.output(driverPUL, GPIO.HIGH)
    time.sleep(pd/1000000) # Para pasarlo a uS
    GPIO.output(driverPUL, GPIO.LOW)
    time.sleep(pd/1000000)
    points = points - 1
    if points < 0:
        points = poiints_per_rev

ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
	help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64,
	help="max buffer size")
args = vars(ap.parse_args())

greenLower = (5, 50, 50)
greenUpper = (15, 255, 255)
pts = deque(maxlen=args["buffer"])

if not args.get("video", False):
	vs = VideoStream(src=1).start()
else:
	vs = cv2.VideoCapture(args["video"])

time.sleep(2.0)

font = cv2.FONT_HERSHEY_SIMPLEX 
posicion = (50, 50)
angulo = (50, 100) 
fontScale = 1   
color = (255, 0, 0) 
thickness = 2

arq_center = (600, 600)
reference = (1000,600)

LIM_INFERIOR = 150
LIM_SUPERIOR = 190

FRAME_WIDTH = 1200
FRAME_HEIGHT = 850

punto_medio = (int(FRAME_WIDTH/2), int(FRAME_HEIGHT))

while True:        
	frame = vs.read()
	frame = frame[1] if args.get("video", False) else frame
	if frame is None:
		break

	frame = imutils.resize(frame, width=FRAME_WIDTH)
	blurred = cv2.GaussianBlur(frame, (11, 11), 0)
	hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
	mask = cv2.inRange(hsv, greenLower, greenUpper)
	mask = cv2.erode(mask, None, iterations=2)
	mask = cv2.dilate(mask, None, iterations=2)

	cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
		cv2.CHAIN_APPROX_SIMPLE)
	cnts = imutils.grab_contours(cnts)
	center = None
	cv2.circle(frame, arq_center, 5, (0, 255, 0), -1)
	cv2.putText(frame, "arq_center", (arq_center[0], arq_center[1] + 30), font, fontScale, (0, 255, 0), thickness, cv2.LINE_AA)
	cv2.circle(frame, reference, 5, (0, 255, 0), -1)
	cv2.putText(frame, "reference", (reference[0], reference[1] + 30), font, fontScale, (0, 255, 0), thickness, cv2.LINE_AA)
	cv2.line(frame, reference, arq_center, (255, 0, 0), thickness)


	if len(cnts) > 0:
		c = max(cnts, key=cv2.contourArea)
		((x, y), radius) = cv2.minEnclosingCircle(c)
		M = cv2.moments(c)
		
		center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"])) # Importante
		
		cv2.putText(frame, str(center), posicion, font, fontScale, color, thickness, cv2.LINE_AA)
		if radius > 10 and radius < 40:
			cv2.circle(frame, (int(x), int(y)), int(radius),
				(0, 255, 255), 2)
			cv2.circle(frame, center, 5, (0, 0, 255), -1)
			cv2.line(frame, center, arq_center, (255, 0, 0), thickness)
			
			v1 = (center[0] - arq_center[0], center[1] - arq_center[1])
			v2 = (reference[0] - arq_center[0], reference[1] - arq_center[1])
			aux1 = (v1[0] * v2[0]) + (v1[1] * v2[1])
			aux2 = sqrt(v1[0]*v1[0] + v1[1]*v1[1])
			aux3 = sqrt(v2[0]*v2[0] + v2[1]*v2[1])
			aux4 = (aux1 / (aux2 * aux3))
			angle = degrees(acos(aux4))
			cv2.putText(frame, str(angle), (100,100), font, fontScale, color, thickness, cv2.LINE_AA)



	pts.appendleft(center)

	for i in range(1, len(pts)):
		if pts[i - 1] is None or pts[i] is None:
			continue

		thickness = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
		cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), thickness)

	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF

	if key == ord("q"):
		break
        
if not args.get("video", False):
	vs.stop()

else:
	vs.release()

cv2.destroyAllWindows()
