# Analog2PWM
ATtiny85 converts analog in (0-Vcc) to PWM.

In response to a request on r/AskElectronics

Single-chip (+ bypass cap) solution.
This application uses a tiny fraction of the resources available and does the job quite well for less than USD 2.

Analog input must not exceed Vcc (5v max, although could be run on lower voltages, see datasheet)
PWM output is 8-bit, so only accurate to 1:255 or about 0.4%.

ATtiny85 fuses set to l:0xe2 h:0xdf e:0xff
(8 MHz internal clk, no BOD) -- Many other possibilities.
