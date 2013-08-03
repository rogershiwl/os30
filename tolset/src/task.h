


struct TSS32 {
	int /*8*/backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int /*10*/eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int /*6*/es, cs, ss, ds, fs, gs;
	int /*2*/ldtr, iomap;
};

extern int task_a_count;
extern int task_b_count;

extern struct TSS32 tss_a;
extern struct TSS32 tss_b;
extern struct TIMER *mt_timer;

extern void task_init(struct MEMMAN *memman, struct SHEET *sht_back);
extern void mt_taskswitch(void);

