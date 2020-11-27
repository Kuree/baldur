#include "../src/sv.hh"
#include "gtest/gtest.h"

using namespace baldur;

TEST(sv, parse) {   // NOLINT
    SystemVerilogInfo info({"test1.sv"}, {});
    info.build_tables();
    auto const *sym_table = info.get_symbol_table();
    auto vars = sym_table->get_symbols("top");
    // in, out, a, b
    EXPECT_EQ(vars.size(), 4);
}
