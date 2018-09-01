all	:
	gcc -Wall -lrt -lpthread -O3 -march=native -mtune=generic -pipe -fstack-protector-strong -fno-plt main.c lib.c msg_queue.c
debug:
	gcc -Wall -lrt -lpthread -O3 -march=native -mtune=generic -pipe -fstack-protector-strong -fno-plt -pg main.c lib.c msg_queue.c
