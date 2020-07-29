/*
 * thumbwheel.c
 *
 * Created: 8/27/2019 4:53:57 PM
 *  Author: Edwin
 */ 

//#include <stdlib.h>
#include <avr/io.h>
#include <math.h>
#include "main.h"
#include "atmel_io.h"
#include "avr035.h"
#include "init.h"
#include "key.h"
#include "motor.h"


/*
uint8_t OP_SPD_ARRAY_VAL[5];
uint8_t RAMP_TIME_ARRAY_VAL[3];
uint8_t DEAD_TIME_ARRAY_VAL[3];
uint8_t SETCNT_CW_ARRAY_VAL[5];
uint8_t SETCNT_CCW_ARRAY_VAL[5];
uint8_t CYCCNT_ARRAY_VAL[6];
*/



uint8_t BCD_VAL_A[11];
uint8_t BCD_VAL_B[11];

uint8_t BCD_val = 0;
uint8_t ctr1 = 0;
uint8_t ctr2 = 0;
uint32_t thumbw_deb_ctr=0;
uint32_t delay_ctr=0;

uint32_t _exp(uint8_t foo)
{
	uint32_t result=0;
	switch (foo)
	{
		case 0:
		result = 1;
		break;
		case 1:
		result = 10;
		break;
		case 2:
		result = 100;
		break;
		case 3:
		result = 1000;
		break;
		case 4:
		result = 10000;
		break;
		case 5:
		result = 100000;
		break;	
	}
	return result; 
}

void thumbwheel_read(uint32_t lapsed_time)
{
	if (CHECKBIT(flag_switch,flag_stop))
	{
		thumbw_deb_ctr += lapsed_time;
		if (thumbw_deb_ctr >= THUMBW_DEB)
		{
			thumbw_deb_ctr = 0;
			if (ctr1<8)
			{
				PORTD = 0xff;
				CLEARBIT(PORTD,ctr1);
				PORTC = 0xff;
				delay_ctr++;
				if (delay_ctr>=2)
				{
					delay_ctr=0;
					BCD_val = ~PINA;
					BCD_VAL_A[ctr1] = BCD_val & 0x0f;
					BCD_VAL_B[ctr1] = (BCD_val>>4);
					ctr1++;
				}
			
			}
			else if (ctr2 < 3)
			{
				PORTC = 0xff;
				CLEARBIT(PORTC,ctr2);
				PORTD = 0xff;
				delay_ctr++;
				if (delay_ctr>=2)
				{
					delay_ctr=0;
					BCD_val = ~PINA;
					BCD_VAL_A[ctr1+ctr2] = BCD_val & 0x0f;
					BCD_VAL_B[ctr1+ctr2] = (BCD_val>>4);
					ctr2++;
				}
			}
			else
			{
				ctr1=0;
				ctr2=0;
				CLEARBIT(flag_switch, MOTOR_1_PARAM_SET);
				op_spd_val_temp = 0;
				cw_setcnt_val_temp = 0;
				ccw_setcnt_val_temp = 0;
				cyc_cnt_val_temp = 0;
				for (int foo=0; foo<=11; foo++)
				{
					if (foo<=4)
					{
						op_spd_val_temp += (BCD_VAL_A[foo])*(_exp(foo));
						ccw_setcnt_val_temp += (BCD_VAL_B[foo])*(_exp(foo));
					}
					else
					{
						if (foo<=9)
						{
							cw_setcnt_val_temp += (BCD_VAL_A[foo])*(_exp(foo-5));
							cyc_cnt_val_temp += (BCD_VAL_B[foo])*(_exp(foo-5));
						}
						else
						cyc_cnt_val_temp += (BCD_VAL_B[foo])*(_exp(foo-5));
					}
				}
				SETBIT(flag_switch, MOTOR_1_PARAM_SET);
			}
		}
    }
}

uint32_t start_deb_ctr=0;
uint32_t stop_deb_ctr=0;

void switches_read(uint32_t lapsed_time)
{
	
	if (CHECKBIT(flag_switch,flag_stop))
	{
		if ((!READ(start_pin)) && (op_spd_val_temp!=0))   //if start pin is pressed and operating speed set is not zero
		{
			if (!CHECKBIT(flag_switch,flag_start))
			{
				start_deb_ctr += lapsed_time;
				if (start_deb_ctr >= SW_DEB)
				{
					SETBIT(flag_switch, flag_start);
					CLEARBIT(flag_switch, flag_stop);
					start_deb_ctr = 0;
				}
			}			
		}
		else start_deb_ctr = 0;		
	}	
	
	
	if (CHECKBIT(flag_switch,flag_start))
	{
		_NOP();
		if (!READ(stop_pin))   //if stop pin is pressed
		{
			if (!CHECKBIT(flag_switch,flag_stop))
			{
				stop_deb_ctr += lapsed_time;
				if (stop_deb_ctr >= SW_DEB)
				{
					SETBIT(flag_switch, flag_stop);
					CLEARBIT(flag_switch, flag_start);
					stop_deb_ctr = 0;
				}
			}
		}
		else stop_deb_ctr = 0;
	}
	
	
}