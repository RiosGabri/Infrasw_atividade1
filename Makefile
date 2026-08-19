CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

TARGET = processflow
OBJECTS = processflow.o control.o task.o parser.o execution.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

processflow.o: processflow.c control.h task.h parser.h execution.h
	$(CC) $(CFLAGS) -c processflow.c

control.o: control.c control.h
	$(CC) $(CFLAGS) -c control.c

task.o: task.c task.h
	$(CC) $(CFLAGS) -c task.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

execution.o: execution.c execution.h task.h
	$(CC) $(CFLAGS) -c execution.c

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
