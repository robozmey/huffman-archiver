#include <iostream>
#include <cstring>

#include "huffman.h"

using namespace hw_02;

int main(int argc, char *argv[]) {
    bool archive = false;
    bool unarchive = false;
    std::string input_filename;
    int input_filename_argument_index = -1;
    std::string output_filename;
    int output_filename_argument_index = -1;

    for (int index = 0; index < argc; index++) {
        if (strcmp(argv[index], "-c") == 0)
            archive = true;
        else if (strcmp(argv[index], "-u") == 0)
            unarchive = true;
        else if (strcmp(argv[index], "-f") == 0 || strcmp(argv[index], "--file") == 0)
            input_filename_argument_index = index;
        else if (strcmp(argv[index], "-o") == 0 || strcmp(argv[index], "--output") == 0)
            output_filename_argument_index = index;

        if (index == input_filename_argument_index + 1)
            input_filename = argv[index];
        if (index == output_filename_argument_index + 1)
            output_filename = argv[index];
    }

    if ((archive && unarchive) || ((!archive) && (!unarchive)) || input_filename_argument_index == -1 ||
        output_filename_argument_index == -1) {
        std::cout << "Wrong commandline options!" << std::endl;
    } else if (archive) {
        auto statistics = HuffmanArchiver::archive(input_filename, output_filename);
        std::cout << "Original file size: " << statistics.first << "\n";
        std::cout << "Archived file size: " << statistics.second << std::endl;
    } else if (unarchive) {
        auto statistics = HuffmanArchiver::unarchive(input_filename, output_filename);
        std::cout << "Archived file size: " << statistics.first << "\n";
        std::cout << "Original file size: " << statistics.second << std::endl;
    }

}