#ifndef IMAGEBIRB_BMPFILEHEADER_H
#define IMAGEBIRB_BMPFILEHEADER_H

#include "Scanner.h"

//Forward declaration of the ImgData class, to avoid recursive includes
class ImgData;

//Class that contains all the BMP file header data, as defined by microsoft docs
class BMPFileHeader : public Scanner {
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
    //Constructor that ingests header data from file
    BMPFileHeader(std::ifstream &inFile);

    //Constructor that calculates header data from image parameters
    BMPFileHeader(ImgData &outImgData);

    //Getter functions for their respective fields
    DWORD getHeight() const;

    DWORD getWidth() const;

    WORD getBitCount() const;

    DWORD getOffset() const;

    //Writes header data to output file
    void write(std::ofstream &outFile);
};
#endif //IMAGEBIRB_BMPFILEHEADER_H