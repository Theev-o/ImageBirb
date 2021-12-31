//
// Created by Michał Kuć on 31/12/2021.
//

#ifndef IMAGEBIRB_IMGDATA_H
#define IMAGEBIRB_IMGDATA_H

#include <vector>
#include "Scanner.h"""

struct Pixel{
    unsigned char B;
    unsigned char G;
    unsigned char R;
};

class ImgData: Pixel {
private:
    std::vector <Pixel> pixelData;
    unsigned int stride;
    unsigned int yRes;
    unsigned int xRes;
public:
    void read();
    void write();
};

#endif //IMAGEBIRB_IMGDATA_H
