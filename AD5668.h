/*
  AD5668.h, version 1.0, written by Robert Hart, October 12, 2015

  This library implements both hardware-SPI and software SPI constructors.

  This is an Arduino library for the Analog Devices AD5668 16-bit, 8 Channel
  Digital-to-Analog converter chip. It should also be usable with the Texas
  Instruments DAC8568, a pin and command compatible part. However, to maintain
  full compatibility with the AD5668, this library does not impliment the
  "Flexible Mode" internal reference command write sequences of the DAC8568
  (0b1001).

  Portions of this code were based upon prior work by crx and Stephan Bergemann,
  specifically in the structure of the writeDAC and init functions, as well as
  the use of a command definition list. This library impliments all of the
  command functions of the chip, and allows for tying the ~LDAC pin to ground.
  When tying ~LDAC to ground, DO NOT use the toggleLDAC function. Also, use of
  the toggleDAC function is only needed in conjuction with command function
  writeChannel (chip command 0). For a full explanation of the various functions,
  refer to the readme file and the examples.

  The AD5668 library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3 of the License, which should be included
  in this distribution. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AD5668_h
#define AD5668_h

#include "arduino.h"

// Command definition list
#define WRITE_INPUT_REGISTER 0
#define UPDATE_OUTPUT_REGISTER 1
#define WRITE_INPUT_REGISTER_UPDATE_ALL 2
#define WRITE_INPUT_REGISTER_UPDATE_N 3
#define POWER_DOWN_UP_DAC 4
#define LOAD_CLEAR_CODE_REGISTER 5
#define LOAD_LDAC_REGISTER 6
#define RESET_POWER_ON 7
#define SETUP_INTERNAL_REF 8

#include <SPI.h>

class AD5668 {
  public:

  // Hardware SPI Constructor
  AD5668(int ssPin, int clrPin, int ldacPin = -1);

  // Software SPI constructor
  AD5668(int mosiPin, int sclkPin, int ssPin, int clrPin, int ldacPin = -1);

  /* Initialize the DAC
     vRegBit turns internal voltage reference on (1) or leaves it off (0)
     channelsN specifies which channels to power up normal, the value is a
     number representing the 8 channel bits A - H, i.e. power up channels
      A - D = 15, 255 = all channels. Can be in form BXXXXXXXX, i.e.
      B01010101 = channels A, C, E, G on, B11111111 all on.
  */
  void init(int vRegBit, int channelsN);

  // Write data for output value to DAC channel N input register
  void writeChannel(byte channel, unsigned int value);

  // Move a value in channel N input register out
  void updateChannel(byte channel);

  // Write an output value to channel N and update all (software ~LDAC)
  void writeChUpdateAll(byte channel, unsigned int value);

  // Write and update an output value out of channel N
  void writeUpdateCh(byte channel, unsigned int value);

  // Power up DAC channels normal operation
  void powerDAC_Normal(int channelsN);

  // Power down DAC channels modes

  //Power down DAC channels to 1KOhm to GND state
  void powerDAC_Down1K(int channelsN);

  //Power down DAC channels to 100KOhm to GND state
  void powerDAC_Down100K(int channelsN);

  //Power down DAC channels to Tri-state
  void powerDAC_DownTri(int channelsN);

  /* Clear Code Register Mode values:
   0 clears to 0x0000 (zero scale) = 0 volts out
   1 clears to 0x8000 (mid-scale) = half scale voltage out
   2 clears to 0xFFFF (full scale) = full voltage out
   3 clears to no operation (ignores ~clr pin) */
  void setClearCode(int clearCode);

  // Set bits for the ~LDAC Register
  void setSoftLDAC(int channelsN);

  // Reset the DAC
  void reset();

  // Enable the internal voltage reference (off by default)
  void enableInternalRef();

  // Disable the internal voltage reference
  void disableInternalRef();

  // Toggle the ~LDAC pin LOW then HIGH to load all output registers
  void toggleLDAC();

  private:
  void writeDAC(byte command, byte address, unsigned int data, byte function);
  boolean  _hwSPI;
  int _mosiPin, _sclkPin, _ssPin, _ldacPin, _clrPin, _vRegBit, _channel, _channelsN, _clearCode;
  unsigned int _value;
};
#endif
