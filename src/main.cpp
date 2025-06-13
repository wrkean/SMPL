#include "visitor/ast_printer.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

// External library
#include <magic_enum/magic_enum.hpp>

std::string file_to_strbuf(const std::string& path) {
    std::ifstream source(path);
    if (!source.is_open()) {
        std::cerr << "Could not open file\n";
        return "";
    }

    std::stringstream buf;
    buf << source.rdbuf();

    return buf.str();
}

void run(const std::string& source) {
    Lexer lexer(std::move(source)); // Lexer owns the source string
    auto& tokens = lexer.lex();

    for (auto& token : tokens) {
        std::cout << token.lexeme << " <=> " << magic_enum::enum_name(token.kind) << "\n";
    }

    Parser parser(std::move(tokens));
    auto expr = parser.parse();
    ASTPrinter printer;
    printer.print(*expr);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source>\n";
        return 1;
    }

    std::string source = file_to_strbuf(argv[1]);
    run(source);

    return 0;
}
