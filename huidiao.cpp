
#include <opencv2/opencv.hpp>
using namespace cv;

#define WINDOW "程序窗口"

void on_MouseHandle(int event, int x, int y, int flags, void *param);
void DrawRectangle(Mat &img, Rect &box);
void shotScreen(Mat &mat);

Rect g_rectangle;           //用来保存截图的位置信息
bool g_bDrawingBox = false; //绘制标识符
Mat srcROI;                 //用来保存截图

int main(int argc, char **argv)
{
  g_rectangle = Rect(-1, -1, 0, 0);
  Mat srcImage, tempImage;
  srcImage = imread("Lena.jpg"); //读取一张图片
  srcImage.copyTo(tempImage);

  namedWindow(WINDOW);                                         //定义一个窗口
  setMouseCallback(WINDOW, on_MouseHandle, (void *)&srcImage); //对该窗口进行鼠标检测

  while (1)
  {
    srcImage.copyTo(tempImage); //不断的用读取的图片更新临时图片tempImage
    if (g_bDrawingBox)
      DrawRectangle(tempImage, g_rectangle); //将矩形g_rectangle画到tempImage上
    imshow(WINDOW, tempImage);               //展示tempImage
    if (waitKey(10) == 27)
      break; //当按下Esc时程序结束
  }
  return 0;
}

void on_MouseHandle(int event, int x, int y, int flags, void *param)
{
  Mat &image = *(Mat *)param;
  switch (event)
  { //检查鼠标事件
  case EVENT_MOUSEMOVE:
  { //如果检测到鼠标移动
    if (g_bDrawingBox)
    { //如果绘制标识符为真，则记录下移动时矩形的宽，高
      g_rectangle.width = x - g_rectangle.x;
      g_rectangle.height = y - g_rectangle.y;
    }
  }
  break;
  case EVENT_LBUTTONDOWN:
  {                                 //检测到鼠标左键按下
    g_bDrawingBox = true;           //将绘制标识符设为真，准备开始绘图
    g_rectangle = Rect(x, y, 0, 0); //用g_rectangle保存下起点
  }
  break;
  case EVENT_LBUTTONUP:
  {                        //检测到鼠标左键抬起
    g_bDrawingBox = false; //停止绘图
    if (g_rectangle.width < 0)
    {                                     //对截取的进行处理（因为向上截取则宽，高为负
      g_rectangle.x += g_rectangle.width; //向下截取宽高为正）
      g_rectangle.width *= (-1);
    }
    if (g_rectangle.height < 0)
    {
      g_rectangle.y += g_rectangle.height;
      g_rectangle.height *= (-1);
    }
    srcROI = image(g_rectangle); //用全局变量srcROI将截取的位置保存
    shotScreen(srcROI);          //自定义函数，用来展示截图并保存截图
  }
  break;
  }
}

void DrawRectangle(Mat &img, Rect &box)
{
  rectangle(img, box.tl(), box.br(), Scalar(0, 0, 255));
}

void shotScreen(Mat &mat)
{
  imshow("截图", mat);
  imwrite("截取的图片.jpg", mat);
}
