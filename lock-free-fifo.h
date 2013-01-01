#ifndef LOCK_FREE_FIFO_H_
#define LOCK_FREE_FIFO_H_

/**

Lock-free FIFO implementation. The FIFO is organized as follows:

 +-----------------------------------------------------------------------+
 | address  |             Organization in memory                         |
 |          |   31 | 30                                                0 |
 +----------+------------------------------------------------------------+
 |    0     | Flag |  Write Counter (wr_cnt)                             |
 +----------+------+-----------------------------------------------------+
 |    1     | Flag |  Read Coutner (rd_cnt)                              |
 +----------+------+-----------------------------------------------------+
 |    2     |                D A T A                                     |
 +----------+------------------------------------------------------------+
 |    3     |                D A T A                                     |
 +----------+------------------------------------------------------------+
 |   ...    |                D A T A                                     |


The first two 32-bit words are used to store the write and read counters.
The write/read counters words consist of two parts:
 1) A flag used to detect counter overflow
 2) 31-bit counter.
The maximum buffer size supported by this implementation on a 32-bit machine is
0x7FFFFFFF or 2 GB.

*/

#define FLAG_MASK	0x80000000
#define REG_MASK	0x7FFFFFFF
#define MAX_BUF_SIZE	0x7FFFFFFF

#define reg(x)	(x & REG_MASK)
#define flag(x)	(x & FLAG_MASK)
#define UI	(unsigned int)

/*
 rdiff and wdiff are macros to determine the difference between the
 read and write counters
*/
#define rdiff(w,r)	(flag(w) == flag(r) ? \
				( reg(w) >= reg(r) ? \
					UI (reg(w) - reg(r)) : 0 ) \
				: UI (reg(w) - reg(r) + MAX_BUF_SIZE ))

#define wdiff(w,r)	(flag(w) == flag(r) ? \
				UI (REG_MASK - wc) \
				: (reg(w) > reg(r) ? \
					UI (reg(w) - reg(r)) \
					: UI(reg(r) - reg(w))))


/* nsleep() is used to put the thread into sleeping state.
   It uses nanosleep() from the POSIX standard */
void nsleep(unsigned long seconds, unsigned long nanoseconds);

/* The read and write primitives */
void read_fifo(void *buf, void *value, unsigned int len,
		unsigned int fifo_size, unsigned long period);
void write_fifo(void *buf, void *value, unsigned int len,
		unsigned int fifo_size, unsigned long period);

#endif
