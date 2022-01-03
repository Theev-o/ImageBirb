#ifndef IMAGEBIRB_IMGDATA_H
#define IMAGEBIRB_IMGDATA_H

#include <vector>

#include "Scanner.h"
#include "BMPFileHeader.h"
//Inherits from the Scanner class to have access to reading pixel data
class ImgData : public Scanner {
private:
    std::vector<Pixel> pixelData;   //All the pixels the image consists of
    unsigned int stride;            //The number of bytes required to go from one row to the next
    unsigned int yRes;              //Vertical resolution
    unsigned int xRes;              //Horizontal resolution
public:
    //Empty constructor, returns an empty object
    ImgData();

    //Instantiates the object with data read from the image file and header
    ImgData(std::ifstream &inFile, BMPFileHeader &header);

    //Halves the resolutions, reducing image size by 4 times
    void downscale4X(ImgData &inImage);

    //Doubles the resolutions and interpolates empty pixels, increasing image size by 4 times
    void upscale4X(ImgData &image);

    //Flips the image horizontally
    void imgFlip(ImgData &image);

    //Transforms the image to greyscale
    void greyscale(ImgData &image);

    //Fills an image of the same dimensions with the input image's average color value
    void avgClr(ImgData &image);

    //Returns the average color of two pixels
    Pixel avgPix(Pixel &a, Pixel &b);

    //Getter functions, simply return their respective fields
    int getYRes();

    int getXRes();

    int getStride();

    //Writes the data stored in pixelData to the output file
    void write(std::ofstream &outFile);
};
#endif //IMAGEBIRB_IMGDATA_H