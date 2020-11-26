#ifndef BALDUR_SYMBOL_HH
#define BALDUR_SYMBOL_HH

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// this is lightweight wrapper class around slang's symbol

namespace slang {
class Symbol;
class Statement;
}  // namespace slang

namespace baldur {

class SymbolTable {
public:
    void add_symbol(const std::string &path, const slang::Symbol* symbol);
    const slang::Symbol * get_symbol(const std::string &path) const;
    std::unordered_set<const slang::Symbol*> get_symbols(const std::string &path) const;

private:
    using ScopeTable = std::unordered_map<std::string, std::shared_ptr<SymbolTable>>;
    std::unordered_map<std::string, const slang::Symbol*> symbols_;
    ScopeTable child_scopes_;

    void add_symbol(const std::vector<std::string>::iterator &begin,
                    const std::vector<std::string>::iterator &end, const slang::Symbol* symbol);
    SymbolTable& add_scope(const std::string &name);
    void add_symbol_(const std::string &name, const slang::Symbol * symbol);
    const slang::Symbol *get_symbol(const std::vector<std::string>::iterator &begin,
                                    const std::vector<std::string>::iterator &end) const;
    const slang::Symbol *get_symbol_(const std::string &path) const;
};

}  // namespace baldur

#endif  // BALDUR_SYMBOL_HH
