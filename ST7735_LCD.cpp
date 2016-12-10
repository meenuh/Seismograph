/*
 * ST7735_LCD.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: dearm
 */

#include "ST7735_LCD.hpp"

//TODO change SPI names to LCD for clarity/avoid confusion
//TODO set reset pin and DC pin to GPIO

//part one
static const uint8_t commands[] = {                 // Init for 7735R, part 1 (red or green tab)
	    15,                       // 15 commands in list:
	    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
	      150,                    //     150 ms delay
	    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
	      255,                    //     500 ms delay
	    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
	      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
	    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
	      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
	    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
	      0x01, 0x2C, 0x2D,       //     Dot inversion mode
	      0x01, 0x2C, 0x2D,       //     Line inversion mode
	    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
	      0x07,                   //     No inversion
	    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
	      0xA2,
	      0x02,                   //     -4.6V
	      0x84,                   //     AUTO mode
	    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
	      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
	    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
	      0x0A,                   //     Opamp current small
	      0x00,                   //     Boost frequency
	    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
	      0x8A,                   //     BCLK/2, Opamp current small & Medium low
	      0x2A,
	    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
	      0x8A, 0xEE,
	    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
	      0x0E,
	    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
	    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
	      0xC8,                   //     row addr/col addr, bottom to top refresh
	    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
	      0x05 };

//part two
static uint8_t Rcmd2green[] = {
		2,							// 2 commands in the list
		ST7735_CASET, 4,			// 1: Column address set - 4 args - no delay
		0x00, 0x02,					//    XSTART = 0
		0x00, 0x7F+0x02,			//    XEND = 127
		ST7735_RASET, 4,			// 2: Row address set - 4 args - no delay
		0x00, 0x01,					//    XSTART = 0
		0x00, 0x9F+0x01				//    XEND = 159
};

//part 3
static  uint8_t Rcmd3[] = {
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

ST7735_LCD::ST7735_LCD(){

}

void ST7735_LCD::initLCD(){
	//SPI set up
	u0_dbg_printf("before setup\n");
	setupSPI();
	u0_dbg_printf("after setup\n");
	commandList(commands);
	u0_dbg_printf("command list 1\n");
	commandList(Rcmd2green);
	u0_dbg_printf("command list 2\n");
	commandList(Rcmd3);
	u0_dbg_printf("command list 3\n");
}

//uses SSP1
void ST7735_LCD::setupSPI(){
	//config gpio port to be used for cs
	LPC_PINCON->PINSEL0 &= ~(3 << 0);
	LPC_GPIO0->FIODIR |= (1 << 0); //currently set as output
	//LPC_GPIO0->FIOSET = (1 << 0);

	//config gpio port to be used for rst
	LPC_PINCON->PINSEL0 &= ~(3 << 2);
	LPC_GPIO0->FIODIR |= (1 << 1); //currently set as output

	LPC_GPIO0->FIOCLR = (1 << 1); //disable reset

	//set power to ssp1 and clk
	LPC_SC->PCONP |= (1 << 10);
	LPC_SC->PCLKSEL0 &= ~(3 << 20);
	LPC_SC->PCLKSEL0 |= (1 << 20);
	LPC_PINCON->PINSEL0 &= ~(3 << 20);
	LPC_PINCON->PINSEL0 |= (1 << 20);

	//select pins to enable to be used as MOSI, MISO, and SCK
	LPC_PINCON->PINSEL0 &= ~( (3 << 14) | (3 << 16) | (3 << 18) );
	LPC_PINCON->PINSEL0 |=  ( (2 << 14) | (2 << 16) | (2 << 18) );

	//set up control registers
	LPC_SSP1->CR0 = 7;
	LPC_SSP1->CR1 = (1 << 1);
	LPC_SSP1->CPSR = 8;
}

void ST7735_LCD::SPI_writecommand(uint8_t c){
	SPI_enable();
	SPI_exchangeByte(c);
	SPI_disable();
}

void ST7735_LCD::commandList(const uint8_t *addr){
	 uint8_t  numCommands, numArgs;
	  uint16_t ms;

	  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
	  while(numCommands--) {
	    SPI_writecommand(pgm_read_byte(addr++)); //   Read, issue command
	    numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
	    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
	    numArgs &= ~DELAY;                   //   Mask out delay bit
	    while(numArgs--) {                   //   For each argument...
	      SPI_writecommand(pgm_read_byte(addr++));  //     Read, issue argument
	    }

	    if(ms) {
	      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
	      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
	      delay_ms(ms);
	    }
	  }
}

char ST7735_LCD::SPI_exchangeByte(char out){
	LPC_SSP1->DR = out;
	while(LPC_SSP1->SR & (1 << 4));
	return LPC_SSP1->DR;
}

void ST7735_LCD::SPI_enable(){
	LPC_GPIO0->FIOCLR = (1 << 0);
}

void ST7735_LCD::SPI_disable(){
	LPC_GPIO0->FIOSET = (1 << 0);
}



