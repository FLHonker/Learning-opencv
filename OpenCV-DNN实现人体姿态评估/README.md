# OpenCV深度神经网络实现人体姿态评估

## OpenCV DNN模块介绍

OpenCV自从发布了DNN模块之后，就开始以开挂的方式支持各种深度学习预训练模型的调用，DNN模块的全称为深度神经网络，但是并不是所有深度学习模型导出到OpenCV DNN模块中都可以使用，只有那些OpenCV声明支持的层与网络模型才会被DNN模块接受,当期OpenCV支持的模型与层类型可以在下面链接中找到相关文档

<https://github.com/opencv/opencv/wiki/Deep-Learning-in-OpenCV>

## 模型下载(卡耐基梅隆大学openpose姿态训练模型)

OpenCV3.4.x的版本开始支持在OpenCV DNN模块中使用openopse的深度学习模型,实现人体单人姿态评估, 首先需要下载人体姿态评估的预训练模型。基于COCO数据集训练的模型下载地址如下：

<http://posefs1.perception.cs.cmu.edu/OpenPose/models/pose/coco/pose_iter_440000.caffemodel>

基于MPI数据集训练的模型下载地址如下：

<http://posefs1.perception.cs.cmu.edu/OpenPose/models/pose/mpi/pose_iter_160000.caffemodel>

关于本模型你也可以去`卡耐基梅隆大学`的openpose项目仓库中进行了解：

<https://github.com/CMU-Perceptual-Computing-Lab/openpose>

模型`.prototxt`描述文件在[models/](https://github.com/CMU-Perceptual-Computing-Lab/openpose/models)中。

[CMU Panoptic DataSet](http://domedb.perception.cs.cmu.edu/index.html)

## 代码实现

下面只需要如下几步就可以实现基于OpenCV的单人姿态评估：

1. 定义COCO数据集支持的18点人体位置与关系位置

```python
BODY_PARTS = { "Nose": 0, "Neck": 1, "RShoulder": 2, "RElbow": 3, "RWrist": 4,
               "LShoulder": 5, "LElbow": 6, "LWrist": 7, "RHip": 8, "RKnee": 9,
               "RAnkle": 10, "LHip": 11, "LKnee": 12, "LAnkle": 13, "REye": 14,
               "LEye": 15, "REar": 16, "LEar": 17, "Background": 18 }

POSE_PAIRS = [ ["Neck", "RShoulder"], ["Neck", "LShoulder"], ["RShoulder", "RElbow"],
               ["RElbow", "RWrist"], ["LShoulder", "LElbow"], ["LElbow", "LWrist"],
               ["Neck", "RHip"], ["RHip", "RKnee"], ["RKnee", "RAnkle"], ["Neck", "LHip"],
               ["LHip", "LKnee"], ["LKnee", "LAnkle"], ["Neck", "Nose"], ["Nose", "REye"],
               ["REye", "REar"], ["Nose", "LEye"], ["LEye", "LEar"] ]
```

2. 定义MPI数据集支持的15点人体位置与关系位置

```python
BODY_PARTS = { "Head": 0, "Neck": 1, "RShoulder": 2, "RElbow": 3, "RWrist": 4,
               "LShoulder": 5, "LElbow": 6, "LWrist": 7, "RHip": 8, "RKnee": 9,
               "RAnkle": 10, "LHip": 11, "LKnee": 12, "LAnkle": 13, "Chest": 14,
               "Background": 15 }

POSE_PAIRS = [ ["Head", "Neck"], ["Neck", "RShoulder"], ["RShoulder", "RElbow"],
               ["RElbow", "RWrist"], ["Neck", "LShoulder"], ["LShoulder", "LElbow"],
               ["LElbow", "LWrist"], ["Neck", "Chest"], ["Chest", "RHip"], ["RHip", "RKnee"],
               ["RKnee", "RAnkle"], ["Chest", "LHip"], ["LHip", "LKnee"], ["LKnee", "LAnkle"] ]
```

3. 根据不同数据集调用DNN模块加载指定的预训练模型

```python
inWidth = 368
inHeight = 368
thr = 0.1
protoc = "D:/projects/pose_body/mpi/pose_deploy_linevec_faster_4_stages.prototxt"
model = "D:/projects/pose_body/mpi/pose_iter_160000.caffemodel"
net = cv.dnn.readNetFromCaffe(protoc, model)
```

4. 调用OpenCV打开摄像头

```python
cap = cv.VideoCapture(0)
height = cap.get(cv.CAP_PROP_FRAME_HEIGHT)
width = cap.get(cv.CAP_PROP_FRAME_WIDTH)
```

5. 使用前馈网络模型预测

```python
frameWidth = frame.shape[1]
frameHeight = frame.shape[0]
inp = cv.dnn.blobFromImage(frame, 1.0 / 255, (inWidth, inHeight),
                          (0, 0, 0), swapRB=False, crop=False)
net.setInput(inp)
out = net.forward()
```
6. 绘制检测到人体姿态关键点位置

```python
points = []
for i in range(len(BODY_PARTS)):
    # Slice heatmap of corresponging body's part.
    heatMap = out[0, i, :, :]

    # Originally, we try to find all the local maximums. To simplify a sample
    # we just find a global one. However only a single pose at the same time
    # could be detected this way.
    _, conf, _, point = cv.minMaxLoc(heatMap)
    x = (frameWidth * point[0]) / out.shape[3]
    y = (frameHeight * point[1]) / out.shape[2]

    # Add a point if it's confidence is higher than threshold.
    points.append((x, y) if conf > thr else None)

for pair in POSE_PAIRS:
    partFrom = pair[0]
    partTo = pair[1]
    assert(partFrom in BODY_PARTS)
    assert(partTo in BODY_PARTS)

    idFrom = BODY_PARTS[partFrom]
    idTo = BODY_PARTS[partTo]
    if points[idFrom] and points[idTo]:
        x1, y1 = points[idFrom]
        x2, y2 = points[idTo]
        cv.line(frame, (np.int32(x1), np.int32(y1)), (np.int32(x2), np.int32(y2)), (0, 255, 0), 3)
        cv.ellipse(frame, (np.int32(x1), np.int32(y1)), (3, 3), 0, 0, 360, (0, 0, 255), cv.FILLED)
        cv.ellipse(frame, (np.int32(x2), np.int32(y2)), (3, 3), 0, 0, 360, (0, 0, 255), cv.FILLED)
```

## 完整的代码如下：

```python
import cv2 as cv
import numpy as np


dataset = 'MPI'
if dataset == 'COCO':
    BODY_PARTS = { "Nose": 0, "Neck": 1, "RShoulder": 2, "RElbow": 3, "RWrist": 4,
                   "LShoulder": 5, "LElbow": 6, "LWrist": 7, "RHip": 8, "RKnee": 9,
                   "RAnkle": 10, "LHip": 11, "LKnee": 12, "LAnkle": 13, "REye": 14,
                   "LEye": 15, "REar": 16, "LEar": 17, "Background": 18 }

    POSE_PAIRS = [ ["Neck", "RShoulder"], ["Neck", "LShoulder"], ["RShoulder", "RElbow"],
                   ["RElbow", "RWrist"], ["LShoulder", "LElbow"], ["LElbow", "LWrist"],
                   ["Neck", "RHip"], ["RHip", "RKnee"], ["RKnee", "RAnkle"], ["Neck", "LHip"],
                   ["LHip", "LKnee"], ["LKnee", "LAnkle"], ["Neck", "Nose"], ["Nose", "REye"],
                   ["REye", "REar"], ["Nose", "LEye"], ["LEye", "LEar"] ]
else:
    assert(dataset == 'MPI')
    BODY_PARTS = { "Head": 0, "Neck": 1, "RShoulder": 2, "RElbow": 3, "RWrist": 4,
                   "LShoulder": 5, "LElbow": 6, "LWrist": 7, "RHip": 8, "RKnee": 9,
                   "RAnkle": 10, "LHip": 11, "LKnee": 12, "LAnkle": 13, "Chest": 14,
                   "Background": 15 }

    POSE_PAIRS = [ ["Head", "Neck"], ["Neck", "RShoulder"], ["RShoulder", "RElbow"],
                   ["RElbow", "RWrist"], ["Neck", "LShoulder"], ["LShoulder", "LElbow"],
                   ["LElbow", "LWrist"], ["Neck", "Chest"], ["Chest", "RHip"], ["RHip", "RKnee"],
                   ["RKnee", "RAnkle"], ["Chest", "LHip"], ["LHip", "LKnee"], ["LKnee", "LAnkle"] ]

inWidth = 368
inHeight = 368
thr = 0.1
protoc = "./models/mpi/pose_deploy_linevec_faster_4_stages.prototxt"
model = "./modesl//mpi/pose_iter_160000.caffemodel"
net = cv.dnn.readNetFromCaffe(protoc, model)

cap = cv.VideoCapture(0)
height = cap.get(cv.CAP_PROP_FRAME_HEIGHT)
width = cap.get(cv.CAP_PROP_FRAME_WIDTH)
video_writer = cv.VideoWriter("D:/pose_estimation_demo.mp4", cv.VideoWriter_fourcc('D', 'I', 'V', 'X'), 15, (640, 480), True)
while cv.waitKey(1) < 0:
    hasFrame, frame = cap.read()
    if not hasFrame:
        cv.waitKey()
        break

    frameWidth = frame.shape[1]
    frameHeight = frame.shape[0]
    inp = cv.dnn.blobFromImage(frame, 1.0 / 255, (inWidth, inHeight),
                              (0, 0, 0), swapRB=False, crop=False)
    net.setInput(inp)
    out = net.forward()

    print(len(BODY_PARTS), out.shape[0])
    # assert(len(BODY_PARTS) == out.shape[1])

    points = []
    for i in range(len(BODY_PARTS)):
        # Slice heatmap of corresponging body's part.
        heatMap = out[0, i, :, :]

        # Originally, we try to find all the local maximums. To simplify a sample
        # we just find a global one. However only a single pose at the same time
        # could be detected this way.
        _, conf, _, point = cv.minMaxLoc(heatMap)
        x = (frameWidth * point[0]) / out.shape[3]
        y = (frameHeight * point[1]) / out.shape[2]

        # Add a point if it's confidence is higher than threshold.
        points.append((x, y) if conf > thr else None)

    for pair in POSE_PAIRS:
        partFrom = pair[0]
        partTo = pair[1]
        assert(partFrom in BODY_PARTS)
        assert(partTo in BODY_PARTS)

        idFrom = BODY_PARTS[partFrom]
        idTo = BODY_PARTS[partTo]
        if points[idFrom] and points[idTo]:
            x1, y1 = points[idFrom]
            x2, y2 = points[idTo]
            cv.line(frame, (np.int32(x1), np.int32(y1)), (np.int32(x2), np.int32(y2)), (0, 255, 0), 3)
            cv.ellipse(frame, (np.int32(x1), np.int32(y1)), (3, 3), 0, 0, 360, (0, 0, 255), cv.FILLED)
            cv.ellipse(frame, (np.int32(x2), np.int32(y2)), (3, 3), 0, 0, 360, (0, 0, 255), cv.FILLED)

    t, _ = net.getPerfProfile()
    freq = cv.getTickFrequency() / 1000
    cv.putText(frame, '%.2fms' % (t / freq), (10, 20), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0))
    # video_writer.write(frame);
    # cv.imwrite("D:/pose.png", frame)
    cv.imshow('OpenPose using OpenCV', frame)
```

## 运行结果如下：

![result](./result.png)