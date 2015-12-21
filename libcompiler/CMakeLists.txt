add_library(compiler
    src/code_point.cpp
    src/line_splicer.cpp
    src/token.cpp
    src/tokenizer.cpp
    src/trigraph_processor.cpp
    src/universal_character_name_decoder.cpp
    src/utf8_decoder.cpp
)

target_include_directories(compiler
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
)
