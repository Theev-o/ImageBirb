//
// Created by Michał Kuć on 31/12/2021.
//
#include <fstream>
#include "Scanner.h"

Scanner::WORD Scanner::readWord(std::ifstream &file) {
    Scanner::WORD value = 0;
    for(int i = 0; i < sizeof(Scanner::WORD); ++i ){
        value << 8;
        value += file.get();
    }
    return value;
}

Scanner::DWORD Scanner::readDWord(std::ifstream &file) {
    Scanner::DWORD value = 0;
    for(int i = 0; i < sizeof(Scanner::DWORD); ++i ){
        value << 8;
        value += file.get();
    }
    return value;
}

Pixel Scanner::readPixel(std::ifstream &file) {
    Pixel pix {};
    pix.B = file.get();
    pix.G = file.get();
    pix.R = file.get();
    return pix;
}

