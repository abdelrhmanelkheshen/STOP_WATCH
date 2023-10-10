/*
 * main.c
 *
 *  Created on: Apr 11, 2023
 *      Author: Abdelrhman Hesham
 */
#include<avr/io.h>
#include<util\delay.h>
#include<avr/interrupt.h>

/*Global Variables to present the time*/

char Secs_U = 0;
char Secs_T=0;
char Mins_U=0;
char Mins_T=0;
char Hours_U=0;
char Hours_T = 0;

/****************************************/

unsigned char tick = 0;

/*TIMER1 Initiation Function*/

void TIMER1_INIT(void)
{
	TCCR1A |= (1<<FOC1A) ; // Using TIMER1A in NO PWM MODE
	TCCR1B |= (1<<WGM12) | (1<<CS10) | (1<<CS12); // Setting Compare mode and pre-scaler value
	TCNT1 = 0; // Timer initial value is zero
	OCR1A = 1000; // compare match value which interrupt occur when timer reaches 1000 count
	TIMSK |= (1<<OCIE1A); // Enabling timer interrupt
	//TIFR |= (1<<OCF1A);
}

/*TIMER1 Stopping Function*/

void TIMER1_OFF(void)
{
	TCCR1B &= ~(1<<CS10);// Closing the clock means stopping the timer
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS12);
}

/*TIMER1 Resuming Function*/

void TIMER1_ON(void)
{
	TCCR1B |= (1<<CS10);
	TCCR1B |= (1<<CS12); // Turning the clock on once more

}

/*External Interrupt0 Initiation Function*/

void INT0_INIT(void)
{

	DDRD &= ~(1<<PD2); // PIN2 in PORTD I/P
	PORTD |= (1<<PORTD2);// Enabling internal PULL-UP Resistance
	MCUCR |= (1<<ISC01);// Enabling the Interrupt Bit for INT0
	GICR |= (1<<INT0);// Enabling External Interrupts
}

/*External Interrupt1 Initiation Function*/

void INT1_INIT(void)
{
	DDRD &= ~(1<<PD3);// PIN3 in PORTD I/P
	MCUCR |= (1<<ISC10) | (1<<ISC11);// Enabling the Interrupt Bit for INT1
	GICR |= (1<<INT1);
}

/*External Interrupt1 Initiation Function*/

void INT2_INIT(void)
{
	DDRB &= ~(1<<PB2);//PIN2 in PORTB I/P
	PORTB |= (1<<PORTB2);//Enabling internal PULL-UP Resistance
	MCUCSR &= ~(1<<ISC2);// Enabling the Interrupt Bit for INT0
	GICR |= (1<<INT2);
}

/*Interrupt Service Routine Function OR Interrupt0 Handler*/
/*Resetting the time by making the variables zeroes*/

ISR(INT0_vect)
{

	Secs_U = 0;
	Secs_T=0;
	Mins_U=0;
	Mins_T=0;
	Hours_U=0;
	Hours_T = 0;

}

/*Interrupt Service Routine Function OR Interrupt1 Handler*/
/*Pausing the time by disabling the clock*/

ISR(INT1_vect)
{
	TIMER1_OFF();
}

/*Interrupt Service Routine Function OR Interrupt2 Handler*/
/*Resuming the time by enabling the clock*/

ISR(INT2_vect)
{
	TIMER1_ON();

}

/*Interrupt Service Routine Function OR Timer1 Interrupt Handler*/
/*Incrementing the clock each second*/

ISR(TIMER1_COMPA_vect)
{
	tick ++;
	if (tick == 1)
	{
		Secs_U ++;
		if (Secs_U == 10)
		{
			Secs_U = 0;
			Secs_T ++;
		}
		if((Secs_T == 6) && (Secs_U == 0))
		{
			Secs_U = 0;
			Secs_T = 0;
			Mins_U ++;
		}
		if (Mins_U == 10)
		{
			Mins_U = 0;
			Mins_T ++;
		}
		if((Mins_T == 6) && (Mins_U == 0))
		{
			Mins_U = 0;
			Mins_T = 0;
			Hours_U ++;
		}
		if (Hours_U == 10)
		{
			Hours_U = 0;
			Hours_T ++;
		}
		tick = 0;
	}

}

/*Displaying Functions*/

void Display_Secs_U(char x)
{
	PORTA = (1<<0);
	PORTC = (PORTC & 0xF0)|(x & 0x0F);
}

void Display_Secs_T(char y)
{
	PORTA = (1<<1);
	PORTC = (PORTC & 0xF0)|(y & 0x0F);
}

void Display_Mins_U(char z)
{
	PORTA = (1<<2);
	PORTC = (PORTC & 0xF0)|(z & 0x0F);
}

void Display_Mins_T(char a)
{
	PORTA = (1<<3);
	PORTC = (PORTC & 0xF0)|(a & 0x0F);
}

void Display_Hours_U(char b)
{
	PORTA = (1<<4);
	PORTC = (PORTC & 0xF0)|(b & 0x0F);
}

void Display_Hours_T(char c)
{
	PORTA = (1<<5);
	PORTC = (PORTC & 0xF0)|(c & 0x0F);
}



int main(void)
{

	DDRA = 0x3F;
	DDRC = 0x0F;
	SREG |= (1<<7);//Enabling I-BIT
	TIMER1_INIT();
	INT0_INIT();
	INT1_INIT();
	INT2_INIT();
	for(;;)
	{
		Display_Secs_U(Secs_U);
		_delay_ms(2);
		Display_Secs_T(Secs_T);
		_delay_ms(2);
		Display_Mins_U(Mins_U);
		_delay_ms(2);
		Display_Mins_T(Mins_T);
		_delay_ms(2);
		Display_Hours_U(Hours_U);
		_delay_ms(2);
		Display_Hours_T(Hours_T);
		_delay_ms(2);
	}
}
