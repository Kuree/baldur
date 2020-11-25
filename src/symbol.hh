#ifndef BALDUR_SYMBOL_HH
#define BALDUR_SYMBOL_HH

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

// this is lightweight wrapper class around slang's symbol

namespace baldur {

class Scope;

enum class SymbolKind { Net, Register, Scope, Parameter };

class Symbol {
public:
    std::string name;
    Scope *parent;
    SymbolKind kind;
};

class Scope : public Symbol {
public:
    void add_symbol(std::unique_ptr<Symbol> symbol);
    void add_symbol(const std::string &name, std::unique_ptr<Symbol> symbol);

private:
    // scope owns the child symbols and scopes
    std::unordered_set<std::unique_ptr<Symbol>> symbols_;

    // this data structure is designed for fast lookups, where the members are not mutable
    // notice that not all symbols has names, e.g. un-named statement blocks
    std::unordered_map<std::string, const Symbol *> symbol_map_;
};

}  // namespace baldur

#endif  // BALDUR_SYMBOL_HH
