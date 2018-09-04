CFLAGS = -Wall -O3 -march=native -mtune=generic -pipe -fstack-protector-strong -fno-plt
PROGRAM = show_thread
SRCS = show_thread.c
OBJS = $(SRCS:%.c=%.o)
LDLIBS = -Llib/
LDFLAGS = -lrt -lpthread -lmsgq
CC = gcc
.PHONY: all clean debug

all: $(PROGRAM)
$(PROGRAM): lib/libmsgq.a $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $@ $(LDFLAGS)
debug:
	$(CC) $(CFLAGS) -g $^ $(LDLIBS) -o $@ $(LDFLAGS)
clean:
	$(RM) $(PROGRAM) $(OBJS)
	rm -rf lib include
lib/libmsgq.a : ../../src/msg_queue.c ../../src/lib.c
	/bin/bash -c "cd ../../ &&  autoreconf -fi && ./configure --prefix=$(PWD) && make -j8 && make install"
