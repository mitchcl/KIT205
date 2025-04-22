CC = gcc
CFLAGS = -Wall -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Default target
all: directories $(BIN_DIR)/airline_system

# Create necessary directories
directories:
    mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Main executable
$(BIN_DIR)/airline_system: $(OBJ_DIR)/airline_system.o $(OBJ_DIR)/flight_management.o $(OBJ_DIR)/passenger_management.o $(OBJ_DIR)/reservation_management.o
    $(CC) $(CFLAGS) -o $@ $^

# Object files
$(OBJ_DIR)/airline_system.o: $(SRC_DIR)/airline_system.c
    $(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/flight_management.o: $(SRC_DIR)/prototype1/flight_management.c
    $(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/passenger_management.o: $(SRC_DIR)/prototype1/passenger_management.c
    $(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/reservation_management.o: $(SRC_DIR)/prototype1/reservation_management.c
    $(CC) $(CFLAGS) -c -o $@ $<

# Clean
clean:
    rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/airline_system
