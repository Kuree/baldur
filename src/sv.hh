#ifndef BALDUR_SV_HH
#define BALDUR_SV_HH
#include <vector>

#include "slang/compilation/Compilation.h"
#include "slang/text/SourceManager.h"
#include "symbol.hh"

namespace baldur {

class SystemVerilogInfo {
public:
    SystemVerilogInfo(const std::vector<std::string>& files,
                      const std::vector<std::string>& defines);

    void build_tables();

    const SymbolTable* get_symbol_table() const { return symbol_table_.get(); }
    const StatementTable* get_stmt_table() const { return stmt_table_.get(); }

private:
    std::unique_ptr<SymbolTable> symbol_table_;
    std::unique_ptr<StatementTable> stmt_table_;
    // slang component
    std::unique_ptr<slang::Compilation> compilation_;
};
}  // namespace baldur
#endif  // BALDUR_SV_HH
