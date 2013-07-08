void io_hlt(void);

void HariMain(void)
{
	char *p = 0;
	int i = 0;

	/* set color in 0xa0000 - 0xaffff */
	p = (char *)0xa0000;
	
	for (i = 0; i < 0x10000; i++) {
		/*
		0x00 - black
		0x0F - white
		0x0E - yellow, 0x0D - pink
		0x0C - red   , 0x0B -     , 0x0A - green 
		0x09 - blue
		0x08 - gray  , 0x07 - sorrow gray
		0x70 - dark red
		p[i] = i & 0x0F;
		p[i] = (i & 0x0F) | 0x07;
		p[i] = (i & 0x0F) | 0x0C;
		*/
		p[i] = (i & 0x0F) | 0x0E;
	}
	
	while(1) {
		io_hlt();
	}

}
