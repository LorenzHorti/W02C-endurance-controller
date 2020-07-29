/*
 * key.h
 *
 * Created: 8/27/2019 4:54:15 PM
 *  Author: Edwin
 */ 


#ifndef KEY_H_
#define KEY_H_

/*
extern uint8_t OP_SPD_ARRAY_VAL[5];
extern uint8_t RAMP_TIME_ARRAY_VAL[3];
extern uint8_t DEAD_TIME_ARRAY_VAL[3];
extern uint8_t SETCNT_CW_ARRAY_VAL[5];
extern uint8_t SETCNT_CCW_ARRAY_VAL[5];
extern uint8_t CYCCNT_ARRAY_VAL[6];
*/


#define SW_DEB  20000		//10 ms
#define THUMBW_DEB  20000	//10 ms

void thumbwheel_read(uint32_t lapsed_time);
void switches_read(uint32_t lapsed_time);
uint32_t _exp(uint8_t foo);

#endif /* KEY_H_ */