// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#include <compiler/token.h>

#include <exception>

namespace compiler {

const char* to_string(OpOrPunc op_or_punc)
{
    switch (op_or_punc) {
    case OpOrPunc::ALT_AND:                     return "and";
    case OpOrPunc::ALT_BITAND:                  return "bitand";
    case OpOrPunc::ALT_BITAND_ASSIGN:           return "and_eq";
    case OpOrPunc::ALT_BITOR:                   return "bitor";
    case OpOrPunc::ALT_BITOR_ASSIGN:            return "or_eq";
    case OpOrPunc::ALT_BRACE_LEFT:              return "<%";
    case OpOrPunc::ALT_BRACE_RIGHT:             return "%>";
    case OpOrPunc::ALT_BRACKET_LEFT:            return "<:";
    case OpOrPunc::ALT_BRACKET_RIGHT:           return ":>";
    case OpOrPunc::ALT_COMPL:                   return "compl";
    case OpOrPunc::ALT_NOT:                     return "not";
    case OpOrPunc::ALT_NOT_EQUAL:               return "not_eq";
    case OpOrPunc::ALT_NUMBER_SIGN:             return "%:";
    case OpOrPunc::ALT_NUMBER_SIGN_NUMBER_SIGN: return "%:%:";
    case OpOrPunc::ALT_OR:                      return "or";
    case OpOrPunc::ALT_XOR:                     return "xor";
    case OpOrPunc::ALT_XOR_ASSIGN:              return "xor_eq";
    case OpOrPunc::AND:                         return "&&";
    case OpOrPunc::ARROW:                       return "->";
    case OpOrPunc::ARROW_STAR:                  return "->*";
    case OpOrPunc::ASSIGN:                      return "=";
    case OpOrPunc::BITAND:                      return "&";
    case OpOrPunc::BITAND_ASSIGN:               return "&=";
    case OpOrPunc::BITOR:                       return "|";
    case OpOrPunc::BITOR_ASSIGN:                return "|=";
    case OpOrPunc::BRACE_LEFT:                  return "{";
    case OpOrPunc::BRACE_RIGHT:                 return "}";
    case OpOrPunc::BRACKET_LEFT:                return "[";
    case OpOrPunc::BRACKET_RIGHT:               return "]";
    case OpOrPunc::COLON:                       return ":";
    case OpOrPunc::COLON_COLON:                 return "::";
    case OpOrPunc::COMMA:                       return ",";
    case OpOrPunc::COMPL:                       return "~";
    case OpOrPunc::DELETE:                      return "delete";
    case OpOrPunc::DOT:                         return ".";
    case OpOrPunc::DOT_DOT_DOT:                 return "...";
    case OpOrPunc::DOT_STAR:                    return ".*";
    case OpOrPunc::EQUAL:                       return "==";
    case OpOrPunc::GREATER_THAN:                return ">";
    case OpOrPunc::GREATER_THAN_EQUAL:          return ">=";
    case OpOrPunc::LESS_THAN:                   return "<";
    case OpOrPunc::LESS_THAN_EQUAL:             return "<=";
    case OpOrPunc::MINUS:                       return "-";
    case OpOrPunc::MINUS_ASSIGN:                return "-=";
    case OpOrPunc::MINUS_MINUS:                 return "--";
    case OpOrPunc::NEW:                         return "new";
    case OpOrPunc::NOT:                         return "!";
    case OpOrPunc::NOT_EQUAL:                   return "!=";
    case OpOrPunc::NUMBER_SIGN:                 return "#";
    case OpOrPunc::NUMBER_SIGN_NUMBER_SIGN:     return "##";
    case OpOrPunc::OR:                          return "||";
    case OpOrPunc::PARENTHESES_LEFT:            return "(";
    case OpOrPunc::PARENTHESES_RIGHT:           return ")";
    case OpOrPunc::PERCENT:                     return "%";
    case OpOrPunc::PERCENT_ASSIGN:              return "%=";
    case OpOrPunc::PLUS:                        return "+";
    case OpOrPunc::PLUS_ASSIGN:                 return "+=";
    case OpOrPunc::PLUS_PLUS:                   return "++";
    case OpOrPunc::QUESTION_MARK:               return "?";
    case OpOrPunc::SEMICOLON:                   return ";";
    case OpOrPunc::SHIFT_LEFT:                  return "<<";
    case OpOrPunc::SHIFT_LEFT_ASSIGN:           return "<<=";
    case OpOrPunc::SHIFT_RIGHT:                 return ">>";
    case OpOrPunc::SHIFT_RIGHT_ASSIGN:          return ">>=";
    case OpOrPunc::SLASH:                       return "/";
    case OpOrPunc::SLASH_ASSIGN:                return "/=";
    case OpOrPunc::STAR:                        return "*";
    case OpOrPunc::STAR_ASSIGN:                 return "*=";
    case OpOrPunc::XOR:                         return "^";
    case OpOrPunc::XOR_ASSIGN:                  return "^=";
    }
    std::terminate();
}

} // namespace compiler {
