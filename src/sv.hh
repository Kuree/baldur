#ifndef BALDUR_SV_HH
#define BALDUR_SV_HH
#include <vector>
#include "symbol.hh"

namespace baldur {

std::unique_ptr<Symbol> parse_sv(const std::vector<std::string>& files,
                                 const std::vector<std::string>& defines,
                                 const std::vector<std::string>& includes);
}
#endif  // BALDUR_SV_HH
