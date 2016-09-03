#include <SPI.h>
#define _CS_PIN_ 10 //Pin 10 defined as the CS Pin.
void setup()
{
  Serial.begin(115200); //Initalize the Serial Port to print the Analog Readings from MCP3208 onto teh Terminal.
  pinMode(_CS_PIN_,OUTPUT); //Pin 10 acting as OUTPUT as CS is a OUTPUT Pin. 
  digitalWrite(_CS_PIN_,HIGH);
  SPI.begin(); //Intialize SPI PORT
  SPI.setClockDivider(SPI_CLOCK_DIV2);


}

void loop()
{

     Serial.println(readADC(4)); //Reads Cahnnel 0 From MCP3208 and Prints the Value. Change it If u want t o read Other Cahnnel.
     delay(2000); //Delay
}

uint16_t readADC(int channel)
{
  uint16_t output;
  byte channelBits = channel<<6;

 //if(channel>3)
 
  //Select ADC
  digitalWrite(_CS_PIN_, LOW); //Select the Connected MCP3208 by pulling _CS_PIN_ LOW.
  if(channel>3)
    SPI.transfer(B00000111); //Defines Single mode of Operation for the ADC. D2=1 Start Bit. D1=1 Sinfgle Ended Mode D0=0 For Channel 0-3. 1 For Channel 4-7
  else
    SPI.transfer(B00000110);

  byte msb = SPI.transfer(channelBits); //Transfers 2nd Byte from MCU to MCP3208 which returns  MSB of the read data. 
  byte lsb = SPI.transfer(0x00);//Transfers 3rd Byte from MCU to MCP3208 which returns  LSB of the read data. 
 
  digitalWrite(_CS_PIN_,HIGH); //Deselect the Connected MCP3208 by pulling _CS_PIN_ HIGH.
 
  
  msb = msb & B00001111; // Make all 4 Higher bits of the MSB 0 as 12 Bit Resolution is provided by MCP3208
  
  output = msb<<8 | lsb; //Combine MSB with LSB to form the 12 Bit Analog read  Value.

  return output; //Output Value
}
