// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

#include <compiler/trigraph_processor.h>
#include <compiler/member_state_function.h>

namespace compiler {

class utf8_decoder {
private:
    using state = member_state_function<utf8_decoder, char>;
    state m_state;
    code_point m_code_point;
    trigraph_processor& m_output;

public:
    explicit utf8_decoder(trigraph_processor&);

    void process(char);

private:
    state start(char);
    state expect1(char);
    state expect2(char);
    state expect3(char);
    state error(char);
};

} // namespace compiler {
