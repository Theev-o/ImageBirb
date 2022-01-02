//
// Created by Michał Kuć on 31/12/2021.
//

#include "ImgData.h"
#include "Exceptions.h"
#include "BMPFileHeader.h"
#include <fstream>
#include <iostream>
#include <cmath>

ImgData::ImgData() {
    xRes = yRes = stride = 0;
    pixelData = {};
}

ImgData::ImgData(std::ifstream &inFile, BMPFileHeader &header) {
    yRes = header.getHeight();
    xRes = header.getWidth();
    //magic, I have no idea what this calculation does or how it works, but it does, and I'm not touching it.
    stride =  ((((xRes * header.getBitCount()) + 31) & ~31) >> 3);

    for(int i = 0; i < yRes; ++i){
        inFile.seekg((i * stride) + header.getOffset());
        for(int j = 0; j < xRes; ++j){
            pixelData.push_back(readPixel(inFile));
        }
    }
    std::cout << "Read " << pixelData.size() << " pixels.\n";

    if(pixelData.size() != xRes * yRes){
        throw PIXEL_DATA_MISMATCH();
    }
    if(inFile.eof() || inFile.bad()){
        throw FILE_READ_ERROR();
    }
}

void ImgData::downscale4X(ImgData &inImage) {

}

void ImgData::imgFlip(ImgData &image) {

}

void ImgData::greyscale(ImgData &image) {
    yRes = image.yRes;
    xRes = image.xRes;
    stride = image.stride;
    for(Pixel pix : image.pixelData){
        unsigned char grey = sqrt((0.299f * pow(pix.R, 2))
                                + (0.587f * pow(pix.G, 2))
                                + (0.114f * pow(pix.B, 2)));
        pixelData.push_back({grey, grey, grey});
    }
}

void ImgData::avgClr(ImgData &image) {

}

void ImgData::upscale4X(ImgData &image) {

}

int ImgData::getYRes() {
    return yRes;
}

int ImgData::getXRes() {
    return xRes;
}

int ImgData::getStride() {
    return stride;
}

void ImgData::write(std::ofstream &outFile) {
    for(int i = 0; i < yRes; ++i){
        for(int j = 0; j < xRes; ++j){
            outFile << pixelData[(xRes * i) + j].B
                    << pixelData[(xRes * i) + j].G
                    << pixelData[(xRes * i) + j].R;
        }
        if(xRes % 4){
            for (int j = 0; i < xRes % 4; ++j){
                outFile << (unsigned char)0;
            }
        }
    }
}


