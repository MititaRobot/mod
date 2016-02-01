CC = g++
 
LDFLAGS = -lopencv_legacy -lopencv_highgui -lopencv_core -lopencv_ml -lopencv_video -lopencv_imgproc -lopencv_calib3d -lopencv_objdetect -L/usr/lib -pthread -w
CPPFLAGS = -lm -g -I/usr/include/opencv -I/usr/include/opencv2 -pthread -w

TARGET = asano2
SRCS = motor.c onpa.c camera.c compass.c playSound.c libconnect_julius.c libget_word.c
OBJS = motor.o onpa.o camera.o compass.o playSound.o libconnect_julius.o libget_word.o

TARGETCAMERA = test_camera
SRCSCAMERA = camera.c test_camera.c
OBJSCAMERA = camera.o test_camera.o

RM = rm -f

all: $(TARGET) $(TARGETCAMERA)

$(TARGET): $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

$(TARGETCAMERA): $(OBJSCAMERA)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CPPFLAGS) -c $<

motor.o: motor.h
motor.o: playSound.h
camera.o: camera.h
onpa.o: onpa.c
compass.o: compass.h
playSound.o: playSound.h
test_camera.o: camera.h

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJSCAMERA)

clean_target:
	$(RM) $(TARGET)
	$(RM) $(TARGETCAMERA)

clean_all:
	$(RM) $(TARGET) $(OBJS)
	$(RM) $(TARGETCAMERA) $(OBJSCAMERA)
