CC = g++
 
LDFLAGS = -lopencv_legacy -lopencv_highgui -lopencv_core -lopencv_ml -lopencv_video -lopencv_imgproc -lopencv_calib3d -lopencv_objdetect -L/usr/lib
CPPFLAGS = -lm -g -I/usr/include/opencv -I/usr/include/opencv2

TARGET = janken
SRCS = janken.cpp playSound.c 
OBJS = janken.o playSound.o

RM = rm -f

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CPPFLAGS) -c $<
janken.o: playSound.h janken.cpp
playSound.o: playSound.h
clean:
	$(RM) $(OBJS)

clean_target:
	$(RM) $(TARGET)

clean_all:
	$(RM) $(TARGET) $(OBJS)
