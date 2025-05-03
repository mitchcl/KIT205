# Makefile for Airline Reservation System - KIT205 Assignment 1
CC = gcc
CFLAGS = -Wall -Wextra -g -I$(SRCDIR)
SRCDIR = src
BINDIR = bin
OBJDIR = obj

# Output binary
SYSTEM_TARGET = $(BINDIR)/airline_system

# Source files
PROTO1_SRC = $(SRCDIR)/prototype1/flight_management.c \
            $(SRCDIR)/prototype1/passenger_management.c \
            $(SRCDIR)/prototype1/reservation_management.c \
            $(SRCDIR)/prototype1/flight_search.c \
            $(SRCDIR)/prototype1/passenger_search.c

PROTO2_SRC = $(SRCDIR)/prototype2/flight_management_avl.c \
            $(SRCDIR)/prototype2/passenger_management_hash.c \
            $(SRCDIR)/prototype2/reservation_management_bst.c \
            $(SRCDIR)/prototype2/flight_search_avl.c \
            $(SRCDIR)/prototype2/passenger_search_hash.c

COMMON_SRC = $(SRCDIR)/file_loader.c $(SRCDIR)/data_generator.c $(SRCDIR)/test_framework.c

SYSTEM_SRC = $(SRCDIR)/airline_system.c $(PROTO1_SRC) $(PROTO2_SRC) $(COMMON_SRC)

all: directories $(SYSTEM_TARGET)

directories:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)

$(SYSTEM_TARGET): $(SYSTEM_SRC)
	$(CC) $(CFLAGS) -o $@ $(SRCDIR)/airline_system.c \
		$(SRCDIR)/prototype1/flight_management.c \
		$(SRCDIR)/prototype1/passenger_management.c \
		$(SRCDIR)/prototype1/reservation_management.c \
		$(SRCDIR)/prototype1/flight_search.c \
		$(SRCDIR)/prototype1/passenger_search.c \
		$(SRCDIR)/prototype2/flight_management_avl.c \
		$(SRCDIR)/prototype2/passenger_management_hash.c \
		$(SRCDIR)/prototype2/reservation_management_bst.c \
		$(SRCDIR)/prototype2/flight_search_avl.c \
		$(SRCDIR)/prototype2/passenger_search_hash.c \
		$(SRCDIR)/file_loader.c $(SRCDIR)/data_generator.c $(SRCDIR)/test_framework.c

run: all
	./$(SYSTEM_TARGET)

run-generate: all
	./$(SYSTEM_TARGET) --generate

clean:
	rm -f $(SYSTEM_TARGET)
	rm -rf $(OBJDIR)/*

.PHONY: all clean run run-generate directories
