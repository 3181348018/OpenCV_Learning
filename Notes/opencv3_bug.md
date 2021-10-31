# opencv3 中的常见错误

## 1.cvtColor参数问题

cvtColor函数中颜色转换系的宏，全替换为”COLOR_”前缀，如CV_BGR2HSV改为COLOR_BGR2HSV

在caffe有时还要在前加cv::

opencv3中取消了opencv1中的”cv_”命名规范，对于这些cv_前缀的使用了新的命名规范。这里大概可以大概分为以下这几类。

(1)直接去掉”CV_”前缀
1)namedWindow中将cv_WINDOW_AUTOSIZE改为WINDOW_AUTOSIZE 
2)line函数及其同一类的绘图函数，cv_filed改为filled 
3)threshold函数中将cv_thresh_binary改为thresh_binary 
4)remap函数中，cv_inter_linear改为inter_linear 
5)鼠标操作函数setmousecallback中将cv_event_lbuttonup改为event_lbuttonup，等等一些直接将cv去除。 
6)imwrite中，将cv_inpaint_telea改为inpaint_telea，cv_inpaint_ns改为inpaint_ns 
7)设置摄像头尺寸时将cv_cap_prop_frame_width改为cap_prop_frame_width，cv_cap_prop_frame_height 改为 cap_prop_frame_height 
..等等一些函数可以直接将前缀去除。

(2)需要新的前缀
比较常用的一些如 
1)line函数中一些绘函数中的参数，cv_AA改为LINE_AA 
2)cvtColor函数中颜色转换系的宏，全替换为”COLOR_”前缀，如CV_BGR2HSV改为COLOR_BGR2HSV 
3)normalize中，将cv_MINMAX改为NORM_MINMAX 等等

(3)其他
1)定义尺寸时将cvSize(-1,-1)改为Size(-1,-1) 
2)在表示颜色时，将cv_RGB改为Scalar。 
其中Videowriter中的fourcc，cv2.x版本中中形式为cv2.cv.fourcc，在cv3,0以上的版本中应为cv2.VideoWriter_fourcc，需要特别注意。 

---
## VideoCapture打开视频失败

需要将**opencv_videoio_ffmpeg454_64.dll**文件放进工作目录中