#include <stdio.h>
#include <stdlib.h>  // chg akihito
#include <math.h>    // chg akihito
#include <unistd.h>         /* for read, write, close */
#include <sys/ioctl.h>      /* for ioctl */
#include <sys/types.h>      /* for open */
#include <sys/stat.h>       /* for open */
#include <fcntl.h>          /* for open */

#include <signal.h>
#include "driver/urbtc.h"          /* Linux specific part */
#include "driver/urobotc.h"        /* OS independent part */
#include "motor.h"
#include "compass.h"

#define PI 3.1415926535

#define SET_OFFSET(speed) do { \
				cmd->offset[0] =  cmd->offset[1] = 0x7fff + speed; \
				cmd->offset[2] = cmd->offset[3] = 0x7fff - speed; \
			} while (0); 
#define SET_OFFSET2(speedl,speedr) do { \
				cmd->offset[0] =  cmd->offset[1] = 0x7fff + speedl; \
				cmd->offset[2] = cmd->offset[3] = 0x7fff - speedr; \
			} while (0); 
#define SET_ZERO() ( cmd->offset[0] = cmd->offset[1] = cmd->offset[2] = cmd->offset[3] = 0x7fff)

//怒りのグローバル
int motor_fd = 0;
struct ccmd* cmd;
int quit_flag = 1;

int main(int argc, char **argv)
{
  int i;

  signal(SIGINT, exit_program);
  motor_init();
	i = 0;

	move_left(100,5000);
  while (quit_flag)  {
		usleep(1000);
  }
  
	SET_ZERO();
  write_command();

  motor_close();
  return 0;
}


void exit_program(int sig)
{
  quit_flag = 0;
  fprintf(stderr, "kill signal is received\n");
}

void rotate(int angle)
{

  cmd->offset[0] = speed;
  cmd->offset[1] = speed;
  cmd->offset[2] = speed;
  cmd->offset[3] = speed;
}

void move(int distance)
{
	distance <<= 5;
  unsigned short speed1 = 0x7fff + distance;
	unsigned short speed2 = 0x7fff - distance;
  if (distance > 0 && speed1 < 0x7fff) {
    speed1 = 0xffff;
    speed2 = 0x0;
  }
  if (distance < 0 && speed1 > 0x7fff) {
    speed1 = 0x0;
    speed2 = 0xffff;
  }
  cmd->offset[0] = speed1;
  cmd->offset[1] = speed1;
  cmd->offset[2] = speed2;
  cmd->offset[3] = speed2;
	printf("offset0: %x, offset2: %x\n", cmd->offset[0], cmd->offset[2]);
}

void move_left(int radius,int speed) 
{	
	const float size = 30;
	int speedr = speed * (float)(radius + size) / size;
	fprintf(stderr,"%d %d",speed,speedr);
	SET_OFFSET2(speed,speedr);
	write_command();
}

void move_right(int radius,int speed) 
{	
	const float size = 30;
	int speedl = speed * (float)(radius + size) / size;
	SET_OFFSET2(speedl,speed);
	write_command();
}
void turn_left(int speed)
{
	SET_OFFSET2(-speed,speed);
	write_command();
}

void turn_right(int speed)
{
	turn_left(-speed);
}

void move_forward(int speed)
{
	SET_OFFSET(speed);
	write_command();
}

void move_backward(int speed)
{
	SET_OFFSET(-speed);
	write_command();
}

void stop_temp()
{
	int time = 0;
	int speed = cmd->offset[0] - 0x7fff;
	speed = speed < 0x7fff - cmd->offset[2] ? 0x7fff - cmd->offset[2] : speed;
	while ((time += 30) < 101) {
		int temp = speed * cos((time * 0.0157));
		SET_OFFSET(temp);
		write_command();
		usleep(100);
	}
	SET_ZERO();
	write_command();
}

void stop()
{
	SET_ZERO();
	write_command();
}


void move_gap(int distance, int gap)
{
	int tmp = distance;
	distance *= 32;
	gap *= distance / 900;
	if (abs(gap) <= 30) {
		gap = 0;
	}

  unsigned short speed1 = 0x7fff - gap + distance;
	unsigned short speed2 = 0x7fff - gap - distance;
  
	if (distance > 0 && speed1 < 0x7fff) {
    speed1 = 0xffff;
    speed2 = 0x0;
  }

  if (distance < 0 && speed1 > 0x7fff) {
    speed1 = 0x0;
    speed2 = 0xffff;
  }

	if (tmp < 15) {
		speed1 = 0x7fff;
		speed2 = 0x7fff;
	}

  cmd->offset[0] = speed1;
  cmd->offset[1] = speed1;
  cmd->offset[2] = speed2;
  cmd->offset[3] = speed2;
	printf("distance:%d, gap:%d, offset0: %x, offset2: %x\n", distance, gap, cmd->offset[0] - 0x7fff, 0x7fff - cmd->offset[2]);
}
/*
void set_offset(int speed)
{
	cmd->offset[0] = cmd->offset[1] = 0x7fff + speed;
	cmd->offset[2] = cmd->offset[3] = 0x7fff - speed;
}

void set_offset2(int speedl,int speedr)
{
	cmd->offset[0] = cmd->offset[1] = 0x7fff + speedl;
	cmd->offset[2] = cmd->offset[3] = 0x7fff - speedr;
}
void set_zero()
{
	cmd->offset[0] = cmd->offset[0] = cmd->offset[0] = cmd->offset[0] = 0x7fff;
}
*/

void write_command()
{
  if (ioctl(motor_fd, URBTC_COUNTER_SET) < 0) {
    fprintf(stderr, "ioctl: URBTC_COUNTER_SET error\n");
    exit(1);
  }

  if (!write(motor_fd, cmd, sizeof(struct ccmd)) > 0) {
    fprintf(stderr, "write cmd error\n");
    exit(1);
  }
}

void motor_init()
{

	if ((cmd = (struct ccmd *)malloc(sizeof(struct ccmd))) == NULL) {
		fprintf(stderr,"Cannot allocate memory.\n");
		exit(1);
	}
	

  char *dev = "/dev/urbtc0";
  if ((motor_fd = open(dev, O_RDWR)) == -1) {
    fprintf(stderr, "%s: Open errorr%d \n", dev,motor_fd);
    exit(1);
  }
  cmd->retval = 0 /* RETURN_VAL */;
  cmd->setoffset  = CH0 | CH1 | CH2 | CH3;
  cmd->setcounter = CH0 | CH1 | CH2 | CH3;
  cmd->resetint   = CH0 | CH1 | CH2 | CH3;

  cmd->selin = CH0 | CH1 | CH2 | CH3 | SET_SELECT;

  cmd->dout = 0;

  cmd->selout = SET_SELECT | CH0 | CH1 | CH2 | CH3;

  cmd->offset[0] = cmd->offset[1] = cmd->offset[2] = cmd->offset[3] = 0x7fff;
  cmd->counter[0] = cmd->counter[1] = cmd->counter[2] = cmd->counter[3] = 0;
  cmd->magicno = 0x00;

  cmd->posneg = SET_POSNEG | CH0 | CH1 | CH2 | CH3;
  cmd->breaks = SET_BREAKS | CH0 | CH1 | CH2 | CH3;

  cmd->wrrom = 0;
}


void motor_close()
{
	close(motor_fd);
	free(cmd);
}
