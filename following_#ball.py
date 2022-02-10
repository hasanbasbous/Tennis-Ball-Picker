# Code source (Matt-Timmons Brown): https://github.com/the-raspberry-pi-guy/raspirobots
# Tweaked Brown's code to adapt to the project requirements
from picamera.array import PiRGBArray
from picamera import PiCamera
from gpiozero import LED
from time import sleep
import cv2
import numpy as np
import gpiozero
 
camera = PiCamera()
camera.rotation = 180
image_width = 640
image_height = 480
camera.resolution = (image_width, image_height)
camera.framerate = 31
rawCapture = PiRGBArray(camera, size=(image_width, image_height))
center_image_x = image_width / 2
center_image_y = image_height / 2
minimum_area = 50
maximum_area = 15000 
 
robot = gpiozero.Robot(left=(25,8), right=(23,24))
forward_speed = 0.75
turn_speed = 0.65
 
HUE_VAL = 30
count = 0
count1 = 0
 
lower_color = np.array([HUE_VAL-10,100,100])
upper_color = np.array([HUE_VAL+10, 255, 255])

# lower_color = np.array([50,50,0])
# upper_color = np.array([100, 255, 20])

pin1 = LED(17)
pin2 = LED(27)
pin3 = LED(22)

pin1.value = 0
pin2.value = 0
pin3.value = 0

sensitivity = 4
flag = 0

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    image = frame.array
 
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
 
    color_mask = cv2.inRange(hsv, lower_color, upper_color)
 
    image2, countours, hierarchy = cv2.findContours(color_mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
 
    object_area = 0
    object_x = 0
    object_y = 0
 
    for contour in countours:
        x, y, width, height = cv2.boundingRect(contour)
        found_area = width * height
        center_x = x + (width / 2)
        center_y = y + (height / 2)
        if object_area < found_area:
            object_area = found_area
            object_x = center_x
            object_y = center_y
    if object_area > 0:
        ball_location = [object_area, object_x, object_y]
    else:
        ball_location = None
 
    if ball_location:
        if (ball_location[0] > minimum_area) and (ball_location[0] < maximum_area):
            print(ball_location[0])
            if ball_location[0] > 2000 and flag == 0:
                flag = 1
                pin1.value = 0
                pin2.value = 0
                pin3.value = 1
                print("collect ball")
            if ball_location[0] > 4000 and flag == 1 and count == 2:
                flag = 2
                sensitivity = 12
                turn_speed = 0.65
                print("changed area")
            if ball_location[1] > (center_image_x + (image_width/sensitivity)):
                robot.right(turn_speed)
                print("Turning right")
            elif ball_location[1] < (center_image_x - (image_width/sensitivity)):
                robot.left(turn_speed)
                print("Turning left")
            else:
                robot.forward(forward_speed)
                print("Forward")
        elif (ball_location[0] < minimum_area):
            robot.left(turn_speed)
            print("Target isn't large enough, searching")
        elif (count == 2):
            print("Target large enough, stopping")
            robot.stop()
            print("box found")
            
            pin1.value = 0
            pin2.value = 1
            pin3.value = 1
            robot.forward(1)
            sleep(3)
            robot.stop()
            
            sleep(1)
            if count1 < 2:
                robot.backward(1)
                sleep(4)
                robot.left(1)
                sleep(1)
                count1 = count1 + 1
                count = 0
                flag = 0
                lower_color = np.array([30-10,100,100])
                upper_color = np.array([30+10, 255, 255])
                maximum_area = 15000
                minimum_area = 50
                sensitivity = 4
                turn_speed = 0.65
                pin1.value = 0
                pin2.value = 0
                pin3.value = 0
            else:
                robot.stop()
                break
        else:
            print("Target large enough, stopping")
            robot.stop()
#             break
            count = 2
            ball_location = None

            lower_color = np.array([58-10,100,100])
            upper_color = np.array([58+10, 255, 255])
            maximum_area = 13500
            minimum_area = 50
            sensitivity = 5
#             forward_speed = 1
            turn_speed = 0.65
            pin1.value = 0
            pin2.value = 1
            pin3.value = 0
            print("changing to green")
            
    else:
        robot.left(turn_speed)
        print("Target not found, searching")
        if(count == 1):
            count = 2
            rawCapture.truncate(0)
 
    rawCapture.truncate(0)
    
print (1)
