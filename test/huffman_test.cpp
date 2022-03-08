#include "huffman_test.h"

#include <fstream>
#include <iostream>

using namespace hw_02;
using namespace hw_02_test;


std::string hw_02_test::archive_unarchive(std::string in_text) {

    std::string text_filename = "tmp.txt";
    std::string binary_filename = "tmp.bin";
    write_text(text_filename, in_text);
    HuffmanArchiver::archive(text_filename, binary_filename);
    //std::cout << st1.second * 100 / st1.first << std::endl;
    HuffmanArchiver::unarchive(binary_filename, text_filename);
    auto out_text = read_text(text_filename);
    return out_text;
}

std::vector<bool> hw_02_test::read_bits(std::string filename) {
    std::ifstream in(filename, std::ios::binary);

    std::vector<bool> result;
    char byte;
    while (in.read(&byte, 1)) {
        for (int bit_index = 0; bit_index < 8; bit_index++) {
            result.push_back((byte & (1 << bit_index)) > 0);
        }
    }

    in.close();
    return result;
}

void hw_02_test::write_bits(std::string filename, const std::vector<bool> &bits) {
    std::ofstream out(filename, std::ios::binary);

    for (int byte_index = 0; byte_index < (bits.size() + 7) / 8; byte_index++) {
        unsigned char byte = 0;
        for (int bit_index = 0; bit_index < 8 && byte_index * 8 + bit_index < bits.size(); bit_index++) {
            if (bits[byte_index * 8 + bit_index])
                byte |= (1 << (bit_index));
        }
        out << byte;
    }
    out.close();
}

std::string hw_02_test::read_text(std::string filename) {
    std::ifstream in(filename);

    std::string result;
    char chr;
    while (in.get(chr)) {
        result += chr;
    }

    in.close();
    return result;
}

void hw_02_test::write_text(std::string filename, const std::string &text) {
    std::ofstream out(filename);

    out << text;

    out.close();
}

HuffmanArchiver::HuffTree hw_02_test::treeByText(std::string text) {
    std::vector<uint32_t> counts(CHAR_MAX + 1);
    for (auto chr : text) {
        counts[chr]++;
    }
    uint32_t input_length = text.length();
    HuffmanArchiver::HuffTree tree(counts);
    return tree;
}

