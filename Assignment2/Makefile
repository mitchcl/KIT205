CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
SRCDIR = src
TESTDIR = tests

# Main program - transport analysis
TARGET = transport_analysis
SOURCES = $(SRCDIR)/main_transport.c $(SRCDIR)/transport_graph.c
OBJECTS = $(SOURCES:.c=.o)

# Test program
TEST_TARGET = test_transport
TEST_SOURCES = $(TESTDIR)/test_transport.c $(SRCDIR)/transport_graph.c
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

# GTFS analysis program (for real data)
GTFS_TARGET = gtfs_analysis
GTFS_SOURCES = $(SRCDIR)/main_gtfs.c $(SRCDIR)/transport_graph.c $(SRCDIR)/gtfs_parser.c $(SRCDIR)/network_analysis.c
GTFS_OBJECTS = $(GTFS_SOURCES:.c=.o)

# Default target - build everything
all: $(TARGET) $(TEST_TARGET) $(GTFS_TARGET)

# Main executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Test executable  
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# GTFS analysis executable
$(GTFS_TARGET): $(GTFS_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(SRCDIR)/*.o $(TESTDIR)/*.o $(TARGET) $(TEST_TARGET) $(GTFS_TARGET)
	rm -f metro_analysis metro_gtfs_analysis

# Run tests (required by assignment)
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Run main program (sample data analysis)
run: $(TARGET)
	./$(TARGET)

# Run GTFS analysis (real data)
run-gtfs: $(GTFS_TARGET)
	./$(GTFS_TARGET)

# Complete demo for marker - tests + sample analysis + real data
demo: $(TEST_TARGET) $(TARGET) $(GTFS_TARGET)
	@echo "=== Running Unit Tests ==="
	./$(TEST_TARGET)
	@echo ""
	@echo "=== Sample Data Analysis ==="
	./$(TARGET)
	@echo ""
	@echo "=== Real GTFS Data Analysis ==="
	./$(GTFS_TARGET)

.PHONY: all clean test run run-gtfs demo
