#pragma once

#include "filter.h"

class NegativeFilter : public Filter {
public:
    NegativeFilter() = default;
    
    void Apply(Image& image) const override;
    const char* GetName() const override;
};