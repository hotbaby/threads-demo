CC = gcc
LDFLAGS = -lpthread
CLFAGS = -Werror
TARGET = app

all:$(TARGET)

app:
	$(CC) threads_demo.c $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm $(TARGET)
