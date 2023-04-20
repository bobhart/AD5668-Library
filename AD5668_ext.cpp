/*
  ADD5668_ext.cpp, version 1.1, written by George "Blaze" Kleitsiotis, March 21, 2023

  This library implements extended SPI constructor

  This is an Arduino DUE library for the Analog Devices ADD5668_ext 16-bit, 8 Channel
  Digital-to-Analog converter chip. It should also be usable with the Texas
  Instruments DAC8568, a pin and command compatible part. However, to maintain
  full compatibility with the ADD5668_ext, this library does not impliment the
  "Flexible Mode" internal reference command write sequences of the DAC8568
  (0b1001).

  Original code by Robert Hart 15/10/2015

  Portions of the original code were based upon prior work by crx and Stephan Bergemann,
  specifically in the structure of the writeDAC and init functions, as well as
  the use of a command definition list. This library impliments all of the
  command functions of the chip, and allows for tying the ~LDAC pin to ground.
  When tying ~LDAC to ground, DO NOT use the toggleLDAC function. Also, use of the
  toggleDAC function is probably only needed in conjuction with command function
  writeChannel (chip command 0). For a full explanation of the various functions,
  refer to the readme file and the examples.

  The ADD5668_ext library is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation, version 3 of the License, which should be included in this
  this distribution. If not, see <http://www.gnu.org/licenses/>.

  When editing: 
  ~~~Built with care!!! Treat like a vergin!!!~~~

*/

#include "AD5668_ext.h"
#include <SPI.h>

// Hardware SPI Constructor, must use hardware SPI bus pins
AD5668_ext::AD5668_ext(uint8_t ssPin, uint8_t clrPin, int8_t ldacPin) {
  _ssPin = ssPin;
  _clrPin = clrPin;
  _ldacPin = ldacPin;
  pinMode(_clrPin, OUTPUT);
  if (_ldacPin > 0) {
    pinMode(_ldacPin, OUTPUT);
  }
}


void AD5668_ext::init() {
SPI.begin(_ssPin);
SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE3));//SPI_MODE3 @ 1MHz
if(_ldacPin>0){
  digitalWrite(_ldacPin,HIGH);}

  digitalWrite(_clrPin,LOW);
  delayMicroseconds(1);
  digitalWrite(_clrPin,HIGH);
  delayMicroseconds(1);

}

void AD5668_ext::writeDAC(uint8_t command, uint8_t address, unsigned int data, uint8_t function) {
  byte b1 = command;
  byte b2 = address << 4 | data >> 12; //4 address bits and 4 MSBs of data
  byte b3 = data >> 4; // middle 8 bits of data
  byte b4 = 0xF0 & (data << 4) >> 8 | function;
    SPI.transfer(_ssPin,b1,SPI_CONTINUE);
    SPI.transfer(_ssPin,b2,SPI_CONTINUE);
    SPI.transfer(_ssPin,b3,SPI_CONTINUE);
    SPI.transfer(_ssPin,b4);
  }

void AD5668_ext::writeChannel(uint8_t channel, unsigned int value) {
  _channel = channel;
  _value = value;
  writeDAC(WRITE_INPUT_REGISTER, _channel, _value, 15);
}

void AD5668_ext::updateChannel(uint8_t channel) {
  _channel = channel;
  writeDAC(UPDATE_OUTPUT_REGISTER, _channel, 0, 15);
}

void AD5668_ext::writeChUpdateAll(uint8_t channel, unsigned int value) {
  _channel = channel;
  _value = value;
  writeDAC(WRITE_INPUT_REGISTER_UPDATE_ALL, _channel, _value, 15);
}

void AD5668_ext::writeUpdateCh(uint8_t channel, unsigned int value) {
  _channel = channel;
  _value = value;
  writeDAC(WRITE_INPUT_REGISTER_UPDATE_N, _channel, _value, 15);
}

void AD5668_ext::powerDAC_Normal(uint8_t channelsN) {
  _channelsN = channelsN;
  unsigned int modeChE_H = _channelsN >> 4;
  byte chA_D = 0x0F & _channelsN;
  writeDAC(POWER_DOWN_UP_DAC, 0, modeChE_H, chA_D);
}

void AD5668_ext::powerDAC_Down1K(uint8_t channelsN) {
  _channelsN = channelsN;
  unsigned int modeChE_H = 0x10 | (_channelsN >> 4);
  byte chA_D = 0x0F & _channelsN;
  writeDAC(POWER_DOWN_UP_DAC, 0, modeChE_H, chA_D);
}

void AD5668_ext::powerDAC_Down100K(uint8_t channelsN) {
  _channelsN = channelsN;
  unsigned int modeChE_H = 0x20 | (_channelsN >> 4);
  byte chA_D = 0x0F & _channelsN;
  writeDAC(POWER_DOWN_UP_DAC, 0, modeChE_H, chA_D);
}

void AD5668_ext::powerDAC_DownTri(uint8_t channelsN) {
  _channelsN = channelsN;
  unsigned int modeChE_H = 0x30 | (_channelsN >> 4);
  byte chA_D = 0x0F & _channelsN;
  writeDAC(POWER_DOWN_UP_DAC, 0, modeChE_H, chA_D);
}

void AD5668_ext::setClearCode(uint8_t clearCode) {
  _clearCode = clearCode;
  writeDAC(LOAD_CLEAR_CODE_REGISTER, 0, 0, _clearCode);
}

void AD5668_ext::setSoftLDAC(uint8_t channelsN) {
  _channelsN = channelsN;
  unsigned int chE_H = _channelsN >> 4;
  byte chA_D = 0x0F & _channelsN;
  writeDAC(LOAD_LDAC_REGISTER, 0, chE_H, chA_D);
}

void AD5668_ext::reset() {
  writeDAC(RESET_POWER_ON, 0, 0, 0);
}

void AD5668_ext::enableInternalRef() {
  writeDAC(SETUP_INTERNAL_REF, 0, 0, 1);
}

void AD5668_ext::disableInternalRef() {
  writeDAC(SETUP_INTERNAL_REF, 0, 0, 0);
}

void AD5668_ext::toggleLDAC() {
  digitalWrite(_ldacPin, LOW);
  delayMicroseconds(1);
  digitalWrite(_ldacPin, HIGH);
  delayMicroseconds(1);
}
