/*
 * init.h
 *
 * Created: 8/27/2019 1:14:00 PM
 *  Author: Edwin
 */ 


#ifndef INIT_H_
#define INIT_H_

/* inputs */
#define start_pin	2,E		//pin 4
#define stop_pin	3,E		//pin 5
#define sensor_pin  4,E		//pin 6

#define BCD_1A	0,A		//pin 51
#define BCD_2A	1,A		//pin 50
#define BCD_4A	2,A		//pin 49
#define BCD_8A	3,A		//pin 48
#define BCD_1B	4,A		//pin 47
#define BCD_2B	5,A		//pin 46
#define BCD_4B	6,A		//pin 45
#define BCD_8B	7,A		//pin 44

/* outputs */

#define cw_port		5,B		//pin 15
#define ccw_port	6,B		//pin 16

#define DG1		0,D		//pin 25
#define DG2		1,D		//pin 26
#define DG3		2,D		//pin 27
#define DG4		3,D		//pin 28
#define DG5		4,D		//pin 29		
#define DG6		5,D		//pin 30
#define DG7		6,D		//pin 31
#define DG8		7,D		//pin 32
#define DG9		0,C		//pin 35
#define DG10	1,C		//pin 36
#define DG11	2,C		//pin 37

void init_timer();
void init_ports();

#endif /* INIT_H_ */