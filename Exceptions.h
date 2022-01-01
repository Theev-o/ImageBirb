//
// Created by Michał Kuć on 01/01/2022.
//

#ifndef IMAGEBIRB_EXCEPTIONS_H
#define IMAGEBIRB_EXCEPTIONS_H

#include <iostream>
#include <exception>

struct WRONG_FILE_TYPE : public std::exception{
    [[nodiscard]] const char * what () const noexcept override;
};

struct FILE_READ_ERROR: public std::exception{
    [[nodiscard]] const char * what () const noexcept override;
};

struct PIXEL_DATA_MISMATCH: public std::exception{
    [[nodiscard]] const char * what () const noexcept override;
};


#endif //IMAGEBIRB_EXCEPTIONS_H
