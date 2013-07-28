
#include "timer.h"
#include "string.h"

/*
comments about Timer:
ic		: PIT 6254
		  clk freq is 1.19318 MHz
IRQ		: IRQ0
*/

struct timer pit_timer;

/*
1st
pit timer init
*/
unsigned int timer_init(unsigned int start, unsigned int end)
{
	io_out8(PIT_CTRL, VAL_0X34);
	/* set timer freq */
	io_out8(PIT_COUNT0, VAL_CYCLE_LOW);
	io_out8(PIT_COUNT0, VAL_CYCLE_HIGH);

	pit_timer.count = 0;
}

/*
2nd
pit timer en
PIC0_IMR	bit0 set to 0
*/

/*
3rd
pit timer intr handler
inthandler20 defined in int.c
_asm_inthandler20 defined in naskfunc.nas
void inthandler20(int *esp)
//
PIT timer intr handler :
	20 + intrnum
OCW2 :
	bit0-2 is intr num
	bit5-7(011) is to end the intr
//
{
	io_out8(PIC0_OCW2, 0x60);
	return;
}
*/

/*
4th
pit timer intr handler register
in init_gdtidt() with set_gatedesc()
*/


