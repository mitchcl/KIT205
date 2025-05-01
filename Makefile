CC = gcc
CFLAGS = -Wall -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
SYSTEM_TARGET = airline_system

# Default target
all: directories $(BIN_DIR)/$(SYSTEM_TARGET)

# Create necessary directories
directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Main executable
$(BIN_DIR)/$(SYSTEM_TARGET): $(OBJ_DIR)/airline_system.o $(OBJ_DIR)/flight_management.o $(OBJ_DIR)/passenger_management.o $(OBJ_DIR)/reservation_management.o $(OBJ_DIR)/test_framework.o $(OBJ_DIR)/data_generator.o
	$(CC) $(CFLAGS) $^ -o $@

# Object files
$(OBJ_DIR)/airline_system.o: $(SRC_DIR)/airline_system.c $(SRC_DIR)/airline_types.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/flight_management.o: $(SRC_DIR)/prototype1/flight_management.c $(SRC_DIR)/prototype1/flight_management.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(OBJ_DIR)/passenger_management.o: $(SRC_DIR)/prototype1/passenger_management.c $(SRC_DIR)/prototype1/passenger_management.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(OBJ_DIR)/reservation_management.o: $(SRC_DIR)/prototype1/reservation_management.c $(SRC_DIR)/prototype1/reservation_management.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(OBJ_DIR)/test_framework.o: $(SRC_DIR)/test_framework.c $(SRC_DIR)/test_framework.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(OBJ_DIR)/data_generator.o: $(SRC_DIR)/data_generator.c $(SRC_DIR)/data_generator.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	cd $(BIN_DIR) && ./$(SYSTEM_TARGET)