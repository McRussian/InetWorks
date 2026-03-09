#include "color.h"

Color::Color() : r_(0), g_(0), b_(0) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b) : r_(r), g_(g), b_(b) {}

uint8_t Color::GetR() const { return r_; }
uint8_t Color::GetG() const { return g_; }
uint8_t Color::GetB() const { return b_; }

void Color::SetR(uint8_t r) { r_ = r; }
void Color::SetG(uint8_t g) { g_ = g; }
void Color::SetB(uint8_t b) { b_ = b; }

void Color::SetRGB(uint8_t r, uint8_t g, uint8_t b) {
    r_ = r;
    g_ = g;
    b_ = b;
}