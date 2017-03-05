//
// BSD 3-Clause License
//
// Copyright (c) 2017, Tero Saarni
//
//
// This software re-programs ATtiny85 fuses using the high-voltage
// serial programming method.
//
//
// References
//
//   * ATtiny85 datasheet -
//     http://www.atmel.com/images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf
//
//   * Online fuse value calculator -
//     http://www.engbedded.com/fusecalc/
//
//
// High-voltaga serial programming instruction set (from ATtiny85 datasheet)
//
//   Write Fuse Low Bits
//
//         Instr.1/5        Instr.2/6        Instr.3          Instr.4          Operation remark
//   SDI   0_0100_0000_00   0_A987_6543_00   0_0000_0000_00   0_0000_0000_00   Wait after Instr. 4 until SDO
//   SII   0_0100_1100_00   0_0010_1100_00   0_0110_0100_00   0_0110_1100_00   goes high. Write A - 3 = “0” to
//   SDO   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   program the Fuse bit.
//
//   Write Fuse High Bits
//
//         Instr.1/5        Instr.2/6        Instr.3          Instr.4          Operation remark
//   SDI   0_0100_0000_00   0_IHGF_EDCB_00   0_0000_0000_00   0_0000_0000_00   Wait after Instr. 4 until SDO
//   SII   0_0100_1100_00   0_0010_1100_00   0_0111_0100_00   0_0111_1100_00   goes high. Write I - B = “0” to
//   SDO   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   program the Fuse bit.
//
//   Write Fuse Extended Bits
//
//         Instr.1/5        Instr.2/6        Instr.3          Instr.4          Operation remark
//   SDI   0_0100_0000_00   0_0000_000J_00   0_0000_0000_00   0_0000_0000_00   Wait after Instr. 4 until SDO
//   SII   0_0100_1100_00   0_0010_1100_00   0_0110_0110_00   0_0110_1110_00   goes high. Write J = “0” to
//   SDO   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   x_xxxx_xxxx_xx   program the Fuse bit.
//

#include <Arduino.h>


// ATtiny85 pin mapaping at Arduino nano programmer
#define SDI_PIN 9     // serial data input
#define SII_PIN 10    // serial instruction input
#define SDO_PIN 11    // serial data output
#define SCI_PIN 12    // serial clock input
#define RESET_PIN 13  // reset


void avr_hvsp_write(uint8_t sdi, uint8_t sii)
{
   digitalWrite(SDI_PIN, LOW);
   digitalWrite(SII_PIN, LOW);
   digitalWrite(SCI_PIN, HIGH);
   digitalWrite(SCI_PIN, LOW);

   for (uint8_t i = 0; i < 8; i++)
   {
      digitalWrite(SDI_PIN, (sdi & (1 << (7-i))));
      digitalWrite(SII_PIN, (sii & (1 << (7-i))));
      digitalWrite(SCI_PIN, HIGH);
      digitalWrite(SCI_PIN, LOW);
   }

   digitalWrite(SDI_PIN, LOW);
   digitalWrite(SII_PIN, LOW);
   digitalWrite(SCI_PIN, HIGH);
   digitalWrite(SCI_PIN, LOW);
   digitalWrite(SCI_PIN, HIGH);
   digitalWrite(SCI_PIN, LOW);
}

void avr_hvsp_write_lfuse(uint8_t lfuse)
{
   avr_hvsp_write(0b01000000, 0b01001100);
   avr_hvsp_write(lfuse,      0b00101100);
   avr_hvsp_write(0b00000000, 0b01100100);
   avr_hvsp_write(0b00000000, 0b01101100);
   while (digitalRead(SDO_PIN) == LOW) { };
}

void avr_hvsp_write_hfuse(uint8_t hfuse)
{
   avr_hvsp_write(0b01000000, 0b01001100);
   avr_hvsp_write(hfuse,      0b00101100);
   avr_hvsp_write(0b00000000, 0b01110100);
   avr_hvsp_write(0b00000000, 0b01111100);
   while (digitalRead(SDO_PIN) == LOW) { };
}

void avr_hvsp_write_efuse(uint8_t efuse)
{
   avr_hvsp_write(0b01000000, 0b01001100);
   avr_hvsp_write(efuse,      0b00101100);
   avr_hvsp_write(0b00000000, 0b01100110);
   avr_hvsp_write(0b00000000, 0b01101110);
   while (digitalRead(SDO_PIN) == LOW) { };
}


void setup()
{
   Serial.begin(9600);

   // setup pin modes
   pinMode(SDI_PIN, OUTPUT);
   pinMode(SII_PIN, OUTPUT);
   pinMode(SDO_PIN, OUTPUT);
   pinMode(SCI_PIN, OUTPUT);
   pinMode(RESET_PIN, OUTPUT);

   // enter programming mode
   digitalWrite(SDI_PIN, LOW);
   digitalWrite(SII_PIN, LOW);
   digitalWrite(SDO_PIN, LOW);
   digitalWrite(RESET_PIN, HIGH); // pull attiny85 reset pin to ground
   delayMicroseconds(20);
   digitalWrite(RESET_PIN, LOW);  // attiny85 reset pin to 12V
   delayMicroseconds(10);
   pinMode(SDO_PIN, INPUT);
   delayMicroseconds(300);

   // write default fuse values
   avr_hvsp_write_lfuse(0x62);
   avr_hvsp_write_hfuse(0xdf);
   avr_hvsp_write_efuse(0xff);

   // exit programming mode
   digitalWrite(RESET_PIN, HIGH);  // pull attiny85 reset pin to ground (arduino nano LED will be on)
}


void loop()
{
   // nothing to do here

   Serial.print("Programming done");
   while (true)
   {
      delay(1000);
      Serial.print(".");
   }
}
