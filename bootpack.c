void io_hlt(void);

void HariMain(void)
{

fin:
	io_hlt();
	/* ここにHLTを入れたいのだが、C言語ではHLTが使えない！ */
	goto fin;

}
