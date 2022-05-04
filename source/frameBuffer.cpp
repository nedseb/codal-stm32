#include "frameBuffer.h"
#include "font.h"

#include <cmath>
#include <vector>

using namespace codal;
using namespace std;

FrameBuffer::FrameBuffer( unsigned widthPixel, unsigned heightPixel, FrameBuffer::Format format ) 
    : width(widthPixel), height(heightPixel), format(format){

    switch( this->format ){
        case Format::MONO_VLSB:
            pages = height / 8;
            bufferSize = pages * width;
            break;
    }


    buffer = new uint8_t[bufferSize];
    
}

FrameBuffer::~FrameBuffer(){
    delete[] buffer;
}

void FrameBuffer::fill( uint16_t color ){

    switch( format ){
        case Format::MONO_VLSB:
            for( unsigned i = 0; i < bufferSize; ++i ){
                buffer[i] = color > 0 ? 0xFF : 0x00;
            }
            break;
    }
}

void FrameBuffer::drawPixel( unsigned x, unsigned y, uint16_t color){

    if( x >= width || y >= height ){ return; }

    switch( format ){
        case Format::MONO_VLSB:
            int shift = y % 8;
            int index = x + ( ( y / 8 ) * width );
            uint8_t * data = &buffer[index];

            if(color > 0){
                *data |= (0x01 << shift);
            }
            else{
                *data &= ~(0x01 << shift);
            }
            break;
    }
}

void FrameBuffer::drawChar( char c, unsigned x, unsigned y, uint16_t color){
    
    for( int i = 0; i < 5; ++i){

        uint8_t currentChar = ASCII_FONT[ c * 5 + i ];

        for(int j = 0; j < 8; ++j){
            if( ( currentChar & ( 0x01 << j) ) > 0 ){
                drawPixel( x + i, y + j, color );
            }
        }
    }
}

void FrameBuffer::drawText(string str, unsigned x, unsigned y, uint16_t color){
    for( size_t i = 0; i < str.length(); ++i ){
        drawChar( str[i], x + i * 6, y, color );
    }
}


uint16_t FrameBuffer::getPixelColor( uint8_t x, uint8_t y ) {
    if( x >= width || y >= height ){ return 0; }

    switch( format ){
        case Format::MONO_VLSB:
            int shift = y % 8;
            int index = x + ( ( y / 8 ) * width );

            if ((buffer[index] & (uint8_t) (0x01 << shift)) == 0) {
                return 0;
            }
            else {
                return 1;
            }
            break;
    }
}


void swapPoint(int16_t& x1, int16_t& y1, int16_t& x2, int16_t& y2) {
    int16_t tempo = x1;
    x1 = x2;
    x2 = tempo;

    tempo = y1;
    y1 = y2;
    y2 = tempo;
}

void FrameBuffer::drawSegment(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t size, bool invert, uint16_t color) {
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;

    if ( abs(dy) < abs(dx) ) {
        if ( x2 < x1 ) {
            swapPoint( x1, y1, x2, y2 );
        }
        for ( unsigned s = 0; s < size; ++s ) {
            for ( int16_t x = x1; x <= x2; ++x ) {
                int16_t y = y1 + dy * (x - x1) / dx + s;
                if ( x > 128 || y > 64 ) continue;
                if ( invert && getPixelColor( x, y ) == color ) {
                    color = color ^ 1;
                }
                drawPixel( x, y, color );
            }        
        }
    }
    else {
        if ( y2 < y1 ) {
            swapPoint( x1, y1, x2, y2 );
        }
        for ( unsigned s = 0; s < size; ++s ) {
            for ( int16_t y = y1; y <= y2; ++y ) {
                int16_t x = x1 + dx * (y - y1) / dy + s;
                if ( x > 128 || y > 64 ) continue;
                if ( invert && getPixelColor( x, y ) == color ) {
                    color = color ^ 1;
                }
                drawPixel( x, y, color );
            }
        }
    }
}

void FrameBuffer::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t size, bool invert, uint16_t color) {
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;

    if ( abs(dy) < abs(dx) ) {
        if ( x2 < x1 ) {
            swapPoint( x1, y1, x2, y2 );
        }
        for ( unsigned s = 0; s < size; ++s ) { 
            for ( int16_t x = 0; x < 128; x++ ) {
                int16_t y = y1 + std::ceil(dy * (x - x1)) / dx + s;
                if ( y < 0 || x > 128 || y > 64 ) continue;
                if ( invert && getPixelColor( x, y ) == color ) {
                    color = color ^ 1;
                }
                drawPixel( x, y, color );
            }
        }
    }
    else {
        if ( y2 < y1 ) {
            swapPoint( x1, y1, x2, y2 );
        }
        for ( unsigned s = 0; s < size; ++s ) { 
            for ( int16_t y = 0; y < 64; y++ ) {
                int16_t x = x1 + std::ceil(dx * (y - y1)) / dy + s;
                if ( x < 0 || x > 128 ||  y > 64 ) continue;
                if ( invert && getPixelColor( x, y ) == color ) {
                    color = color ^ 1;
                }
                drawPixel( x, y, color );
            }
        }
    }
}

int16_t getPt( int16_t n1 , int16_t n2 , float perc ) {
    int16_t diff = n2 - n1;
    return n1 + ( diff * perc );
}    

// Pas sur de le garder car pas très utile
void FrameBuffer::drawBezierCurve(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint16_t color) {
    int16_t oui;
    if (x1 < x3 && y1 < y3) {
        oui = abs(x3 - x1 + y3 - y1);
    }
    else if (x1 > x3 && y1 < y3) {
        oui = abs(x1 - x3 + y3 - y1);
    }
    else {
        oui = abs(x3 - x1 + y1 - y3);
    }

    for( float i = 0 ; i < oui ; ++i ) {
        uint16_t xa = getPt( x1 , x2 , i / oui );
        uint16_t ya = getPt( y1 , y2 , i / oui );
        uint16_t xb = getPt( x2 , x3 , i / oui );
        uint16_t yb = getPt( y2 , y3 , i / oui );

        uint16_t x = getPt( xa , xb , i / oui );
        uint16_t y = getPt( ya , yb , i / oui );

        drawPixel( x , y , 1 );
    }
}


void FrameBuffer::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool fill, bool invert, uint16_t color) {
    if (fill) {
        uint8_t x = x1;
        if (x2 < x1) {
            x = x2;
        }
        drawSegment(x, y1, x, y2, abs(x2 - x1), invert, color);
        return;
    }

    drawSegment(x1, y1, x2, y1, 1, invert, color);
    drawSegment(x1, y1, x1, y2, 1, invert, color);
    drawSegment(x2, y2, x2, y1, 1, invert, color);
    drawSegment(x2, y2, x1, y2, 1, invert, color);
}

void FrameBuffer::drawSquare(uint16_t x, uint16_t y, uint16_t lenght, bool fill, bool invert, uint16_t color) {
    drawRectangle(x, y, (x + lenght), (y + lenght), fill, invert, color);
}


void FrameBuffer::drawEllipse( int width, int height, int xc, int yc, bool fill, bool invert, uint16_t color ) {
    int dx, dy, d1, d2, x, y;
    x = 0;
    y = height;

    d1 = (height * height) - (width * width * height) + (0.25 * width * width);
    dx = 2 * height * height * x;
    dy = 2 * width * width * y;
    bool start = true;
    while (dx < dy) {
        if (fill) {
            if (start) {
                drawSegment(x + xc, y + yc, x + xc, -y + yc, 1, invert, color);
                start = false;
                continue;
            }
            drawSegment(x + xc, y + yc, x + xc, -y + yc, 1, invert, color);
            drawSegment(-x + xc, y + yc, -x + xc, -y + yc, 1, invert, color);
        }
        else {
            drawPixel(x + xc, y + yc, color);
            drawPixel(-x + xc, y + yc, color);
            drawPixel(x + xc, -y + yc, color);
            drawPixel(-x + xc, -y + yc, color);
        }

        if (d1 < 0) {
            x++;
            dx = dx + (2 * height * height);
            d1 = d1 + dx + (height * height);
        }
        else {
            x++;
            y--;
            dx = dx + (2 * height * height);
            dy = dy - (2 * width * width);
            d1 = d1 + dx - dy + (height * height);
        }
    }

    d2 = ((height * height) * ((x + 0.5) * (x + 0.5))) + ((width * width) * ((y - 1) * (y - 1))) - (width * width * height * height);
    uint8_t lastX = 0;
    while (y >= 0) {
        if (fill) {
            if (lastX != x + xc) {
                lastX = x + xc;
                drawSegment(x + xc, y + yc, x + xc, -y + yc, 1, invert, color);
                drawSegment(-x + xc, y + yc, -x + xc, -y + yc, 1, invert, color);
            }
        }
        else {
            drawPixel(x + xc, y + yc, color);
            drawPixel(-x + xc, y + yc, color);
            drawPixel(x + xc, -y + yc, color);
            drawPixel(-x + xc, -y + yc, color);
        }
        
        if (d2 > 0) {
            y--;
            dy = dy - (2 * width * width);
            d2 = d2 + (width * width) - dy;
        }
        else {
            y--;
            x++;
            dx = dx + (2 * height * height);
            dy = dy - (2 * width * width);
            d2 = d2 + dx - dy + (width * width);
        }
    }
}

void FrameBuffer::drawCircle(int x, int y, int radius, bool fill, bool invert, uint16_t color) {
    drawEllipse(radius * 2, radius * 2, x, y, fill, invert, color);
}


void FrameBuffer::drawPolygon(uint8_t x, uint8_t y, uint8_t line, uint8_t radius, uint8_t size, uint16_t color) {
    if (line < 3) return;

    int lastX = x + radius - 1;
    int lastY = y;
    int angleEcart = 360 / line;

    for (unsigned i = angleEcart; i < 360; i = i + angleEcart) {
        float radToDeg = i * 0.01745329251;
        int newX = (cos(radToDeg) * radius) + x;
        int newY = (sin(radToDeg) * radius) + y;
        
        drawPixel(newX, newY, color);
        drawSegment(newX, newY, lastX, lastY, size, false, color);

        lastX = newX;
        lastY = newY;
    }
    drawSegment(lastX, lastY, x + radius - 1, y, size, false, color);
}


void FrameBuffer::drawMatrix( std::vector<std::vector<unsigned>> matrix, unsigned x, unsigned y) {
    for (unsigned i = 0; i < matrix.size(); ++i) {
        for (unsigned j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 1) {
                drawPixel(j + x, i + y, 1);
            }
            else {
                drawPixel(j + x, i + y, 0);
            }
        }
    }
}