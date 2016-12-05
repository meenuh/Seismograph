#include "ST7735_LCD.hpp"
#include <LPC17xx.h> //for memory mapped values

//part one
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

//part two
static const uint8_t Rcmd2green[] = {
		2,							// 2 commands in the list
		ST7735_CASET, 4,			// 1: Column address set - 4 args - no delay
		0x00, 0x02,					//    XSTART = 0
		0x00, 0x7F+0x02,			//    XEND = 127
		ST7735_RASET, 4,			// 2: Row address set - 4 args - no delay
		0x00, 0x01,					//    XSTART = 0
		0x00, 0x9F+0x01				//    XEND = 159
};

//part 3
static const uint8_t Rcmd3[] = {
		4,							// 4 commands in list
		ST7735_GMCTRP1, 16,			// 1: Magical unicorn dust - 16 args - no delay
		0x02, 0x1c, 0x07, 0x12,
		0x37, 0x32, 0x29, 0x2d,
		0x29, 0x25, 0x2B, 0x39,
		0x00, 0x01, 0x03, 0x10,
		ST7735_GMCTRN1, 16,			// 2: Sparkles and rainbows - 16 args - no delay
	    0x03, 0x1d, 0x07, 0x06,
	    0x2E, 0x2C, 0x29, 0x2D,
	    0x2E, 0x2E, 0x37, 0x3F,
		0x00, 0x00, 0x02, 0x10,
		ST7735_NORON, DELAY,		// 3: Normal display on - no args - with delay
		10, 						//    10 ms delay
		ST7735_DISPON, DELAY,		// 4: Main screen turn on - no args - with delay
		100							//    100 ms delay
};

void ST7735_LCD::initLCD(){

}
