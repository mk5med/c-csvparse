CC = gcc
CFLAGS = -I. -g
ODIR = ./.build
OBJ = $(ODIR)/src/main.o $(ODIR)/src/print_help.o $(ODIR)/src/parse_csv.o $(ODIR)/src/lib/padding.o
DEPS = src/lib/padding.h
TARGET = csvparse

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

init:
	mkdir $(ODIR)
	mkdir $(ODIR)/src
	mkdir $(ODIR)/src/lib
	# You can now run 'make' or 'make clean'

clean:
	rm -r $(ODIR)
	# You can now run 'make init'