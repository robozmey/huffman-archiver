#pragma once

#include <string>

#include "huffman.h"

using namespace hw_02;

namespace hw_02_test {


    std::string archive_unarchive(std::string str);

    std::vector<bool> read_bits(std::string filename);

    void write_bits(std::string filename, const std::vector<bool> &bits);

    std::string read_text(std::string filename);

    void write_text(std::string filename, const std::string &text);

    HuffmanArchiver::HuffTree treeByText(std::string text);

}

