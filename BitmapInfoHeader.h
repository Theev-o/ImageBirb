//
// Created by Michał Kuć on 31/12/2021.
//

#ifndef IMAGEBIRB_BITMAPINFOHEADER_H
#define IMAGEBIRB_BITMAPINFOHEADER_H

#include "Scanner.h"

class BitmapInfoHeader: public Scanner{
private:
    DWORD biSize;            //The size of this header in bytes (40)
    DWORD biWidth;           //Bitmap width in pixels
    DWORD biHeight;          //Bitmap height in pixels
    WORD biPlanes;           //Number of color planes (1)
    WORD biBitCount;         //Color depth of image (working with 24 for simplicity)
    DWORD biCompression;     //Compression method (0 for uncompressed)
    DWORD biSizeImage;       //Size of the image (use 0 when uncompressed)
    DWORD biXPelsPerMeter;   //Horizontal PPM, use 0
    DWORD biYPelsPerMeter;   //Vertical PPM, use 0
    DWORD biClrUsed;         //Numbers of colors in palette, use 0 to default to 2^n
    DWORD biClrImportant;    //Number of important colors, use 0
public:
    void read();
    void write();
};


#endif //IMAGEBIRB_BITMAPINFOHEADER_H
