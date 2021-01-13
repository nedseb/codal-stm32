#include "ssd1306.h"

using namespace codal;

constexpr uint8_t SET_CONTRAST        = 0x81;
constexpr uint8_t SET_ENTIRE_ON       = 0xa4;
constexpr uint8_t SET_NORM_INV        = 0xa6;
constexpr uint8_t SET_DISP            = 0xae;
constexpr uint8_t SET_MEM_ADDR        = 0x20;
constexpr uint8_t SET_COL_ADDR        = 0x21;
constexpr uint8_t SET_PAGE_ADDR       = 0x22;
constexpr uint8_t SET_DISP_START_LINE = 0x40;
constexpr uint8_t SET_SEG_REMAP       = 0xa0;
constexpr uint8_t SET_MUX_RATIO       = 0xa8;
constexpr uint8_t SET_COM_OUT_DIR     = 0xc0;
constexpr uint8_t SET_DISP_OFFSET     = 0xd3;
constexpr uint8_t SET_COM_PIN_CFG     = 0xda;
constexpr uint8_t SET_DISP_CLK_DIV    = 0xd5;
constexpr uint8_t SET_PRECHARGE       = 0xd9;
constexpr uint8_t SET_VCOM_DESEL      = 0xdb;
constexpr uint8_t SET_CHARGE_PUMP     = 0x8d;


SSD1306::SSD1306(unsigned width, unsigned height, bool externalVCC)
    : FrameBuffer( width, height, FrameBuffer::Format::MONO_VLSB),  externalVCC(externalVCC) {

}

SSD1306::~SSD1306(){}

void SSD1306::initDisplay(){
    /*
        From python driver :

            SET_DISP | 0x00, # off
            # address setting
            SET_MEM_ADDR, 0x00, # horizontal
            # resolution and layout
            SET_DISP_START_LINE | 0x00,
            SET_SEG_REMAP | 0x01, # column addr 127 mapped to SEG0
            SET_MUX_RATIO, self.height - 1,
            SET_COM_OUT_DIR | 0x08, # scan from COM[N] to COM0
            SET_DISP_OFFSET, 0x00,
            SET_COM_PIN_CFG, 0x02 if self.height == 32 else 0x12,
            # timing and driving scheme
            SET_DISP_CLK_DIV, 0x80,
            SET_PRECHARGE, 0x22 if self.external_vcc else 0xf1,
            SET_VCOM_DESEL, 0x30, # 0.83*Vcc
            # display
            SET_CONTRAST, 0xff, # maximum
            SET_ENTIRE_ON, # output follows RAM contents
            SET_NORM_INV, # not inverted
            # charge pump
            SET_CHARGE_PUMP, 0x10 if self.external_vcc else 0x14,
            SET_DISP | 0x01
    */



    writeCommand(SET_DISP | 0x00); //off
   
    // address setting
    writeCommand(SET_MEM_ADDR); writeCommand(0x00); // horizontal

    // Resolution and layout
    writeCommand(SET_DISP_START_LINE | 0x00);
    writeCommand(SET_SEG_REMAP | 0x01);  // column addr 127 mapped to SEG0
    writeCommand(SET_MUX_RATIO); writeCommand(height - 1);
    writeCommand(SET_COM_OUT_DIR | 0x08); //scan form COM[N] to COM0
    writeCommand(SET_DISP_OFFSET); writeCommand(0x00);
    writeCommand(SET_COM_PIN_CFG); writeCommand( height == 32 ? 0x02 : 0x12 );

    // Timing and driving scheme
    writeCommand(SET_DISP_CLK_DIV); writeCommand(0x80);
    writeCommand(SET_PRECHARGE); writeCommand( externalVCC ? 0x22 : 0xF1);
    writeCommand(SET_VCOM_DESEL); writeCommand(0x30); // 0.83 * Vcc

    // Display
    writeCommand(SET_CONTRAST); writeCommand(0xFF); // Maximum
    writeCommand(SET_ENTIRE_ON); // output follows RAM contents
    writeCommand(SET_NORM_INV); // not inverted

    // Charged pump
    writeCommand(SET_CHARGE_PUMP); writeCommand( externalVCC ? 0x10 : 0x14);
    writeCommand(SET_DISP | 0x01);
}

void SSD1306::powerOff(){
    writeCommand(SET_DISP | 0x00);
}

void SSD1306::powerOn(){
    writeCommand(SET_DISP | 0x01);
}

void SSD1306::contrast( uint8_t contrast ){
    writeCommand( SET_CONTRAST );
    writeCommand( contrast );
}

void SSD1306::invert( bool invert ){
    writeCommand(SET_NORM_INV | (invert & 1));
}

void SSD1306::show(){
    uint8_t x0 = 0;
    uint8_t x1 = width - 1;

    if( width == 64 ){
        // Display with width of 64 pixels are shifted by 32
        x0 += 32;
        x1 += 32;
    }

    writeCommand(SET_COL_ADDR);
    writeCommand(x0);
    writeCommand(x1);

    writeCommand(SET_PAGE_ADDR);
    writeCommand(0);
    writeCommand(pages - 1);

    writeData(buffer, bufferSize);
}
