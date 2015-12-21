// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#include <compiler/tokenizer.h>

#include <compiler/code_point.h>
#include <compiler/token.h>
#include "state_function.h"

#include <algorithm>
#include <string>
#include <stdexcept>

#if 0
#include <iostream>
#define DEBUG_STATE(c) std::cerr << "in state " << __FUNCTION__ << " got \'" << (char)c << "\'\n";
#define DEBUG_CONTEXT() std::cerr << "performing " << __FUNCTION__ << "\n";
#else
#define DEBUG_STATE(c)
#define DEBUG_CONTEXT()
#endif

namespace compiler {

namespace {

struct string_token_type {
    const char* string;
    OpOrPunc token_type;
};
const string_token_type string_to_token_type[] = {
    {"and",    OpOrPunc::ALT_AND},
    {"and_eq", OpOrPunc::ALT_BITAND_ASSIGN},
    {"bitand", OpOrPunc::ALT_BITAND},
    {"bitor",  OpOrPunc::ALT_BITOR},
    {"compl",  OpOrPunc::ALT_COMPL},
    {"delete", OpOrPunc::DELETE},
    {"new",    OpOrPunc::NEW},
    {"not",    OpOrPunc::ALT_NOT},
    {"not_eq", OpOrPunc::ALT_NOT_EQUAL},
    {"or",     OpOrPunc::ALT_OR},
    {"or_eq",  OpOrPunc::ALT_BITOR_ASSIGN},
    {"xor",    OpOrPunc::ALT_XOR},
    {"xor_eq", OpOrPunc::ALT_XOR_ASSIGN},
};
const auto string_to_token_type_begin = std::begin(string_to_token_type);
const auto string_to_token_type_end = std::end(string_to_token_type);

class context {
private:
    std::string buffer;
    token_processor& processor;

public:
    // TODO privatise this
    enum class include_stage {
        START,
        AFTER_NUMBER_SIGN,
        AFTER_INCLUDE,
        FINISH,
    };

    include_stage m_include_stage = include_stage::START;

    explicit context(token_processor& processor_)
        : processor(processor_)
    {
    }

    void header_name()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::FINISH;

        processor.process_header_name(std::move(buffer));
        buffer.clear();
    }

    void identifier()
    {
        DEBUG_CONTEXT();

        m_include_stage = [&] {
            if (m_include_stage == include_stage::AFTER_NUMBER_SIGN) {
                if (buffer == "include") {
                    return include_stage::AFTER_INCLUDE;
                }
            }
            return include_stage::FINISH;
        } ();

        const auto it = std::find_if(
            string_to_token_type_begin,
            string_to_token_type_end,
            [this] (const string_token_type& x) {
                return x.string == buffer;
            });
        if (it != string_to_token_type_end) {
            processor.process_preprocessing_op_or_punc(it->token_type);
        } else {
            processor.process_identifier(std::move(buffer));
        }
        buffer.clear();
    }

    void pp_number()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::FINISH;

        processor.process_pp_number(std::move(buffer));
        buffer.clear();
    }

    void character_literal()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::FINISH;

        processor.process_character_literal(std::move(buffer));
        buffer.clear();
    }

    void user_defined_character_literal()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::FINISH;

        processor.process_user_defined_character_literal(std::move(buffer));
        buffer.clear();
    }

    void string_literal()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::FINISH;

        processor.process_string_literal(std::move(buffer));
        buffer.clear();
    }

    void user_defined_string_literal()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::FINISH;

        processor.process_user_defined_string_literal(std::move(buffer));
        buffer.clear();
    }

    void preprocessing_op_or_punc(OpOrPunc op_or_punc)
    {
        DEBUG_CONTEXT();

        m_include_stage = [&] {
            if (m_include_stage == include_stage::START) {
                if (op_or_punc == OpOrPunc::NUMBER_SIGN || op_or_punc == OpOrPunc::ALT_NUMBER_SIGN) {
                    return include_stage::AFTER_NUMBER_SIGN;
                }
            }
            return include_stage::FINISH;
        } ();

        processor.process_preprocessing_op_or_punc(op_or_punc);
        buffer.clear();
    }

    void non_whitespace_char()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::FINISH;

        processor.process_non_whitespace_char(std::move(buffer));
        buffer.clear();
    }

    void whitespace_sequence()
    {
        DEBUG_CONTEXT();

        // m_include_stage = m_include_stage; // just to be clear

        processor.process_whitespace_sequence();
        buffer.clear();
    }

    void new_line()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::START;

        processor.process_new_line();
        buffer.clear();
    }

    void eof()
    {
        DEBUG_CONTEXT();

        m_include_stage = include_stage::FINISH;

        processor.process_eof();
        buffer.clear();
    }

    void emit(code_point c)
    {
        // utf8 encode

        if (c < 0x00000080) {
            buffer.push_back(static_cast<char>(0x00 | ((c >> 0 * 6) & 0x7f)));
            return;
        }
        if (c < 0x00000800) {
            buffer.push_back(static_cast<char>(0xc0 | ((c >> 1 * 6) & 0x1f)));
            buffer.push_back(static_cast<char>(0x80 | ((c >> 0 * 6) & 0x3f)));
            return;
        }
        if (c < 0x00010000) {
            buffer.push_back(static_cast<char>(0xe0 | ((c >> 2 * 6) & 0x0f)));
            buffer.push_back(static_cast<char>(0x80 | ((c >> 1 * 6) & 0x3f)));
            buffer.push_back(static_cast<char>(0x80 | ((c >> 0 * 6) & 0x3f)));
            return;
        }
        if (c < 0x00110000) {
            buffer.push_back(static_cast<char>(0xf0 | ((c >> 3 * 6) & 0x07)));
            buffer.push_back(static_cast<char>(0x80 | ((c >> 2 * 6) & 0x3f)));
            buffer.push_back(static_cast<char>(0x80 | ((c >> 1 * 6) & 0x3f)));
            buffer.push_back(static_cast<char>(0x80 | ((c >> 0 * 6) & 0x3f)));
            return;
        }
        throw std::runtime_error{"utf8encode"};
    }
};

using state = state_function<context&, code_point>;

state::type start;
state::type character_literal;
state::type string_literal;
state::type pp_number;
state::type percent;
state::type dot;
state::type comment_singleline;
state::type comment_multiline;

// Identifier

state identifier(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (is_identifier_tail(c)) {
        context.emit(c);
        return identifier;
    }
    context.identifier();
    return start(context, c);
}

// Raw string literal

state raw_string_quote_rest(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '\"') {
        context.emit(c);
        context.string_literal();
        return start;
    }
    context.emit(c);
    return raw_string_quote_rest;
}

state raw_string_quote(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '(') {
        context.emit(c);
        return raw_string_quote_rest;
    }
    context.emit(c);
    return raw_string_quote;
}

state raw_string_literal(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '\"') {
        context.emit(c);
        return raw_string_quote;
    }
    throw std::runtime_error{"not implemented"};
}

// Character or string literal

state letter_u_8(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '\"') {
        context.emit(c);
        return string_literal;
    }
    return identifier(context, c);
}

state letter_u(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '8') {
        context.emit(c);
        return letter_u_8;
    }
    if (c == '\'') {
        context.emit(c);
        return character_literal;
    }
    if (c == '\"') {
        context.emit(c);
        return string_literal;
    }
    return identifier(context, c);
}

state letter_LU(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '\'') {
        context.emit(c);
        return character_literal;
    }
    if (c == '\"') {
        context.emit(c);
        return string_literal;
    }
    return identifier(context, c);
}

state character_literal_user_defined(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (is_identifier_tail(c)) {
        context.emit(c);
        return character_literal_user_defined;
    }
    context.user_defined_character_literal();
    return start(context, c);
}

state character_literal_end(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (is_identifier_head(c)) {
        context.emit(c);
        return character_literal_user_defined;
    }
    context.character_literal();
    return start(context, c);
}

state character_literal_escape_sequence(context& context, code_point c)
{
    DEBUG_STATE(c);
    context.emit(c);
    return character_literal;
}

state character_literal(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '\'') {
        context.emit(c);
        return character_literal_end;
    }
    if (c == '\\') {
        context.emit(c);
        return character_literal_escape_sequence;
    }
    if (c == END_OF_FILE) {
        throw std::runtime_error{"partial character literal"};
    }
    context.emit(c);
    return character_literal;
}

state string_literal_user_defined(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (is_identifier_tail(c)) {
        context.emit(c);
        return string_literal_user_defined;
    }
    context.user_defined_string_literal();
    return start(context, c);
}

state string_literal_end(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (is_identifier_head(c)) {
        context.emit(c);
        return string_literal_user_defined;
    }
    context.string_literal();
    return start(context, c);
}

state string_literal_escape_sequence(context& context, code_point c)
{
    DEBUG_STATE(c);
    context.emit(c);
    return string_literal;
}

state string_literal(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '\"') {
        context.emit(c);
        return string_literal_end;
    }
    if (c == '\\') {
        context.emit(c);
        return string_literal_escape_sequence;
    }
    if (c == END_OF_FILE) {
        throw std::runtime_error{"partial string literal"};
    }
    context.emit(c);
    return string_literal;
}

// Preprocessing number

state pp_number_eE(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '+' || c == '-') {
        context.emit(c);
        return pp_number;
    }
    return pp_number(context, c);
}

state pp_number(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == 'e' || c == 'E') {
        context.emit(c);
        return pp_number_eE;
    }
    if (is_digit(c) || is_non_digit(c) || c == '.') {
        context.emit(c);
        return pp_number;
    }
    context.pp_number();
    return start(context, c);
}

// Possibly multi-character punctuation or operation

state plus(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::PLUS_ASSIGN);
        return start;
    }
    if (c == '+') {
        context.preprocessing_op_or_punc(OpOrPunc::PLUS_PLUS);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::PLUS);
    return start(context, c);
}

state minus_greater_than(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '*') {
        context.preprocessing_op_or_punc(OpOrPunc::ARROW_STAR);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::ARROW);
    return start(context, c);
}

state minus(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::MINUS_ASSIGN);
        return start;
    }
    if (c == '-') {
        context.preprocessing_op_or_punc(OpOrPunc::MINUS_MINUS);
        return start;
    }
    if (c == '>') {
        return minus_greater_than;
    }
    context.preprocessing_op_or_punc(OpOrPunc::MINUS);
    return start(context, c);
}

state asterisk(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::STAR_ASSIGN);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::STAR);
    return start(context, c);
}

state caret(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::XOR_ASSIGN);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::XOR);
    return start(context, c);
}

state ampersand(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::BITAND_ASSIGN);
        return start;
    }
    if (c == '&') {
        context.preprocessing_op_or_punc(OpOrPunc::AND);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::BITAND);
    return start(context, c);
}

state vertical_bar(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::BITOR_ASSIGN);
        return start;
    }
    if (c == '|') {
        context.preprocessing_op_or_punc(OpOrPunc::OR);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::BITOR);
    return start(context, c);
}

state exclamation_mark(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::NOT_EQUAL);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::NOT);
    return start(context, c);
}

state equals(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::EQUAL);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::ASSIGN);
    return start(context, c);
}

state colon(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '>') {
        context.preprocessing_op_or_punc(OpOrPunc::ALT_BRACKET_RIGHT);
        return start;
    }
    if (c == ':') {
        context.preprocessing_op_or_punc(OpOrPunc::COLON_COLON);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::COLON);
    return start(context, c);
}

state percent_colon_percent(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == ':') {
        context.preprocessing_op_or_punc(OpOrPunc::ALT_NUMBER_SIGN_NUMBER_SIGN);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::ALT_NUMBER_SIGN);
    return percent(context, c);
}

state percent_colon(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '%') {
        return percent_colon_percent;
    }
    context.preprocessing_op_or_punc(OpOrPunc::ALT_NUMBER_SIGN);
    return start(context, c);
}

state percent(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '>') {
        context.preprocessing_op_or_punc(OpOrPunc::ALT_BRACE_RIGHT);
        return start;
    }
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::PERCENT_ASSIGN);
        return start;
    }
    if (c == ':') {
        return percent_colon;
    }
    context.preprocessing_op_or_punc(OpOrPunc::PERCENT);
    return start(context, c);
}

state dot_dot(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '.') {
        context.preprocessing_op_or_punc(OpOrPunc::DOT_DOT_DOT);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::DOT);
    return dot(context, c);
}

state dot(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '.') {
        return dot_dot;
    }
    if (c == '*') {
        context.preprocessing_op_or_punc(OpOrPunc::DOT_STAR);
        return start;
    }
    if (is_digit(c)) {
        context.emit('.');
        context.emit(c);
        return pp_number;
    }
    context.preprocessing_op_or_punc(OpOrPunc::DOT);
    return start(context, c);
}

state less_than_colon_colon(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == ':' || c == '>') {
        context.preprocessing_op_or_punc(OpOrPunc::ALT_BRACKET_LEFT);
        return colon(context, c);
    }
    context.preprocessing_op_or_punc(OpOrPunc::LESS_THAN);
    context.preprocessing_op_or_punc(OpOrPunc::COLON_COLON);
    return start(context, c);
}

state less_than_colon(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == ':') {
        return less_than_colon_colon;
    }
    context.preprocessing_op_or_punc(OpOrPunc::ALT_BRACKET_LEFT);
    return start(context, c);
}

state less_than_less_than(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::SHIFT_LEFT_ASSIGN);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::SHIFT_LEFT);
    return start(context, c);
}

state less_than(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '%') {
        context.preprocessing_op_or_punc(OpOrPunc::ALT_BRACE_LEFT);
        return start;
    }
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::LESS_THAN_EQUAL);
        return start;
    }
    if (c == ':') {
        return less_than_colon;
    }
    if (c == '<') {
        return less_than_less_than;
    }
    context.preprocessing_op_or_punc(OpOrPunc::LESS_THAN);
    return start(context, c);
}

state greater_than_greater_than(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::SHIFT_RIGHT_ASSIGN);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::SHIFT_RIGHT);
    return start(context, c);
}

state greater_than(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '>') {
        return greater_than_greater_than;
    }
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::GREATER_THAN_EQUAL);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::GREATER_THAN);
    return start(context, c);
}

state slash(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '/') {
        return comment_singleline;
    }
    if (c == '*') {
        return comment_multiline;
    }
    if (c == '=') {
        context.preprocessing_op_or_punc(OpOrPunc::SLASH_ASSIGN);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::SLASH);
    return start(context, c);
}

state number_sign(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '#') {
        context.preprocessing_op_or_punc(OpOrPunc::NUMBER_SIGN_NUMBER_SIGN);
        return start;
    }
    context.preprocessing_op_or_punc(OpOrPunc::NUMBER_SIGN);
    return start(context, c);
}

state header_name_h(context& context, code_point c)
{
    DEBUG_STATE(c);
    context.emit(c);
    if (c == '>') {
        context.header_name();
        return start;
    }
    if (c == '\n') {
        throw std::runtime_error{"newline in h-chars"};
    }
    return header_name_h;
}

state header_name_q(context& context, code_point c)
{
    DEBUG_STATE(c);
    context.emit(c);
    if (c == '\"') {
        context.header_name();
        return start;
    }
    if (c == '\n') {
        throw std::runtime_error{"newline in q-chars"};
    }
    return header_name_q;
}

state whitespace_slash(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '/') {
        return comment_singleline;
    }
    if (c == '*') {
        return comment_multiline;
    }
    context.whitespace_sequence();
    return slash(context, c);
}

state whitespace(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (is_whitespace(c)) {
        return whitespace;
    }
    if (c == '/') {
        return whitespace_slash;
    }
    if (c == '\n') {
        context.whitespace_sequence();
        context.new_line();
        return start;
    }
    context.whitespace_sequence();
    return start(context, c);
}

state comment_singleline(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '\n') {
        context.whitespace_sequence();
        context.new_line();
        return start;
    }
    return comment_singleline;
}

state comment_multiline_end(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '/') {
        return whitespace;
    }
    return comment_multiline(context, c);
}

state comment_multiline(context&, code_point c)
{
    DEBUG_STATE(c);
    if (c == '*') {
        return comment_multiline_end;
    }
    if (c == END_OF_FILE) {
        throw std::runtime_error{"partial comment"};
    }
    return comment_multiline;
}

// Main dispatch

state start(context& context, code_point c)
{
    DEBUG_STATE(c);
    if (c == '\n') {
        context.new_line();
        return start;
    }
    if (is_whitespace(c)) {
        return whitespace;
    }
    if (c == '#') {
        return number_sign;
    }
    if (c == '<') {
        if (context.m_include_stage == context::include_stage::AFTER_INCLUDE) {
            context.emit(c);
            return header_name_h;
        } else {
            return less_than;
        }
    }
    if (c == '\"') {
        if (context.m_include_stage == context::include_stage::AFTER_INCLUDE) {
            context.emit(c);
            return header_name_q;
        } else {
            context.emit(c);
            return string_literal;
        }
    }
    if (c == '/') {
        return slash;
    }
    if (c == END_OF_FILE) {
        context.eof();
        return start;
    }
    if (c == '{') {
        context.preprocessing_op_or_punc(OpOrPunc::BRACE_LEFT);
        return start;
    }
    if (c == '}') {
        context.preprocessing_op_or_punc(OpOrPunc::BRACE_RIGHT);
        return start;
    }
    if (c == '[') {
        context.preprocessing_op_or_punc(OpOrPunc::BRACKET_LEFT);
        return start;
    }
    if (c == ']') {
        context.preprocessing_op_or_punc(OpOrPunc::BRACKET_RIGHT);
        return start;
    }
    if (c == '(') {
        context.preprocessing_op_or_punc(OpOrPunc::PARENTHESES_LEFT);
        return start;
    }
    if (c == ')') {
        context.preprocessing_op_or_punc(OpOrPunc::PARENTHESES_RIGHT);
        return start;
    }
    if (c == ';') {
        context.preprocessing_op_or_punc(OpOrPunc::SEMICOLON);
        return start;
    }
    if (c == '?') {
        context.preprocessing_op_or_punc(OpOrPunc::QUESTION_MARK);
        return start;
    }
    if (c == '~') {
        context.preprocessing_op_or_punc(OpOrPunc::COMPL);
        return start;
    }
    if (c == ',') {
        context.preprocessing_op_or_punc(OpOrPunc::COMMA);
        return start;
    }
    if (c == ':') {
        return colon;
    }
    if (c == '%') {
        return percent;
    }
    if (c == '.') {
        return dot;
    }
    if (c == '+') {
        return plus;
    }
    if (c == '-') {
        return minus;
    }
    if (c == '*') {
        return asterisk;
    }
    if (c == '^') {
        return caret;
    }
    if (c == '&') {
        return ampersand;
    }
    if (c == '|') {
        return vertical_bar;
    }
    if (c == '!') {
        return exclamation_mark;
    }
    if (c == '=') {
        return equals;
    }
    if (c == '>') {
        return greater_than;
    }

    context.emit(c);
    if (c == '\'') {
        return character_literal;
    }
    if (c == 'u') {
        return letter_u;
    }
    if (c == 'L' || c == 'U') {
        return letter_LU;
    }
    if (c == 'R') {
        return raw_string_literal;
    }
    if (is_identifier_head(c)) {
        return identifier;
    }
    if (is_digit(c)) {
        return pp_number;
    }
    context.non_whitespace_char();
    return start;
}

class tokenizer final : public code_point_processor {
private:
    state m_state;
    context m_context;

public:
    explicit tokenizer(token_processor& processor)
        : m_state(start)
        , m_context(processor)
    {
    }

    void process(code_point c) final
    {
        m_state = (*m_state)(m_context, c);
    }
};

} // namespace {

code_point_processor_ptr make_tokenizer(token_processor& output)
{
    return std::make_unique<tokenizer>(output);
}

} // namespace compiler {
