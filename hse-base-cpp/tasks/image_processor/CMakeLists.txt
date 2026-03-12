cmake_minimum_required(VERSION 3.10)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 11)

# Добавляем директорию src для поища исходников
include_directories(src)

add_executable(image_processor
    image_processor.cpp
    src/color.cpp
    src/image.cpp
    src/loader.cpp
    src/filter.cpp
    src/crop_filter.cpp
    src/grayscale_filter.cpp
    src/negative_filter.cpp
    src/sharp_filter.cpp
)