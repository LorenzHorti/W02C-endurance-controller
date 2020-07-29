/*
 * avr035.h
 *
 * Created: 8/27/2019 12:02:54 PM
 *  Author: Edwin
 */ 


#ifndef AVR035_H_
#define AVR035_H_

/**	write a logic 1 to BIT of ADDRESS*/
#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
/**	write a logic 0 to BIT of ADDRESS*/
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
/**	Toggle state of BIT of ADDRESS*/
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
/**	Read BIT of ADDRESS*/
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))

#if 0
#define SETBITMASK(x,y) (x |= (y))
#define CLEARBITMASK(x,y) (x &= (~y))
#define FLIPBITMASK(x,y) (x ^= (y))
#define CHECKBITMASK(x,y) (x & (y))

#define VARFROMCOMB(x, y) x
#define BITFROMCOMB(x, y) y

#define C_SETBIT(comb) SETBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_CLEARBIT(comb) CLEARBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_FLIPBIT(comb) FLIPBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_CHECKBIT(comb) CHECKBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#endif



#endif /* AVR035_H_ */