CC = gcc
CFLAGS = -Wall -Werror -g

# Core objects
OBJECTS = airline_system.o test_framework.o file_loader.o data_generator.o \
         prototype1/flight_management.o prototype1/passenger_management.o prototype1/reservation_management.o \
         prototype1/flight_search.o prototype1/passenger_search.o \
         prototype2/flight_management_avl.o prototype2/passenger_management_hash.o prototype2/reservation_management_bst.o \
         prototype2/flight_search_avl.o prototype2/passenger_search_hash.o

# Target binary
TARGET = airline_system

all: create_dirs $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Create prototype folders if they don't exist
create_dirs:
	mkdir -p prototype1 prototype2

# Main program file
airline_system.o: airline_system.c airline_types.h \
                 prototype1/flight_management.h prototype1/passenger_management.h prototype1/reservation_management.h \
                 prototype2/flight_management_avl.h prototype2/passenger_management_hash.h prototype2/reservation_management_bst.h \
                 file_loader.h data_generator.h test_framework.h
	$(CC) $(CFLAGS) -c airline_system.c

# Test framework
test_framework.o: test_framework.c test_framework.h airline_types.h \
                  prototype1/flight_management.h prototype1/passenger_management.h prototype1/reservation_management.h \
                  prototype2/flight_management_avl.h prototype2/passenger_management_hash.h prototype2/reservation_management_bst.h
	$(CC) $(CFLAGS) -c test_framework.c

# File loader for CSV files
file_loader.o: file_loader.c file_loader.h airline_types.h
	$(CC) $(CFLAGS) -c file_loader.c

# Data generator for large datasets
data_generator.o: data_generator.c data_generator.h airline_types.h
	$(CC) $(CFLAGS) -c data_generator.c

# Prototype 1 implementations
prototype1/flight_management.o: prototype1/flight_management.c prototype1/flight_management.h airline_types.h
	$(CC) $(CFLAGS) -c prototype1/flight_management.c -o $@

prototype1/passenger_management.o: prototype1/passenger_management.c prototype1/passenger_management.h airline_types.h
	$(CC) $(CFLAGS) -c prototype1/passenger_management.c -o $@

prototype1/reservation_management.o: prototype1/reservation_management.c prototype1/reservation_management.h \
                                  prototype1/flight_management.h prototype1/passenger_management.h airline_types.h
	$(CC) $(CFLAGS) -c prototype1/reservation_management.c -o $@

prototype1/flight_search.o: prototype1/flight_search.c airline_types.h prototype1/flight_management.h
	$(CC) $(CFLAGS) -c prototype1/flight_search.c -o $@

prototype1/passenger_search.o: prototype1/passenger_search.c airline_types.h prototype1/passenger_management.h
	$(CC) $(CFLAGS) -c prototype1/passenger_search.c -o $@

# Prototype 2 implementations
prototype2/flight_management_avl.o: prototype2/flight_management_avl.c prototype2/flight_management_avl.h airline_types.h
	$(CC) $(CFLAGS) -c prototype2/flight_management_avl.c -o $@

prototype2/passenger_management_hash.o: prototype2/passenger_management_hash.c prototype2/passenger_management_hash.h airline_types.h
	$(CC) $(CFLAGS) -c prototype2/passenger_management_hash.c -o $@

prototype2/reservation_management_bst.o: prototype2/reservation_management_bst.c prototype2/reservation_management_bst.h \
                                      prototype2/flight_management_avl.h prototype2/passenger_management_hash.h airline_types.h
	$(CC) $(CFLAGS) -c prototype2/reservation_management_bst.c -o $@

prototype2/flight_search_avl.o: prototype2/flight_search_avl.c airline_types.h prototype2/flight_management_avl.h
	$(CC) $(CFLAGS) -c prototype2/flight_search_avl.c -o $@

prototype2/passenger_search_hash.o: prototype2/passenger_search_hash.c airline_types.h prototype2/passenger_management_hash.h
	$(CC) $(CFLAGS) -c prototype2/passenger_search_hash.c -o $@

clean:
	rm -f *.o prototype1/*.o prototype2/*.o $(TARGET)

# Run the program with generated data
run_with_generated_data: $(TARGET)
	./$(TARGET) --generate

# Run the program with file data
run: $(TARGET)
	./$(TARGET)
