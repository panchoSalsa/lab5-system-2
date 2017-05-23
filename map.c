#include <stdio.h>


int map(int adc, int low_adc, int high_adc, int low_frequency, int high_frequency);

main() {
	printf("Hello World\n");

	for (int i = 0; i < 1024; ++i) {
		printf("adc %d -> frequency %d\n", i, map(i,0,1023,7643,15295));
	}
	return 0; 
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