// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

#include <string>

namespace compiler {

// [lex.operators]
enum class OpOrPunc {
    ALT_AND,                     // and
    ALT_BITAND,                  // bitand
    ALT_BITAND_ASSIGN,           // and_eq
    ALT_BITOR,                   // bitor
    ALT_BITOR_ASSIGN,            // or_eq
    ALT_BRACE_LEFT,              // <%
    ALT_BRACE_RIGHT,             // %>
    ALT_BRACKET_LEFT,            // <:
    ALT_BRACKET_RIGHT,           // :>
    ALT_COMPL,                   // compl
    ALT_NOT,                     // not
    ALT_NOT_EQUAL,               // not_eq
    ALT_NUMBER_SIGN,             // %:
    ALT_NUMBER_SIGN_NUMBER_SIGN, // %:%:
    ALT_OR,                      // or
    ALT_XOR,                     // xor
    ALT_XOR_ASSIGN,              // xor_eq
    AND,                         // &&
    ARROW,                       // ->
    ARROW_STAR,                  // ->*
    ASSIGN,                      // =
    BITAND,                      // &
    BITAND_ASSIGN,               // &=
    BITOR,                       // |
    BITOR_ASSIGN,                // |=
    BRACE_LEFT,                  // {
    BRACE_RIGHT,                 // }
    BRACKET_LEFT,                // [
    BRACKET_RIGHT,               // ]
    COLON,                       // :
    COLON_COLON,                 // ::
    COMMA,                       // ,
    COMPL,                       // ~
    DELETE,                      // delete
    DOT,                         // .
    DOT_DOT_DOT,                 // ...
    DOT_STAR,                    // .*
    EQUAL,                       // ==
    GREATER_THAN,                // >
    GREATER_THAN_EQUAL,          // >=
    LESS_THAN,                   // <
    LESS_THAN_EQUAL,             // <=
    MINUS,                       // -
    MINUS_ASSIGN,                // -=
    MINUS_MINUS,                 // --
    NEW,                         // new
    NOT,                         // !
    NOT_EQUAL,                   // !=
    NUMBER_SIGN,                 // #
    NUMBER_SIGN_NUMBER_SIGN,     // ##
    OR,                          // ||
    PARENTHESES_LEFT,            // (
    PARENTHESES_RIGHT,           // )
    PERCENT,                     // %
    PERCENT_ASSIGN,              // %=
    PLUS,                        // +
    PLUS_ASSIGN,                 // +=
    PLUS_PLUS,                   // ++
    QUESTION_MARK,               // ?
    SEMICOLON,                   // ;
    SHIFT_LEFT,                  // <<
    SHIFT_LEFT_ASSIGN,           // <<=
    SHIFT_RIGHT,                 // >>
    SHIFT_RIGHT_ASSIGN,          // >>=
    SLASH,                       // /
    SLASH_ASSIGN,                // /=
    STAR,                        // *
    STAR_ASSIGN,                 // *=
    XOR,                         // ^
    XOR_ASSIGN,                  // ^=
};

const char* to_string(OpOrPunc);

} // namespace compiler {
