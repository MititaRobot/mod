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

void write_command(struct ccmd *);
//怒りのグローバル
int motor_fd = 0;

int quit_flag = 1;
//int fd;
int quit = 0;

int speed = 0x7fff;
int ikiti = 0x22a0;


void speed_up()
{
	speed += 5;
}

int main(int argc, char **argv)
{
  struct uin ibuf;
  struct uout obuf;
  struct ccmd cmd;
  int i;

  char *dev = "/dev/urbtc0";

  signal(SIGINT, exit_program);
//  signal(SIGTSTP, speed_up);
	motor_init(&cmd);
	i = 0;
  // short p = 0xf0;
	cmd.offset[0] = cmd.offset[1] = 0xffff; 
	cmd.offset[2] = cmd.offset[3] = 0;
	//fprintf(stderr, "speed1:%d, speed2:%d\n",a1,a2);
  write_command(&cmd);
  while (quit_flag)  {
  	//cmd.offset[0] = cmd.offset[1] = cmd.offset[2] = cmd.offset[3] = speed;
		sleep(1);
		//move(speed,&cmd);
  }

  
	/* end */
  cmd.offset[0] = cmd.offset[1] = cmd.offset[2] = cmd.offset[3] = 0x7fff;
  write_command(&cmd);

  motor_close();
  return 0;
}

void exit_program(sig, code, scp, addr)
int sig;
int code;
struct sigcontext *scp;
char *addr;
{
  quit_flag = 0;
  fprintf(stderr, "kill signal is received\n");
}
void rotate(int angle, struct ccmd *cmd)
{
  angle <<= 5;
  unsigned short speed = 0x7fff + angle;
  if (angle > 0 && speed < 0x7fff)
    speed = 0xffff;
  else if (angle < 0 && speed > 0x7fff)
    speed = 0x0;
  cmd->offset[0] = speed;
  cmd->offset[1] = speed;
  cmd->offset[2] = speed;
  cmd->offset[3] = speed;
}

void move(int distance, struct ccmd *cmd)
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



void motor_init(struct ccmd* cmd)
{

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

void write_command(struct ccmd *cmd)
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

void motor_close()
{
	close(motor_fd);
}
