
#include <unistd.h>
#include <assert.h>

#include "lock-free-fifo.h"


void read_fifo(void *pos, void *value, unsigned int len, unsigned int fifo_size, unsigned long period) 
{
	int i = 0;
	volatile int *fifo = (int *) pos;
	int r_cnt = fifo[1];
	int w_cnt = fifo[0];
	int rc = 0;
	volatile int *v = NULL;

	while ( w_cnt == r_cnt || rdiff(w_cnt,r_cnt) < len ) { 
		usleep(period); 
		w_cnt = fifo[0]; 
	}

	rc = R(r_cnt);
	v = (volatile int *) value;

	for ( i = 0; i < len; i++ ) {
		v[i] = fifo[rc + 2 + i];
	}

	r_cnt += len;

	if ( R(r_cnt) == fifo_size ) {
		r_cnt &= FLAG_MASK;
		r_cnt ^= FLAG_MASK;
	}

	fifo[1] = r_cnt;
} 


void write_fifo(void *pos, void *value, unsigned int len, unsigned int fifo_size, unsigned long period)
{
	int i = 0;
	volatile int *fifo = (int *) pos;
	int w_cnt = fifo[0];
	int r_cnt = fifo[1];
	int wc = 0;
	volatile int *v = NULL;

	while ( r_cnt == (w_cnt ^ FLAG_MASK) || wdiff(w_cnt, r_cnt) < len ) { 
		usleep(period); 
		r_cnt = fifo[1]; 
	}

	wc = R(w_cnt);
	v = (volatile int *) value;

	for ( i = 0; i < len; i++ ) {
		fifo[wc + 2 + i] = v[i];
	}

	w_cnt += len;

	if ( R(w_cnt) == fifo_size ) {
		w_cnt &= FLAG_MASK;
		w_cnt ^= FLAG_MASK;
	}

	fifo[0] = w_cnt;
} 

