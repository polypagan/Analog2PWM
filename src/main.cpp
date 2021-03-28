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
const uint8_t PWM = PB4;  // pin 3

void setup() {
//  pinMode(PWM, OUTPUT);
  DDRB |= (1 << PB4); // Set the pin PB4 as output
//  DDRB |= (1 << PB1); // Set MISO as output for testing
//  PORTB &= ~(1<<PB1); // And drive low.
//  pinMode(PB1, OUTPUT);
//  digitalWrite(PB1, LOW);

// fast PWM mode
  GTCCR = 1<<PWM1B | 3<<COM1B0;
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
            (1 << ADPS2) |     // set prescaler to 64, bit 2 
            (1 << ADPS1) |     // set prescaler to 64, bit 1 
            (0 << ADPS0);      // set prescaler to 64, bit 0  

  ADCSRA |= (1 << ADSC);         // start first conversion          
}

void loop() {
// read ADC, shift to 8 bits an output as PWM. 
//  analogWrite(PWM, (analogRead(A3) >> 2));
while (ADCSRA & (1 << ADSC)) {        // may need to wait here...
//    PORTB = 1<<PB1;                               // raise MISO flag while wait
//      digitalWrite(PB1, HIGH);
    }
//  PORTB &= ~(1<<PB1);                             // drop MISO,wait done.
//    digitalWrite(PB1, LOW);

  OCR1B = 255 - ADCH;
  ADCSRA |= (1 << ADSC);         // start next ADC conversion

}
/*
 * Possible refinements:
 * 
 * Run ADC in left-justified mode &
 * grab ADCH (8-bits) to pass to PWM.
 *  (saves right shift by 2): done.
 * 
 * Run ADC in timer mode (automatic acquisitions)
 * & read in each loop, or read, do PWM,
 * then start next conversion & exit loop().
 *  (hide SAR time behind execution)
 * 
 * Do read and PWM in ADCdone ISR.
 *  (ADC-driven timing)
 *  OCR1B = 255 - ADCH;
 * 
 * Do fast PWM:
 * // Configure counter/timer1 for fast PWM on PB4
 * GTCCR = 1<<PWM1B | 3<<COM1B0;
 * TCCR1 = 3<<COM1A0 | 7<<CS10;
 * then:
 * OCR1B = 255 - value;
 * done.
 * 
 */