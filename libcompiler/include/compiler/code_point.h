// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

namespace compiler {

using code_point = char32_t;

constexpr code_point END_OF_FILE = -1;

bool is_whitespace(code_point);
bool is_non_digit(code_point);
bool is_digit(code_point);
bool is_identifier_head(code_point);
bool is_identifier_tail(code_point);

} // namespace compiler {
