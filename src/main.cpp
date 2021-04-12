#include <Arduino.h>

/*
 * Analog to PWM converter
 * 
 * DdelV 20210326
 * 
 * Convert Analog signal 
 * to PWM.
 * 
 * Pin Choices
 * 
 * Pin ADC PWM  PB  Other
 *  1   0       5   Reset
 *  2   3   Y   3
 *  3   2   Y   4
 *  5       Y   0   MOSI/SDA
 *  6       Y   1   MISO
 *  7   1       2   SCK/SCL
 */

// Analog In is pin 2 (ADC3)
// PWM out is pin 3

void setup() {
  DDRB |= (1 << PB4); // Set the pin PB4 as output for PWM

// fast PWM mode
  GTCCR = 1<<PWM1B | 2<<COM1B0; // was 3<<COm1b0
  TCCR1 = 3<<COM1A0 | 1<<CS10;    // was 7<<CS10;

  ADMUX =
            (1 << ADLAR) |     // left shift result
            (0 << REFS1) |     // Sets ref. voltage to VCC, bit 1
            (0 << REFS0) |     // Sets ref. voltage to VCC, bit 0
            (0 << MUX3)  |     // use ADC3 for input (PB3), MUX bit 3
            (0 << MUX2)  |     // use ADC3 for input (PB3), MUX bit 2
            (1 << MUX1)  |     // use ADC3 for input (PB3), MUX bit 1
            (1 << MUX0);       // use ADC3 for input (PB3), MUX bit 0

  ADCSRA = 
            (1 << ADEN)  |     // Enable ADC 
            (1 << ADATE) |
            (1 << ADIE)  |     // FOR FREE RUN
            (1 << ADPS2) |     // set prescaler to 64, bit 2 
            (1 << ADPS1) |     // set prescaler to 64, bit 1 
            (0 << ADPS0);      // set prescaler to 64, bit 0  

  ADCSRB = 0<<ADTS0;                       // Free running     

  ADCSRA |= (1 << ADSC);         // start first conversion

}

ISR(ADC_vect) {     // for free-run
// ADC conversion complete
  OCR1B = ADCH;
}

void loop() {
// Nothing to do. Wait here while all the action happens in ISR.
}
