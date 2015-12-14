#ifndef MOTOR_H
#define MOTOR_H


extern int motor_fd;

void exit_program();
void motor_init(struct ccmd *);
void motor_close();
void rotate(int, struct ccmd *);
void move(int, struct ccmd *);

#endif
