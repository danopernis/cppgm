// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

#include <compiler/code_point.h>

namespace compiler {

class code_point_processor {
public:
    virtual ~code_point_processor() = default;

    virtual void process(code_point) = 0;
};

} // namespace compiler {
