#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "camera.h"

CvCapture *capture = 0;
IplImage *frame = 0;

// double width = 160, height = 120;
// double width = 320, height = 240;
// double width = 640, height = 480;
// double width = 800, height = 600;
void camera_init(double width, double height)
{
  // カメラに対するキャプチャ構造体を作成。
  capture = cvCreateCameraCapture(0);

  // キャプチャサイズの設定。
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, width);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, height);
}

void camera_close()
{ 
  // 後片付け。
  cvReleaseCapture(&capture);
}

void camera_capture()
{
  // 画像キャプチャ。
  frame = cvQueryFrame(capture);
}

int test_method()
{  
  int i, j;
  uchar p[3];
  uchar tmp[160][5];
  uchar ret[160];
  int h = frame->height;
  int w = frame->width;
  int begin = -1;
  int end = -1;
  for(j = 0; j < 5; j++) {
    for (i = 0; i < w; i++) {
      p[0] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3];//G
      p[1] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3 + 1];//B
      p[2] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3 + 2];//R
      
      if (p[2] > p[0] + p[1])
        tmp[i][j] = 1;
      else
        tmp[i][j] = 0;
    }
  }
  for (i = 0; i < w; i++) {
    ret[i] = (tmp[i][0] + tmp[i][1] + tmp[i][2] + tmp[i][3] + tmp[i][4]) / 3;
  }
  for (i = 0; i < w; i++) {
    if (ret[i] == 1) {
      begin = i;
      break;
    }
  }
  for (i = w - 1; i >= 0; i--) {
    if (ret[i] == 1) {
      end = i;
      break;
    }
  }

  return (begin + end) / 2;
}

void set_begin_end(int *begin, int *end)
{
  int i, j;
  uchar p[3];
  uchar tmp[160][5];
  uchar ret[160];
  int h = frame->height;
  int w = frame->width;
  *begin = -1;
  *end = -1;
  for(j = 0; j < 5; j++) {
    for (i = 0; i < w; i++) {
      p[0] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3];//G
      p[1] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3 + 1];//B
      p[2] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3 + 2];//R
      
      if (p[2] > p[0] + p[1])
        tmp[i][j] = 1;
      else
        tmp[i][j] = 0;
    }
  }
  for (i = 0; i < w; i++) {
    ret[i] = (tmp[i][0] + tmp[i][1] + tmp[i][2] + tmp[i][3] + tmp[i][4]) / 3;
  }
  for (i = 0; i < w; i++) {
    if (ret[i] == 1) {
      *begin = i;
      break;
    }
  }
  for (i = w - 1; i >= 0; i--) {
    if (ret[i] == 1) {
      *end = i;
      break;
    }
  }
}


int identifyRed(){
  int i, j;
  uchar p[3];
  uchar tmp[160][5];
  uchar ret[160];
  int h = frame->height;
  int w = frame->width;
  int begin = -1;
  int end = -1;
  float count = 0.0;

  for(j = 0; j < 5; j++) {
    for (i = 0; i < w; i++) {
      p[0] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3];//G
      p[1] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3 + 1];//B
      p[2] = frame->imageData[frame->widthStep * (h/2 + j) + i * 3 + 2];//R
      
      if (p[2] > p[0] + p[1])
        tmp[i][j] = 1;
      else
        tmp[i][j] = 0;
    }
  }
  for (i = 0; i < w; i++) {
    ret[i] = (tmp[i][0] + tmp[i][1] + tmp[i][2] + tmp[i][3] + tmp[i][4]) / 3;
  }

  for (i = 0; i < w; i++ ){
	count += ret[i];  
  }

  if((count / w) >= 0.2){
	return 1;	  
  }

 return 0; 
}
