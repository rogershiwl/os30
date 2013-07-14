/* bootpackのメイン */

#include "bootpack.h"
#include <stdio.h>

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256];
	int mx, my;
	unsigned char data = 0;
	char sdata[32];
	int rd, wt;
	unsigned int memsize = 0;

	init_gdtidt();
	init_pic();
	kb_fifo_init();

	io_sti(); /* IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除 */

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2; /* 画面中央になるように座標計算 */
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	memsize = mem_test(0x400000, 0xbfffffff);
	sprintf(s, "MEM=%d MB", memsize/(1024 * 1024));
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 30, COL8_FFFFFF, s);
	
	io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
	io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

	for (;;) {
		if(!kb_fifo_with_data()) {
			io_stihlt();
		} else {
			kb_fifo_get_data(&data);
			kb_fifo_pointer(&rd, &wt);
			sprintf(sdata, "0x%2x, rd=%d, wt=%d", data, rd, wt);

			boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 32 * 8 - 1, 15);
			putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, sdata);
		}
	}
}
