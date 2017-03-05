

# AVR high-voltage serial programming for ATtiny

## Bricked your MCU?

Changing [AVR fuse values](http://www.engbedded.com/fusecalc/) in an
unintended way can render the microcontroller practically dead.  For
example you might have altered clock source bits in a way that the
processor cannot be programmed anymore.  Trying to fix the problem by
applying default fuse values fails:

    $ avrdude -p attiny85 -c usbasp -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

    avrdude: warning: cannot set sck period. please check for usbasp firmware update.
    avrdude: error: program enable: target doesn't answer. 1
    avrdude: initialization failed, rc=-1
             Double check connections and try again, or use -F to override
             this check.

    avrdude done.  Thank you.


Even using slower bitclock does not necessarily work anymore:


    $ avrdude -p attiny85 -c usbasp -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m -B1000 -F

    avrdude: set SCK frequency to 1000 Hz
    avrdude: warning: cannot set sck period. please check for usbasp firmware update.
    avrdude: error: program enable: target doesn't answer. 1
    avrdude: initialization failed, rc=-1
    avrdude: AVR device initialized and ready to accept instructions
    avrdude: Device signature = 0x88fb38
    avrdude: Expected signature for ATtiny85 is 1E 93 0B

    avrdude done.  Thank you.


## High-voltage serial programming to the help

Atmel
[ATtiny datasheet](http://www.atmel.com/images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf)
chapter "High-voltage Serial Programming" describes alternative way of
programming ATtiny microcontroller.  With this mechanism it is
possible to bring the MCU back to life.

The code in this project uses Arduino Nano to re-program ATtiny85
microcontroller fuses to default values using the HVSP method.  Normal
serial programming method can be used again after running the program.


## Schematic

Following schema describes the circuit for running the programmer.
The circuit requires 12V voltage source for applying 12V to RESET pin
of ATtiny85.

![schema](http://i.imgur.com/vxteam4.png)
