#include "spi.h"

void delay_us(int t) 
{ 
		for(volatile int i=0; i<t*5; i++); 
}
void delay_ms(int t) 
{ 
		for(volatile int i=0; i<t*4000; i++); 
}

void SW_SPI_Write(uint8_t data) 
{
    for(int i=0; i<8; i++) 
		{
        SCK_L;
        if(data & 0x80) 
				{
					MOSI_H; 
				}
				else 
				{
					MOSI_L;
				}
        delay_us(1); 
        SCK_H; 
        delay_us(1);
        data <<= 1;
    }
}
