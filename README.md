AD5668 library, version 1.1, written by Robert Hart, October 14, 2015

For version changes see the VERSION.txt

This is an Arduino library for use with the Analog Devices AD5668 8-Channel, 16-bit 
Digital-to-Analog Converter (DAC) chip. The chip utilizes a SPI bus interface and 
supports a data rate up to 50MHz, and has an internal voltage reference (off by 
default) that is either 1.25/2.5V (-1 version) or 2.5/5V (-2 and -3 versions). This 
library has constructors for both hardware and software SPI connections, and fully 
implements all chip command functions. Refer to the data sheet 
<http://www.analog.com/media/en/technical-documentation/data-sheets/AD5628_5648_5668.pdf> 
for full specifications. It should also be usable with the Texas Instruments DAC8568, a 
pin and command compatible part. However, to maintain full compatibility with the AD5668, 
this library does not impliment the  "Flexible Mode" internal reference command write 
sequences of the DAC8568 (0b1001).

The hardware SPI constructor requires connecting the AD5668's Din pin to the Arduino's 
MOSI pin (UNO pin 11, MEGA 2650 pin 51, or ICSP) and the chip's SCLK pin (pin 16) to the 
Arduino's SCK pin (UNO pin 13, MEGA 2650 pin 52, or ICSP). Any digital pins may be used 
with the software SPI constructor.

Portions of the code used in library were based upon prior work by crx and Stephan 
Bergemann, specifically in the structure of the writeDAC and init functions, as well as 
the use of a command definition list.

The AD5668 library is free software: you can redistribute it and/or modify it under the 
terms of the GNU General Public License as published by the Free Software Foundation, 
version 3 of the License, which should be included in this distribution. If not, see 
<http://www.gnu.org/licenses/>.

=============================================================================================
Explanation of library functions:

Constructors: 

AD5668(ssPin, clrPin, ldacPin); 
  Hardware SPI constructor, 
    ssPin = AD5668 ~SYNC pin 2; 
    clrPin = AD5668 ~CLR pin 9; 
    ldacPin = AD5668 ~LDAC pin 1, (optional in pin is tied to ground); 

AD5668(mosiPin, sclkPin, ssPin, clrPin, ldacPin); 
  Software SPI constructor, 
    mosiPin = AD5668 Din pin 15; 
    clrPin = AD5668 SCLK pin 16; 
    ssPin = AD5668 ~SYNC pin 2; 
    clrPin = AD5668 ~CLR pin 9; 
    ldacPin = AD5668 ~LDAC pin 1, (optional in pin is tied to ground); 


Initializer: 

init(); 
  This initializes the communication pins for the device and, if hardware SPI is used, 
  begins SPI and sets the bit order.
    

Command Functions: 

void writeChannel(channel, value); 
  Writes a value to DAC channel N input register, the value can then be shifted (updated) 
  to the channel's output register either by toggling the ~LDAC pin low, or by using one 
  of the following functions that update a single or all channels. 
  
    channel = 0 to 7, corresponds to DAC channels A to H, can also use 15 to write to all 
      channels; 
    value = 0 to 65535. 


void updateChannel(channel); 
  Updates channel N with contents of the channel's input register. 
  
    channel = 0 to 7, corresponds to DAC channels A to H, can also use 15 to write to all 
      channels; 


void writeChUpdateAll(channel, value); 
  Writes a value to DAC channel N input register, then shifts (updates) all new input values 
  to their repective channel's output register. 
  
    channel = 0 to 7, corresponds to DAC channels A to H, can also use 15 to write to all 
      channels; 
    value = 0 to 65535. 


void writeUpdateCh(channel, value); 
  Writes a value to DAC channel N input register, then shifts (updates) that value to the 
  channel's output register. 
  
    channel = 0 to 7, corresponds to DAC channels A to H, can also use 15 to write to all 
      channels; 
    value = 0 to 65535. 


void powerDAC_Normal(channelsN); 
  Sets channels specified by channelsN to powered up condition. 
  
    channelsN = a byte value, with each bit assigned to a channel, the LSB for channel A 
	  to MSB for channel H. As such, it may be easier to write the value as a byte, 
	  i.e. B11111111. 


void powerDAC_Down1K(channelsN); 
  Connects the output of channels specified by channelsN internally to ground through a 1K 
  resister. 
  
    channelsN = a byte value, with each bit assigned to a channel, the LSB for channel A 
	  to MSB for channel H. As such, it may be easier to write the value as a byte, 
	  i.e. B11111111. 


void powerDAC_Down100K(channelsN); 
  Connects the output of channels specified by channelsN internally to ground through a 
  100K resister. 
  
    channelsN = a byte value, with each bit assigned to a channel, the LSB for channel A 
	  to MSB for channel H. As such, it may be easier to write the value as a byte, 
	  i.e. B11111111. 


void powerDAC_DownTri(channelsN); 
  Sets the output of channels specified by channelsN to an open-circuit (tri-state) condition. 
  
    channelsN = a byte value, with each bit assigned to a channel, the LSB for channel A 
	  to MSB for channel H. As such, it may be easier to write the value as a byte, 
	  i.e. B11111111. 


void setClearCode(clearCode); 
  This function writes a value to the Clear Code register to specify the output state of all 
  channels when the ~CLR pin is toggled LOW. 
  
    clearCode = one of three values: 0 for 0 scale out, 1 for half-scale out and 2 for full- 
	  scale out. 


void setSoftLDAC(channelsN); 
  This function sets Soft ~LDAC on or off for selected channels. When set to on, a value 
  written to that channel is immediately transfered to the output, as if the ~LDAC pin 
  wsa tied to ground, while other channels are not affected. 
  
    channelsN = a byte value, with each bit assigned to a channel, the LSB for channel A 
	  to MSB for channel H. As such, it may be easier to write the value as a byte, 
	  i.e. B11111111. 


void reset(); 
  Resets DAC to initial powered-on condition, clear code is set to chip's default and the 
  internal reference voltage is turned off. 


void enableInternalRef(); 
  Turns on the chip's internal voltage reference, DO NOT connect an external voltage when 
  using this command. 


void disableInternalRef(); 
  Turns off the chip's internal voltage reference, this is the device's default condition 
  at power-up. 


void toggleLDAC(); 
  This function will toggle the ~LDAC pin LOW, then High to clock new data from the input 
  registers to the output registers. Only needed if not shifting data to output registers 
  through any of the above functions. 
