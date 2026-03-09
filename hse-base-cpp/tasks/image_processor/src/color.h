#pragma once

#include <cstdint>

class Color {
private:
    uint8_t r_, g_, b_;

public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    
    uint8_t GetR() const;
    uint8_t GetG() const;
    uint8_t GetB() const;
    
    void SetR(uint8_t r);
    void SetG(uint8_t g);
    void SetB(uint8_t b);
    void SetRGB(uint8_t r, uint8_t g, uint8_t b);
};