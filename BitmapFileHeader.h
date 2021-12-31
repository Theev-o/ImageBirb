//
// Created by Michał Kuć on 31/12/2021.
//

#ifndef IMAGEBIRB_BITMAPFILEHEADER_H
#define IMAGEBIRB_BITMAPFILEHEADER_H

#include "Scanner.h"

class BitmapFileHeader: public Scanner {
private:
    WORD bfType; //The type of the file (BM in hex chars)
    DWORD bfSize;       //The filesize in bytes
    DWORD reserved;     //Not important, data from program that created image
    DWORD ofOffBits;
public:
    void read();
    void write();
};


#endif //IMAGEBIRB_BITMAPFILEHEADER_H
