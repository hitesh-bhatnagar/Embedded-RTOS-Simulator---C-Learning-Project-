CC = gcc
CFLAGS = -Wall -Iinclude  	# 	Show all warnings & include headers from include/ folder

SRC = src/main.c src/task.c src/scheduler.c src/ipc.c src/uart.c src/semaphore.c src/interrupt.c src/timer.c		# Source files to compile
OUT = rtos 

all: $(OUT)			# 	Compiles the files into an output file called rtos
$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean: 	# 	Optional target to remove the executable
	rm -f $(OUT)