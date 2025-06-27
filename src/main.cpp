#include "code_generator/codegen.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "semantic_analyzer/semantic_analyzer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

#include "magic_enum.hpp"

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

void compile_c_output(const std::string& filename, const std::string& output_name = "a.out") {
    std::string cmd = std::format("gcc {} -o {}", filename, output_name);
    int result = std::system(cmd.c_str());

    if (result != 0) {
        std::cerr << "Compilation failed.\n";
    } else {
        std::cout << "Compiled successfully to: " << output_name << "\n";
    }
}

void generate_c(const std::string& source) {
    Lexer lexer(std::move(source)); // Lexer owns the source string
    auto& tokens = lexer.lex();

    for (auto& token : tokens) {
        std::cout << token.lexeme << " <=> " << magic_enum::enum_name(token.kind) << "\n";
    }

    Parser parser(std::move(tokens));
    auto statements = parser.parse();
    // for (auto& statement : statements) {
    //     statement->print();
    //     std::cout << "\n";
    // }

    SemanticAnalyzer analyzer(statements);
    if (!analyzer.analyze()) {
        CodeGenerator codegen(statements, analyzer);
        codegen.gen();
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source> <optional-output>\n";
        return 1;
    }

    std::string source = file_to_strbuf(argv[1]);
    generate_c(source);

    if (argc > 2) {
        compile_c_output("../c_code/generated.c", argv[2]);
    } else {
        compile_c_output("../c_code/generated.c", "exe");
    }

    return 0;
}
