/*
 * motor.c
 *
 * Created: 8/28/2019 1:09:52 PM
 *  Author: Edwin
 */ 



#include <avr/io.h>
#include "avr035.h"
#include "main.h"
#include "motor.h"
#include "atmel_io.h"
#include "init.h"


volatile uint8_t flag_motor_1, flag_motor2;
uint32_t ramp_steps;
volatile uint32_t ramp_inc_dec = 0;
volatile uint32_t ramp_OCR1A = 0;
volatile uint32_t rev_count_inc=0;    //no. of rev during ramp up

void motor_set_param()
{
	op_spd_val_temp = op_spd_val_temp<2?2:op_spd_val_temp;		//min operating speed = 2 rpm
	op_spd_val = op_spd_val_temp>2000?2000:op_spd_val_temp;		//max operating speed = 2000 rpm
	cw_setcnt_val = cw_setcnt_val_temp;
	ccw_setcnt_val = ccw_setcnt_val_temp;
	cyc_cnt_val = cyc_cnt_val_temp;
	
	if (op_spd_val<20)
	{
		ramp_time_val=0;
	}
	else if(op_spd_val<50)
	{
		ramp_time_val=1;
	}
	else ramp_time_val=4;
	//ramp_time_val = ramp_time_val_temp;
	/*if (op_spd_val<20)
	{
		ramp_time_val=0;
	}
	else if (op_spd_val<50)
	{
		ramp_time_val=1;
	}
	else if (op_spd_val<100)
	{
		ramp_time_val=2;
	}
	else if (op_spd_val<150)
	{
		ramp_time_val=3;
	}
	else if (op_spd_val<200)
	{
		ramp_time_val=4;
	}
	else if (op_spd_val<250)
	{
		ramp_time_val=5;
	}
	else if (op_spd_val<300)
	{
		ramp_time_val=6;
	}
	else if (op_spd_val<350)
	{
		ramp_time_val=7;
	}
	else if (op_spd_val<400)
	{
		ramp_time_val=8;
	}
	else if (op_spd_val<450)
	{
		ramp_time_val=9;
	}
	else
	{
		ramp_time_val=10;
	}*/
	dead_time_val = dead_time_val_temp;
	ramp_steps = ramp_time_val*10;			//ramp every 100 ms
	ramp_inc_dec = op_spd_val/ramp_steps;	//increment/decrement of rpm per 100ms	
	
	if (cw_setcnt_val <= 0)
	{
		CLEARBIT(flag_motor2,MOTOR_2_DIR_CW);
	}
	
}

float Hz;


int motor_acceleration(void)
{
	int result=0;
	if (ramp_time_val==0)
	{
		ramp_OCR1A=op_spd_val;
		//CLEARBIT(TCCR1B, CS11); //timer stop, prescale=8
		OCR1A = (1/(ramp_OCR1A*.000008333))-1;
		if (TCNT1>OCR1A)
		{
			TCNT1=0;
		}
		SETBIT(TCCR1B, CS11); //timer start, prescale=8
	}
	else ramp_OCR1A += ramp_inc_dec;
	
	if ( ramp_OCR1A < op_spd_val)
	{
		//CLEARBIT(TCCR1B, CS11); //timer stop, prescale=8
		OCR1A = (1/(ramp_OCR1A*.000008333))-1;
		if (TCNT1>OCR1A)
		{
			TCNT1=0;
		}
		SETBIT(TCCR1B, CS11); //timer start, prescale=8
	}
	else 
	{	
		//SETBIT(flag_motor2, MOTOR_2_RAMPUP_DONE);
		rev_count_inc = !CHECKBIT(flag_motor2,MOTOR_2_DIR_CW)?ccw_pulse_ctr:cw_pulse_ctr;
		flag_motor_1 = MOTOR_1_OPRTNG_SPD;
		result=1;
	}
	return result;
}
int motor_operating_spd(void)
{
	int result=0;
	OCR1A = (1/(op_spd_val*.000008333))-1;
	if ( (cw_pulse_ctr>=((cw_setcnt_val*1000) - (rev_count_inc*1.02)) && CHECKBIT(flag_motor2,MOTOR_2_DIR_CW)) ||
	     (ccw_pulse_ctr>=((ccw_setcnt_val*1000) - (rev_count_inc*1.02)) && !CHECKBIT(flag_motor2,MOTOR_2_DIR_CW)) )
	{
		result=1;	
	}
	if (  ((rev_count_inc*2)>(cw_setcnt_val*1000) && CHECKBIT(flag_motor2,MOTOR_2_DIR_CW))
			|| ((rev_count_inc*2)>(ccw_setcnt_val*1000) && !CHECKBIT(flag_motor2,MOTOR_2_DIR_CW)) )
	{
		
		SETBIT(flag_switch, flag_stop);
		CLEARBIT(flag_switch, flag_start);
	}
	return result;
}
void motor_deceleration()
{
	if (  ( CHECKBIT(flag_switch, flag_start) && (ramp_OCR1A>ramp_inc_dec) && 
			( (cw_pulse_ctr<=(cw_setcnt_val*1000) && CHECKBIT(flag_motor2,MOTOR_2_DIR_CW)) ||  
			  (ccw_pulse_ctr<=(ccw_setcnt_val*1000) && !CHECKBIT(flag_motor2,MOTOR_2_DIR_CW)) ) )   
		  ||  ((CHECKBIT(flag_switch, flag_stop) && (ramp_OCR1A>ramp_inc_dec)) ) )
	{
		ramp_OCR1A-=ramp_inc_dec;
		OCR1A = (1/(ramp_OCR1A*.000008333))-1;
	}
	else if(CHECKBIT(flag_switch, flag_stop))
	{
		CLEARBIT(TCCR1B, CS11); // timer stopped
		flag_motor_1=MOTOR_1_IDLE;
		cw_pulse_ctr = 0;
		ccw_pulse_ctr = 0;
		rev_count_inc = 0;
		ramp_OCR1A = 0;
		cyccnt_ctr=0;
		WRITELOW(cw_port);
		WRITELOW(ccw_port);
		SETBIT(flag_motor2, MOTOR_2_DIR_CW);
		CLEARBIT(flag_switch, flag_param_saved);
		
	}
}


void motor_stop(uint32_t lapsed_time)
{
	if (flag_motor_1 != MOTOR_1_IDLE)
	{
		timer0_100ms += lapsed_time;
		if (timer0_100ms >= TIMER0_100MS)
		{
			timer0_100ms = 0;
			//if (motor_deceleration())
			{
				flag_motor_1 = MOTOR_1_IDLE;
				SETBIT(flag_motor2, MOTOR_2_DIR_CW);
				CLEARBIT(flag_switch, flag_param_saved);
				CLEARBIT(flag_motor2, MOTOR_2_100MS_DECEL);
			}
		}
	}
	else
	{
		cw_pulse_ctr = 0;
		ccw_pulse_ctr = 0;
		rev_count_inc = 0;
		ramp_OCR1A = 0;
		cyccnt_ctr=0;
		WRITELOW(cw_port);
		WRITELOW(ccw_port);
		SETBIT(flag_motor2, MOTOR_2_DIR_CW);
		CLEARBIT(flag_switch, flag_param_saved);
	}
}