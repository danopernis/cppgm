// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#include <compiler/line_splicer.h>

#include <cassert>

namespace compiler {

line_splicer::line_splicer(code_point_processor& output)
    : m_state(&line_splicer::newline)
    , m_output(output)
{
}

void line_splicer::process(code_point c)
{
    m_state = (this->*m_state)(c);
}

void line_splicer::enable()
{
    assert(m_state == &line_splicer::disabled);
    m_state = &line_splicer::start;
}

void line_splicer::disable()
{
    assert(m_state == &line_splicer::start);
    m_state = &line_splicer::disabled;
}

line_splicer::state line_splicer::backslash(code_point c)
{
    if (c == U'\n') {
        return &line_splicer::start;
    }
    m_output.process(U'\\');
    m_output.process(c);
    return &line_splicer::start;
}

line_splicer::state line_splicer::newline(code_point c)
{
    if (c == U'\\') {
        return &line_splicer::backslash;
    }
    if (c == U'\n') {
        m_output.process(U'\n');
        return &line_splicer::newline;
    }
    m_output.process(c);
    return &line_splicer::start;
}

line_splicer::state line_splicer::start(code_point c)
{
    if (c == U'\\') {
        return &line_splicer::backslash;
    }
    if (c == U'\n') {
        m_output.process(U'\n');
        return &line_splicer::newline;
    }
    if (c == END_OF_FILE) {
        m_output.process(U'\n');
        m_output.process(END_OF_FILE);
        return &line_splicer::start;
    }
    m_output.process(c);
    return &line_splicer::start;
}

line_splicer::state line_splicer::disabled(code_point c)
{
    m_output.process(c);
    return &line_splicer::disabled;
}

} // namespace compiler {
