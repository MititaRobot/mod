#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <opencv/highgui.h>
#include "camera.h"

int main()
{
  // double width = 160, height = 120;
  // double width = 320, height = 240;
  // double width = 640, height = 480;
  // double width = 800, height = 600;
  int i;
  int *begin, *end;
  camera_init(160, 120);  
  for (i = 0; i < 10; i++) {
    camera_capture();
    printf("Value: %d\n", set_begin_end());
    sleep(1);
  }
  camera_close();

  return 0;
}
