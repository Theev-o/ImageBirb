#include "Exceptions.h"

//All the exception return values for what() - error messages
const char *WRONG_FILE_TYPE::what() const throw() {
    return "This is not a BMP file\n";
}

const char *FILE_READ_ERROR::what() const throw() {
    return "Something went wrong when reading the input file\n";
}

const char *PIXEL_DATA_MISMATCH::what() const noexcept {
    return "Error reading pixel data - wrong pixel count\n";
}