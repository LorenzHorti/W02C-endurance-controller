/*
 * init.c
 *
 * Created: 8/27/2019 1:14:13 PM
 *  Author: Edwin
 */ 

#include <avr/io.h>
#include "init.h"
#include "atmel_io.h"
#include "avr035.h"

void init_ports()
{
	/***** outputs *****/
	
	DDRD = 0xFF;			//com ports for thumb wheel switches
	DDRC = 0xFF;			//com ports for thumb wheel switches
	
	//output ports to motor driver
	DDR_OUT(cw_port);		//for clockwise motor control
	DDR_OUT(ccw_port);		//for counter-clockwise motor control
	
	/***** inputs *****/		
	DDRA = 0x00;			//input pins for BCD data
	DDR_IN(start_pin);		//for start push button	
	DDR_IN(stop_pin);		//for stop push button
	DDR_IN(sensor_pin);		//for photo-int/encoder 
	
	WRITEHIGH(stop_pin);	//enable internal pull ups
	WRITEHIGH(start_pin);
	PORTA = 0xFF;			//ports for thumbwheel switches
}

void init_timer()
{
	/* prescale = 8
	timer1_ticks= cpu_clk/prescale 
				= (16MHz/8) 	
				= 2MHz or 0.5us		
	timer1_ovf	= 32.768ms					  
	*/
	TCCR1A = 0;
	TCCR1B |= (1<<WGM12);	//CTC
	TIMSK1 |= (1<<OCIE1A);  //enable timer 1 interrupt	
	//TIMSK1 |= (1<<OCIE1A);  
	OCR1A = 0;
			
	/*timer0_ticks = 500ns		
	timer0_oc_ovf   = 128us		
	*/
	TCCR0A |= (1<<CS01);	 // prescale=8
	TIMSK0 |= (1<<TOIE0);
}

