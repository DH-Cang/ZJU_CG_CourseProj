# README

## 操作说明

鼠标控制方向

WASD控制前后左右移动；空格竖直向上移动；左shift竖直向下移动

QE表示摄像机向左/向右旋转

1拉近视角；2拉远视角；R恢复正常视角



## 修改日志

#### 12.8

main函数创建了一个空窗口，导入了部分通用类。shader类不用修改；model类被删除了，因为助教使用的tiny_obj_loader好像会报错。按照learnopengl的教程，我们可以用assimp库实现obj文件的读取

添加了一个test类，作为样例演示如何渲染一个简单的矩形

#### 12.9

添加了读取obj文件的部分，还有bug需要修改

在test::test()中采用了绝对路径，如果要跑的话记得修改，目前还没摸索出相对路径的用法

参考代码 https://blog.csdn.net/manipu1a/article/details/85340045

#### 12.14

完成assimp库读取.obj和.mtl文件

#### 12.16

完成了单平行光源光照和移动的部分
