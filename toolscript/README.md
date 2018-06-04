# opencv单源码文件快速编译bash脚本

 @author: Frank Liu
 @time: 2018-06-04
 @description: 编译链接opencv源码的脚本,输入参数为*.cpp或*.c文件，
 默认输出程序名与源码文件名一致。

## 使用方法：

> ./build_cv.sh input.cpp(.c) [-o outfile]

* input.cpp/input.c: opencv项目源码文件名；
* [-o outfile]: 可选参数项，输出程序文件名；若无此项则与源码文件名一致。