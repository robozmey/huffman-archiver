#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <climits>

namespace hw_02 {

    class HuffmanArchiver {
    public:
        class HuffTree {
        public:
            explicit HuffTree(std::vector<uint32_t> &counts);

            class TreeNode {
            public:
                TreeNode() = default;

                TreeNode(unsigned char chr, uint32_t count) : chr(chr), count(count), is_end(true) {}

                TreeNode(TreeNode *left, TreeNode *right) : left(left), right(right), count(left->count + right->count),
                                                            is_end(false) {}

                unsigned char chr = 0;
                bool is_end = true;
                uint32_t count = 0;
                TreeNode *left = nullptr;     // 0
                TreeNode *right = nullptr;    // 1

            };

            TreeNode *head = nullptr;
            std::map<unsigned char, std::vector<bool> > codes;
            bool message_is_empty = true;

        private:
            void buildTree(std::vector<uint32_t> &counts);

            void buildCodes(TreeNode *node, std::vector<bool> &);

            TreeNode _data[512];
        };

        static std::pair<int, int> archive(std::string input_filename, std::string output_filename);

        static std::pair<int, int> unarchive(std::string input_filename, std::string output_filename);

    };

    class BitInput {
    public:
        unsigned char cache = 0;
        int id = 8;
        std::ifstream in;

        explicit BitInput(const std::string& filename) : in(filename) {}

        BitInput &operator>>(bool &);

        BitInput &operator>>(uint32_t &);

        bool fail() const { return in.fail(); }

        bool bad() const { return in.bad(); }

        bool eof() const { return in.eof(); }

        bool good() const { return in.good(); }

        void close() { in.close(); }

        explicit operator bool() { return in.operator bool(); }

    private:
        void pull();
    };

    class BitOutput {
    public:
        unsigned char cache = 0;
        int id = 0;
        std::ofstream out;

        explicit BitOutput(const std::string& filename) : out(filename) {}

        ~BitOutput() { close(); }

        BitOutput &operator<<(const bool &);

        BitOutput &operator<<(const uint32_t &);

        bool fail() const { return out.fail(); }

        bool bad() const { return out.bad(); }

        bool eof() const { return out.eof(); }

        bool good() const { return out.good(); }

        void close();

        explicit operator bool() { return out.operator bool(); }

    private:
        void push();
    };

    class HuffmanException : public std::exception {
    public:
        const char *what() const throw() override {
            return message.c_str();
        }

        explicit HuffmanException(const char *message) : message(message) {}
        explicit HuffmanException(std::string message) : message(message) {}

        HuffmanException() = default;

        std::string message = "Huffman's exception";

    };

}

