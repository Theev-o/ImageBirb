#include "Scanner.h"

Scanner::WORD Scanner::readWord(std::ifstream &file) {
    //Some simple binary data manipulation to shove two chars into a short
    Scanner::WORD value = file.get() + (file.get() << 8);
    return value;
}

Scanner::DWORD Scanner::readDWord(std::ifstream &file) {
    //Same here, except slightly less simple. Little endian by the way.
    Scanner::DWORD value = 0;
    for (int i = 0; i < 4; i++) {
        value += file.get() << (8 * i);
    }
    return value;
}

Pixel Scanner::readPixel(std::ifstream &file) {
    //Simply reads the next 3 characters, nothing crazy
    Pixel pix{};
    pix.B = file.get();
    pix.G = file.get();
    pix.R = file.get();
    return pix;
}