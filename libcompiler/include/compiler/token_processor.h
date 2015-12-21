// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

#include <compiler/token.h>
#include <string>

namespace compiler {

// [lex.pptoken]
class token_processor {
public:
    virtual ~token_processor() = default;

    virtual void process_whitespace_sequence() = 0;
    virtual void process_new_line() = 0;
    virtual void process_header_name(std::string) = 0;
    virtual void process_identifier(std::string) = 0;
    virtual void process_pp_number(std::string) = 0;
    virtual void process_character_literal(std::string) = 0;
    virtual void process_user_defined_character_literal(std::string) = 0;
    virtual void process_string_literal(std::string) = 0;
    virtual void process_user_defined_string_literal(std::string) = 0;
    virtual void process_preprocessing_op_or_punc(compiler::OpOrPunc) = 0;
    virtual void process_non_whitespace_char(std::string) = 0;
    virtual void process_eof() = 0;
};

} // namespace compiler {
