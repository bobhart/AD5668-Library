/********************************************************
 AD5668_SoftLDAC
 Example sketch for AD5668 libray

 This sketch sets the Soft ~LDAC bits on for channels A -
 D, then writes a full-scale value to all channels,
 showing how channels A - D are updated immediately. The
 remaining channels are updated using other commands. The
 results at each step are read at the Arduino A0 - A7 pins
 and displayed in the Serial Monitor.

 Wire the DAC's output channels A - H to the Arduino's
 ADC inputs A0 - A7 respectively, and open the Serial
 Monitor after uploading to see the results. This sketch
 assumes that the vRef pin is connected to an external
 reference.

      **********************************************
      *                                            *
      * WARNING FOR DUE AND OTHER 3.3V CONTROLLERS *
      *                                            *
      **********************************************

 Do NOT connect the output of these chips to the analog
 inputs of any of the 3.3V uControllers, like the DUE, if
 using a reference voltage higher than 3.3V for the DAC.
 Applying voltages higher than 3.3V to these inputs will
 probably damage the inputs and may distroy the board.U

 Bob Hart, October 16, 2015
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
int dacAOut = 0;
int dacBOut = 0;
int dacCOut = 0;
int dacDOut = 0;
int dacEOut = 0;
int dacFOut = 0;
int dacGOut = 0;
int dacHOut = 0;

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
  DAC.init();

    //DAC.enableInternalRef(); // Uncomment this line to turn on the internal reference.
  DAC.powerDAC_Normal(B11111111); // Power up all channels normal

  DAC.setSoftLDAC(B00001111);
  showOutput();
  delay(5000);
  DAC.writeChannel(15, 65535);
  showOutput();
  delay(5000);
  DAC.updateChannel(4);
  showOutput();
  delay(5000);
  DAC.writeChUpdateAll(5, 32767);
  showOutput();
}

void loop() {

}

void showOutput() {
  dacAOut = analogRead(A0);
  dacBOut = analogRead(A1);
  dacCOut = analogRead(A2);
  dacDOut = analogRead(A3);
  dacEOut = analogRead(A4);
  dacFOut = analogRead(A5);
  dacGOut = analogRead(A6);
  dacHOut = analogRead(A7);
  Serial.print("Channel A = ");
  Serial.println(dacAOut);
  Serial.print("Channel B = ");
  Serial.println(dacBOut);
  Serial.print("Channel C = ");
  Serial.println(dacCOut);
  Serial.print("Channel D = ");
  Serial.println(dacDOut);
  Serial.print("Channel E = ");
  Serial.println(dacEOut);
  Serial.print("Channel F = ");
  Serial.println(dacFOut);
  Serial.print("Channel G = ");
  Serial.println(dacGOut);
  Serial.print("Channel H = ");
  Serial.println(dacHOut);
}
