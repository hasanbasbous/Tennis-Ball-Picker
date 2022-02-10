# Code source (Matt-Timmons Brown): https://github.com/the-raspberry-pi-guy/raspirobots
# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
 
 
# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.rotation = 180
camera.resolution = (640, 480)
# camera.resolution = (320, 240)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))
 
while True:
    while True:
        try:
            hue_value = int(input("Hue value between 10 and 245: "))
            if (hue_value < 10) or (hue_value > 245):
                raise ValueError
        except ValueError:
            print("That isn't an integer between 10 and 245, try again")
        else:
            break
 
    lower_red = np.array([hue_value-10,100,100])
    upper_red = np.array([hue_value+10, 255, 255])
    
#     lower_red = np.array([0,0,0])
#     upper_red = np.array([5, 50, 30])
#     
# #     lower_red = np.array([34,47,0])
#     upper_red = np.array([70, 103, 29])
    
#     red_bag
#     lower_red = np.array([160,100,100])
#     upper_red = np.array([180, 255, 255])

#     lower_red = np.array([50,50,0])
#     upper_red = np.array([100, 255, 20])

        #green
#     lower_red = np.array([55-10,100,100])
#     upper_red = np.array([55+10, 255, 255])
        
 
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        image = frame.array
 
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
 
        color_mask = cv2.inRange(hsv, lower_red, upper_red)
 
        result = cv2.bitwise_and(image, image, mask= color_mask)
 
        cv2.imshow("Camera Output", image)
        cv2.imshow("HSV", hsv)
        cv2.imshow("Color Mask", color_mask)
        cv2.imshow("Final Result", result)
 
        rawCapture.truncate(0)
 
        k = cv2.waitKey(5) #& 0xFF
        if "q" == chr(k & 255):
            break2