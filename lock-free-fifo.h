#ifndef LOCK_FREE_FIFO_H_
#define LOCK_FREE_FIFO_H_

#define FLAG_MASK	0x80000000
#define REG_MASK	0x7FFFFFFF
#define MAX_BUF_SIZE	REG_MASK

#define R(x)		(x & REG_MASK)
#define F(x)		(x & FLAG_MASK)

#define rdiff(w_cnt,r_cnt)	(F(w_cnt) == F(r_cnt) ? \
					( R(w_cnt) >= R(r_cnt) ? (unsigned int) (R(w_cnt) - R(r_cnt)) : 0 ) \
					: (unsigned int) (R(w_cnt) - R(r_cnt) + MAX_BUF_SIZE ))

#define wdiff(w_cnt,r_cnt)	(F(w_cnt) == F(r_cnt) ? \
					(unsigned int) (REG_MASK - wc) \
					: (R(w_cnt) > R(r_cnt) ? (unsigned int) (R(w_cnt) - R(r_cnt)) \
								: (unsigned int)(R(r_cnt) - R(w_cnt))))


void read_fifo(void *pos, void *value, unsigned int len, unsigned int fifo_size, unsigned long period);
void write_fifo(void *pos, void *value, unsigned int len, unsigned int fifo_size, unsigned long period);


#endif
