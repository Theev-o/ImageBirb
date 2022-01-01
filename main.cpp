#include <iostream>
#include <fstream>

#include "Settings.h"
#include "Exceptions.h"
#include "BitmapFileHeader.h"
#include "ImgData.h"

int main(int argc, char *argv[]) {
    //Create a params object that holds the program settings
    std::unique_ptr<Settings> params;
    //Populate the settings by ingesting the input arguments, close program if an error occurs
    try {
        params = std::make_unique<Settings>(argc, argv);
    } catch (std::length_error &e){
        std::cerr << e.what();
        return 1;
    } catch (std::invalid_argument &e) {
        std::cerr << e.what();
        return 2;
    }
    //If the program was ran with -h, display the help screen
    if(params -> operatingMode == Settings::Help){
        params -> displayHelp();
        return 0;
    }
    //Open the user-specified file, close program if unable to open
    std::ifstream inFile;
    try {
        inFile.open(params -> inputPath, std::ios::binary);
        if(!inFile.is_open()){
            throw std::ios_base::failure ("Unable to open input file.");
        }
    } catch (std::ios_base::failure &e) {
        std::cerr << e.what();
        return 3;
    }

    std::unique_ptr<BitmapFileHeader> inFileHeader;
    try {
        inFileHeader = std::make_unique<BitmapFileHeader>(inFile);
    } catch (FILE_READ_ERROR &e) {
        std::cerr << e.what();
        return 4;
    } catch (WRONG_FILE_TYPE &e) {
        std::cerr << e.what();
        return 5;
    }

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





    return 0;
}
