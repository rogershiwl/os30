#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};


void io_hlt(void);
void io_sti(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);


void init_gdtidt(void)
{
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
	struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) 0x0026f800;
	int i;

#if 0
	/* GDT�̏����� */
	for (i = 0; i <= LIMIT_GDT / 8; i++) {
		set_segmdesc(gdt + i, 0, 0, 0);
	}
	set_segmdesc(gdt + 1, 0xffffffff,   0x00000000, AR_DATA32_RW);
	set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
	load_gdtr(LIMIT_GDT, ADR_GDT);
#endif

	/* IDT�̏����� */
	for (i = 0; i <= 0x000007ff / 8; i++) {
		set_gatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(0x000007ff, 0x0026f800);

	/* IDT�̐ݒ� */
	set_gatedesc(idt + 0x21, (int) asm_inthandler21, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x27, (int) asm_inthandler27, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32);

	return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
	gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
	return;
}


void init_pic(void)
/* PIC�̏����� */
{
	io_out8(PIC0_IMR,  0xff  ); /* �S�Ă̊��荞�݂��󂯕t���Ȃ� */
	io_out8(PIC1_IMR,  0xff  ); /* �S�Ă̊��荞�݂��󂯕t���Ȃ� */

	io_out8(PIC0_ICW1, 0x11  ); /* �G�b�W�g���K���[�h */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7�́AINT20-27�Ŏ󂯂� */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1��IRQ2�ɂĐڑ� */
	io_out8(PIC0_ICW4, 0x01  ); /* �m���o�b�t�@���[�h */

	io_out8(PIC1_ICW1, 0x11  ); /* �G�b�W�g���K���[�h */
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15�́AINT28-2f�Ŏ󂯂� */
	io_out8(PIC1_ICW3, 2     ); /* PIC1��IRQ2�ɂĐڑ� */
	io_out8(PIC1_ICW4, 0x01  ); /* �m���o�b�t�@���[�h */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1�ȊO�͑S�ċ֎~ */
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 �S�Ă̊��荞�݂��󂯕t���Ȃ� */

	return;
}

/*
Keyboard
*/
int kbtimes = 0;
void inthandler21(int *esp)
/* PS/2�L�[�{�[�h����̊��荞�� */
{
	if (kbtimes > 300 )
		kbtimes = 0;
	draw_rect( 8, 20, 20, 20+kbtimes, 20);
	kbtimes+=10;

	for (;;) {
		io_hlt();
	}
}

/*
Mouse
*/
void inthandler2c(int *esp)
/* PS/2�}�E�X����̊��荞�� */
{
	draw_rect( 8, 20, 60, 40, 20);

	for (;;) {
		io_hlt();
	}
}

void inthandler27(int *esp)
/* PIC0����̕s���S���荞�ݑ΍� */
/* Athlon64X2�@�Ȃǂł̓`�b�v�Z�b�g�̓s���ɂ��PIC�̏��������ɂ��̊��荞�݂�1�x���������� */
/* ���̊��荞�ݏ����֐��́A���̊��荞�݂ɑ΂��ĉ������Ȃ��ł��߂��� */
/* �Ȃ��������Ȃ��Ă����́H
	��  ���̊��荞�݂�PIC���������̓d�C�I�ȃm�C�Y�ɂ���Ĕ����������̂Ȃ̂ŁA
		�܂��߂ɉ����������Ă��K�v���Ȃ��B									*/
{
	draw_rect( 8, 20, 100, 80, 20);

	io_out8(PIC0_OCW2, 0x67); /* IRQ-07��t������PIC�ɒʒm(7-1�Q��) */
	return;
}


/*
full screen set : 0, 0, 320, 200
*/
void draw_rect(int color, int x0, int y0, int length, int width)
{
	int y = 0, x = 0;
	int xsize = 320;
	char *p = 0;

	/* sram baseaddr : 0xa0000 */
	p = (char *)0xa0000;
	for (y = y0; y < y0 + width; y++) {
		for (x = x0; x < x0 + length; x++) {
			p[y * xsize + x] = color;
		}
	}
}

void HariMain(void)
{
	int xsize = 320;
	int ysize = 200;

	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC�̏��������I������̂�CPU�̊��荞�݋֎~������ */
	

	/* background */
	draw_rect(14, 0, 0, xsize, ysize);
	/* bottom bar top line */
	draw_rect( 8, 0, ysize-28, xsize, 1);
	/* bottom bar top2 line */
	draw_rect( 7, 0, ysize-27, xsize, 1);
	/* bottom bar */
	draw_rect( 8, 0, ysize-26, xsize, 26);


	/* left button */
	draw_rect(7,  3, ysize-24, 60,  1);
	draw_rect(7,  2, ysize-24,  1, 21);
	draw_rect(7,  3, ysize- 4, 60, 21);
	draw_rect(7, 59, ysize-23,  1, 19);
	draw_rect(7,  2, ysize- 3, 58,  1);
	draw_rect(7, 60, ysize-24,  1, 22);

	/* right button */
	draw_rect(15, xsize-50, ysize-24, 46,  1);
	draw_rect(15, xsize-50, ysize-23, 46, 18);
	draw_rect( 7, xsize-50, ysize- 3, 46,  1);
	draw_rect( 7, xsize- 3, ysize-24,  1, 22);
	
	io_out8(PIC0_IMR, 0xf9); /* PIC1?A?L?[?{?[?h?d??��A(11111001) */
	io_out8(PIC1_IMR, 0xef); /* ?}?E?X?d??��A(11101111) */
	
	while(1) {
		io_hlt();
	}

}
