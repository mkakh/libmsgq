all	:
	gcc -Wall -lrt -lpthread -O3 -march=native -mtune=generic -pipe -fstack-protector-strong -fno-plt show_thread.c ../../src/lib.c ../../src/msg_queue.c -o show_thread
debug:
	gcc -Wall -lrt -lpthread -O3 -march=native -mtune=generic -pipe -fstack-protector-strong -fno-plt -pg show_thread.c ../../src/lib.c ../../src/msg_queue.c -o show_thread
