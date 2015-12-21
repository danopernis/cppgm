// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

#include <compiler/code_point_processor.h>
#include <compiler/member_state_function.h>

namespace compiler {

class line_splicer {
private:
    using state = member_state_function<line_splicer, code_point>;
    state m_state;
    code_point_processor& m_output;

public:
    explicit line_splicer(code_point_processor&);

    void process(code_point);

    void enable();
    void disable();

private:
    state backslash(code_point);
    state newline(code_point);
    state start(code_point);
    state disabled(code_point);
};

} // namespace compiler {
