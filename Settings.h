//
// Created by Michał Kuć on 31/12/2021.
//

#ifndef IMAGEBIRB_SETTINGS_H
#define IMAGEBIRB_SETTINGS_H

#include <string>

class Settings {
public:
    std::string inputPath, outputPath;
    enum mode { Downscale4X, Greyscale, Upscale4X, Flip, AvgColor, Help } operatingMode;

    Settings(int argc, char *argv[]);
    void displayHelp();
};

#endif //IMAGEBIRB_SETTINGS_H
