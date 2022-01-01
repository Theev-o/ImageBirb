//
// Created by Michał Kuć on 31/12/2021.
//

#include "BitmapFileHeader.h"
#include "Exceptions.h"
#include <fstream>

BitmapFileHeader::BitmapFileHeader(std::ifstream &inFile) {
    //Read file header portion
    bfType = readWord(inFile);
    bfSize = readDWord(inFile);
    reserved = readDWord(inFile);
    ofOffBits = readDWord(inFile);
    //Read info header portion
    biSize = readDWord(inFile);
    biWidth = readDWord(inFile);
    biHeight = readDWord(inFile);
    biPlanes = readWord(inFile);
    biBitCount = readDWord(inFile);
    biCompression = readDWord(inFile);
    biSizeImage = readDWord(inFile);
    biXPelsPerMeter = readDWord(inFile);
    biYPelsPerMeter = readDWord(inFile);
    biClrUsed = readDWord(inFile);
    biClrImportant = readDWord(inFile);

    //This magic number fuckery is the integer representation of a big-endian "BM" WORD.
    if(bfType != 19778) {
        throw WRONG_FILE_TYPE();
    }
    if(inFile.eof() || inFile.bad()){
        throw FILE_READ_ERROR();
    }
}

Scanner::DWORD BitmapFileHeader::getHeight() const {
    return biHeight;
}

Scanner::DWORD BitmapFileHeader::getWidth() const {
    return biWidth;
}

Scanner::WORD BitmapFileHeader::getBitCount() const {
    return biBitCount;
}

Scanner::DWORD BitmapFileHeader::getOffset() const{
    return ofOffBits;
}