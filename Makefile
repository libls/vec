CC = cc
CFLAGS = -I . -Wall -Wextra -Wpedantic -pedantic
C89FLAGS = -std=c89

TARGET = examples/simple
SOURCE = examples/simple.c

all: tests/tests $(TARGET)

$(TARGET): $(SOURCE) ls_vec.h
	$(CC) -o $(TARGET) $(SOURCE) $(CFLAGS) $(C89FLAGS)

tests/tests: ls_vec.h tests/tests.c tests/ls_test.h
	$(CC) -o $@ tests/tests.c -Itests -I.

clean:
	rm -f $(TARGET)
	rm -f tests/tests

.PHONY: clean all
