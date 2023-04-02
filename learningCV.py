import cv2
import numpy as np
import time

cap = cv2.VideoCapture(0)
time.sleep(1.000)
while(1):
    _,frame=cap.read()
    red =frame[:,:2]
    green=frame[:,:,1]
    blue=frame[:,:,0]
    cv2.imshow('figure 1',frame)
    k=cv2.waitKey(5)
    if k==27:
        break
    

cv2.desroyAllWindows()
print(frame.shape)

