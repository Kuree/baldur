#include "symbol.hh"

#include "util.hh"

namespace baldur {

void SymbolTable::add_symbol(const std::string &path, const slang::Symbol *symbol) {
    // need to figure out the path
    auto tokens = get_tokens(path, ".");
    if (tokens.size() == 1) {
        // this is the current level of table
        add_symbol_(tokens[0], symbol);
    } else {
        // the first one is a scope
        auto &scope = add_scope(tokens[0]);
        scope.add_symbol(tokens.begin() + 1, tokens.end(), symbol);
    }
}

SymbolTable &SymbolTable::add_scope(const std::string &name) {
    if (child_scopes_.find(name) != child_scopes_.end()) {
        return *child_scopes_.at(name);
    } else {
        auto scope = std::make_shared<SymbolTable>();
        child_scopes_.emplace(name, scope);
        return *scope;
    }
}

void SymbolTable::add_symbol(const std::vector<std::string>::iterator &begin,
                             const std::vector<std::string>::iterator &end,
                             const slang::Symbol *symbol) {
    if (begin + 1 == end) {
        // only have one element, call the base function
        add_symbol_(*begin, symbol);
    } else {
        auto scope_name = *begin;
        auto &scope = add_scope(scope_name);
        scope.add_symbol(begin + 1, end, symbol);
    }
}

void SymbolTable::add_symbol_(const std::string &name, const slang::Symbol *symbol) {
    symbols_.emplace(name, symbol);
}

const slang::Symbol *SymbolTable::get_symbol(const std::string &path) const {
    auto tokens = get_tokens(path, ".");
    if (tokens.size() == 1) {
        return get_symbol_(path);
    } else {
        auto const &scope_name = tokens[0];
        if (child_scopes_.find(scope_name) == child_scopes_.end()) {
            return nullptr;
        } else {
            auto const &scope = child_scopes_.at(scope_name);
            return scope->get_symbol(tokens.begin() + 1, tokens.end());
        }
    }
}

const slang::Symbol *SymbolTable::get_symbol_(const std::string &path) const {
    if (symbols_.find(path) == symbols_.end()) {
        return nullptr;
    } else {
        return symbols_.at(path);
    }
}

const slang::Symbol *SymbolTable::get_symbol(const std::vector<std::string>::iterator &begin,
                                             const std::vector<std::string>::iterator &end) const {
    if (begin + 1 == end) {
        // only have one left
        return get_symbol_(*begin);
    } else {
        return get_symbol(begin + 1, end);
    }
}

std::unordered_set<const slang::Symbol *> SymbolTable::get_symbols(const std::string &path) const {
    auto tokens = get_tokens(path, ".");
    auto const *scope = this;
    for (auto const &name : tokens) {
        if (scope->child_scopes_.find(name) == scope->child_scopes_.end()) {
            // unable to find anything
            scope = nullptr;
            break;
        } else {
            scope = child_scopes_.at(name).get();
        }
    }
    if (scope) {
        std::unordered_set<const slang::Symbol *> result;
        for (auto const &iter : scope->symbols_) {
            result.emplace(iter.second);
        }
        return result;
    } else {
        return {};
    }
}

void StatementTable::add_stmt(const std::string &filename, uint64_t line_num,
                              const slang::Statement *stmt) {
    SourceInfo info{filename, line_num};
    stmts_map_.emplace(info, stmt);
}

bool StatementTable::has_stmt(const std::string &filename, uint64_t line_num) const {
    return get_stmt(filename, line_num) != nullptr;
}

const slang::Statement *StatementTable::get_stmt(const std::string &filename,
                                                 uint64_t line_num) const {
    SourceInfo info{filename, line_num};
    if (stmts_map_.find(info) != stmts_map_.end()) {
        return stmts_map_.at(info);
    } else {
        return nullptr;
    }
}

}  // namespace baldur