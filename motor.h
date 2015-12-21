#ifndef MOTOR_H
#define MOTOR_H


extern int motor_fd;

void write_command();
void exit_program();
void motor_init();
void motor_close();
void rotate(int);
void move(int);
void move_forward(int);
void move_backward(int);
void turn_left(int);
void turn_right(int);
void move_left(int,int);
void move_right(int,int);


#endif
