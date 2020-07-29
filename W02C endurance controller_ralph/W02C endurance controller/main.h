/*
 * main.h
 *
 * Created: 8/28/2019 7:52:38 AM
 *  Author: Edwin
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define INTERRUPTS_TIMER_MAIN_OCR	256
//#define TIME_MOTOR_STEP			91
#define TIMER0_100MS				200000

extern volatile uint32_t op_spd_val_temp, op_spd_val;
extern volatile uint32_t cw_setcnt_val_temp, cw_setcnt_val;
extern volatile uint32_t ccw_setcnt_val_temp, ccw_setcnt_val;
extern volatile uint32_t cyc_cnt_val_temp, cyc_cnt_val;
extern float ramp_time_val_temp, ramp_time_val;
extern float dead_time_val_temp, dead_time_val;

extern volatile uint32_t cw_preset_count, ccw_preset_count;
extern volatile uint32_t cw_pulse_ctr;
extern volatile uint32_t ccw_pulse_ctr;
extern volatile uint32_t cyccnt_ctr;

extern volatile uint32_t timer0_100ms;

extern volatile uint8_t flag_switch;
	#define flag_start				0	//set when start switch is pressed
	#define flag_stop				1	//set when stop switch is pressed 
	#define flag_param_saved		2	//set when parameters values are saved  

extern volatile uint8_t flag_timer;	
	#define flag_timer_1ms_start			0	 

	
#endif /* MAIN_H_ */