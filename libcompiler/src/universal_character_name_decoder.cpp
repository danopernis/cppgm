// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#include <compiler/universal_character_name_decoder.h>

#include <array>

namespace compiler {

namespace {

inline bool isHexadecimal(int c)
{
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'A':
    case 'a':
    case 'B':
    case 'b':
    case 'C':
    case 'c':
    case 'D':
    case 'd':
    case 'E':
    case 'e':
    case 'F':
    case 'f':
        return true;
    }
    return false;
}

// given hex digit character c, return its value
inline int HexCharToValue(int c)
{
    switch (c) {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'A':
        return 10;
    case 'a':
        return 10;
    case 'B':
        return 11;
    case 'b':
        return 11;
    case 'C':
        return 12;
    case 'c':
        return 12;
    case 'D':
        return 13;
    case 'd':
        return 13;
    case 'E':
        return 14;
    case 'e':
        return 14;
    case 'F':
        return 15;
    case 'f':
        return 15;
    default:
        throw std::logic_error("HexCharToValue of nonhex char");
    }
}

class universal_character_name_decoder final : public code_point_processor {
private:
    enum class State {
        START,
        BACKSLASH,
        HEX_4,
        HEX_8,
    } state = State::START;
    std::array<int, 10> buffer;
    int n;
    code_point_processor& output;

public:
    explicit universal_character_name_decoder(code_point_processor& output_)
        : output(output_)
    {
    }

    void process(code_point c) final
    {
        switch (state) {
        case State::START:
            n = 0;
            if (c == '\\') {
                buffer[0] = c;
                state = State::BACKSLASH;
            } else {
                output.process(c);
            }
            break;
        case State::BACKSLASH:
            ++n;
            if (c == 'u') {
                buffer[n] = c;
                state = State::HEX_4;
            } else if (c == 'U') {
                buffer[n] = c;
                state = State::HEX_8;
            } else {
                for (int i = 0; i < n; ++i)
                    output.process(buffer[i]);
                state = State::START;
                process(c);
            }
            break;
        case State::HEX_4:
            ++n;
            if (isHexadecimal(c)) {
                buffer[n] = c;
                if (n == 5) {
                    int val = 0;
                    for (int i = 2; i < 6; ++i) {
                        val <<= 4;
                        val |= HexCharToValue(buffer[i]);
                    }
                    output.process(val);
                    state = State::START;
                }
            } else {
                for (int i = 0; i < n; ++i) {
                    output.process(buffer[i]);
                }
                state = State::START;
                process(c);
            }
            break;
        case State::HEX_8:
            ++n;
            if (isHexadecimal(c)) {
                buffer[n] = c;
                if (n == 9) {
                    int val = 0;
                    for (int i = 2; i < 10; ++i) {
                        val <<= 4;
                        val |= HexCharToValue(buffer[i]);
                    }
                    output.process(val);
                    state = State::START;
                }
            } else {
                for (int i = 0; i < n; ++i) {
                    output.process(buffer[i]);
                }
                state = State::START;
                process(c);
            }
            break;
        }
    }
};

} // namespace {

code_point_processor_ptr make_universal_character_name_decoder(code_point_processor& output)
{
    return std::make_unique<universal_character_name_decoder>(output);
}

} // namespace compiler {
