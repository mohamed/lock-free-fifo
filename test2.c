
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#include "lock-free-fifo.h"

#define PERIOD 10000000L

#define N 64

#define sizeof_fifo(T,N) N*((sizeof(T)+(sizeof(T)%4)+3)>>2)
#define sizeof_token(T) ((sizeof(T)+(sizeof(T)%4)+3)/4)

/* Messages Buffers , + 2 is for the WR/RD registers */
int msg_buf1[N + 2];
int msg_buf2[N + 2];

void *thread1(void *args)
{
	static int c = 0;
	int buf[2];
	unsigned long period = PERIOD;

	while (1) {
		buf[0] = c;
		c++;
		buf[1] = c;
		c++;
		write_fifo((void *)msg_buf1, (void *)buf, sizeof_token(buf), sizeof_fifo(int,N), period);
		nsleep(0, period);
	}   
}

void *thread2(void *args)
{
	int buf[2];
	unsigned long period = PERIOD;
	
	while (1) {
		read_fifo((void *)msg_buf1, (void *)buf, sizeof_token(buf), sizeof_fifo(int,N), period);
		buf[0] = buf[0] * 2;
		buf[1] = buf[1] * 2;
		write_fifo((void *)msg_buf2, (void *)buf, sizeof_token(buf), sizeof_fifo(int,N), period);
		nsleep(0, period);
	}
}

void *thread3(void *args)
{
	int val;
	static int exp = 0;
	unsigned long period = PERIOD/2;
	
	while (1) {
		read_fifo((void *)msg_buf2, (void *)&val, sizeof_token(val), sizeof_fifo(int,N), period); 
		assert (val == exp);
		exp += 2;
		fprintf(stdout,"Thread 3: Read value = %d\n", val);
		nsleep(0, period);
	}
}


int main()
{
	pthread_t t[3];
	
	msg_buf1[0] = msg_buf1[1] = 0;
	msg_buf2[0] = msg_buf2[1] = 0;
	
	pthread_create( &t[0], NULL, thread1, NULL);
	pthread_create( &t[1], NULL, thread2, NULL);
	pthread_create( &t[2], NULL, thread3, NULL);
	
	while (1) {
		nsleep(1, 0);
		printf("Hello from main\n");
	}
		
	   
	return 0;
}
