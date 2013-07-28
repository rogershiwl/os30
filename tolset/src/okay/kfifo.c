/* äÑÇËçûÇ›ä÷åW */

#include "bootpack.h"
#include "kfifo.h"


struct fifo kb_fifo;

void kb_fifo_init(void)
{
	kb_fifo.rd = 0;
	kb_fifo.wt = 0;
	kb_fifo.fg = 0;
	
	memset(kb_fifo.dbuf, 0, sizeof(FIFO_BUF_LEN));
}

void kb_fifo_set_data(char data)
{
	int wt = kb_fifo.wt;
	int rd = kb_fifo.rd;
	int fg = kb_fifo.fg;
	
	/* check free space */
	if((wt == rd) && (fg != 0)) {
		/* wt catch rd, one old data erased */
		rd = (++rd) < FIFO_BUF_LEN ? rd : 0;
	}

	kb_fifo.dbuf[wt] = data;
	wt = (++wt) < FIFO_BUF_LEN ? wt : 0;
	kb_fifo.wt = wt;
	kb_fifo.fg = 1;
	kb_fifo.rd = rd;
}
/*
return value:
0 : OK
1 : Error
*/
int kb_fifo_get_data(char *pdata)
{
	int wt = kb_fifo.wt;
	int rd = kb_fifo.rd;
	
	/* check data */
	*pdata = 0xFF;
	if(0 == kb_fifo.fg) {
		return 1;
	}
	
	*pdata = kb_fifo.dbuf[rd];
	rd = (++rd) < FIFO_BUF_LEN ? rd : 0;
	kb_fifo.rd = rd;

	/* check data remained */
	if(wt == rd) {
		/* rd catch wt, no data left */
		kb_fifo.fg = 0;
	}
	
	return 0;
}
int kb_fifo_with_data(void)
{
	return kb_fifo.fg;
}
void kb_fifo_pointer(int *prd, int *pwt)
{
	*prd = kb_fifo.rd;
	*pwt = kb_fifo.wt;
}

