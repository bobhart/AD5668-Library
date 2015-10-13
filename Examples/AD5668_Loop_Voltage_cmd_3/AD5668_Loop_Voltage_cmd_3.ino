/********************************************************
 AD5668_Loop_Voltage_cmd_3
 Example sketch for AD5668 libray

 This sketch uses the function writeUpdateCh (command 3)
 to continuosly loop a cycle of 6 voltages on all outputs.
 The values written are 0V to the reference voltage in
 1/5 VRef increments.

 Any one of the outputs can be monitored through the
 Serial Monitor by connecting it to A0. The result in the
 Monitor will be the 10-bit ADC value of the voltage.
 Conversely, you can directly measure the results with a
 voltmeter. This sketch assumes that the vRef pin is
 connected to an external reference.
 
 Bob Hart, October 2, 2015
 ********************************************************/
#include "AD5668.h"
#include <SPI.h>

#define mosiPin 5
#define sclkPin 6
#define ssPin 7
#define clrPin 8
#define ldacPin 9

// These variables are for the A0 measurements, connect one of the DAC outputs
// to analog 0.
int analogPin = A0;
int dacOut = 0;
unsigned int vOut = 0;

/* 
 *  Hardware SPI instance, "AD5668(ssPin, clrPin, ldacPin);"
 *  ssPin is connected to AD5668 ~SYNC pin (pin 2), clrPin is connected to
 *  AD5668 ~CLR pin (9) and ldacPin is connected to AD5668 ~LDAC pin (1).
 *  Connect the AD5668's Din pin (15) to the Arduino's MOSI pin, and the SCLK
 *  pin (16) to the Arduino's SCK pin.
 */
AD5668 DAC = AD5668(ssPin, clrPin, ldacPin);

/* 
 *  Software SPI instance, "AD5668(mosiPin, sclkPin ,ssPin, clrPin, ldacPin);"
 *  mosiPin is connected to AD5668 Din pin (15) and sclkPin to AD5668 SCK
 *  pin (16). Remaining connections as explained above.
 */
//AD5668 DAC = AD5668(mosiPin, sclkPin ,ssPin, clrPin, ldacPin);

void setup() {
  Serial.begin(9600);
  // initialize the DAC
  DAC.init(0, B11111111); //initialize DAC with internal vRef off, power up all
}

void loop() {
  for (int v=0; v<=5; v++) {
    for (int chAdr=0; chAdr<=7; chAdr++) {
      if (v == 0) {
        vOut = 0;
      }
      vOut = (v * 13107);
      DAC.writeUpdateCh(chAdr, vOut);
    }
    dacOut = analogRead(analogPin);
    Serial.println(dacOut);
    delay(5000);
  }
}
