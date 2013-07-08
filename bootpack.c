void io_hlt(void);

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
	
	while(1) {
		io_hlt();
	}

}
