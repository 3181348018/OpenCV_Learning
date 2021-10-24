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



