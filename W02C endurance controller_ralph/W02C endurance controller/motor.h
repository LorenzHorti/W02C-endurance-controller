/*
 * motor.h
 *
 * Created: 8/28/2019 1:09:42 PM
 *  Author: Edwin
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

extern volatile uint8_t flag_motor_1;
	#define MOTOR_1_IDLE				0
	#define MOTOR_1_PARAM_SET			1   
	#define MOTOR_1_ACCEL				2	
	#define MOTOR_1_OPRTNG_SPD			3
	#define MOTOR_1_DECEL				4	
	
extern volatile uint8_t flag_motor2;
	#define MOTOR_2_DIR_CW				0
	#define MOTOR_2_100MS_ACCEL			1
	#define MOTOR_2_100MS_DECEL			2
	#define MOTOR_2_RAMPUP_DONE			3
	
#define STEPS_PER_REV	500	

extern volatile uint32_t rev_count_inc; 
extern volatile uint32_t ramp_inc_dec;
extern volatile uint32_t ramp_OCR1A;
void motor_set_param();
void motor_stop(uint32_t lapsed_time);
int motor_acceleration(void);
int motor_operating_spd(void);
void motor_deceleration();



#endif /* MOTOR_H_ */