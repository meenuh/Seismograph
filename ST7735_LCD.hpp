#ifndef L5_APPLICATION_ST7735_LCD_HPP_
#define L5_APPLICATION_ST7735_LCD_HPP_

#include <LPC17xx.h> //for memory mapped values
#include "tasks.hpp"
#include "printf_lib.h"
#include "utilities.h"

#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#define DELAY 0x80
#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT  160

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
//1111100000000000
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

class ST7735_LCD: public scheduler_task
{
public:
	ST7735_LCD(uint8_t priority);
	bool init();
	bool run(void *p);
	char SPI_exchangeByte(char out);
	void commandList(const uint8_t *addr);
	void LCD_writecommand(uint8_t c);
	void LCD_writedata(uint8_t c);
	void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void fillrect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	void writeRGB(uint16_t color, uint32_t repeat);
	void drawPixel(int16_t x, int16_t y, uint16_t color);
private:
	void setupSPI();
	void SPI_enable();
	void SPI_disable();

};

#endif /* L5_APPLICATION_ST7735_LCD_HPP_ */
