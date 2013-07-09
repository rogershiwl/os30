/* 割り込み関係 */

#include "bootpack.h"

void init_pic(void)
/* PICの初期化 */
{
	io_out8(PIC0_IMR,  0xff  ); /* 全ての割り込みを受け付けない */
	io_out8(PIC1_IMR,  0xff  ); /* 全ての割り込みを受け付けない */

	io_out8(PIC0_ICW1, 0x11  ); /* エッジトリガモード */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7は、INT20-27で受ける */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1はIRQ2にて接続 */
	io_out8(PIC0_ICW4, 0x01  ); /* ノンバッファモード */

	io_out8(PIC1_ICW1, 0x11  ); /* エッジトリガモード */
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15は、INT28-2fで受ける */
	io_out8(PIC1_ICW3, 2     ); /* PIC1はIRQ2にて接続 */
	io_out8(PIC1_ICW4, 0x01  ); /* ノンバッファモード */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1以外は全て禁止 */
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 全ての割り込みを受け付けない */

	return;
}

#define FIFO_BUF_LEN		4
struct fifo {
	char dbuf[FIFO_BUF_LEN];
	int rd; /* default 0, point to the position data to be read */
			/* old data would be erased by new one */
	int wt; /* default 0, point to the position data to be writen */
	int fg;	/* default 0, 1 for data buffered */
};
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

void inthandler21(int *esp)
/* PS/2キーボードからの割り込み */
{
	io_out8(PIC0_OCW2, 0x61);	/* IRQ-01 done */

	kb_fifo_set_data(io_in8(0x60));

	return;
}

void inthandler2c(int *esp)
/* PS/2マウスからの割り込み */
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
	for (;;) {
		io_hlt();
	}
}

void inthandler27(int *esp)
/* PIC0からの不完全割り込み対策 */
/* Athlon64X2機などではチップセットの都合によりPICの初期化時にこの割り込みが1度だけおこる */
/* この割り込み処理関数は、その割り込みに対して何もしないでやり過ごす */
/* なぜ何もしなくていいの？
	→  この割り込みはPIC初期化時の電気的なノイズによって発生したものなので、
		まじめに何か処理してやる必要がない。									*/
{
	io_out8(PIC0_OCW2, 0x67); /* IRQ-07受付完了をPICに通知(7-1参照) */
	return;
}
