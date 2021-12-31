cmake_minimum_required(VERSION 3.21)
project(ImageBirb)

set(CMAKE_CXX_STANDARD 23)

add_executable(ImageBirb main.cpp BitmapInfoHeader.cpp BitmapInfoHeader.h BitmapFileHeader.cpp BitmapFileHeader.h Scanner.cpp Scanner.h ImgData.cpp ImgData.h)
