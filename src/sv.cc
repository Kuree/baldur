#include "sv.hh"

#include <filesystem>
#include <type_traits>

#include "fmt/format.h"
#include "slang/compilation/Compilation.h"
#include "slang/parsing/Preprocessor.h"
#include "slang/symbols/CompilationUnitSymbols.h"
#include "slang/symbols/Symbol.h"
#include "slang/syntax/SyntaxTree.h"
#include "slang/text/SourceManager.h"
#include "slang/symbols/ASTVisitor.h"

namespace fs = std::filesystem;

// visitor to visit all the symbols if possible
class SymbolVisitor {
public:
    SymbolVisitor(slang::Compilation &compilation, baldur::Symbol &root)
        : compilation_(compilation), root_(root) {}

    template <typename T>
    void visit(const T &elem) {
        // depends on the type
        if constexpr (std::is_base_of_v<slang::Statement, T>) {
        } else if constexpr (std::is_base_of_v<slang::Expression, T>) {
        } else if constexpr (std::is_base_of_v<slang::ValueSymbol, T>) {
        }
        else if  constexpr (std::is_base_of_v<slang::Scope, T>) {
            if (!elem.empty()) {
                for (auto const &mem: elem.members())
                    visit(mem);
            }
        }
    }

private:
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
    visitor.visit(root);
    return result;
}
}  // namespace baldur