

#define M_5A5A		0x5a5a5a5a
#define M_A5A5		0xa5a5a5a5
#define M_MASK		0xffffffff
unsigned int mem_test(unsigned int start, unsigned int end)
{
	volatile unsigned int *ptr = 0;
	volatile unsigned int old = 0;
		
	for(ptr = (unsigned int *)start; ptr < (unsigned int *)end; ptr+=0x1000) {
		old = *ptr;
		*ptr = M_5A5A;
		*ptr ^= M_MASK;
		if(M_A5A5 != *ptr) {
			*ptr = old;
			break;
		}
		*ptr ^= M_MASK;
		if(M_5A5A != *ptr) {
			*ptr = old;
			break;
		}
		*ptr = old;
	}

	return (unsigned int)ptr;
}

