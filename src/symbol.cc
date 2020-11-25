#include "symbol.hh"

namespace baldur {

void Scope::add_symbol(std::unique_ptr<Symbol> symbol) { add_symbol("", std::move(symbol)); }

void Scope::add_symbol(const std::string &name, std::unique_ptr<Symbol> symbol) {
    auto *symbol_ptr = symbol.get();
    symbols_.emplace(std::move(symbol));
    if (!name.empty()) {
        symbol_map_.emplace(name, symbol_ptr);
    }
}

}  // namespace baldur