#ifndef IMAGEBIRB_SETTINGS_H
#define IMAGEBIRB_SETTINGS_H

#include <string>

//Class that holds the relevant program settings
class Settings {
public:
    //File paths for the input and output files
    std::string inputPath, outputPath;
    //The operating mode for the program, decides which functions to use
    enum mode { Downscale4X, Greyscale, Upscale4X, Flip, AvgColor, Help } operatingMode;

    //Constructor that ingests user-passed arguments from the command line
    Settings(int argc, char *argv[]);
    //Displays help message
    void displayHelp();
};
#endif //IMAGEBIRB_SETTINGS_H