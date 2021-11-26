# Opencv C++ 基本数据结构 Mat

## Mat

Mat代表矩阵，该类声明在头文件opencv2/core/core.hpp中
其构造函数为：

```C++
Mat(int rows, int cols, int type)
```

---

##构造单通道Mat对象

```C++
# include <opencv2\core\core.hpp>
using namespace cv;
int main() {
	//构造两行三列的float型矩阵
	Mat m = Mat(2, 3, CV_32FC(1));
	//利用Size对象构造两行3列矩阵，Size（列，行）
	Mat m1 = Mat(Size(3, 2), CV_32FC(1));
	//使用Mat类中的成员函数create完成Mat对象构造
	Mat m2;
	m2.create(Size(2, 3), CV_32FC1);
	//构造零矩阵和1矩阵
	Mat o = Mat::ones(2, 3, CV_32FC1);
	Mat z = Mat::zeros(Size(3,2), CV_32FC1);
	//初始化小型矩阵
	Mat m = (Mat_<int>(2, 3) << 1, 2, 3, 4, 5, 6);
	return 0;
}
```
 
**例：**
$$
m=\begin{bmatrix}
1 & 12 \\
33 & 43 \\
51 & 16    
\end{bmatrix}
$$

```C++
# include <opencv2\core\core.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main() {
	//快速构造矩阵
	Mat m = (Mat_<int>(3, 2) << 11, 12, 33, 43, 51, 16);
	//矩阵的行数
	cout << "行数：" << m.rows << endl;
	//矩阵的列数
	cout << "行数：" << m.cols << endl;
	return 0;
}
```

---

##  常用成员函数：

1.size():获取矩阵的尺寸

```C++
Size size = m.size();
cout << "尺寸" << size << endl;
```

2.channels():获取矩阵的通道数

3.total():获得面积即行数乘列数

4.dims():获取维数

**访问单通道对象中的值**

1.使用成员函数at，例如访问单通道数据类型为CV_32F的对象m,访问其第r行c列的值：
格式为：m.at<类型>(行,列)

`m.at<float>(r,c);`

2.使用成员函数ptr

矩阵每一行的值值是连续存储在内存中的，行与行在内存中也可能存在间隔
通过ptr成员函数可以获得执行每一行地址的指针
格式为：m.ptr<类型>（第几行）

3.使用成员函数isContinuous 和 ptr
矩阵在存储时如果每一行在内存中没有间隔则isContinuous返回true

4、使用成员变量step和data
对于单通道矩阵来说，step[0]代表每一行所占的字节数，如果有间隔则间隔也作为字节数被计算在内；step[1]代表每一个数值所占的字节数，data是一个指针类型为uchar，它指向第一个数值。
因此无论矩阵行与行之间在内存中是否有间隔，都可以使用以下代码来访问第r行c列：

```C++
*((int *)(m.data+m.step[0]*r+c*m.step[1]))
```

**向量类Vec(构建多通道Mat的基础)**

可以把这里的向量理解为列向量，构造一个 _cn行x1列的数据类型为_Tp的列向量格式：Vec<Typename _Tp, int _cn>

单通道矩阵每个元素都是一个数值，多通道矩阵每个元素可以看作一个向量。

## 构造多通道Mat对象

构造一个由n个rows*cols二维浮点型矩阵组成的三维矩阵的格式如下：
```C++
Mat(int rows,int cols,CV_32FC(n));
//例如构造一个2行2列的float类型三通道矩阵
Mat mm = (Mat_<Vec3f>(2, 2) << Vec3f(1, 11, 21), Vec3f(2, 12, 32),
		Vec3f(3, 13, 23), Vec3f(4, 24, 34));

```

多通道的Mat可以看作特殊的二维数组，每一个元素不是数值而是一个向量。

分离通道使用split()函数

合并通道使用merge函数

使用 **rowRange或colRange** 得到矩阵的连续行或连序列

注意：成员函数row、 col 、 rowRange 、colRange 返回的矩阵是指向原矩阵的，例如改变c_range第1行1列的值，原矩阵也会改变。使用成员函数clone解决。

## 使用Rect类

如果我们需要矩阵中一块矩形区域，我们可以使用rowRange和colRange来定位，但是Opencv提供了Rect类来简化操作。知道矩形的左上角坐标，和矩形的宽高就可以确定一个矩形，所以其构造函数为：

`Rect(int _x,int _y, int _width, int _height);`

也可以将 _width 和 _height保存在一个Size中

`Rect(int _x,int _y, Size size);`

如果知道左上角和右下角的坐标也可以确定一个矩形，所以构造函数为：

`Rect(Point2i &pt1,Point2i &pt2);`

---

## uchar与char

char 是有符号的 unsigned char 是无符号的,里面全是正数 两者都作为字符用的话是没有区别的，但当整数用时有区别： char 整数范围为-128到127( 0x80__0x7F)， 而unsigned char 整数范围为0到255( 0__0xFF ) 有时候想把整数数值限在255范围内，也用unsigned char，这个类型在嵌入式用的多

## Mat数据类型指针ptr的使用

```C++
    cv::Mat image = cv::Mat(400, 600, CV_8UC1); //宽400，长600
    uchar * data00 = image.ptr<uchar>(0);
    uchar * data10 = image.ptr<uchar>(1);
    uchar * data01 = image.ptr<uchar>(0)[1];
```

>定义了一个Mat变量image。
data00是指向image第一行第一个元素的指针。
data10是指向image第二行第一个元素的指针。
data01是指向image第一行第二个元素的指针。

注意： 
如果你的程序使用来image.ptr指针，并且出现了下面这样的错误：（假使用的软件是Visual Studio 201x）
>某某.exe中的 0x75065b68 处有未经处理的异常:Microsoft C++ 异常; 内存位置0x85e790处的cv::Exception。
1

这可能是因为不理解image.ptr这个指针，犯了这样的错误：image.ptr(1);指的不是image中第二个像素，而是第一行第二个像素的指针。 
使用上面的代码举例：image有400行，有400*600个像素。假设现在你想得到第3行第42个像素的指针，如果你写成：

```C++
uchar * data = image.ptr<uchar>(3*image.cols + 41);
```

这样写是错误的，会出现上面的错误。你得到的不是第３行第４２个像素的指针，而是第（3×image.cols + 41）行第0个像素的指针，因为没有（3×image.cols + 41）行，所以没有这个指针，所以错误。 
正确的写法：

```C++
uchar * data = image.ptr<uchar>(3)[41];
```

所以要注意这一点：如果程序可以正常编译，但是运行时出错，很有可能是你给指针赋值的时候，索引值溢出指定范围，指针乱指，导致程序跑偏，所以只有在运行时才能发现错误。








