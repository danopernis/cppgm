// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

#include <compiler/code_point_processor_ptr.h>
#include <compiler/token_processor.h>

namespace compiler {

code_point_processor_ptr make_tokenizer(token_processor&);

} // namespace compiler {
