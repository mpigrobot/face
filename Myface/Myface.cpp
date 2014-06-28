// Myface.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

/*����ͷ�ļ�*/

int _tmain(int argc, _TCHAR* argv[])
{
	CvCapture* capture=0;
	
	/*��ʼ��һ����Ƶ������������ߵײ�Ĳ���api�����Capture1.avi�в���ͼƬ��
	  �ײ�api����Ⲣѡ����Ӧ�Ľ�����������׼������*/
	
	capture = cvCaptureFromFile( "F:\\1.avi");  //����Ҫ������Ƶ��avi��ʽ��

	static CvMemStorage* storage = 0;
	static CvHaarClassifierCascade* cascade = 0;//haar�������������ڲ���ʶ��ʽ

	cascade = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml",0,0,0);
	//����������������
	if( !cascade || !capture ) //���û���ҵ�������
		return -1;
	storage = cvCreateMemStorage(0);//��̬�洢�ṹ�������洢������ͼ���е�λ��
  
	/*����һ�����ڣ��á�Video����Ϊ���ڵı�ʶ��*/
	cvNamedWindow( "Video",1);
	
	/*�����ʼ��ʧ�ܣ���ôcaptureΪ��ָ�룬����ֹͣ��������벶��ѭ��*/
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
			//Ŀ����
			faces = cvHaarDetectObjects( img, cascade, storage,1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(30,30) );//����������ؾ�������������������ƽ�����ޱ�Ե������
			
			

			for( int i = 0; i < (faces ? faces->total : 0); i++ ) 
			{
				CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
				cvRectangle( img, cvPoint(r->x,r->y),
					cvPoint(r->x+r->width,r->y+r->height), CV_RGB(255,0,0), 1);
			}
			cvShowImage( "Video", img );
			
			
			
			/*���˼��̣����˳����򣬷������������һ֡*/
			if( cvWaitKey(10)>0 )
				break;
		}
		/*�˳�֮ǰ�����ײ�api�Ĳ������,�����ʹ�ñ�ĳ����޷������Ѿ������Ǵ򿪵��ļ�*/
	    cvReleaseCapture( &capture);
	}
	/*���ٴ���*/
	cvDestroyWindow("Video");

	return 0;
}

