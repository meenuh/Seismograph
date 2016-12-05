#include "ST7735_LCD.hpp"
#include <LPC17xx.h> //for memory mapped values

static const uint8_t commands[] = {
		15,							// # of commands in this array
		ST7735_SWRESET, DELAY,		// 1: Software reset with delays
		150, 						//    150 ms delay
		ST7735_SLPOUT, DELAY,		// 2: Out of sleep mode with delay
		255,						// 	  255 ms delay
		ST7735_FRMCTR1, 3,			// 3: frame rate control - normal mode - 3 args
		0x01, 0x2C, 0x2D,			//    Rate = fosc/(1x2+40) * (LINE+2C+2D)
		ST7735_FRMCTR2, 3,			// 4: Frame rate control - idle mode - 3 args
		0x01, 0x2C, 0x2D,			//    Rate = fosc/(1x2+40) * (LINE+2C+2D)
		ST7735_FRMCTR3, 6,			// 5: Frame rate control - partial mode - 3 args
		0x01, 0x2C, 0x2D,			// 	  Dot inversion mode
		0x01, 0x2C, 0x2D,			//	  Line inversion mode
		ST7735_INVCTR, 1,			// 6: Display inversion control - 1 arg - no delay
		0x07,						// 	  no inversion
		ST7735_PWCTR1, 3,			// 7: Power control - 3 args - no delay
		0xA2, 0x02, 0x84,			// 	  0xA2: --- 0x02: -4.6V 0x84: Automode
		ST7735_PWCTR2, 1,			// 8: Power control - 1 arg - no delay
		0xC5,						//    VGH25 = 2.4C VGSEL = -10 VGH = 2 * AVDD
		ST7735_PWCTR3, 2,			// 9: Power control - 2 args - no delay
		0x0A, 0x00,					//    0x0A: opamp current small 0x00: Boost frequency
		ST7735_PWCTR4, 2,			//10: Power control - 2 args - no delay
		0x8A, 0x2A,					//    BCLK/2, Opamp current small & Medium low
		ST7735_PWCTR5, 2, 			//11: Power control - 2 args - no delay
		0x8A, 0xEE,
		ST7735_VMCTR1, 1,			//12: Power control - 1 arg - no delay
		0x0E,
		ST7735_INVOFF, 0, 			//13: Don't invert display - no args - no delay
		ST7735_MADCTL, 1, 			//14: Memory access control (directions) - 1 arg
		0xC8,						//    row addr/col addr, bottom to top refresh
		ST7735_COLMOD, 1,			//15: set color mode - 1 arg - no delay
		0x05						//    16 bit color
};

void ST7735_LCD::initLCD(){

}
