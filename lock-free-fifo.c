
/* The following define is needed to expose nanosleep() */
#define _POSIX_C_SOURCE 200809L
#include <time.h> /* needed for nanosleep() */
#include <assert.h>

#include "lock-free-fifo.h"

void nsleep(unsigned long seconds, unsigned long nanoseconds)
{
	struct timespec s;
	s.tv_sec = seconds;
	s.tv_nsec = nanoseconds;
	nanosleep(&s, NULL);
}

void read_fifo(void *fifo, void *value, unsigned int len,
		unsigned int fifo_size, unsigned long period) 
{
	assert(len <= MAX_BUF_SIZE);
	assert(fifo_size <= MAX_BUF_SIZE);
	assert(fifo != NULL);
	assert(period != 0);

	int i = 0;
	volatile int *rfifo = (int *) fifo;
	int r_cnt = rfifo[1];
	int w_cnt = rfifo[0];
	int rc = 0;
	volatile int *v = NULL;

	while ( w_cnt == r_cnt || rdiff(w_cnt,r_cnt) < len ) {
		nsleep(0, period);
		w_cnt = rfifo[0];
	}

	rc = reg(r_cnt);
	v = (volatile int *) value;

	for ( i = 0; i < len; i++ ) {
		v[i] = rfifo[rc + 2 + i];
	}

	rc = reg(r_cnt);
	r_cnt += len;

	if ( reg(r_cnt) == fifo_size || rc > reg(r_cnt) ) {
		r_cnt &= FLAG_MASK;
		r_cnt ^= FLAG_MASK;
	}

	rfifo[1] = r_cnt;
} 

void write_fifo(void *fifo, void *value, unsigned int len,
		unsigned int fifo_size, unsigned long period)
{
	assert(len <= MAX_BUF_SIZE);
	assert(fifo_size <= MAX_BUF_SIZE);
	assert(fifo != NULL);
	assert(period != 0);

	int i = 0;
	volatile int *wfifo = (int *) fifo;
	int w_cnt = wfifo[0];
	int r_cnt = wfifo[1];
	int wc = 0;
	volatile int *v = NULL;

	while ( r_cnt == (w_cnt ^ FLAG_MASK) || wdiff(w_cnt, r_cnt) < len ) {
		nsleep(0, period);
		r_cnt = wfifo[1];
	}

	wc = reg(w_cnt);
	v = (volatile int *) value;

	for ( i = 0; i < len; i++ ) {
		wfifo[wc + 2 + i] = v[i];
	}

	wc = reg(w_cnt);
	w_cnt += len;

	if ( reg(w_cnt) == fifo_size || wc > reg(w_cnt) ) {
		w_cnt &= FLAG_MASK;
		w_cnt ^= FLAG_MASK;
	}

	wfifo[0] = w_cnt;
} 

