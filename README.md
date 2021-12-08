# README

## 环境配置

参考 https://jingyan.baidu.com/article/6f2f55a193029bf4b83e6c33.html

安装vcpkg包管理器，和visual studio绑定，并安装如下的库

```
vcpkg install glfw3 glad glm assimp
```

配置完成后应该编译运行出一个空白窗口

## 实验要求

<img src="README.assets/image-20211208134614055.png" alt="image-20211208134614055" style="zoom:67%;" />

## 修改日志

#### 12.8

main函数创建了一个空窗口，导入了部分通用类。shader类不用修改；model类被删除了，因为助教使用的tiny_obj_loader好像会报错。按照learnopengl的教程，我们可以用assimp库实现obj文件的读取
