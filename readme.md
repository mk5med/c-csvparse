This project creates a simple CSV file parser such that reading a csv of any size will print in the following format:

## Usage:
Help
```
Available commands:
    -w, --col-width Set column width (-1 for default separation between columns). 1 is the default.
    -c, --columns   A column index to display. The argument can also be called in a string format.
                            Indexes start at 0 and must be in increasing order.
    -h, --help      Print this help menu.
```

Simple
```
./csvparse < log.csv
./csvparse -c '0 1 5' < log.csv
```
