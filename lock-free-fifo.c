
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "lock-free-fifo.h"


unsigned int rdiff(int w_cnt, int r_cnt)
{
    int wc = w_cnt & 0x7FFFFFFF;
    int rc = r_cnt & 0x7FFFFFFF;
    int wf = w_cnt & 0x80000000;
    int rf = r_cnt & 0x80000000;
    
    unsigned int ret = 0;
    
    if (wf == rf) { 
        if (wc >= rc) {
            ret = (unsigned int)(wc - rc);
        }
        else {
            /* We should never reach here? */
            assert(1 == 0);
        }
    }
    else {
        ret = (unsigned int)(wc - rc + 0x7FFFFFFF);
    }

    return ret;
}


unsigned int wdiff(int w_cnt, int r_cnt)
{
    int wc = w_cnt & 0x7FFFFFFF;
    int rc = r_cnt & 0x7FFFFFFF;
    int wf = w_cnt & 0x80000000;
    int rf = r_cnt & 0x80000000;
    
    unsigned int ret = 0;
    
    if (wf == rf) { 
        ret = (unsigned int)(0x7FFFFFFF - wc);
    }
    else {
        if (wc > rc)
            ret = (unsigned int)(wc - rc);
        else 
            ret = (unsigned int)(rc - wc);
    }

    return ret;
}


void read_fifo(void *pos, void *value, unsigned int len, unsigned int fifo_size, unsigned long period) 
{
    int i;
    volatile int *fifo = (int *) pos;
    int r_cnt = fifo[1];
    int w_cnt = fifo[0];
    
    while ( w_cnt == r_cnt || rdiff(w_cnt,r_cnt) < len ) { 
        fprintf(stdout, "FIFO is empty\n");
        usleep(period); 
        w_cnt = fifo[0]; 
    }
    
    for ( i = 0; i < len; i++ ) {
        ((volatile int *) value)[i] = fifo[(r_cnt & 0x7FFFFFFF) + 2 + i];
    }
    
    r_cnt += len;
    
    if( (r_cnt & 0x7FFFFFFF) == fifo_size ) {
        r_cnt &= 0x80000000;
        r_cnt ^= 0x80000000;
    }
    
    fifo[1] = r_cnt;
} 

void write_fifo(void *pos, void *value, unsigned int len, unsigned int fifo_size, unsigned long period)
{
    int i;
    volatile int *fifo = (int *) pos;
    int w_cnt = fifo[0];
    int r_cnt = fifo[1];
    
    while ( r_cnt == (w_cnt ^ 0x80000000) || wdiff(w_cnt, r_cnt) < len ) { 
        fprintf(stdout, "FIFO is full\n");
        usleep(period); 
        r_cnt = fifo[1]; 
    }
    
    for ( i = 0; i < len; i++ ) {
        fifo[(w_cnt & 0x7FFFFFFF) + 2 + i] = ((volatile int *) value)[i];
    }
    
    w_cnt += len;

    if( (w_cnt & 0x7FFFFFFF) == fifo_size ) {
        w_cnt &= 0x80000000;
        w_cnt ^= 0x80000000;
    }

    fifo[0] = w_cnt;
} 

