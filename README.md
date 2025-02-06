FINAL PI FEBRERO 2025
# Traffic Violations Analysis System

This project processes and analyzes traffic violations data from different cities (NYC and Chicago), generating various statistical reports in CSV format.

## Project Structure

The project is organized into frontend and backend components:

### Frontend (`/frontend`)
Contains the data processing and query generation logic.

#### Source Files (`/frontend/src/`)
- `queries.c`: Implements the generation of statistical reports (CSV files)
- `processData.c`: Handles the parsing and processing of input CSV files
- `dataValidation.c`: Validates input data and file formats

#### Header Files (`/frontend/inc/`)
- `queries.h`: Declarations for query generation functions
- `processData.h`: Declarations for data processing functions

### Backend (`/backend`)
Contains the core data structures and operations.

#### Source Files (`/backend/src/`)
- `cityADT.c`: Implementation of the City Abstract Data Type

#### Header Files (`/backend/inc/`)
- `cityADT.h`: Interface for the City ADT

### Root Directory
- `main.c`: Program entry point and main execution flow
- `Makefile`: Build configuration

## Compilation

To compile the project, use the following command in the root directory:

```bash
make NYC
# or
make CHI
```

This will generate two executables:
- `parkingTicketsNYC`: For processing New York City traffic violations
- `parkingTicketsCHI`: For processing Chicago traffic violations

## Usage

The program requires two input CSV files:

```bash
./parkingTicketsNYC tickets.csv infractions.csv
# or
./parkingTicketsCHI tickets.csv infractions.csv
```

Where:
- `tickets.csv`: Contains the traffic violations data
- `infractions.csv`: Contains the infractions reference data

The program will generate four output files:
- `query1.csv`: Infractions statistics by year
- `query2.csv`: Top plates by agency
- `query3.csv`: Most common infractions by month
- `query4.csv`: Daily average statistics by agency

## Authors

* **65127** Natalia Besuschio
* **65732** Enzo Canelo
* **65741** Matias Sanchez Novelli
