/*
 * DoAn2.c
 *
 * Created: 18/6/2023 5:04:55 PM
 * Author : Kiet Nguyen
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "LCD_16x2_H_file.h"

#define degree_sysmbol 0xdf

void ADC_Init(){
	DDRA = 0x00;	        /* Make ADC port as input */
	ADCSRA = 0x87;          /* Enable ADC, with freq/128  */
	ADMUX = 0x40;           /* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)
{
	ADMUX = 0x40 | (channel & 0x07);   /* set input channel to read */
	ADCSRA |= (1<<ADSC);               /* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));     /* Wait until end of conversion by polling ADC interrupt flag */
	ADCSRA |= (1<<ADIF);               /* Clear interrupt flag */
	_delay_ms(1);                      /* Wait a little bit */
	return ADCW;                       /* Return ADC word */
}

int main()
{
	char Temperature[10];
	float Temp;

	LCD_Init();                 /* initialize 16x2 LCD*/
	ADC_Init();                 /* initialize ADC*/
	//DDRA |= 0xFF;
	//PORTA |= 0xFF;
	LCD_Clear();
	LCD_String_xy(1, 0, "Temperature:");
	
	while(1)
	{
		_delay_ms(1000);
		Temp = (float)(ADC_Read(7) * 4.8828);
		Temp = (float)(Temp / 10);
		sprintf(Temperature, "%0.1f%cC", (float)Temp, degree_sysmbol);
		LCD_String_xy(2, 0, Temperature);
		_delay_ms(500);
	}
	return 0;
}

