//
// Created by Michał Kuć on 31/12/2021.
//

#ifndef IMAGEBIRB_IMGDATA_H
#define IMAGEBIRB_IMGDATA_H

#include <vector>
#include "Scanner.h"
#include "BitmapFileHeader.h"


class ImgData: public Scanner{
private:
    std::vector <Pixel> pixelData;
    unsigned int stride;
    unsigned int yRes;
    unsigned int xRes;
public:
    ImgData(std::ifstream &inFile, BitmapFileHeader &header);
    void write();
};

#endif //IMAGEBIRB_IMGDATA_H
