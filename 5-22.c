// reading adc value
// source=http://maxembedded.com/2011/06/the-adc-of-the-avr/
// source=https://hekilledmywire.wordpress.com/2011/03/16/using-the-adc-tutorial-part-5/
#define F_CPU 16000000UL    // Clock Speed

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 9600           // Baud Speed
#define MYUBRR F_CPU/16/BAUD-1

void UART_Init( unsigned int ubrr);
void UART_Transmit( unsigned char data );
unsigned char UART_Receive();
void PutStr(char* s);

void initADC();
uint16_t getADCResult();

void initPWM();

void InitTimer1();

int map(int adc, int low_adc, int high_adc, int low_frequency, int high_frequency);

int top;

int main(void)
{
	InitTimer1();
	initADC();
	initPWM();
	UART_Init(MYUBRR);

	while (1) {
	}
}

void UART_Init( unsigned int ubrr) {
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// Set frame format: 8data, 2stop bit
	//UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	
	// Set frame format: 8data, 1stop bit
	UCSR0C = 3<<UCSZ00;
}

void UART_Transmit( unsigned char data ) {
	/* Wait for empty transmit buffer */

	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char UART_Receive() {
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

void PutStr(char* s) {
	while(*s)
	{
		UART_Transmit(*s++);
	}
}

void initADC() {
	DDRC &= 0x00; // set entire C Port Pins to inputs
	
	// (1 << MUX0) input voltage at ADC1; Pin PC1
	// (1 << REFS0) set power source V5+
	ADMUX = ( 1 << MUX0) | (1 << REFS0);
	
	// (1 << ADEN) enables the adc circuitry
	// (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) prescalar set to 128 (104us per conversion)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// do initial conversion
	ADCSRA |= (1<<ADSC);
}

uint16_t getADCResult() {
	// start adc conversion
	ADCSRA |= (1 << ADSC);
	
	// wait for conversion to complete
	while(ADCSRA & 1 << ADSC);

	// ADCW reads value from both ADCH and ADCL
	return (ADCW);
}

// source=http://www.nerdkits.com/forum/thread/190/
void initPWM() {
	DDRB |= (1 << PB1)

	// set none-inverting mode
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1);

	// set Fast PWM mode using ICR1 as TOP
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
}


void InitTimer1(){
	// set up timer with prescaler = 8
	TCCR1B |= (1 << CS11);
	
	// enable global interrupts
	sei();
	// enable overflow interrupt
	TIMSK1 |= (1 << TOIE1);
}


// TIMER1 overflow interrupt service routine
// called whenever TCNT1 overflows
// source=http://maxembedded.com/2011/06/avr-timers-timer1/
ISR(TIMER1_OVF_vect) {
	uint16_t adc = getADCResult();
	adc = getADCResult();

	// OCR1A acts as a single 16-bit integer value, and it includes an implicit cast to that int type
	top = map(adc,0,1023,7643,15295); 

	OCR1A = top - 7000;
	ICR1 = top;
}


// TOP must 7643 to produce a frequency of 2093hz
// TOP must 15295 to produce a frequency of 15295hz
// source=https://sites.google.com/site/qeewiki/books/avr-guide/pwm-on-the-atmega328
// PWM_fequency = clock_speed / [Prescaller_value * (1 + TOP_Value) ]
// clock_speed = 16Mhz
// Prescaller_value = 1


// map(adc,0,1023,7643,15295)
// source=https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another
int map(int adc, int low_adc, int high_adc, int low_frequency, int high_frequency) {
	return (adc - low_adc) * (high_frequency - low_frequency) / (high_adc - low_adc) + low_frequency;
}