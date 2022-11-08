CC = gcc
CFLAGS = -I. -g
ODIR = ./.build
OBJ = $(ODIR)/main.o $(ODIR)/Libraries/padding.o
DEPS = Libraries/padding.h
TARGET = csvparse

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

init:
	mkdir $(ODIR)
	mkdir $(ODIR)/Libraries
	# You can now run 'make' or 'make clean'

clean:
	rm -r $(ODIR)
	# You can now run 'make init'