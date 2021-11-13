
#include <opencv2/opencv.hpp>
using namespace cv;

#define WINDOW "���򴰿�"

void on_MouseHandle(int event, int x, int y, int flags, void *param);
void DrawRectangle(Mat &img, Rect &box);
void shotScreen(Mat &mat);

Rect g_rectangle;           //���������ͼ��λ����Ϣ
bool g_bDrawingBox = false; //���Ʊ�ʶ��
Mat srcROI;                 //���������ͼ

int main(int argc, char **argv)
{
  g_rectangle = Rect(-1, -1, 0, 0);
  Mat srcImage, tempImage;
  srcImage = imread("Lena.jpg"); //��ȡһ��ͼƬ
  srcImage.copyTo(tempImage);

  namedWindow(WINDOW);                                         //����һ������
  setMouseCallback(WINDOW, on_MouseHandle, (void *)&srcImage); //�Ըô��ڽ��������

  while (1)
  {
    srcImage.copyTo(tempImage); //���ϵ��ö�ȡ��ͼƬ������ʱͼƬtempImage
    if (g_bDrawingBox)
      DrawRectangle(tempImage, g_rectangle); //������g_rectangle����tempImage��
    imshow(WINDOW, tempImage);               //չʾtempImage
    if (waitKey(10) == 27)
      break; //������Escʱ�������
  }
  return 0;
}

void on_MouseHandle(int event, int x, int y, int flags, void *param)
{
  Mat &image = *(Mat *)param;
  switch (event)
  { //�������¼�
  case EVENT_MOUSEMOVE:
  { //�����⵽����ƶ�
    if (g_bDrawingBox)
    { //������Ʊ�ʶ��Ϊ�棬���¼���ƶ�ʱ���εĿ���
      g_rectangle.width = x - g_rectangle.x;
      g_rectangle.height = y - g_rectangle.y;
    }
  }
  break;
  case EVENT_LBUTTONDOWN:
  {                                 //��⵽����������
    g_bDrawingBox = true;           //�����Ʊ�ʶ����Ϊ�棬׼����ʼ��ͼ
    g_rectangle = Rect(x, y, 0, 0); //��g_rectangle���������
  }
  break;
  case EVENT_LBUTTONUP:
  {                        //��⵽������̧��
    g_bDrawingBox = false; //ֹͣ��ͼ
    if (g_rectangle.width < 0)
    {                                     //�Խ�ȡ�Ľ��д�����Ϊ���Ͻ�ȡ�����Ϊ��
      g_rectangle.x += g_rectangle.width; //���½�ȡ���Ϊ����
      g_rectangle.width *= (-1);
    }
    if (g_rectangle.height < 0)
    {
      g_rectangle.y += g_rectangle.height;
      g_rectangle.height *= (-1);
    }
    srcROI = image(g_rectangle); //��ȫ�ֱ���srcROI����ȡ��λ�ñ���
    shotScreen(srcROI);          //�Զ��庯��������չʾ��ͼ�������ͼ
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
  imshow("��ͼ", mat);
  imwrite("��ȡ��ͼƬ.jpg", mat);
}
