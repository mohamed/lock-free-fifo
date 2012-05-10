#ifndef LOCK_FREE_FIFO_H_
#define LOCK_FREE_FIFO_H_

unsigned int rdiff(int w_cnt, int r_cnt);
unsigned int wdiff(int w_cnt, int r_cnt);

void read_fifo(void *pos, void *value, unsigned int len, unsigned int fifo_size, unsigned long period);
void write_fifo(void *pos, void *value, unsigned int len, unsigned int fifo_size, unsigned long period);


#endif
