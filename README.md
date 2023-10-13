# 从0开始写一个软光栅渲染器

#### 目标

实现基本的渲染管线，模拟GPU的VS和PS，并在此之上实现光照、阴影的计算，并尝试学习相关阴影和GI算法。

功能：

- 基本的渲染管线功能，如vs、ps，render target
- 深度测试

- 处理obj格式模型、png等贴图格式
- 透视投影（正交和非正交）、屏幕映射
- 纹理mapping
- 光照计算，如直射光。以及相关的反射和折射，计划写一个pbr。
- 阴影计算，软阴影算法，如pcf



#### 所要用到的知识

- 渲染流程：渲染管线，包括透视投影矩阵、ScreenMapping，裁剪渲染项
- 光栅化：三角形设置、三角形遍历、重心插值
- 深度测试：深度缓冲区，深度非线性变换

- 光照计算：光照模型
- 阴影计算：包括光源空间矩阵的变换，shadowmap的绘制
- 纹理映射：mipmap、filter

- 颜色准度：色调映射



#### 项目环境

- vs2022
- Eigen矩阵运算库



#### 项目架构

**基础运算类**：

- `Vector`：向量，由eigen提供
- `Matrix`：矩阵，同上
- `MyMath`：自己写的一些小函数

**游戏对象类：**

- `GameObject`：游戏对象，存放所有的组件
- `Component`：属于GameObject，存放游戏对象相关的空间信息
- `Transform`：属于Component，存放空间变换等信息

**渲染相关类：**

`Renderer`：渲染画面的类

`Camera`：相机

`Light`：光源，可以实现直射光、点光源等

`Buffer`：缓冲区，可以分配如zbuffer、framebuffer等

`Material`：材质，属于`Component`

`Mesh`：存放mesh信息

`Texture`：存放纹理信息

`Color`：存放颜色信息

`ObjectLoader`：用来读取obj信息

`TextureLoader`：用来读取Texture

`IShader`：shader接口纯虚函数

**辅助类：**

#### ObjLoader

首先来明确以下一个obj文件的成员：

- v：表示顶点，后面跟着的是模型空间坐标
- vn：表示顶点法线
- vt：表示uv坐标
- f：表示一个三角形面由哪三个顶点组成。
  其中f比较不一样，它由v/vt/vn的索引组成，一行存储三个，如下：
  **f 48994//37093 34556//37079 34563//25342**



#### 开发顺序

首先把大致的框架搭好。我的开发顺序如下：

- 先把需要用到的几个关键类给写了，包括`GameObject`、`Component`、`Resource`等。物体的mesh读入函数暂时搁置，先手动写入。因为是个小项目，就直接把Mesh信息存到GameObject里
- `Camera`相关变换矩阵，先写透视投影，正交矩阵暂时搁置。
- `Component`里存更新物体的世界矩阵函数
- `Cbuffer`、`ObjectBuffer`。Cbuffer是共用的，在Renderer里面维护一份即可，ObjectBuffer就需要每个物体单独一份
- `VertexShader()`：顶点着色器，per vertex，输入`vertex`输出`VertexOutPut`。`VertexOutPut`也弄了个多态。这样写统一点，不同的shader都能以一套模式去处理。
- `FragmentShader()`：片元着色器，FS先暂时不动，输出默认值黑色。
- `DrawPoint()`：画点

- `DrawRenderItems()`：逐个画物体
- `DrawIndexdInstance()`：一次DrawCall
- `LoadObject()`：载入物体

这样写完就能跑图了。

![Image_1697174191633](https://github.com/LuHec/Compute-Graphic-Study/assets/92776720/7ff56bb3-ee11-47b8-b99e-9e376f08aad1)

![image](https://github.com/LuHec/Compute-Graphic-Study/assets/92776720/6f53e096-a251-41e6-8719-c89bb9765fb6)

