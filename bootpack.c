void io_hlt(void);

void HariMain(void)
{

fin:
	io_hlt();
	/* ������HLT����ꂽ���̂����AC����ł�HLT���g���Ȃ��I */
	goto fin;

}
