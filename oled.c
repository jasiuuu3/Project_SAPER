#include "oled.h"

uint8_t buffer[1024];

void OLED_Cmd(uint8_t cmd) 
{ 
    CS_L; DC_L; 
    SW_SPI_Write(cmd); 
    CS_H; 
}
/* --- INITIALIZATION OLED --- */
void OLED_Init(void) 
{
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
    
    PORTB->PCR[MOSI_PIN] = PORT_PCR_MUX(1);
    PORTB->PCR[SCK_PIN]  = PORT_PCR_MUX(1);
    PORTB->PCR[RES_PIN]  = PORT_PCR_MUX(1);
    PORTA->PCR[CS_PIN]   = PORT_PCR_MUX(1);
    PORTA->PCR[DC_PIN]   = PORT_PCR_MUX(1);
    
    PTB->PDDR |= (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << RES_PIN);
    PTA->PDDR |= (1 << CS_PIN) | (1 << DC_PIN);
    
    CS_H; SCK_L; MOSI_L;
    
    RES_H; delay_ms(10); 
    RES_L; delay_ms(100); 
    RES_H; delay_ms(100);
    
    OLED_Cmd(0xAE); OLED_Cmd(0xA1); OLED_Cmd(0xC8); OLED_Cmd(0xA8); OLED_Cmd(0x3F);
    OLED_Cmd(0xD3); OLED_Cmd(0x00); OLED_Cmd(0x40);
    OLED_Cmd(0xAD); OLED_Cmd(0x8B); 
    OLED_Cmd(0x81); OLED_Cmd(0xFF);
    OLED_Cmd(0xA4); OLED_Cmd(0xAF);
}
/* --- DRAWING PIXEL ON OLED --- */
void DrawPixel(int x, int y, int color) 
{
    if(x < 0 || x >= 128 || y < 0 || y >= 64) return;
    int page = y / 8; 
    int bit = y % 8;
    if(color) 
		{
			buffer[x + (page * 128)] |= (1 << bit);
		}
    else 
		{
			buffer[x + (page * 128)] &= ~(1 << bit);
		}
}
/* --- CLEARING BUFFER --- */
void OLED_ClearBuffer(void) 
{
    for(int i=0; i<1024; i++) 
		{
			buffer[i] = 0;
		}
}
/* --- DISPLAY ON SCREEN --- */
void OLED_Display(void)
{
    for(uint8_t page = 0; page < 8; page++) 
		{
        OLED_Cmd(0xB0 + page); 
        OLED_Cmd(0x02);        
        OLED_Cmd(0x10);    
        CS_L; DC_H;
        for(int x = 0; x < 128; x++) 
				{
					SW_SPI_Write(buffer[x + (page * 128)]);
				}
        CS_H;
    }
}
