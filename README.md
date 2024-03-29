# 2D_WallPaper

**始于纪中，没有最终**

收集的目的：

1. 用在以后`AI`生成二次元图片

2. 研究用`c++`代码批量下载东西，文件相似度比较

## 文件说明

### 根目录

可以做电脑壁纸，博客背景的图片

大部分的图片都在里面

### history.txt

保存了下载过了图片的文件名

使得程序不会下载重复

如果本地有该图片，而此文本中没有

那么程序会自动添加

### temp.txt

保存json的内容以便于解析

### url&json.txt

里面有一些api网站

### /Avatar

可以当头像(或者手机壁纸)的图片

### /Minecraft_Wallpapers

form https://t.bilibili.com/387278090140124626?tab=2

### /download

这里面最有用的东西

可以批量下载这些图片的程序&源代码

通过一些网络上的$API$接口

条件是这些接口要能返回`json`数据

如果用的不是源代码里面的地址的话,`get_json()`函数的参数需要改一下。

使用建议：

先把根目录的history.txt拷贝到/download中，再在download中运行该程序

## 主程序/download/download.cpp

### 程序的原理

核心代码：

`str="certutil -urlcache -split -f "+down_url+" "+name;`

`system(str.c_str());`

### 双重查重

程序开始时，读取`history.txt`里面的数据

获得一个`json`后

首先从`his`(一个`set`)里面看有没有数据

* 有的话就跳过
	
* 要是没有，则用cmd dir \a来判断同一层目录里面有没有同名文件

（当然，还是有可能下载到不同名但是内容相同的图片……）

## 去重

**如何防止下载到内容相同，名字不同的图片？**

### 方法一

当两张同内容但是不同名的图片都在`history.txt`中被标记后，您可以在本地中任意删除其中的一张，因为他们都不会再下载了。

用`set`是为了把时间复杂度优化到$\log$级别的。

用`hash`还是有点不太现实，因为图片的数量还是有点多的。

### 方法二

下载了一张图片后，将其与本地的其他图片进行内容对比（也可以将其MD5或SHA1等值进行对比），若相同，则删去

但是此方法的效率不高，且容错率太小了，暂不使用

### 方法三

**upd-2020.12.12 23:10**

读取出每一张图片的像素，并对其进行分析。

这里又有两种方法

1. ~~用**卷积神经网络**比对图片相似度，后选择一个上界，相似度大于上界则判断为相同图片~~
2. 类比网站[WAIT: What Anime Is This? - Anime Scene Search Engine](https://trace.moe/)的原理[$CLD$](https://en.wikipedia.org/wiki/Color_layout_descriptor)​，把图像先分割成$64$个(或者更多，设分割成n个格子)个大格子，计算每个格子中所有颜色的`RGB`平均值($x_{1\sim n}$)。然后用这个平均值数列与其他图片的平均值做减法，再套用某些小学数学模型比如**方差公式**。若差异度小于某个下界，那么就可以认为这两张图片相同。

显然，后者更直接更方便。

### Step

* [x] 把所有图片重命名为$MD5$。
* [x] 写一个程序(或者函数)，接受多个参数，参数为本地图片地址，调用python计算矩阵。
* [x] 用python分析图片计算$CLD$。
* [ ] 预处理出已经下载好的图片的$CLD$至`lib.txt`
* [x] 在`download.cpp`中调用这个程序，以后下载时用这个判重。

那么现在的判重顺序就有：

1. `lib.txt`历史记录判重
2. 文件名MD5
3. $CLD$

没了。

### 下载流程

$$
download(json)\to download(picture)\to ask(MD5)\to check(MD5)\\\to rename(MD5)\to calc(CLD)\to read(CLD)\to check(CLD)\to delete(MD5)
$$
