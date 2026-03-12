#pragma once

#include "filter.h"

class GrayscaleFilter : public Filter {
public:
    GrayscaleFilter() = default;
    
    void Apply(Image& image) const override;
    const char* GetName() const override;
};