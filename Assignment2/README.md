# Tasmania Transport Network Analysis
## KIT205 Assignment 2

This project implements a graph-based transport network analysis system for Tasmania's public transportation network. The implementation provides tools for analysing accessibility, service frequency, and path finding.

## Building the Project

To build the project, use the provided Makefile:

```bash
make
```

This will compile three executables:
- `transport_analysis` - Main transport network analysis tool
- `test_transport` - Test suite for transport functionality
- `gtfs_analysis` - GTFS data parser and analysis tool

## Running the Project

### Transport Network Analysis

To run the main transport network analysis:

```bash
./transport_analysis
```

### Running Tests

To run the test suite:

```bash
./test_transport
```

### GTFS Data Analysis

To analyse GTFS data from the data directory:

```bash
./gtfs_analysis
```

## Input Data

The project uses GTFS (General Transit Feed Specification) data stored in the `data/gtfs/` directory. The provided dataset includes Tasmania's public transport information such as:

- Stops (`stops.txt`)
- Routes (`routes.txt`) 
- Trip schedules (`trips.txt`, `stop_times.txt`)
- Calendar information (`calendar.txt`, `calendar_dates.txt`)

## Project Structure

- `src/` - Source code files
  - `transport_graph.c` - Core graph implementation
  - `gtfs_parser.c` - GTFS data parser
  - `network_analysis.c` - Network analysis algorithms
  - `graph.h` - Main header file defining data structures
  - `main_transport.c` - Entry point for transport analysis
  - `main_gtfs.c` - Entry point for GTFS analysis
- `tests/` - Test files
  - `test_transport.c` - Tests for transport network functionality
- `data/` - Input data files
  - `gtfs/` - GTFS-formatted public transport data
  - `services/` - Service location data

## Academic Integrity Statement

This project includes proper attribution for algorithms and reference materials used, as documented in the source code header comments. 
