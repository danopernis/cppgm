// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details

#include <compiler/line_splicer.h>
#include <compiler/token.h>
#include <compiler/tokenizer.h>
#include <compiler/trigraph_processor.h>
#include <compiler/universal_character_name_decoder.h>
#include <compiler/utf8_decoder.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>

class debug_token_processor final : public compiler::token_processor {
private:
    void write_token(const char* type, std::string data)
    {
        std::cout << type << " " << data.size() << " " << data << "\n";
    }

public:
    void process_whitespace_sequence() final
    {
        std::cout << "whitespace-sequence 0 \n";
    }

    void process_new_line() final
    {
        std::cout << "new-line 0 \n";
    }

    void process_header_name(std::string data) final
    {
        write_token("header-name", std::move(data));
    }

    void process_identifier(std::string data) final
    {
        write_token("identifier", std::move(data));
    }

    void process_pp_number(std::string data) final
    {
        write_token("pp-number", std::move(data));
    }

    void process_character_literal(std::string data) final
    {
        write_token("character-literal", std::move(data));
    }

    void process_user_defined_character_literal(std::string data) final
    {
        write_token("user-defined-character-literal", std::move(data));
    }

    void process_string_literal(std::string data) final
    {
        write_token("string-literal", std::move(data));
    }

    void process_user_defined_string_literal(std::string data) final
    {
        write_token("user-defined-string-literal", std::move(data));
    }

    void process_preprocessing_op_or_punc(compiler::OpOrPunc op_or_punc) final
    {
        write_token("preprocessing-op-or-punc", to_string(op_or_punc));
    }

    void process_non_whitespace_char(std::string data) final
    {
        write_token("non-whitespace-character", std::move(data));
    }

    void process_eof() final
    {
        std::cout << "eof\n";
    }
};

int main()
try {
    debug_token_processor token_processor;

    // phase 3
    auto tokenizer = compiler::make_tokenizer(token_processor);
    auto ucnd = compiler::make_universal_character_name_decoder(*tokenizer);
    // phase 2
    compiler::line_splicer line_splicer(*ucnd);
    // phase 1
    compiler::trigraph_processor trigraph_processor{line_splicer};
    compiler::utf8_decoder ud{trigraph_processor};

    std::for_each(
        std::istreambuf_iterator<char>(std::cin),
        std::istreambuf_iterator<char>(),
        [&] (char c) { ud.process(c); });
    trigraph_processor.process(compiler::END_OF_FILE);

    return EXIT_SUCCESS;
}
catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
