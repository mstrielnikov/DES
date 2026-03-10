#include <iostream>
#include "cpp/des_cli.hpp"

void print_help(const char* prog) {
    std::cout << "DES Encryption Tool (Modern C++ CLI + C Implementation)\n\n";
    std::cout << "Usage: " << prog << " [options] [input]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -e, --encrypt    Encrypt mode (default)\n";
    std::cout << "  -d, --decrypt    Decrypt mode\n";
    std::cout << "  -k, --key <key>  8-byte key (required)\n";
    std::cout << "  -x, --hex        Input/Output in hex format\n";
    std::cout << "  -o, --output <f> Output file (default: stdout)\n";
    std::cout << "  -h, --help       Show this help\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << prog << " -k mykey123 -e \"Hello World\"\n";
    std::cout << "  " << prog << " -k mykey123 -d -x 8CA64DE9C1B123A7\n";
    std::cout << "  " << prog << " -k mykey123 -e -o encrypted.txt < plain.txt\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        print_help(argv[0]);
        return 0;
    }

    if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
        print_help(argv[0]);
        return 0;
    }

    auto opts = des::CLI::parse(argc, argv);
    
    if (opts.key.empty()) {
        std::cerr << "Error: Key required (-k <key>)\n";
        std::cerr << "Run with -h for usage.\n";
        return 1;
    }

    return des::CLI::run(opts);
}
