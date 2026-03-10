# DES - Data Encryption Standard Implementation

A C implementation of the DES cipher with an example of a modern C++ CLI interface.

**NOTE**: This is a simplified implementation of DES for educational purposes. The DES standard is deprecated and should not be used in production. The library shows example of building C/C++ projects and composition of symmetric exncryption primitives (permutation, substitution, s-boxes, key schedule, etc.)

## Architecture

```
include/          - C headers (public API)
src/              - C implementation (zero-dependency)
cpp/              - C++ CLI wrapper (modern interface)
tests/            - C test suite
main.cpp          - C++ CLI entry point
```

## Building

```bash
make build        # Build C++ CLI (default)
make test         # Run C test suite
make rebuild      # Clean and rebuild
make build-test   # Clean, build and test
make clean        # Remove build artifacts
```

## C

The core DES implementation is in pure C (`src/*.c`) with no external dependencies:

```bash
# Build and run tests
make test
```

Test results show encrypt/decrypt roundtrip verification for multiple test vectors.

## C++ CLI Interface

Modern wrapper with less boilerplate code for command-line interface:

```bash
# Show help
./bin/des_cli -h

# Encrypt string (8-byte key required)
./bin/des_cli -k mykey123 -e "Hello World"

# Encrypt with hex output
./bin/des_cli -k mykey123 -e -x "Hello World"

# Decrypt hex
./bin/des_cli -k mykey123 -d -x 63A93A00B066

# Encrypt to file
./bin/des_cli -k mykey123 -e -o encrypted.bin < plain.txt

# Pipe usage
echo "message" | ./bin/des_cli -k mykey123 -e
```

### CLI Options

| Option                | Description                      |
| --------------------- | -------------------------------- |
| `-k, --key <key>`     | 8-byte encryption key (required) |
| `-e, --encrypt`       | Encrypt mode (default)           |
| `-d, --decrypt`       | Decrypt mode                     |
| `-x, --hex`           | Input/output in hex format       |
| `-o, --output <file>` | Output file (default: stdout)    |
| `-h, --help`          | Show help                        |

## Key Features

- **Pure C Implementation**: Zero external dependencies, portable
- **Modern C++ CLI**: Clean interface with hex I/O support
- **Self-Contained**: No external libraries required
- **Tested**: Self-consistency tests verify encrypt/decrypt roundtrip

## Files

| File                   | Description              |
| ---------------------- | ------------------------ |
| `include/des.h`        | C API header             |
| `src/des.c`            | Core DES algorithm       |
| `src/permutations.c`   | S-boxes and permutations |
| `src/key.c`            | Key schedule             |
| `cpp/des_cli.hpp`      | C++ wrapper classes      |
| `tests/test_vectors.c` | Test suite               |
