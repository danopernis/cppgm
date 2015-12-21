// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#include <compiler/code_point.h>

#include <algorithm>

namespace compiler {

namespace {

struct code_point_range {
    code_point_range(code_point lo_, code_point hi_) : lo(lo_), hi(hi_) { }

    bool operator()(code_point c) const { return lo <= c && c <= hi; }

private:
    const code_point lo;
    const code_point hi;
};

const auto annex_E1 = {
    code_point_range(0x000A8, 0x000A8),
    code_point_range(0x000AA, 0x000AA),
    code_point_range(0x000AD, 0x000AD),
    code_point_range(0x000AF, 0x000AF),
    code_point_range(0x000B2, 0x000B5),
    code_point_range(0x000B7, 0x000BA),
    code_point_range(0x000BC, 0x000BE),
    code_point_range(0x000C0, 0x000D6),
    code_point_range(0x000D8, 0x000F6),
    code_point_range(0x000F8, 0x000FF),
    code_point_range(0x00100, 0x0167F),
    code_point_range(0x01681, 0x0180D),
    code_point_range(0x0180F, 0x01FFF),
    code_point_range(0x0200B, 0x0200D),
    code_point_range(0x0202A, 0x0202E),
    code_point_range(0x0203F, 0x02040),
    code_point_range(0x02054, 0x02054),
    code_point_range(0x02060, 0x0206F),
    code_point_range(0x02070, 0x0218F),
    code_point_range(0x02460, 0x024FF),
    code_point_range(0x02776, 0x02793),
    code_point_range(0x02C00, 0x02DFF),
    code_point_range(0x02E80, 0x02FFF),
    code_point_range(0x03004, 0x03007),
    code_point_range(0x03021, 0x0302F),
    code_point_range(0x03031, 0x0303F),
    code_point_range(0x03040, 0x0D7FF),
    code_point_range(0x0F900, 0x0FD3D),
    code_point_range(0x0FD40, 0x0FDCF),
    code_point_range(0x0FDF0, 0x0FE44),
    code_point_range(0x0FE47, 0x0FFFD),
    code_point_range(0x10000, 0x1FFFD),
    code_point_range(0x20000, 0x2FFFD),
    code_point_range(0x30000, 0x3FFFD),
    code_point_range(0x40000, 0x4FFFD),
    code_point_range(0x50000, 0x5FFFD),
    code_point_range(0x60000, 0x6FFFD),
    code_point_range(0x70000, 0x7FFFD),
    code_point_range(0x80000, 0x8FFFD),
    code_point_range(0x90000, 0x9FFFD),
    code_point_range(0xA0000, 0xAFFFD),
    code_point_range(0xB0000, 0xBFFFD),
    code_point_range(0xC0000, 0xCFFFD),
    code_point_range(0xD0000, 0xDFFFD),
    code_point_range(0xE0000, 0xEFFFD),
};

const auto annex_E2 = {
    code_point_range(0x0300, 0x036F),
    code_point_range(0x1DC0, 0x1DFF),
    code_point_range(0x20D0, 0x20FF),
    code_point_range(0xFE20, 0xFE2F),
};

template<class RR>
bool is_annex(code_point c, const RR& rr)
{
    return std::any_of(begin(rr), end(rr), [&c] (const code_point_range& r) {
        return r(c);
    });
}

} // namespace {

bool is_whitespace(code_point c)
{
    switch (c) {
    case ' ': case '\t':
        return true;
    }
    return false;
}

bool is_non_digit(code_point c)
{
    switch (c) {
    case 'a': case 'b': case 'c': case 'd': case 'e':
    case 'f': case 'g': case 'h': case 'i': case 'j':
    case 'k': case 'l': case 'm': case 'n': case 'o':
    case 'p': case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x': case 'y':
    case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E':
    case 'F': case 'G': case 'H': case 'I': case 'J':
    case 'K': case 'L': case 'M': case 'N': case 'O':
    case 'P': case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X': case 'Y':
    case 'Z':
    case '_':
        return true;
    }
    return false;
}

bool is_digit(code_point c)
{
    switch (c) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return true;
    }
    return false;
}

bool is_identifier_head(code_point c)
{
    if (is_non_digit(c)) {
        return true;
    }
    if (is_annex(c, annex_E2)) {
        return false;
    }
    if (is_annex(c, annex_E1)) {
        return true;
    }
    return false;
}

bool is_identifier_tail(code_point c)
{
    if (is_non_digit(c)) {
        return true;
    }
    if (is_digit(c)) {
        return true;
    }
    if (is_annex(c, annex_E1)) {
        return true;
    }
    return false;
}

} // namespace compiler {
