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

static uint8_t Rcmd2green[] = {              // Init for 7735R, part 2 (red tab only)
  2,                        //  2 commands in list:
  ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
    0x00, 0x00,             //     XSTART = 0
    0x00, 0x7F,             //     XEND = 127
  ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
    0x00, 0x00,             //     XSTART = 0
    0x00, 0x9F };

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

ST7735_LCD::ST7735_LCD(uint8_t priority) : scheduler_task("LCD", 2048, priority) {

}

bool ST7735_LCD::init(){
	//SPI set up
	setupSPI();
	//u0_dbg_printf("befor set command\n");
	commandList(commands);
	//u0_dbg_printf("222222 set command\n");
	commandList(Rcmd2green);
	//u0_dbg_printf("33333 set command\n");
	commandList(Rcmd3);
	//u0_dbg_printf("44444 set command\n");
    LCD_writecommand(ST7735_MADCTL);
    //u0_dbg_printf("5555 set command\n");
    LCD_writedata(0xC0);
	return true;
}

bool ST7735_LCD::run(void *p){
	//u0_dbg_printf("rect\n");
	//fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, ST7735_WHITE);
	fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, ST7735_CYAN);
	delay_ms(500);
	return true;
}

/** SSP1
 * 0.0 - CS
 * 0.1 - reset
 * 1.29 - DC
 */
void ST7735_LCD::setupSPI(){
	//config gpio port to be used for cs
	u0_dbg_printf("start init\n");
	LPC_PINCON->PINSEL0 &= ~(3 << 0);
	LPC_GPIO0->FIODIR |= (1 << 0); //currently set as output

	//config gpio port to be used for rst
	LPC_PINCON->PINSEL0 &= ~(3 << 2);
	LPC_GPIO0->FIODIR |= (1 << 1); //currently set as output

	//config gpio port for DC 1.29
	LPC_PINCON->PINSEL3 &= ~(3 << 26);
	LPC_GPIO1->FIODIR |= (1 << 29);

	LPC_GPIO0->FIOSET = (1 << 1); //disable reset

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
	u0_dbg_printf("done\n");
}

void ST7735_LCD::fillrect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color) {

	int32_t width, height;

	width = x1 - x0 + 1;
	height = y1 - y0 + 1;
	setAddrWindow(x0, y0, x1, y1);
	//LCD_writecommand(ST7735_RAMWR);
	writeRGB(color, width * height);
}
//0x07E0
void ST7735_LCD::writeRGB(uint32_t color, uint32_t repeat) {
	uint8_t red, green, blue;
	int i;
	red = (color >> 16);
	green = (color >> 8) & 0xFF;
	blue = color & 0xFF;
	for (i = 0; i < repeat; i++) {
		LCD_writedata(red);
		LCD_writedata(green);
		LCD_writedata(blue);
	}
}

void ST7735_LCD::LCD_writecommand(uint8_t c){
	LPC_GPIO1->FIOCLR |= (1 << 29);
	SPI_exchangeByte(c);
}

void ST7735_LCD::LCD_writedata(uint8_t c){
	LPC_GPIO1->FIOSET |= (1 << 29);
	SPI_exchangeByte(c);
}
void ST7735_LCD::commandList(const uint8_t *addr){
	uint8_t numCommands, numArgs;
	uint16_t ms;

	numCommands = pgm_read_byte(addr++);   // Number of commands to follow
	while (numCommands--) {
		LCD_writecommand(pgm_read_byte(addr++)); //   Read, issue command
		numArgs = pgm_read_byte(addr++);    //   Number of args to follow
		ms = numArgs & DELAY;          //   If hibit set, delay follows args
		numArgs &= ~DELAY;                   //   Mask out delay bit
		while (numArgs--) {                   //   For each argument...
			LCD_writedata(pgm_read_byte(addr++));  //     Read, issue argument
		}

		if (ms) {
			ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
			if (ms == 255)
				ms = 500;     // If 255, delay for 500 ms
			delay_ms(ms);
		}
	}
}

void ST7735_LCD::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
//	fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, ST7735_WHITE);
//	fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, ST7735_GREEN); 80 a0 07e0
	LCD_writecommand(ST7735_CASET);
	LCD_writedata(0x00);
	LCD_writedata(x0);
	LCD_writedata(0x00);
	LCD_writedata(x1);

	LCD_writecommand(ST7735_RASET);
	LCD_writedata(0x00);
	LCD_writedata(y0);
	LCD_writedata(0x00);
	LCD_writedata(y1);
	LCD_writecommand(ST7735_RAMWR); // write to RAM

}

char ST7735_LCD::SPI_exchangeByte(char out){
	char temp;

	SPI_enable();
	LPC_SSP1->DR = out;
	while(LPC_SSP1->SR & (1 << 4));
	temp = LPC_SSP1->DR;
	SPI_disable();
	return temp;

}

void ST7735_LCD::SPI_enable(){
	LPC_GPIO0->FIOCLR = (1 << 0);
}

void ST7735_LCD::SPI_disable(){
	LPC_GPIO0->FIOSET = (1 << 0);
}



