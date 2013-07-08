void io_hlt(void);

void HariMain(void)
{

fin:
	io_hlt();
	/* ‚±‚±‚ÉHLT‚ğ“ü‚ê‚½‚¢‚Ì‚¾‚ªACŒ¾Œê‚Å‚ÍHLT‚ªg‚¦‚È‚¢I */
	goto fin;

}
