


#ifndef ATMEL_IO_H_
#define ATMEL_IO_H_

/* Macro function to declare an output pin */
#define DDR_OUT(x)			_out(x)
#define _out(bit,port)	DDR##port |= (1 << bit)

/* Macro function to declare an input pin */
#define DDR_IN(x)			_in(x)
#define _in(bit,port)	DDR##port &= ~(1 << bit)

/* Macro function to set an output pin high */
#define WRITEHIGH(x)			_on(x)
#define _on(bit,port)	PORT##port |= (1 << bit)

/* Macro function to set an output pin low */
#define WRITELOW(x)			_off(x)
#define _off(bit,port)	PORT##port &= ~(1 << bit)

/* Macro function to toggle an output pin */
#define TOGGLE_PIN(x)			_flip(x)
#define _flip(bit,port)	PORT##port ^= (1 << bit)

/* Macro function to get state of input pin */
#define READ(x)			_get(x)
#define _get(bit,port)	(PIN##port & (1 << bit))

#define READOUT(x)			_getout(x)
#define _getout(bit,port)	(PORT##port & (1 << bit))

#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)

#define LOW 0
#define HIGH !LOW



#endif /* ATMEL_IO_H_ */