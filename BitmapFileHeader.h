//
// Created by Michał Kuć on 31/12/2021.
//

#ifndef IMAGEBIRB_BITMAPFILEHEADER_H
#define IMAGEBIRB_BITMAPFILEHEADER_H

#include "Scanner.h"

class BitmapFileHeader: public Scanner {
private:
    WORD bfType;             //The type of the file (BM in hex chars)
    DWORD bfSize;            //The filesize in bytes
    DWORD reserved;          //Not important, data from program that created image
    DWORD ofOffBits;         //The offset to pixel data, in bytes
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
    explicit BitmapFileHeader(std::ifstream &inFile);
    DWORD getHeight() const;
    DWORD getWidth() const;
    WORD getBitCount() const;
    DWORD getOffset() const;
    void write();
};


#endif //IMAGEBIRB_BITMAPFILEHEADER_H
