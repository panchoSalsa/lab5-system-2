#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

//Pizo Speaker connected to PortC pin PC1

#define SPEAKER_PORT	PORTC
#define SPEAKER_DDR  	DDRC
#define SPEAKER_PIN  	1

void PLAYNOTE(float duration, float frequency);
void delay_ms(int d);

int main(void)
{

	while(1) {
		PLAYNOTE(400,880);
		PLAYNOTE(400,932);
		PLAYNOTE(400,988);
		PLAYNOTE(400,1047);
		PLAYNOTE(400,1109);
		PLAYNOTE(400,1175);
		PLAYNOTE(400,1244);
		PLAYNOTE(400,1319);
		PLAYNOTE(400,1397);
		PLAYNOTE(400,1480);
		PLAYNOTE(400,1568);
		PLAYNOTE(400,1660);
	}

}



void PLAYNOTE(float duration, float frequency)
{

	long int i,cycles;
	float half_period;
	float wavelength;
	
	wavelength=(1/frequency)*1000;
	cycles=duration/wavelength;
	half_period = wavelength/2;

	
	SPEAKER_DDR |= (1 << SPEAKER_PIN);

	for (i=0;i<cycles;i++)
	{
/*		_delay_ms(half_period);*/
		delay_ms(half_period);
		SPEAKER_PORT |= (1 << SPEAKER_PIN);
		/*_delay_ms(half_period);*/
		delay_ms(half_period);
		SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
	}

	return;
}

// source=http://www.avrfreaks.net/forum/error-1-builtinavrdelaycycles-expects-integer-constant
void delay_ms(int d)
{
	int i;
	for (i = 0; i < d; i++) {
		_delay_ms(1);
	}
}