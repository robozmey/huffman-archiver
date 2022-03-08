#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "huffman_test.h"

using namespace hw_02;
using namespace hw_02_test;


TEST_SUITE("integration tests") {

    TEST_CASE("Hello world") {
        std::string in_text = "Hello, world!";
        auto out_text = archive_unarchive(in_text);
        CHECK_EQ(in_text, out_text);
    }

    TEST_CASE("two line") {
        std::string in_text = "Hello,\nWorld!";
        auto out_text = archive_unarchive(in_text);
        CHECK_EQ(in_text, out_text);
    }

    TEST_CASE("one letter") {
        for (unsigned char chr = 0; chr <= CHAR_MAX; chr++) {
            std::string in_text = std::to_string(char(chr));
            SUBCASE(in_text.c_str()) {
                auto out_text = archive_unarchive(in_text);
                CHECK_EQ(in_text, out_text);
            }
        }
    }

    TEST_CASE("a lot of one letter") {
        for (unsigned char chr = 0; chr <= CHAR_MAX; chr++) {
            std::string in_text = std::string(int(chr) + 1, char(chr));
            SUBCASE(std::to_string(int(chr)).c_str()) {
                auto out_text = archive_unarchive(in_text);
                CHECK_EQ(in_text, out_text);
            }
        }
    }

    TEST_CASE("empty") {
        std::string in_text = "";
        auto out_text = archive_unarchive(in_text);
        CHECK_EQ(in_text, out_text);
    }

    TEST_CASE("big") {
        std::string in_text;
        for (unsigned char chr = 0; chr <= CHAR_MAX; chr++) {
            in_text += std::string(1 << (chr / 16), char(chr));
        }
        auto out_text = archive_unarchive(in_text);
        CHECK_EQ(in_text, out_text);
    }

    TEST_CASE("big and beautiful") {
        std::string in_text;
        for (unsigned char chr = 0; chr <= 20; chr++) {
            in_text += std::string(1 << chr, char(chr));
        }
        auto out_text = archive_unarchive(in_text);
        CHECK_EQ(in_text, out_text);
    }
}

TEST_SUITE("tree building") {

    TEST_CASE("hello world") {
        std::string in_text = "hello world";
        auto tree = treeByText(in_text);
        CHECK_EQ(tree.codes[' '], std::vector<bool>({1, 1, 1, 0}));
        CHECK_EQ(tree.codes['d'], std::vector<bool>({1, 1, 1, 1}));
        CHECK_EQ(tree.codes['l'], std::vector<bool>({1, 0}));
        CHECK_EQ(tree.codes['o'], std::vector<bool>({1, 1, 0}));
        CHECK_EQ(tree.codes['e'], std::vector<bool>({0, 0, 0}));
        CHECK_EQ(tree.codes['h'], std::vector<bool>({0, 0, 1}));
        CHECK_EQ(tree.codes['r'], std::vector<bool>({0, 1, 0}));
        CHECK_EQ(tree.codes['w'], std::vector<bool>({0, 1, 1}));
    }

    TEST_CASE("beautiful") {
        std::string in_text = "0122333344444444";
        auto tree = treeByText(in_text);
        CHECK_EQ(tree.codes['4'], std::vector<bool>({0}));
        CHECK_EQ(tree.codes['3'], std::vector<bool>({1, 0}));
        CHECK_EQ(tree.codes['2'], std::vector<bool>({1, 1, 0}));
        CHECK_EQ(tree.codes['0'], std::vector<bool>({1, 1, 1, 0}));
        CHECK_EQ(tree.codes['1'], std::vector<bool>({1, 1, 1, 1}));
    }
}