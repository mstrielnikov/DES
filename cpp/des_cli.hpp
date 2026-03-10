#ifndef DES_CLI_HPP
#define DES_CLI_HPP

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <memory>
#include <optional>
#include <cstring>
#include <cstdio>
#include <cstdint>

extern "C" {
#include "des.h"
}

namespace des {

class Block {
public:
    static constexpr size_t BlockSize = 8;

    Block() : data_(new uint8_t[BlockSize]) {
        std::memset(data_.get(), 0, BlockSize);
    }

    explicit Block(const std::array<uint8_t, BlockSize>& bytes) 
        : data_(new uint8_t[BlockSize]) {
        std::memcpy(data_.get(), bytes.data(), BlockSize);
    }

    explicit Block(const std::string& hex) : data_(new uint8_t[BlockSize]) {
        fromHex(hex);
    }

    const uint8_t* data() const { return data_.get(); }
    uint8_t* data() { return data_.get(); }

    std::string hex() const {
        std::string result;
        result.reserve(BlockSize * 2);
        for (size_t i = 0; i < BlockSize; ++i) {
            char buf[3];
            std::snprintf(buf, sizeof(buf), "%02X", data_.get()[i]);
            result += buf;
        }
        return result;
    }

    void fromHex(const std::string& hex) {
        std::memset(data_.get(), 0, BlockSize);
        size_t len = std::min(hex.size(), BlockSize * 2);
        for (size_t i = 0; i < len; i += 2) {
            unsigned int val = 0;
            std::sscanf(hex.c_str() + i, "%02X", &val);
            data_.get()[i / 2] = static_cast<uint8_t>(val);
        }
    }

    bool operator==(const Block& other) const {
        return std::memcmp(data_.get(), other.data_.get(), BlockSize) == 0;
    }

private:
    std::unique_ptr<uint8_t[]> data_;
};

class Cipher {
public:
    static constexpr size_t KeySize = 8;

    explicit Cipher(const std::array<uint8_t, KeySize>& key) : key_(key) {}

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) const {
        std::vector<uint8_t> result(data.size());
        std::memcpy(result.data(), data.data(), data.size());
        
        des_message msg{.data = result.data(), .data_len = result.size()};
        des_encrypt(&msg, key_.data());
        return result;
    }

    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) const {
        std::vector<uint8_t> result(data.size());
        std::memcpy(result.data(), data.data(), data.size());
        
        des_message msg{.data = result.data(), .data_len = result.size()};
        des_decrypt(&msg, key_.data());
        return result;
    }

    std::string encrypt(const std::string& text) const {
        std::vector<uint8_t> input(text.begin(), text.end());
        auto result = encrypt(input);
        return std::string(result.begin(), result.end());
    }

    std::string decryptVec(const std::vector<uint8_t>& data) const {
        auto result = decrypt(data);
        return std::string(result.begin(), result.end());
    }

    std::string decrypt(const std::string& data) const {
        std::vector<uint8_t> input(data.begin(), data.end());
        return decryptVec(input);
    }

private:
    std::array<uint8_t, KeySize> key_;
};

class CLI {
public:
    struct Options {
        std::string key;
        bool encrypt = true;
        bool hex = false;
        std::string input;
        std::optional<std::string> output_file;
    };

    static Options parse(int argc, char* argv[]) {
        Options opts;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg == "-e" || arg == "--encrypt") {
                opts.encrypt = true;
            } else if (arg == "-d" || arg == "--decrypt") {
                opts.encrypt = false;
            } else if (arg == "-x" || arg == "--hex") {
                opts.hex = true;
            } else if (arg == "-o" || arg == "--output") {
                if (++i < argc) opts.output_file = argv[i];
            } else if (arg == "-k" || arg == "--key") {
                if (++i < argc) opts.key = argv[i];
            } else if (arg[0] == '-') {
                // ignore unknown options
            } else {
                opts.input = arg;
            }
        }

        return opts;
    }

    static int run(const Options& opts) {
        if (opts.key.size() != Cipher::KeySize) {
            std::cerr << "Error: Key must be exactly " << Cipher::KeySize << " bytes\n";
            return 1;
        }

        std::array<uint8_t, Cipher::KeySize> key_bytes{};
        std::memcpy(key_bytes.data(), opts.key.data(), Cipher::KeySize);
        Cipher cipher(key_bytes);

        std::string data = opts.input;
        
        if (opts.hex) {
            std::vector<uint8_t> bytes;
            for (size_t i = 0; i < data.size(); i += 2) {
                unsigned int val = 0;
                std::sscanf(data.c_str() + i, "%02X", &val);
                bytes.push_back(static_cast<uint8_t>(val));
            }

            if (opts.encrypt) {
                auto r = cipher.encrypt(bytes);
                std::string hex_result;
                for (auto b : r) {
                    char buf[3];
                    std::snprintf(buf, sizeof(buf), "%02X", b);
                    hex_result += buf;
                }
                if (opts.output_file) {
                    std::ofstream out(*opts.output_file);
                    out << hex_result;
                } else {
                    std::cout << hex_result << "\n";
                }
            } else {
                auto r = cipher.decrypt(bytes);
                std::string result(r.begin(), r.end());
                if (opts.output_file) {
                    std::ofstream out(*opts.output_file, std::ios::binary);
                    out << result;
                } else {
                    std::cout << result;
                }
            }
        } else {
            if (opts.encrypt) {
                auto r = cipher.encrypt(data);
                if (opts.output_file) {
                    std::ofstream out(*opts.output_file, std::ios::binary);
                    out.write(reinterpret_cast<const char*>(r.data()), r.size());
                } else {
                    std::cout.write(reinterpret_cast<const char*>(r.data()), r.size());
                }
            } else {
                auto r = cipher.decrypt(data);
                if (opts.output_file) {
                    std::ofstream out(*opts.output_file, std::ios::binary);
                    out << r;
                } else {
                    std::cout << r;
                }
            }
        }

        return 0;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Block& block) {
    return os << block.hex();
}

inline std::istream& operator>>(std::istream& is, Block& block) {
    std::string hex;
    if (is >> hex) {
        block.fromHex(hex);
    }
    return is;
}

} // namespace des

#endif // DES_CLI_HPP
