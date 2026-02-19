#include "random.h"
#include "spi.h"

unsigned int random_state = 1; 

/* --- INITIALIZE ADC --- */
void ADC_Init(void) 
	{
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; 
    ADC0->CFG1 = ADC_CFG1_ADIV(1) | ADC_CFG1_MODE(2) | ADC_CFG1_ADICLK(0);
    ADC0->SC3 = 0;
}

/* --- GET ANALOG VALUE --- */
unsigned short ADC_Read(unsigned char port) 
{
    ADC0->SC1[0] = port; 
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)); 
    return (unsigned short)ADC0->R[0];
}

/* --- SEED --- */
unsigned int GetNoise(void) 
{
    unsigned int seed = 0;
    for(int i = 0; i < 32; i++) 
		{
        seed = (seed << 1) | (ADC_Read(3) & 1);
        delay_us(100); 
    }
    return seed;
}

/* --- GENERATOR --- */
void MyRandom(unsigned int seed) 
{ 
    random_state = seed; 
}

unsigned int MyRand(void) 
{
		random_state = 1103515245 * random_state + 12345;
		return (random_state / 65536) % 32768;
}
