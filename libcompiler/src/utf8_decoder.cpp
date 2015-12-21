// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#include <compiler/utf8_decoder.h>

#include <stdexcept>

namespace compiler {

utf8_decoder::utf8_decoder(trigraph_processor& m_output_)
    : m_state(&utf8_decoder::start)
    , m_output(m_output_)
{
}

void utf8_decoder::process(char c)
{
    m_state = (this->*m_state)(c);
    if (m_state == &utf8_decoder::error) {
        throw std::runtime_error("invalid unicode character");
    }
}

utf8_decoder::state utf8_decoder::start(char c)
{
    m_code_point = 0;
    if ((c & 0x80) == 0x00) {
        m_code_point |= (c & 0x7f) << 0;
        m_output.process(m_code_point);
        return &utf8_decoder::start;
    }
    if ((c & 0xe0) == 0xc0) {
        m_code_point |= (c & 0x1f) << 6;
        return &utf8_decoder::expect1;
    }
    if ((c & 0xf0) == 0xe0) {
        m_code_point |= (c & 0x0f) << 12;
        return &utf8_decoder::expect2;
    }
    if ((c & 0xf8) == 0xf0) {
        m_code_point |= (c & 0x07) << 18;
        return &utf8_decoder::expect3;
    }
    return &utf8_decoder::error;
}

utf8_decoder::state utf8_decoder::expect1(char c)
{
    if ((c & 0xc0) == 0x80) {
        m_code_point |= (c & 0x3f) << 0;
        m_output.process(m_code_point);
        return &utf8_decoder::start;
    }
    return &utf8_decoder::error;
}

utf8_decoder::state utf8_decoder::expect2(char c)
{
    if ((c & 0xc0) == 0x80) {
       m_code_point |= (c & 0x3f) << 6;
       return &utf8_decoder::expect1;
    }
    return &utf8_decoder::error;
}

utf8_decoder::state utf8_decoder::expect3(char c)
{
    if ((c & 0xc0) == 0x80) {
        m_code_point |= (c & 0x3f) << 12;
        return &utf8_decoder::expect2;
    }
    return &utf8_decoder::error;
}

utf8_decoder::state utf8_decoder::error(char)
{
    return &utf8_decoder::error;
}

} // namespace compiler {
