// Myface.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

/*加载头文件*/

int _tmain(int argc, _TCHAR* argv[])
{
	CvCapture* capture=0;
	
	/*初始化一个视频捕获操作。告诉底层的捕获api我想从Capture1.avi中捕获图片，
	  底层api将检测并选择相应的解码器并做好准备工作*/
	
	capture = cvCaptureFromFile( "F:\\1.avi");  //设置要读的视频（avi格式）

	static CvMemStorage* storage = 0;
	static CvHaarClassifierCascade* cascade = 0;//haar分类器级联的内部标识形式

	cascade = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml",0,0,0);
	//加载人脸检测分类器
	if( !cascade || !capture ) //如果没有找到分类器
		return -1;
	storage = cvCreateMemStorage(0);//动态存储结构，用来存储人脸在图像中的位置
  
	/*创建一个窗口，用“Video”作为窗口的标识符*/
	cvNamedWindow( "Video",1);
	
	/*如果初始化失败，那么capture为空指针，程序停止，否则进入捕获循环*/
	if( capture )
	{
		for(;;)
		{
			IplImage* frame = cvQueryFrame( capture );
			IplImage* img = NULL;
			CvSeq* faces;
			
			if( !frame )
				break;
			img = cvCloneImage(frame); 
			img->origin = 0;
			
			if( frame->origin )
				cvFlip(img,img);
			
			cvClearMemStorage( storage );
			//目标检测
			faces = cvHaarDetectObjects( img, cascade, storage,1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(30,30) );//检测人脸返回矩形人脸：分类器跳过平滑（无边缘）区域
			
			

			for( int i = 0; i < (faces ? faces->total : 0); i++ ) 
			{
				CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
				cvRectangle( img, cvPoint(r->x,r->y),
					cvPoint(r->x+r->width,r->y+r->height), CV_RGB(255,0,0), 1);
			}
			cvShowImage( "Video", img );
			
			
			
			/*敲了键盘，就退出程序，否则继续捕获下一帧*/
			if( cvWaitKey(10)>0 )
				break;
		}
		/*退出之前结束底层api的捕获操作,比如会使得别的程序无法访问已经被它们打开的文件*/
	    cvReleaseCapture( &capture);
	}
	/*销毁窗口*/
	cvDestroyWindow("Video");

	return 0;
}

