

#define FIFO_BUF_LEN		4

struct fifo {
	unsigned char dbuf[FIFO_BUF_LEN];
	int rd; /* default 0, point to the position data to be read */
			/* old data would be erased by new one */
	int wt; /* default 0, point to the position data to be writen */
	int fg;	/* default 0, 1 for data buffered */
};

