# -*- coding: UTF-8 -*-
#/***********************************************
# Copyright (c) 2018, Wuhan
# All rights reserved.
#
# @Filename: textDetect2.py
# @Version：V1.0
# @Author: Frank Liu - frankliu624@gmail.com
# @Description: ---
# @Create Time: 2018-07-25 15:16:01
# @Last Modified: 2018-07-25 15:16:01
#***********************************************/
import numpy as np
import cv2
import nms

img = cv2.imread('calc.png')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
vis = img.copy()
orig = img.copy()
mser = cv2.MSER_create()
regions = mser.detectRegions(gray, None)
hulls = [cv2.convexHull(p.reshape(-1, 1, 2)) for p in regions]
cv2.polylines(img, hulls, 1, (0, 255, 0))
cv2.imshow('img', img)
keep = []
for c in hulls:
    x, y, w, h = cv2.boundingRect(c)
    keep.append([x, y, x + w, y + h])
    cv2.rectangle(vis, (x, y), (x + w, y + h), (255, 255, 0), 1)
print ("[x] %d initial bounding boxes" % (len(keep))
cv2.imshow('hulls', vis)
keep2=np.array(keep)
pick = nms.nms(keep2, 0.5)
print ("[x] after applying non-maximum, %d bounding boxes" % (len(pick)))
# loop over the picked bounding boxes and draw them
for (startX, startY, endX, endY) in pick:
    cv2.rectangle(orig, (startX, startY), (endX, endY), (255, 0, 0), 1)
cv2.imshow('After NMS', orig)
cv2.waitKey(0)
cv2.destroyAllWindows()
