CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -O3

SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = tests

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
OBJ += $(OBJDIR)/main.o
EXECUTABLE = $(BINDIR)/des_example

all: directories $(EXECUTABLE)

directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

test: directories $(OBJDIR)/des.o $(OBJDIR)/permutations.o $(OBJDIR)/key.o
	$(CC) $(CFLAGS) -c $(TESTDIR)/test_vectors.c -o $(OBJDIR)/test_vectors.o
	$(CC) $(CFLAGS) $(OBJDIR)/des.o $(OBJDIR)/permutations.o $(OBJDIR)/key.o $(OBJDIR)/test_vectors.o -o $(BINDIR)/des_test
	@echo "Running DES tests..."
	$(BINDIR)/des_test

rebuild: clean all

build-test: clean all test

help:
	@echo "DES - Data Encryption Standard implementation"
	@echo "Usage:"
	@echo "  make build       - Build the project"
	@echo "  make test       - Build and run tests"
	@echo "  make rebuild    - Clean and rebuild everything"
	@echo "  make build-test - Clean, build and test"
	@echo "  make clean      - Remove build artifacts"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "Output:"
	@echo "  bin/des_example - Example executable"
	@echo "  bin/des_test    - Test suite"

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean test rebuild build-test help
