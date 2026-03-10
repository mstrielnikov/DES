CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -Iinclude -O3
CXXFLAGS = -Wall -Wextra -Iinclude -O3 -std=c++2a

SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = tests
CPPDIR = cpp

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXECUTABLE = $(BINDIR)/des_cli

all: directories $(EXECUTABLE)

directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJ) $(OBJDIR)/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: directories $(OBJDIR)/des.o $(OBJDIR)/permutations.o $(OBJDIR)/key.o
	$(CC) $(CFLAGS) -c $(TESTDIR)/test_vectors.c -o $(OBJDIR)/test_vectors.o
	$(CC) $(CFLAGS) $(OBJDIR)/des.o $(OBJDIR)/permutations.o $(OBJDIR)/key.o $(OBJDIR)/test_vectors.o -o $(BINDIR)/des_test
	@echo "Running DES tests..."
	$(BINDIR)/des_test

rebuild: clean all

build-test: clean all test

help:
	@echo "DES - Data Encryption Standard implementation"
	@echo ""
	@echo "Usage:"
	@echo "  make build       - Build C++ CLI (default)"
	@echo "  make test       - Build and run C tests"
	@echo "  make rebuild    - Clean and rebuild everything"
	@echo "  make build-test - Clean, build and test"
	@echo "  make clean      - Remove build artifacts"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "C++ CLI Usage:"
	@echo "  $(EXECUTABLE) -k <key> -e <plaintext>    Encrypt string"
	@echo "  $(EXECUTABLE) -k <key> -d -x <hex>       Decrypt hex"
	@echo "  $(EXECUTABLE) -k <key> -e -o out.txt    Encrypt file"
	@echo "  $(EXECUTABLE) -h                         Show CLI help"
	@echo ""
	@echo "Output:"
	@echo "  $(BINDIR)/des_cli  - C++ CLI executable"
	@echo "  $(BINDIR)/des_test - C test suite"

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean test rebuild build-test help
