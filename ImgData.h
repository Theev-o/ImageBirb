//
// Created by Michał Kuć on 31/12/2021.
//

#ifndef IMAGEBIRB_IMGDATA_H
#define IMAGEBIRB_IMGDATA_H

#include <vector>
#include "Scanner.h"
#include "BMPFileHeader.h"


class ImgData: public Scanner{
private:
    std::vector <Pixel> pixelData;
    unsigned int stride;
    unsigned int yRes;
    unsigned int xRes;
public:
    ImgData(std::ifstream &inFile, BMPFileHeader &header);
    void downscale4X(ImgData &inImage);
    void upscale4X(ImgData &image);
    void imgFlip(ImgData &image);
    void greyscale(ImgData &image);
    void avgClr(ImgData &image);
    int getYRes();
    int getXRes();
    int getStride();
    void write(std::ofstream &outFile);

   ImgData();
};

#endif //IMAGEBIRB_IMGDATA_H
