#ifndef INCLUDED_CAMERA
#define INCLUDED_CAMERA

#include <opencv/highgui.h>

extern CvCapture *capture;
extern IplImage *frame;

// double width = 160, height = 120;
// double width = 320, height = 240;
// double width = 640, height = 480;
// double width = 800, height = 600;
void camera_init(double width, double height);

void camera_capture();
void camera_close();
int test_method();

#endif
