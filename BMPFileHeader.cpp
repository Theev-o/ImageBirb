//
// Created by Michał Kuć on 31/12/2021.
//

#include "BMPFileHeader.h"
#include "Exceptions.h"
#include "ImgData.h"
#include <fstream>

BMPFileHeader::BMPFileHeader(std::ifstream &inFile) {
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
    if(bfType != 0x4d42) {
        throw WRONG_FILE_TYPE();
    }
    if(inFile.eof() || inFile.bad()){
        throw FILE_READ_ERROR();
    }
}


BMPFileHeader::BMPFileHeader(ImgData &outImgData) {
    bfType = 0x4D42;
    bfSize = 54 + (outImgData.getYRes()*outImgData.getStride());
    reserved = 0;
    ofOffBits = 54;
    biSize = 40;
    biWidth = outImgData.getXRes();
    biHeight = outImgData.getYRes();
    biPlanes = 1;
    biBitCount = 24;
    biCompression = 0;
    biSizeImage = 0;
    biXPelsPerMeter = 0;
    biYPelsPerMeter = 0;
    biClrUsed = 0;
    biClrImportant = 0;
}


Scanner::DWORD BMPFileHeader::getHeight() const {
    return biHeight;
}

Scanner::DWORD BMPFileHeader::getWidth() const {
    return biWidth;
}

Scanner::WORD BMPFileHeader::getBitCount() const {
    return biBitCount;
}

Scanner::DWORD BMPFileHeader::getOffset() const{
    return ofOffBits;
}

void BMPFileHeader::write(std::ofstream &outFile) {
    outFile.write((char*)&bfType, sizeof(bfType));
    outFile.write((char*)&bfSize, sizeof(bfSize));
    outFile.write((char*)&reserved, sizeof(reserved));
    outFile.write((char*)&ofOffBits, sizeof(ofOffBits));
    outFile.write((char*)&biSize, sizeof(biSize));
    outFile.write((char*)&biWidth, sizeof(biWidth));
    outFile.write((char*)&biHeight, sizeof(biHeight));
    outFile.write((char*)&biPlanes, sizeof(biPlanes));
    outFile.write((char*)&biBitCount, sizeof(biBitCount));
    outFile.write((char*)&biCompression, sizeof(biCompression));
    outFile.write((char*)&biSizeImage, sizeof(biSizeImage));
    outFile.write((char*)&biXPelsPerMeter, sizeof(biXPelsPerMeter));
    outFile.write((char*)&biYPelsPerMeter, sizeof(biYPelsPerMeter));
    outFile.write((char*)&biClrUsed, sizeof(biClrUsed));
    outFile.write((char*)&biClrImportant, sizeof(biClrImportant));
}

