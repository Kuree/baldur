#include "sv.hh"

#include <filesystem>
#include <type_traits>

#include "fmt/format.h"
#include "slang/parsing/Preprocessor.h"
#include "slang/symbols/ASTVisitor.h"
#include "slang/symbols/CompilationUnitSymbols.h"
#include "slang/symbols/Symbol.h"
#include "slang/syntax/SyntaxTree.h"

namespace fs = std::filesystem;

// visitor to visit all the symbols if possible
class SymbolVisitor : public slang::ASTVisitor<SymbolVisitor, true, false> {
public:
    explicit SymbolVisitor(slang::Compilation &compilation, baldur::SymbolTable &symbol_table,
                           baldur::StatementTable &stmt_table)
        : compilation_(compilation), symbol_table_(symbol_table), stmt_table_(stmt_table) {}

    void handle(const slang::ExpressionStatement &stmt) { process(stmt); }

    void handle(const slang::ConditionalStatement &stmt) {
        process(stmt);
        visitDefault(stmt);
    }

    void handle(const slang::ContinuousAssignSymbol &symbol) {
        // auto const &stmt = symbol.location
    }

    void handle(const slang::InstanceSymbol &symbol) {
        process(symbol);
        visitDefault(symbol);
    }

    void handle(const slang::PortSymbol &symbol) { process(symbol); }

    void handle(const slang::VariableSymbol &symbol) { process(symbol); }

private:
    void process(const slang::Symbol &symbol) {
        std::string path;
        symbol.getHierarchicalPath(path);
        symbol_table_.add_symbol(path, &symbol);
    }

    void process(const slang::Statement &statement) {
        auto const &start = statement.sourceRange.start();
        auto start_filename = compilation_.getSourceManager()->getFileName(start);
        auto start_line = compilation_.getSourceManager()->getLineNumber(start);
        stmt_table_.add_stmt(std::string(start_filename), start_line, &statement);
        printf("%s %ld\n", start_filename.begin(), start_line);
    }

    slang::Compilation &compilation_;
    baldur::SymbolTable &symbol_table_;
    baldur::StatementTable &stmt_table_;
};

namespace baldur {

SystemVerilogInfo::SystemVerilogInfo(const std::vector<std::string> &files,
                                     const std::vector<std::string> &defines) {
    symbol_table_ = std::make_unique<SymbolTable>();
    stmt_table_ = std::make_unique<StatementTable>();

    slang::PreprocessorOptions ppoptions;
    ppoptions.predefines = defines;

    slang::Bag options;
    options.set(ppoptions);
    compilation_ = std::make_unique<slang::Compilation>(options);
    for (auto const &filename : files) {
        if (!fs::exists(filename)) {
            throw std::runtime_error(fmt::format("{0} does not exists", filename));
        }

        compilation_->addSyntaxTree(slang::SyntaxTree::fromFile(filename));
    }
}

void SystemVerilogInfo::build_tables() {
    SymbolVisitor visitor(*compilation_, *symbol_table_, *stmt_table_);
    visitor.visit(compilation_->getRoot());
}

}  // namespace baldur