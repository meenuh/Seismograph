#ifndef L5_APPLICATION_ST7735_LCD_HPP_
#define L5_APPLICATION_ST7735_LCD_HPP_

#define DELAY 0x80

#define ST7735_SWRESET 0x01
#define ST7735_SLPOUT  0x11
#define ST7735_INVOFF  0x20
#define ST7735_MADCTL  0x36
#define ST7735_COLMOD  0x3A
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4

#define ST7735_VMCTR1  0xC5

class ST7735_LCD
{
public:
	ST7735_LCD();
	void initLCD();

};

#endif /* L5_APPLICATION_ST7735_LCD_HPP_ */
