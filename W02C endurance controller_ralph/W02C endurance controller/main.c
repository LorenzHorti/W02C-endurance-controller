/*
 * W02C endurance controller.c
 *
 * Created: 8/27/2019 9:22:13 AM
 * Author : Edwin
 */ 

//#include <stdlib.h>
//#include <stdint.h>
//#include <stdint-gcc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "atmel_io.h"
#include "avr035.h"
#include "init.h"
#include "key.h"
#include "main.h"
#include "motor.h"

static uint16_t main_time_tick = 0;
static uint32_t main_ovf_ctr = 0;
static uint32_t main_time_curr = 0, main_time_prev = 0, main_time_diff = 0;
volatile uint16_t interrupts_timer_main_ovf_ctr, timer_main_ovf;
volatile uint8_t interrupts_timer_main_ovf_dec, timer_main_ovf_dec;

volatile uint32_t op_spd_val_temp, op_spd_val;
volatile uint32_t cw_setcnt_val_temp, cw_setcnt_val;
volatile uint32_t ccw_setcnt_val_temp, ccw_setcnt_val;
volatile uint32_t cyc_cnt_val_temp, cyc_cnt_val;

float ramp_time_val_temp, ramp_time_val;
float dead_time_val_temp, dead_time_val;

volatile uint8_t flag_switch;
volatile uint8_t flag_timer;
volatile uint32_t cw_preset_count = 0, ccw_preset_count = 0;
volatile uint32_t cw_pulse_ctr=0;
volatile uint32_t ccw_pulse_ctr=0;
volatile uint32_t cyccnt_ctr=0;

volatile uint32_t timer0_100ms = 0;
uint32_t ctr_100ms_int =0;

ISR(TIMER0_OVF_vect)
{
	timer_main_ovf++;
	timer_main_ovf -= timer_main_ovf_dec;
	timer_main_ovf_dec = 0;
	interrupts_timer_main_ovf_ctr++;
	interrupts_timer_main_ovf_ctr -= interrupts_timer_main_ovf_dec;
	interrupts_timer_main_ovf_dec = 0;
}

ISR(TIMER1_COMPA_vect)
{
 	if (CHECKBIT(flag_motor2, MOTOR_2_DIR_CW))
 	{
		TOGGLE_PIN(cw_port);
		cw_pulse_ctr++;
		if(cw_pulse_ctr>=(cw_setcnt_val*1000) && CHECKBIT(flag_switch, flag_start))
		{
			if (!CHECKBIT(PINB,5))	//checks if cw_port/ccw_port is low  
			{
				CLEARBIT(TCCR1B, CS11); // timer stopped
			}
		}
	}
 	else 
	{
		TOGGLE_PIN(ccw_port);
		ccw_pulse_ctr++;
		if(ccw_pulse_ctr>=(ccw_setcnt_val*1000) && CHECKBIT(flag_switch, flag_start))
		{
			if(!CHECKBIT(PINB,6))   //checks if ccw_port/ccw_port is low  
			{
				CLEARBIT(TCCR1B, CS11); // timer stopped
			}
		}
	}
	
}

int main(void)
{
 
 init_ports();
 init_timer();
 
 PORTD = 0xff;
 PORTC = 0xff;
 SETBIT(flag_switch, flag_stop);
 CLEARBIT(flag_switch, flag_start);
 SETBIT(flag_motor2, MOTOR_2_DIR_CW);
 flag_motor_1 = MOTOR_1_IDLE;
 sei();
  
 /* test values  */
/*
 op_spd_val_temp = 650;  //rpm
 cw_setcnt_val_temp = 200;
 ccw_setcnt_val_temp = 200;
 cyc_cnt_val_temp = 200;
 ramp_time_val_temp = 1;	// sec*/

 dead_time_val_temp = 0.2;  // sec

 
    while (1) 
    {	
		main_time_tick = TCNT0;
		if((main_time_prev > main_time_tick) && (!interrupts_timer_main_ovf_ctr))
		{
			interrupts_timer_main_ovf_dec++;
			main_ovf_ctr = interrupts_timer_main_ovf_dec+interrupts_timer_main_ovf_ctr;
		}
		else
		{
			main_ovf_ctr = interrupts_timer_main_ovf_ctr;
			interrupts_timer_main_ovf_ctr -= main_ovf_ctr;
		}
		main_time_curr = (main_ovf_ctr*INTERRUPTS_TIMER_MAIN_OCR)+main_time_tick;
		main_time_diff = main_time_curr - main_time_prev;
		main_time_prev = main_time_tick;
	
		thumbwheel_read(main_time_diff);
		switches_read(main_time_diff);		
		if (CHECKBIT(flag_switch, flag_start))               //if start button is pressed
		{
			switch (flag_motor_1)
			{
				case MOTOR_1_IDLE:
					if (!CHECKBIT(flag_switch, flag_param_saved))
					{
						flag_motor_1 = MOTOR_1_PARAM_SET;
					}
					else
					{
						timer0_100ms+= main_time_diff;
						if (timer0_100ms >= dead_time_val*TIMER0_100MS*10)
						{
							timer0_100ms = 0;
							flag_motor_1 = MOTOR_1_ACCEL;
						}
					}
					break;
				case MOTOR_1_PARAM_SET:
					motor_set_param();
					SETBIT(flag_switch, flag_param_saved);
					//if ((cw_setcnt_val<=0) && (ccw_setcnt_val<=0))
					//{
						//motor_stop(main_time_diff);
					//}
					//else
					 flag_motor_1 = MOTOR_1_ACCEL;
				break;
				case MOTOR_1_ACCEL:
					timer0_100ms+=main_time_diff;
					if (timer0_100ms >= TIMER0_100MS)
					{
						timer0_100ms = 0;
						motor_acceleration();

						if ( (cw_pulse_ctr>=(cw_setcnt_val_temp*1000) && CHECKBIT(flag_motor2,MOTOR_2_DIR_CW)) ||
						(ccw_pulse_ctr>=(ccw_setcnt_val_temp*1000) && !CHECKBIT(flag_motor2,MOTOR_2_DIR_CW)))
						{
							SETBIT(flag_switch, flag_stop);
							CLEARBIT(flag_switch, flag_start);
							cyccnt_ctr=0;
						}

					}
				break;
				case MOTOR_1_OPRTNG_SPD:
					if (motor_operating_spd())
					{
						flag_motor_1 = MOTOR_1_DECEL;
					}
				break;
				case MOTOR_1_DECEL:
					timer0_100ms+=main_time_diff;
					if (timer0_100ms >= TIMER0_100MS)
					{
						timer0_100ms = 0;
						motor_deceleration();
						if(!CHECKBIT(TCCR1B, CS11)) // if timer is stopped
						{
							flag_motor_1=MOTOR_1_IDLE;
							cw_pulse_ctr = 0;
							ccw_pulse_ctr = 0;
							rev_count_inc = 0;
							ramp_OCR1A = 0;
							if (ccw_setcnt_val == 0)	// if ccw value is zero
							{
								cyccnt_ctr++;
								if (cyccnt_ctr>= cyc_cnt_val)
								{
									cyccnt_ctr=0;
									SETBIT(flag_switch, flag_stop);
									CLEARBIT(flag_switch, flag_start);
									CLEARBIT(flag_switch, flag_param_saved);
									SETBIT(flag_motor2, MOTOR_2_DIR_CW);	
								}
							}
							else
							{
								if (!CHECKBIT(flag_motor2, MOTOR_2_DIR_CW))
								{
									cyccnt_ctr++;
									if (cyccnt_ctr>= cyc_cnt_val)
									{
										cyccnt_ctr=0;
										SETBIT(flag_switch, flag_stop);
										CLEARBIT(flag_switch, flag_start);
										CLEARBIT(flag_switch, flag_param_saved);
										SETBIT(flag_motor2, MOTOR_2_DIR_CW);
									}
								}
								FLIPBIT(flag_motor2, MOTOR_2_DIR_CW);
							}
						}
					}
				break;
			}
		}
		
		
		
		if (CHECKBIT(flag_switch, flag_stop))		//if stop button is pressed
		{
			if (flag_motor_1 != MOTOR_1_IDLE)
			{
				timer0_100ms += main_time_diff;
				if (timer0_100ms >= TIMER0_100MS)
				{
					timer0_100ms = 0;
					motor_deceleration();
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
    }
}



