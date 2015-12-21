// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

#include <compiler/line_splicer.h>
#include <compiler/member_state_function.h>

namespace compiler {

class trigraph_processor {
private:
    using state = member_state_function<trigraph_processor, code_point>;
    state m_state;
    line_splicer& m_output;

public:
    explicit trigraph_processor(line_splicer&);

    void process(code_point);

    void enable();
    void disable();

private:
    state question_mark_question_mark(code_point);
    state question_mark(code_point);
    state start(code_point);
    state disabled(code_point);
};

} // namespace compiler {
