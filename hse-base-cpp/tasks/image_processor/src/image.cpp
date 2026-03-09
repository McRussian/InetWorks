#include "image.h"

Image::Image() : width_(0), height_(0) {}

Image::Image(int width, int height) : width_(width), height_(height) {
    pixels_.resize(height, std::vector<Color>(width));
}

int Image::GetWidth() const { return width_; }
int Image::GetHeight() const { return height_; }

bool Image::IsEmpty() const {
    return width_ == 0 || height_ == 0;
}

Color Image::GetPixel(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return Color();
    }
    return pixels_[y][x];
}

void Image::SetPixel(int x, int y, const Color& c) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return;
    }
    pixels_[y][x] = c;
}

void Image::Resize(int newWidth, int newHeight) {
    width_ = newWidth;
    height_ = newHeight;
    pixels_.resize(newHeight, std::vector<Color>(newWidth));
}

void Image::Fill(const Color& c) {
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            pixels_[y][x] = c;
        }
    }
}