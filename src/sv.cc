#include "sv.hh"

#include <filesystem>
#include <type_traits>

#include "fmt/format.h"
#include "slang/compilation/Compilation.h"
#include "slang/parsing/Preprocessor.h"
#include "slang/symbols/ASTVisitor.h"
#include "slang/symbols/CompilationUnitSymbols.h"
#include "slang/symbols/Symbol.h"
#include "slang/syntax/SyntaxTree.h"
#include "slang/text/SourceManager.h"

namespace fs = std::filesystem;

// visitor to visit all the symbols if possible
class SymbolVisitor : public slang::ASTVisitor<SymbolVisitor, true, false> {
public:
    SymbolVisitor(slang::Compilation &compilation, baldur::Symbol &root)
        : compilation_(compilation), root_(root) {}


    void handle(const slang::ExpressionStatement &stmt) {
        process(stmt);
    }

    void handle(const slang::ConditionalStatement &stmt) {
        process(stmt);
        visitDefault(stmt);
    }

    void handle(const slang::ContinuousAssignSymbol &symbol) {
        //auto const &stmt = symbol.location
    }

    void handle(const slang::InstanceSymbol &symbol) {
        visitDefault(symbol);
    }


private:
    void process(const slang::Statement &statement) {
        auto const &start = statement.sourceRange.start();
        auto start_filename = compilation_.getSourceManager()->getFileName(start);
        auto start_line = compilation_.getSourceManager()->getLineNumber(start);
        printf("%s %ld\n", start_filename.begin(), start_line);
    }

    slang::Compilation &compilation_;
    baldur::Symbol &root_;
};

namespace baldur {
std::unique_ptr<Symbol> parse_sv(const std::vector<std::string> &files,
                                 const std::vector<std::string> &defines,
                                 const std::vector<std::string> &includes) {
    slang::SourceManager source_manager;
    slang::PreprocessorOptions ppoptions;
    ppoptions.predefines = defines;

    for (auto const &dirname : includes) {
        if (!fs::exists(dirname)) {
            throw std::runtime_error(fmt::format("{0} does not exists", dirname));
        }
        source_manager.addUserDirectory(std::string_view(dirname));
    }

    std::vector<slang::SourceBuffer> buffers;
    buffers.reserve(files.size());
    for (auto const &filename : files) {
        if (!fs::exists(filename)) {
            throw std::runtime_error(fmt::format("{0} does not exists", filename));
        }
        auto buffer = source_manager.readSource(std::string_view(filename));
        if (!buffer) {
            throw std::runtime_error("Unable to read " + filename);
        }
        buffers.emplace_back(buffer);
    }

    slang::Bag options;
    options.set(ppoptions);
    slang::Compilation compilation(options);
    for (auto const &buffer : buffers) {
        compilation.addSyntaxTree(slang::SyntaxTree::fromBuffer(buffer, source_manager, options));
    }

    // now construct the symbol tree
    auto const &root = compilation.getRoot();
    auto result = std::make_unique<Symbol>();
    SymbolVisitor visitor(compilation, *result);
    root.visit(visitor);
    return result;
}
}  // namespace baldur