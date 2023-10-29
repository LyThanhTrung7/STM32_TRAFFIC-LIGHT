/*
 * user_timer.h
 *
 *  Created on: Oct 21, 2023
 *      Author: lytha
 */

#ifndef INC_USER_TIMER_H_
#define INC_USER_TIMER_H_


extern int timer1_flag; //tao co cho timer 1
extern int timer2_flag; //tao co cho timer 2
extern int timer3_flag; //tao co cho timer 3

void set_timer1(int duration);
void set_timer2(int duration);
void set_timer3(int duration);
void timer_run();

#endif /* INC_USER_TIMER_H_ */
