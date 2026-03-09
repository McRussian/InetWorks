#pragma once

#include "filter.h"

class CropFilter : public Filter {
private:
    int width_;
    int height_;
    
public:
    CropFilter(int width, int height);
    
    void Apply(Image& image) const override;
    const char* GetName() const override;
};