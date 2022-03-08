#include <queue>
#include <fstream>
#include <string>
#include "huffman.h"

hw_02::HuffmanArchiver::HuffTree::HuffTree(std::vector<uint32_t> &counts) {
    buildTree(counts);
}

void hw_02::HuffmanArchiver::HuffTree::buildTree(std::vector<uint32_t> &counts) {
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int >>, std::greater<std::pair<int, int> > > queue;

    for (unsigned char chr = 0; chr <= CHAR_MAX; chr++) {
        _data[chr] = TreeNode(chr, counts[chr]);
        if (counts[chr] > 0) {
            message_is_empty = false;
            queue.push(std::make_pair(counts[chr], chr));
        }
    }

    int chr = CHAR_MAX + 1;

    while (queue.size() > 1) {
        int left = queue.top().second;
        queue.pop();
        int right = queue.top().second;
        queue.pop();
        _data[chr] = TreeNode(&_data[left], &_data[right]);
        queue.push(std::make_pair(_data[chr].count, chr));
        chr++;
    }

    if (!message_is_empty) {
        head = &_data[queue.top().second];
        std::vector<bool> tmp(0);
        buildCodes(head, tmp);
    }

}


void hw_02::HuffmanArchiver::HuffTree::buildCodes(TreeNode *node, std::vector<bool> &code) {
    if (node->is_end) {
        codes[node->chr] = code;
    } else {
        code.push_back(false);
        buildCodes(node->left, code);
        code.pop_back();
        code.push_back(true);
        buildCodes(node->right, code);
        code.pop_back();
    }
}

std::pair<int, int> hw_02::HuffmanArchiver::archive(std::string input_filename, std::string output_filename) {
    std::ifstream in(input_filename);
    BitOutput bout(output_filename);

    if (in.fail())
        throw HuffmanException("Unable to open text input file");
    if (bout.fail())
        throw HuffmanException("Unable to open binary output file");

    // Count length and char counts
    std::vector<uint32_t> counts(CHAR_MAX + 1);
    char chr;
    while (in.get(chr)) {
        counts[chr]++;
    }

    uint32_t input_length = 0;
    for (auto count: counts)
        input_length += count;

    // Write length and char counts
    bout << input_length;
    for (auto count : counts) {
        bout << count;
    }

    // Build tree
    HuffTree tree(counts);

    // Write
    in.clear();
    in.seekg(0, std::ios::beg);

    uint32_t output_length = 32 + counts.size() * 32;

    while (in.get(chr)) {
        output_length += tree.codes[chr].size();
        for (bool bit : tree.codes[chr]) {
            bout << bit;
        }
    }

    if (in.bad())
        throw HuffmanException("Unable to read text input fail");
    if (bout.bad())
        throw HuffmanException("Unable to read binary output fail");

    return std::make_pair(input_length, (output_length + 7) / 8);
}

std::pair<int, int> hw_02::HuffmanArchiver::unarchive(std::string input_filename, std::string output_filename) {
    BitInput bin(input_filename);
    std::ofstream out(output_filename);

    if (bin.fail())
        throw HuffmanException("Unable to open binary input file");
    if (out.fail())
        throw HuffmanException("Unable to open text output file");

    // Read length and counts
    uint32_t expected_output_length;
    bin >> expected_output_length;

    std::vector<uint32_t> counts(CHAR_MAX + 1);

    for (uint32_t &count : counts) {
        bin >> count;
    }

    uint32_t input_length = 32 + counts.size() * 32;

    // Build tree
    HuffTree tree(counts);

    // Get text
    bool bit;
    HuffTree::TreeNode *currentNode = tree.head;
    uint32_t output_length = 0;
    while (currentNode != nullptr && currentNode->is_end) {
        out.put(char(currentNode->chr));
        currentNode = tree.head;
        output_length++;
        if (output_length == expected_output_length) {
            break;
        }
    }
    while (bin >> bit) {
        input_length++;
        if (bit)
            currentNode = currentNode->right;
        else
            currentNode = currentNode->left;

        if (currentNode->is_end) {
            out.put(char(currentNode->chr));
            currentNode = tree.head;
            output_length++;
            if (output_length == expected_output_length) {
                break;
            }
        }
    }

    if (bin.bad())
        throw HuffmanException("Unable to read binary input file");
    if (out.bad())
        throw HuffmanException("Unable to read text output file");
    if (output_length < expected_output_length)
        throw HuffmanException("Archived file is corrupt, expected message length " + std::to_string(expected_output_length) + " but " + std::to_string(output_length) + " found");

    return std::make_pair((input_length + 7) / 8, output_length);
}

////////////////////

hw_02::BitInput &hw_02::BitInput::operator>>(bool &bit) {
    pull();
    bit = (cache & (1 << id)) > 0;
    id++;
    return *this;
}

hw_02::BitInput &hw_02::BitInput::operator>>(uint32_t &x) {
    x = 0;
    for (uint32_t index = 0; index < 32; index++) {
        bool bit;
        *this >> bit;
        if (bit)
            x |= (uint32_t(1) << index);
    }
    return *this;
}

void hw_02::BitInput::pull() {
    if (id == 8) {
        in.read(reinterpret_cast<char *>(&cache), 1);
        id = 0;
    }
}

hw_02::BitOutput &hw_02::BitOutput::operator<<(const bool &bit) {
    if (bit)
        cache |= (1 << id);
    id++;
    push();

    return *this;
}

hw_02::BitOutput &hw_02::BitOutput::operator<<(const uint32_t &x) {
    for (uint32_t index = 0; index < 32; index++) {
        if ((x & (uint32_t(1) << index)) > 0) {
            *this << true;
        } else {
            *this << false;
        }
    }
    return *this;
}

void hw_02::BitOutput::push() {
    if (id == 8) {
        out << cache;
        id = 0;
        cache &= 0;
    }
}

void hw_02::BitOutput::close() {
    push();
    if (id > 0)
        out << cache;
    out.close();
}

