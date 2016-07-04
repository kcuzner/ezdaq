/**
 * AVR Compatiblity macros
 */
#ifndef _AVR_COMPAT_H_
#define _AVR_COMPAT_H_

#define _BV(N) (1<<N)

void avr_init(void);

void _delay_ms(uint16_t ms);

void _delay_us(uint16_t us);

extern volatile uint32_t us_ticks;

#endif //_AVR_COMPAT_H_
