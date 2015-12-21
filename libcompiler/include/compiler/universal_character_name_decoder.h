// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

#include <compiler/code_point_processor.h>
#include <compiler/code_point_processor_ptr.h>

namespace compiler {

code_point_processor_ptr make_universal_character_name_decoder(code_point_processor&);

} // namespace compiler {
