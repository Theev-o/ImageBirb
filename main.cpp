#include "Settings.h"
#include "Exceptions.h"
#include "BMPFileHeader.h"
#include "ImgData.h"

#include <iostream>

int main(int argc, char *argv[]) {
    //Create a params object that holds the program settings
    std::unique_ptr<Settings> params;
    //Populate the settings by ingesting the input arguments, close program if an error occurs
    try {
        params = std::make_unique<Settings>(argc, argv);
    } catch (std::length_error &e) {
        std::cerr << e.what();
        return 1;
    } catch (std::invalid_argument &e) {
        std::cerr << e.what();
        return 2;
    }
    //If the program was run with -h, display the help screen
    if (params->operatingMode == Settings::Help) {
        params->displayHelp();
        return 0;
    }
    //Open the user-specified file, close program if unable to open
    std::ifstream inFile;
    try {
        inFile.open(params->inputPath, std::ios::binary);
        if (!inFile.is_open()) {
            throw std::ios_base::failure("Unable to open input file.");
        }
    } catch (std::ios_base::failure &e) {
        std::cerr << e.what();
        return 3;
    }
    //Read the header info from the input file, throw error if it fails
    std::unique_ptr<BMPFileHeader> inFileHeader;
    try {
        inFileHeader = std::make_unique<BMPFileHeader>(inFile);
    } catch (FILE_READ_ERROR &e) {
        std::cerr << e.what();
        return 4;
    } catch (WRONG_FILE_TYPE &e) {
        std::cerr << e.what();
        return 5;
    }
    //Read pixel data from input file, throw error if it fails
    std::unique_ptr<ImgData> inImgData;
    try {
        inImgData = std::make_unique<ImgData>(inFile, *inFileHeader);
    } catch (PIXEL_DATA_MISMATCH &e) {
        std::cerr << e.what();
        return 6;
    } catch (FILE_READ_ERROR &e) {
        std::cerr << e.what();
        return 7;
    }
    //Create an empty imgData object, populate it with one of the image manipulation functions
    std::unique_ptr<ImgData> outImgData;
    try {
        switch (params->operatingMode) {
            case Settings::Downscale4X:
                outImgData = std::make_unique<ImgData>();
                outImgData->downscale4X(*inImgData);
                break;
            case Settings::Upscale4X:
                outImgData = std::make_unique<ImgData>();
                outImgData->upscale4X(*inImgData);
                break;
            case Settings::Greyscale:
                outImgData = std::make_unique<ImgData>();
                outImgData->greyscale(*inImgData);
                break;
            case Settings::Flip:
                outImgData = std::make_unique<ImgData>();
                outImgData->imgFlip(*inImgData);
                break;
            case Settings::AvgColor:
                outImgData = std::make_unique<ImgData>();
                outImgData->avgClr(*inImgData);
                break;
            case Settings::Help:
                params->displayHelp();
                return 0;
            default:
                throw std::invalid_argument("Something went wrong when choosing operating mode.");
        }
    } catch (std::invalid_argument &e) {
        std::cerr << e.what();
        return 8;
    }
    //Create a new empty BMPFileHeader object, populate it using new image data
    std::unique_ptr<BMPFileHeader> outFileHeader;
    outFileHeader = std::make_unique<BMPFileHeader>(*outImgData);
    //Create the user-specified file, close program if unable to open
    std::ofstream outFile;
    try {
        outFile.open(params->outputPath, std::ios_base::in);
        if (!outFile.is_open()) {
            throw std::ios_base::failure("Unable to create output file.");
        }
        //Write the header and pixel data to the file
        outFileHeader->write(outFile);
        outImgData->write(outFile);
    } catch (std::ios_base::failure &e) {
        std::cerr << e.what();
        return 9;
    }
    return 0;
}