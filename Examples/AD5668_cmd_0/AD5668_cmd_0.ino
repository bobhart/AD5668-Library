/********************************************************
 AD5668_cmd_0
 Example sketch for AD5668 libray

 This sketch uses the function writeChannel (command 0)
 to sequentially load all 8 input registers with a value
 and then loads all output registers at once by toggling
 the ~LDAC pin. The values written cycle from 0V to the
 reference voltage in 1/5 VRef increments. The sequence
 repeats a second time, but demonstrates writing to all
 channels at once using address 0b1111 (15}.

 Any one of the outputs can be monitored through the
 Serial Monitor by connecting it to A0. The result in the
 Monitor will be the 10-bit ADC value of the voltage.
 Conversely, you can directly measure the results with a
 voltmeter.
 
 Bob Hart, October 2, 2015
 ********************************************************/
#include "AD5668.h"
#include <SPI.h>

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
AD5668 DAC = AD5668(7, 8, 9);

/* 
 *  Software SPI instance, "AD5668(mosiPin, sclkPin ,ssPin, clrPin, ldacPin);"
 *  mosiPin is connected to AD5668 Din pin (15) and sclkPin to AD5668 SCK
 *  pin (16). Remaining connections as explained above.
 */
//AD5668 DAC = AD5668(5, 6, 7, 8, 9);

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
      DAC.writeChannel(chAdr, vOut);
    }
    DAC.toggleLDAC();
    dacOut = analogRead(analogPin);
    Serial.println(dacOut);
    delay(5000);
  }
  for (int v=0; v<=5; v++) {
    if (v == 0) {
      vOut = 0;
    }
    vOut = (v * 13107);
    DAC.writeChannel(15, vOut);
    DAC.toggleLDAC();
    dacOut = analogRead(analogPin);
    Serial.println(dacOut);
    delay(5000);
  }
}
