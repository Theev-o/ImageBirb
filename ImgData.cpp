//
// Created by Michał Kuć on 31/12/2021.
//

#include "ImgData.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream>

ImgData::ImgData(std::ifstream &inFile, BitmapFileHeader &header) {
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
