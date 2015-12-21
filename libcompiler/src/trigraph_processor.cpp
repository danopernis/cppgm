// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#include <compiler/trigraph_processor.h>

#include <cassert>

namespace compiler {

trigraph_processor::trigraph_processor(line_splicer& output)
    : m_state(&trigraph_processor::start)
    , m_output(output)
{
}

void trigraph_processor::process(code_point c)
{
    m_state = (this->*m_state)(c);
}

void trigraph_processor::enable()
{
    assert(m_state == &trigraph_processor::disabled);
    m_state = &trigraph_processor::start;
}

void trigraph_processor::disable()
{
    assert(m_state == &trigraph_processor::start);
    m_state = &trigraph_processor::disabled;
}

trigraph_processor::state trigraph_processor::question_mark_question_mark(code_point c)
{
    if (c == U'=') {
        m_output.process(U'#');
        return &trigraph_processor::start;
    }
    if (c == U'/') {
        m_output.process(U'\\');
        return &trigraph_processor::start;
    }
    if (c == U'\'') {
        m_output.process(U'^');
        return &trigraph_processor::start;
    }
    if (c == U'(') {
        m_output.process(U'[');
        return &trigraph_processor::start;
    }
    if (c == U')') {
        m_output.process(U']');
        return &trigraph_processor::start;
    }
    if (c == U'!') {
        m_output.process(U'|');
        return &trigraph_processor::start;
    }
    if (c == U'<') {
        m_output.process(U'{');
        return &trigraph_processor::start;
    }
    if (c == U'>') {
        m_output.process(U'}');
        return &trigraph_processor::start;
    }
    if (c == U'-') {
        m_output.process(U'~');
        return &trigraph_processor::start;
    }
    m_output.process(U'?');
    return question_mark(c);
}

trigraph_processor::state trigraph_processor::question_mark(code_point c)
{
    if (c == U'?') {
        return &trigraph_processor::question_mark_question_mark;
    }
    m_output.process(U'?');
    m_output.process(c);
    return &trigraph_processor::start;
}

trigraph_processor::state trigraph_processor::start(code_point c)
{
    if (c == U'?') {
        return &trigraph_processor::question_mark;
    }
    m_output.process(c);
    return &trigraph_processor::start;
}

trigraph_processor::state trigraph_processor::disabled(code_point c)
{
    m_output.process(c);
    return &trigraph_processor::disabled;
}

} // namespace compiler {
