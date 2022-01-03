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
    yRes = inImage.yRes/2;
    xRes = inImage.xRes/2;
    stride = ((((xRes * 24) + 31) & ~31) >> 3);
    for(int i = 0; i < inImage.yRes; i += 2) {
        for(int j = 0; j < inImage.xRes; j += 2) {
            pixelData.push_back(inImage.pixelData[(inImage.xRes * i) + j]);
        }
    }
}

void ImgData::imgFlip(ImgData &image) {
    yRes = image.yRes;
    xRes = image.xRes;
    stride = image.stride;
    for(int i = 0; i < yRes; ++i) {
        for(int j = xRes-1; j >= 0; --j) {
            pixelData.push_back(image.pixelData[(xRes*i) + j]);
        }
    }
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
    yRes = image.yRes;
    xRes = image.xRes;
    stride = image.stride;

    unsigned long int totalR = 0, totalG = 0, totalB = 0;
    unsigned int pixCount = image.pixelData.size();
    for(Pixel pix : image.pixelData) {
        totalB += pix.B;
        totalG += pix.G;
        totalR += pix.R;
    }

    Pixel avgClr;
    avgClr.B = totalB/pixCount;
    avgClr.G = totalG/pixCount;
    avgClr.R = totalR/pixCount;

    for(int i = 0; i < pixCount; ++i){
        pixelData.push_back(avgClr);
    }
}

void ImgData::upscale4X(ImgData &image) {
    yRes = image.yRes*2;
    xRes = image.xRes*2;
    stride = ((((xRes * 24) + 31) & ~31) >> 3);

    Pixel** pixArray;
    pixArray = new Pixel*[xRes];
    pixArray[0] = new Pixel[yRes * xRes];
    for (int i = 1; i < xRes; i++) {
        pixArray[i] = pixArray[i - 1] + xRes;
    }

    for(int i = 0; i < image.yRes; ++i) {
        for(int j = 0; j < image.xRes; ++j) {
            pixArray[2 * i][(2 * j)] = image.pixelData[(i * image.xRes) + j];
        }
    }

    for(int i = 0; i < yRes-1; i += 2){
        for(int j = 0; j < xRes - 1; j += 2){
            pixArray[i][j + 1] = avgPix(pixArray[i][j], pixArray[i][j+2]);
        }
    }
    //Second pass: Vertical interpolation, pixels without two known neighbors ignored
    for(int i = 1; i < yRes - 1; i += 2) {
        for(int j = 1; j < xRes; ++j){
            pixArray[i][j] = avgPix(pixArray[i+1][j], pixArray[i-1][j]);
        }
    }
    //Third pass: Touching up - Side gaps are narrowed
    for(int i = 0; i < yRes; i += 1) {
        if(i != yRes){
            pixArray[i][0] = avgPix(pixArray[i-1][0], pixArray[i][1]);
            pixArray[i+2][0] = avgPix(pixArray[i+3][0], pixArray[i+2][1]);
            pixArray[i][xRes-1] = avgPix(pixArray[i][xRes-2], pixArray[i-1][xRes-1]);
            pixArray[i+2][xRes-1] = avgPix(pixArray[i+2][xRes-2], pixArray[i+3][xRes-1]);
        }
    }
    pixArray[0][xRes-1] = avgPix(pixArray[1][xRes-1], pixArray[0][xRes-2]);
    //Fifth pass: Touching up - Top row mostly filled in
    for(int i = xRes - 2; i > 0; --i) {
        pixArray[yRes-1][i] = avgPix(pixArray[yRes-2][i], pixArray[yRes-2][i-1]);
    }
    //Sixth pass: Touching up - Fill in corners
    pixArray[yRes-1][0] = avgPix(pixArray[yRes-2][0], pixArray[yRes-1][1]);
    pixArray[yRes-1][xRes-1] = avgPix(pixArray[yRes-1][xRes-2], pixArray[yRes-2][xRes-1]);
    //Put pixel data from array into vector
    for(int i = 0; i < yRes; ++i) {
        for(int j = 0; j < xRes; ++j){
            pixelData.push_back(pixArray[i][j]);
        }
    }
    //Deallocate memory after the array is filled in
    delete[] pixArray[0];
    delete[] pixArray;
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
    std::cout << "\nWrote " << pixelData.size() << " pixels.";
}

Pixel ImgData::avgPix(Pixel &a, Pixel &b) {
    Pixel avgPixel = {};
    unsigned int newB = (a.B + b.B)/2;
    unsigned int newG = (a.G + b.G)/2;
    unsigned int newR = (a.R + b.R)/2;
    avgPixel.B = newB;
    avgPixel.G = newG;
    avgPixel.R = newR;
    return avgPixel;
}


/*
passCol = 0;
for (int j = 0; j < image.xRes; ++j) {
pixArray[passRow][passCol] = image.pixelData[(i * image.xRes) + j];
if (passCol != image.xRes - 1) {
pixArray[passRow][passCol+1] = avgPix(pixArray[passRow][passCol], image.pixelData[(i * image.xRes) + j + 2]);
}
passCol += 2;
}
passRow += 2;
passCol = 0;
++i;
for (int j = 0; j < image.xRes; ++j) {
pixArray[passRow][passCol] = image.pixelData[(i * image.xRes) + j];
if (passCol != image.xRes-1) {
pixArray[passRow][passCol-1] = avgPix(pixArray[passRow][passCol], image.pixelData[(i * image.xRes) + j - 2]);
}
passCol += 2;
}
passRow += 2;*/
