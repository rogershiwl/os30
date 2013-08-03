
#include "bootpack.h"
#include "task.h"
#include "string.h"

/*
comments about Task:
JMP to TSS is the way to switch between tasks
*/

#define DELAY_COUNT		10000

struct TSS32 tss_a;
struct TSS32 tss_b;


int task_a_esp;
int task_b_esp;

int task_a_count = 0;
int task_b_count = 0;

struct TIMER *mt_timer;
int mt_tr;
int task_switch_times = 0;


void task_a_main(void)
{
	volatile int i = 0;

	do {
		if (task_a_count > 160) {
			task_a_count = 0;
		}
		task_a_count++;

		for (i = 0; i < DELAY_COUNT; i++) {
			/**/
		}
	} while (1);
}

void task_b_main(struct SHEET *sht_back)
{
	struct FIFO32 fifo;
	struct TIMER *timer_put, *timer_1s;
	int i, fifobuf[128], count = 0, count0 = 0;
	char s[128];

	fifo32_init(&fifo, 128, fifobuf);
	timer_put = timer_alloc();
	timer_init(timer_put, &fifo, 1);
	timer_settime(timer_put, 1);
	timer_1s = timer_alloc();
	timer_init(timer_1s, &fifo, 100);
	timer_settime(timer_1s, 100);

	for (;;) {
		count++;
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == 1) {
				sprintf(s, "+%11d, times=%d", count, task_switch_times);
				putfonts8_asc_sht(sht_back, 0, 144, COL8_FFFFFF, COL8_008484, s, 31);
				timer_settime(timer_put, 1);
			} else if (i == 100) {
				sprintf(s, "-%11d", count - count0);
				putfonts8_asc_sht(sht_back, 0, 128, COL8_FFFFFF, COL8_008484, s, 31);
				count0 = count;
				timer_settime(timer_1s, 100);
			}
		}
	}

}

/*
1st
task_init
*/
void task_init(struct MEMMAN *memman, struct SHEET *sht_back)
{
	/* init tss_a */
	task_a_esp		= (int) memman_alloc_4k(memman, 4096);
	memset(&tss_a, 0, sizeof(struct TSS32));
	tss_a.ldtr		= 0;
	tss_a.iomap	= 0x40000000;
#if 0
	tss_a.eip		= (int)&task_a_main;
	tss_a.eflags	= 0x00000202;	/**/
	tss_a.esp		= task_a_esp;
	tss_a.es		= 1 * 8;
	tss_a.cs		= 2 * 8;
	tss_a.ss		= 1 * 8;
	tss_a.ds		= 1 * 8;
	tss_a.fs		= 1 * 8;
	tss_a.gs		= 1 * 8;
#endif

	/* init tss_b */
	task_b_esp		= (int) memman_alloc_4k(memman, 4096) + 4096 - 8;
	memset(&tss_b, 0, sizeof(struct TSS32));
	tss_b.ldtr 	= 0;
	tss_b.iomap	= 0x40000000;
	
	tss_b.eip		= (int)&task_b_main;
	tss_b.eflags	= 0x00000202;	/**/
	tss_b.esp		= task_b_esp;
	tss_b.es		= 1 * 8;
	tss_b.cs		= 2 * 8;
	tss_b.ss		= 1 * 8;
	tss_b.ds		= 1 * 8;
	tss_b.fs		= 1 * 8;
	tss_b.gs		= 1 * 8;

	load_tr(3 * 8);

	*((int *) (task_b_esp + 4)) = (int) sht_back;

	mt_timer = timer_alloc();
	timer_settime(mt_timer, 200);
	mt_tr = 3 * 8;

}

/*
2nd
task_switch
*/
void mt_taskswitch(void)
{
	task_switch_times++;
	if (mt_tr == 3 * 8) {
		mt_tr = 4 * 8;
		timer_settime(mt_timer, 2);
		task_switch4();
	} else {
		mt_tr = 3 * 8;
		timer_settime(mt_timer, 2);
		task_switch3();
	}

	return;
}



