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

    // get instance symbol
    auto const *top = sym_table->get_symbol("top");
    EXPECT_NE(top, nullptr);
    EXPECT_EQ(top->name, "top");

    // get an arbitrary symbol
    auto const *a = sym_table->get_symbol("top.a");
    EXPECT_NE(a, nullptr);
    EXPECT_EQ(a->name, "a");

    // get statements
    auto const *stmt_table = info.get_stmt_table();
    EXPECT_TRUE(stmt_table->has_stmt("test1.sv", 9));
    EXPECT_FALSE(stmt_table->has_stmt("test", 9));
    EXPECT_EQ(stmt_table->table_size(), 3);
}
