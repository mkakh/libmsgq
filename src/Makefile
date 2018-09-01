all	:
	gcc -Wall -lrt -lpthread -O3 -march=native -mtune=generic -pipe -fstack-protector-strong -fno-plt sample.c lib.c msg_queue.c -o sample
debug:
	gcc -Wall -lrt -lpthread -O3 -march=native -mtune=generic -pipe -fstack-protector-strong -fno-plt -pg sample.c lib.c msg_queue.c -o sample
