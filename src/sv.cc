#include "sv.hh"

#include <filesystem>

#include "fmt/format.h"
#include "slang/compilation/Compilation.h"
#include "slang/parsing/Preprocessor.h"
#include "slang/syntax/SyntaxTree.h"
#include "slang/text/SourceManager.h"

namespace fs = std::filesystem;

namespace baldur {
std::unique_ptr<Symbol> parse_sv(const std::vector<std::string>& files,
                                 const std::vector<std::string>& defines,
                                 const std::vector<std::string>& includes) {
    // stack-allocated source manager seems to get segfault sometimes
    // with gcc-9+ when destructor is called
    slang::SourceManager source_manager;
    // macro
    slang::PreprocessorOptions ppoptions;
    ppoptions.predefines = defines;
    ppoptions.predefineSource = "<command-line>";

    for (auto const& dirname : includes) {
        if (!fs::exists(dirname)) {
            throw std::runtime_error(fmt::format("{0} does not exists", dirname));
        }
        source_manager.addUserDirectory(std::string_view(dirname));
    }

    std::vector<slang::SourceBuffer> buffers;
    buffers.reserve(files.size());
    for (auto const& filename : files) {
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
    for (auto const &buffer: buffers) {
        compilation.addSyntaxTree(slang::SyntaxTree::fromBuffer(buffer, source_manager, options));
    }

    // now construct the symbol tree
    return nullptr;
}
}  // namespace baldur