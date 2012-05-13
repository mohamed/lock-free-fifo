#ifndef LOCK_FREE_FIFO_H_
#define LOCK_FREE_FIFO_H_

#define FLAG_MASK	0x80000000
#define REG_MASK	0x7FFFFFFF
#define MAX_BUF_SIZE	REG_MASK

#define reg(x)	(x & REG_MASK)
#define flag(x)	(x & FLAG_MASK)
#define UI	(unsigned int)

#define rdiff(w,r)	(flag(w) == flag(r) ? \
			( reg(w) >= reg(r) ? \
			  	UI (reg(w) - reg(r)) \
			  	: 0 ) \
			: UI (reg(w) - reg(r) + MAX_BUF_SIZE ))

#define wdiff(w,r)	(flag(w) == flag(r) ? \
			UI (REG_MASK - wc) \
			: (reg(w) > reg(r) ? \
				UI (reg(w) - reg(r)) \
				: UI(reg(r) - reg(w))))


void read_fifo(void *pos, void *value, unsigned int len, 
		unsigned int fifo_size, unsigned long period);
void write_fifo(void *pos, void *value, unsigned int len, 
		unsigned int fifo_size, unsigned long period);


#endif
