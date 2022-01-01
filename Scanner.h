//
// Created by Michał Kuć on 31/12/2021.
//

#ifndef IMAGEBIRB_SCANNER_H
#define IMAGEBIRB_SCANNER_H

#include <array>
#include <istream>

struct Pixel{
    unsigned char B;
    unsigned char G;
    unsigned char R;
};

class Scanner {
public:
    typedef uint16_t WORD;  //Windows data type consisting of 2 bytes
    typedef uint32_t DWORD; //Windows data type consisting of 4 bytes

    WORD readWord(std::ifstream &file);      //Reads the next word from a file
    DWORD readDWord(std::ifstream &file);    //Reads the next double word from a file
    Pixel readPixel(std::ifstream &file);    //Reads the next pixel from a file

};

#endif //IMAGEBIRB_SCANNER_H
