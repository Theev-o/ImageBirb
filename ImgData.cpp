#include "ImgData.h"
#include "Exceptions.h"
#include "BMPFileHeader.h"

#include <cmath>

ImgData::ImgData() {
    xRes = yRes = stride = 0;
    pixelData = {};
}

ImgData::ImgData(std::ifstream &inFile, BMPFileHeader &header) {
    //Ingest resolution from the header
    yRes = header.getHeight();
    xRes = header.getWidth();
    //magic, I have no idea what this calculation does or how it works, but it does, and I'm not touching it.
    stride = ((((xRes * header.getBitCount()) + 31) & ~31) >> 3);
    //Read pixel data minding the padding bits
    for (int i = 0; i < yRes; ++i) {
        inFile.seekg((i * stride) + header.getOffset());
        for (int j = 0; j < xRes; ++j) {
            pixelData.push_back(readPixel(inFile));
        }
    }
    //Error handling
    if (pixelData.size() != xRes * yRes) {
        throw PIXEL_DATA_MISMATCH();
    }
    if (inFile.eof() || inFile.bad()) {
        throw FILE_READ_ERROR();
    }
}

void ImgData::downscale4X(ImgData &inImage) {
    //Halve resolutions, recalculate stride
    yRes = inImage.yRes / 2;
    xRes = inImage.xRes / 2;
    stride = ((((xRes * 24) + 31) & ~31) >> 3);
    //Skip over every second pixel in every second row of the input image
    for (int i = 0; i < inImage.yRes; i += 2) {
        for (int j = 0; j < inImage.xRes; j += 2) {
            pixelData.push_back(inImage.pixelData[(inImage.xRes * i) + j]);
        }
    }
}

void ImgData::imgFlip(ImgData &image) {
    //Inherit resolution and stride
    yRes = image.yRes;
    xRes = image.xRes;
    stride = image.stride;
    //Read every row of the input image backwards
    for (int i = 0; i < yRes; ++i) {
        for (int j = xRes - 1; j >= 0; --j) {
            pixelData.push_back(image.pixelData[(xRes * i) + j]);
        }
    }
}

void ImgData::greyscale(ImgData &image) {
    //Inherit resolution and stride
    yRes = image.yRes;
    xRes = image.xRes;
    stride = image.stride;
    //Calculate luma value, pass it 3 times to get greyscale pixels
    for (Pixel pix: image.pixelData) {
        unsigned char grey = sqrt((0.299f * pow(pix.R, 2))
                                  + (0.587f * pow(pix.G, 2))
                                  + (0.114f * pow(pix.B, 2)));
        pixelData.push_back({grey, grey, grey});
    }
}

void ImgData::avgClr(ImgData &image) {
    //Inherit resolution and stride
    yRes = image.yRes;
    xRes = image.xRes;
    stride = image.stride;
    //Tally up total values of every color component in the image
    unsigned long int totalR = 0, totalG = 0, totalB = 0;
    unsigned int pixCount = image.pixelData.size();
    for (Pixel pix: image.pixelData) {
        totalB += pix.B;
        totalG += pix.G;
        totalR += pix.R;
    }
    //Create a new pixel with colors that are the average of the color component values
    Pixel avgClr;
    avgClr.B = totalB / pixCount;
    avgClr.G = totalG / pixCount;
    avgClr.R = totalR / pixCount;
    //Fill pixel vector with the new pixel
    for (int i = 0; i < pixCount; ++i) {
        pixelData.push_back(avgClr);
    }
}

void ImgData::upscale4X(ImgData &image) {
    //Double resolutions, recalculate stride
    yRes = image.yRes * 2;
    xRes = image.xRes * 2;
    stride = ((((xRes * 24) + 31) & ~31) >> 3);
    //Dynamically allocate new 2D array (for better indexing)
    Pixel **pixArray;
    pixArray = new Pixel *[xRes];
    pixArray[0] = new Pixel[yRes * xRes];
    for (int i = 1; i < xRes; i++) {
        pixArray[i] = pixArray[i - 1] + xRes;
    }
    //First pass: Put pixels from input image in a grid, spaced out every 2 rows and columns
    for (int i = 0; i < image.yRes; ++i) {
        for (int j = 0; j < image.xRes; ++j) {
            pixArray[2 * i][(2 * j)] = image.pixelData[(i * image.xRes) + j];
        }
    }
    //Second pass: Interpolate rows using known pixel values, ignore pixels without two known neighbors
    for (int i = 0; i < yRes - 1; i += 2) {
        for (int j = 0; j < xRes - 1; j += 2) {
            pixArray[i][j + 1] = avgPix(pixArray[i][j], pixArray[i][j + 2]);
        }
    }
    //Third pass: Vertical interpolation, pixels without two known neighbors ignored
    for (int i = 1; i < yRes - 1; i += 2) {
        for (int j = 1; j < xRes; ++j) {
            pixArray[i][j] = avgPix(pixArray[i + 1][j], pixArray[i - 1][j]);
        }
    }
    //Fourth pass: Touching up - Side gaps are closed
    for (int i = 1; i < yRes; i += 1) {
        if (i != yRes) {
            pixArray[i][0] = avgPix(pixArray[i - 1][0], pixArray[i][1]);
            pixArray[i][xRes - 1] = avgPix(pixArray[i][xRes - 2], pixArray[i - 1][xRes - 1]);
        }
    }
    //Fifth pass: Touching up - Top row mostly filled in
    for (int i = xRes - 2; i > 0; --i) {
        pixArray[yRes - 1][i] = avgPix(pixArray[yRes - 2][i], pixArray[yRes - 2][i - 1]);
    }
    //Sixth pass: Touching up - Fill in remaining corners
    pixArray[yRes - 1][0] = avgPix(pixArray[yRes - 2][0], pixArray[yRes - 1][1]);
    pixArray[yRes - 1][xRes - 1] = avgPix(pixArray[yRes - 1][xRes - 2], pixArray[yRes - 2][xRes - 1]);
    pixArray[0][xRes - 1] = avgPix(pixArray[1][xRes - 1], pixArray[0][xRes - 2]);
    //Put pixel data from array into vector
    for (int i = 0; i < yRes; ++i) {
        for (int j = 0; j < xRes; ++j) {
            pixelData.push_back(pixArray[i][j]);
        }
    }
    //Deallocate memory after the vector is filled in
    delete[] pixArray[0];
    delete[] pixArray;
}

void ImgData::write(std::ofstream &outFile) {
    //Write every pixel in a row, add padding in necessary
    for (int i = 0; i < yRes; ++i) {
        for (int j = 0; j < xRes; ++j) {
            outFile << pixelData[(xRes * i) + j].B
                    << pixelData[(xRes * i) + j].G
                    << pixelData[(xRes * i) + j].R;
        }
        if (xRes % 4) {
            for (int j = 0; i < xRes % 4; ++j) {
                outFile << (unsigned char) 0;
            }
        }
    }
}

Pixel ImgData::avgPix(Pixel &a, Pixel &b) {
    //Self-explanatory, take averages of all the values, put them in a new pixel, return pixel
    Pixel avgPixel = {};
    unsigned int newB = (a.B + b.B) / 2;
    unsigned int newG = (a.G + b.G) / 2;
    unsigned int newR = (a.R + b.R) / 2;
    avgPixel.B = newB;
    avgPixel.G = newG;
    avgPixel.R = newR;
    return avgPixel;
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