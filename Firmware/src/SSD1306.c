
#include "main.h"
#include "SSD1306.h"
#include "i2c.h"


static const uint8_t code num_font_bitmap[] = {
	// 'Font0', 12x16px
	0xf0, 0xfc, 0xfe, 0xff, 0x0f, 0x07, 0x07, 0x0f, 0xff, 0xfe, 0xfc, 0xf0, 0x0f, 0x3f, 0x7f, 0xff,
	0xf0, 0xe0, 0xe0, 0xf0, 0xff, 0x7f, 0x3f, 0x0f,
	// 'Font1', 12x16px
	0x00, 0x00, 0x1c, 0x1e, 0x0e, 0xfe, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
	// 'Font2', 12x16px
	0x1c, 0x1e, 0x0f, 0x07, 0x07, 0x87, 0xc7, 0xff, 0xfe, 0xfe, 0x7c, 0x00, 0xf0, 0xf8, 0xfc, 0xfe,
	0xef, 0xe7, 0xe7, 0xe3, 0xe1, 0xe0, 0xe0, 0x00,
	// 'Font3', 12x16px
	0x00, 0x0e, 0xc7, 0xc7, 0xc7, 0xc7, 0xe7, 0xff, 0x7e, 0x7e, 0x3c, 0x00, 0x70, 0xe0, 0xe1, 0xe1,
	0xe1, 0xe1, 0xf3, 0xff, 0x7f, 0x3f, 0x1e, 0x00,
	// 'Font4', 12x16px
	0x00, 0x80, 0xc0, 0xf0, 0x78, 0x3e, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1e, 0x1f, 0x1f, 0x1d,
	0x1c, 0x1c, 0xff, 0xff, 0xff, 0xff, 0x1c, 0x1c,
	// 'Font5', 12x16px
	0x00, 0xfe, 0xff, 0xff, 0xc7, 0xc7, 0xc7, 0xc7, 0x87, 0x87, 0x00, 0x00, 0x00, 0x71, 0xe1, 0xe1,
	0xe1, 0xe1, 0xf3, 0xff, 0x7f, 0x3f, 0x1f, 0x00,
	// 'Font6', 12x16px
	0xe0, 0xf8, 0xfc, 0xfe, 0x9e, 0xcf, 0xc7, 0xc7, 0xc7, 0xc7, 0x87, 0x00, 0x0f, 0x3f, 0x7f, 0x7f,
	0xf3, 0xe1, 0xe1, 0xf1, 0xff, 0x7f, 0x3f, 0x1f,
	// 'Font7', 12x16px
	0x07, 0x07, 0x07, 0x07, 0x07, 0xc7, 0xf7, 0xff, 0x7f, 0x1f, 0x07, 0x00, 0x00, 0x00, 0xc0, 0xfc,
	0xff, 0xff, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x00,
	// 'Font8', 12x16px
	0x00, 0x3c, 0x7e, 0xfe, 0xff, 0xc7, 0xc7, 0xc7, 0xff, 0x7e, 0x7e, 0x3c, 0x1c, 0x7e, 0x7f, 0xff,
	0xf3, 0xe1, 0xe1, 0xf3, 0xff, 0x7f, 0x7f, 0x1c,
	// 'Font9', 12x16px
	0xf8, 0xfc, 0xfe, 0xff, 0x8f, 0x07, 0x07, 0x8f, 0xff, 0xfe, 0xfc, 0xf0, 0x00, 0xf3, 0xe3, 0xe7,
	0xe7, 0xe7, 0xf7, 0x7b, 0x7f, 0x3f, 0x1f, 0x07,
	// 'FontMinus', 12x16px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00,
};


static const uint8_t code dot_bitmap[] = {
	// 'FontDot', 4x16px
	0x00, 0x00, 0x00, 0x00, 0x60, 0xf0, 0xf0, 0x60
};

/*
static const uint8_t code colon_bitmap[] = {
	// 'FontColon', 4x16px
	0x60, 0xf0, 0xf0, 0x60, 0x60, 0xf0, 0xf0, 0x60,
};
*/

static const uint8_t code time_h_bitmap[] = {
// 'FontTimeH', 6x16px
0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x04, 0x06, 0xfe, 0xfc,
};

static const uint8_t code time_m_bitmap[] = {
// 'FontTimeM', 10x16px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0x04, 0x06, 0xfe, 0xfc,
0x04, 0x06, 0xfe, 0xfc,
};

static const uint8_t code time_s_bitmap[] = {
// 'FontTimeS', 5x16px
0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xde, 0xd6, 0xf6, 0x66,
};

static const uint8_t code initSSD1306sq[] = { // Initialization Sequence
    // Init sequence for Adafruit 128x32 OLED module
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX, 15,        // ratio 16
    SSD1306_SETDISPLAYOFFSET, 0x00,     // no offset
    SSD1306_SETSTARTLINE | 0x00,        // line #0
    SSD1306_CHARGEPUMP, 0x14,          // internal vcc
    SSD1306_MEMORYMODE, 0x00,          // 2 - page, 1 - vertical, 0 - horizontal
    SSD1306_SETCOLADDRESS,0,SSD1306_COLUMNS-1,        // Set column address to indicate boundaries
    SSD1306_SETPAGEADDRESS,0,SSD1306_PAGES-1,        // Set page address to indicate boundaries
#if SSD1306_ROTATION == 0
    SSD1306_SEGREMAP | 0x1,            // column 127 mapped to SEG0
    SSD1306_COMSCANDEC,                // column scan direction reversed
#elif SSD1306_ROTATION == 180
    SSD1306_SEGREMAP | 0x0,            // column 127 mapped to SEG0
    SSD1306_COMSCANINC,                // column scan direction reversed
#endif
    SSD1306_SETCOMPINS, 0x02,          // sequential COM pins, disable remap
    SSD1306_SETCONTRAST, 0xAF,         // contrast control
    SSD1306_SETPRECHARGE, 0xF1,        // pre-charge period (1, 15)
    SSD1306_SETVCOMDETECT, 0x20,       // vcom deselect level
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYALLON_RESUME,
    //SSD1306_DISPLAYALLON,
    SSD1306_NORMALDISPLAY,
    //SSD1306_INVERTDISPLAY,
    //SSD1306_DISPLAYON
};


void ssd1306_printDigitLine(uint8_t line, uint8_t digit);


void ssd1306_init(void){
  uint8_t i,d;
  ssd1306_send_command_start();
  for (i = 0; i < sizeof(initSSD1306sq); i++) {
    d=initSSD1306sq[i];
    I2C_Write(d);
  }
  I2C_Stop();
}


void ssd1306_send_command_start(void){
  i2cBegin(0x3C, 0); // address of SSD1306
  I2C_Write(SSD1306_COMMAND);
}

void ssd1306_send_command(uint8_t cmd){
  ssd1306_send_command_start();
  I2C_Write(cmd);
  I2C_Stop();
}

void ssd1306_write_display_start(void){
  i2cBegin(0x3C, 0); // address of SSD1306
  I2C_Write(SSD1306_DATA);
}


void setCol(uint8_t col) {
  ssd1306_send_command(SSD1306_SETLOWCOLUMN | (col & 0XF));
  ssd1306_send_command(SSD1306_SETHIGHCOLUMN | (col >> 4));
}


void setRow(uint8_t row) {
  ssd1306_send_command(SSD1306_SETSTARTPAGE | row);
}

void ssd1306_clear_display(void){
uint8_t i,i1;
  for(i1=0;i1<SSD1306_PAGES;i1++){
    setCol(0);
    setRow(i1);
    ssd1306_write_display_start();
    for(i=0;i<SSD1306_COLUMNS;i++){
      I2C_Write(0);
    }
    I2C_Stop();
  }
  setCol(0);
  setRow(0);
}

// 0.00 - 9.99
void ssd1306_printNumber(uint8_t x, uint8_t y, int16_t num){
  // convert binary number to BCD
  uint8_t i, a[3];
  int16_t tmp;
  if(num<1000){
      tmp = num;
      a[0] = tmp / 100;
      tmp = tmp % 100;
      a[1] = tmp / 10;
      tmp = tmp % 10;
      a[2] = tmp;
  }else{
      a[0]=10;
      a[1]=10;
      a[2]=10;
  }
  // now print to LCD
  // we print SSD1306_PAGES rows and 3 digits with dot
  for(tmp=0;tmp<SSD1306_PAGES;tmp++){
    setCol(x); // set the position
    setRow(tmp+y);
    // first digit
    ssd1306_write_display_start();
    ssd1306_printDigitLine(tmp,a[0]);
    I2C_Write(0);
    I2C_Write(0);
    // print dot
    for(i=0;i<DOT_WIDTH;i++){
      I2C_Write(dot_bitmap[i+tmp*DOT_WIDTH]);
    }
    I2C_Write(0);
    I2C_Write(0);
    // print second digit
    ssd1306_printDigitLine(tmp,a[1]);
    I2C_Write(0);
    I2C_Write(0);
    // print third digit
    ssd1306_printDigitLine(tmp,a[2]);
    I2C_Stop();
  }

}

void ssd1306_printDigitLine(uint8_t line, uint8_t digit){
  uint8_t i,tmp1;
  uint16_t tmp;
  for(i=0;i<NUMBER_WIDTH;i++){
    tmp = (i+line*NUMBER_WIDTH)+digit*NUMBER_WIDTH*NUMBER_HEIGHT;
    tmp1 = num_font_bitmap[tmp];
    I2C_Write(tmp1);
  }
}

// x,y - position (x:0-127,y:0-3)
//w,h - size (w:0-127,h:0-3)
void ssd1306_printBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t code *bmp){
  uint8_t ih, iw;
  uint16_t addr=0;
  for(ih=0;ih<h;ih++){
    setCol(x); // set the position
    setRow(y);
    ssd1306_write_display_start();
    for(iw=0;iw<w;iw++){
      I2C_Write(bmp[addr]);
      addr++;
    }
    I2C_Stop();
    y++;
  }
}

// x,y - position (x:0-127,y:0-3)
//w,h - size (w:0-127,h:0-3)
void ssd1306_printBitmapClear(uint8_t x, uint8_t y, uint8_t w, uint8_t h){
  uint8_t ih, iw;
  for(ih=0;ih<h;ih++){
    setCol(x); // set the position
    setRow(y);
    ssd1306_write_display_start();
    for(iw=0;iw<w;iw++){
      I2C_Write(0);
    }
    I2C_Stop();
    y++;
  }
}


// 00h - 99h
void ssd1306_printTimeH(uint8_t x, uint8_t y, uint8_t num){
  // convert binary number to BCD
  uint8_t i, a[2];
  int16_t tmp;
  if(num<100){
      tmp = num;
      a[0] = tmp / 10;
      tmp = tmp % 10;
      a[1] = tmp;
  }else{
      a[0]=10;
      a[1]=10;
  }
  // now print to LCD
  // we print 2 digits with h
  for(tmp=0;tmp<SSD1306_PAGES;tmp++){
    setCol(x); // set the position
    setRow(tmp+y);
    // first digit
    ssd1306_write_display_start();
    ssd1306_printDigitLine(tmp,a[0]);
    I2C_Write(0);
    // print second digit
    ssd1306_printDigitLine(tmp,a[1]);
    I2C_Write(0);
    // print h
    for(i=0;i<6;i++){
      I2C_Write(time_h_bitmap[i+tmp*6]);
    }
    I2C_Stop();
  }
}

// 00m - 99m
void ssd1306_printTimeM(uint8_t x, uint8_t y, uint8_t num){
  // convert binary number to BCD
  uint8_t i, a[2];
  int16_t tmp;
  if(num<100){
      tmp = num;
      a[0] = tmp / 10;
      tmp = tmp % 10;
      a[1] = tmp;
  }else{
      a[0]=10;
      a[1]=10;
  }
  // now print to LCD
  // we print 2 digits with h
  for(tmp=0;tmp<SSD1306_PAGES;tmp++){
    setCol(x); // set the position
    setRow(tmp+y);
    // first digit
    ssd1306_write_display_start();
    ssd1306_printDigitLine(tmp,a[0]);
    I2C_Write(0);
    // print second digit
    ssd1306_printDigitLine(tmp,a[1]);
    I2C_Write(0);
    // print h
    for(i=0;i<10;i++){
      I2C_Write(time_m_bitmap[i+tmp*10]);
    }
    I2C_Stop();
  }
}

// 00s - 99s
void ssd1306_printTimeS(uint8_t x, uint8_t y, uint8_t num){
  // convert binary number to BCD
  uint8_t i, a[2];
  int16_t tmp;
  if(num<100){
      tmp = num;
      a[0] = tmp / 10;
      tmp = tmp % 10;
      a[1] = tmp;
  }else{
      a[0]=10;
      a[1]=10;
  }
  // now print to LCD
  // we print 2 digits with h
  for(tmp=0;tmp<SSD1306_PAGES;tmp++){
    setCol(x); // set the position
    setRow(tmp+y);
    // first digit
    ssd1306_write_display_start();
    ssd1306_printDigitLine(tmp,a[0]);
    I2C_Write(0);
    // print second digit
    ssd1306_printDigitLine(tmp,a[1]);
    I2C_Write(0);
    // print h
    for(i=0;i<5;i++){
      I2C_Write(time_s_bitmap[i+tmp*5]);
    }
    I2C_Stop();
  }
}


#ifdef DEBUG
// 0000 - 9999
void ssd1306_printNumberDebug(uint8_t x, uint8_t y, int16_t num){
  // convert binary number to BCD
  uint8_t a[4];
  int16_t tmp;
  if(num<10000){
      tmp = num;
      a[0] = tmp / 1000;
      tmp = tmp % 1000;
      a[1] = tmp / 100;
      tmp = tmp % 100;
      a[2] = tmp / 10;
      tmp = tmp % 10;
      a[3] = tmp;
  }else{
      a[0]=10;
      a[1]=10;
      a[2]=10;
      a[3]=10;
  }
  // now print to LCD
  // we print NUMBER_HEIGHT rows and 4 digits with dot
  for(tmp=0;tmp<NUMBER_HEIGHT;tmp++){
    setCol(x); // set the position
    setRow(tmp+y);
    ssd1306_write_display_start();
    // first digit
    ssd1306_printDigitLine(tmp,a[0]);
    I2C_Write(0);
    // print second digit
    ssd1306_printDigitLine(tmp,a[1]);
    I2C_Write(0);
    // print third digit
    ssd1306_printDigitLine(tmp,a[2]);
    I2C_Write(0);
    // print last digit
    ssd1306_printDigitLine(tmp,a[3]);
    I2C_Stop();
  }
}
#endif

#ifdef SCROLLING
volatile uint8_t xdata scrlbuff[11*2]; // set here maximum xram buffer for image rotation (the size of biggest image to rotate

// copy image to buffer to able to scroll (no output to LCD)
void scroll_init(uint8_t w, uint8_t h, uint8_t code *bmp){
	//uint8_t xdata *buf = scrlbuff;
	uint8_t i;
	for(i=0;i<(w*h);i++){
		scrlbuff[i]=bmp[i];
	}
}

void scroll_down(uint8_t w, uint8_t h){
	// read first vertical image line
	bit carry_f, carry_fbck;
	uint8_t b, v, c;
	for(c=0;c<w;c++){
	  carry_f = 0;
	  carry_fbck = 0;
		for(v=0;v<h;v++){
			b = scrlbuff[v*w+c]; // read first byte
			carry_f = (b > 127 ? 1 : 0); // get bit to carry if needed
			b = b<<1; // scroll one bit to the left
			b |= carry_fbck; // set carry bit
			scrlbuff[v*w+c] = b;
			carry_fbck = carry_f; // store carry bit
		}
		scrlbuff[c] |= carry_fbck; // set carry bit
	}
}

void ssd1306_printBitmapX(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t xdata *bmp){
  uint8_t ih, iw;
  uint16_t addr=0;
  for(ih=0;ih<h;ih++){
    setCol(x); // set the position
    setRow(y);
    ssd1306_write_display_start();
    for(iw=0;iw<w;iw++){
      I2C_Write(bmp[addr]);
      addr++;
    }
    I2C_Stop();
    y++;
  }
}

#endif
