
#include "mem.h"
#include "string.h"


struct mem_struct mems;

/*
test memory size
*/
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

void mem_init(unsigned int start, unsigned int total_size)
{
	memset(&mems, 0, sizeof(mems));

    mems.nodes = 1;
    mems.node[0].addr = start;
    mems.node[0].size = total_size;
	mems.node[0].busy = 0;
}

/*
return :
invalid : 0, NULL
*/
unsigned int mem_malloc(unsigned int size)
{
	unsigned int nodes = 0;
	unsigned int addr = 0;
	unsigned int i = 0, j = 0;
	
	nodes = mems.nodes;
	for (i = 0; i < nodes; i++) {
		if((mems.node[i].busy == 0) && (size <= mems.node[i].size)) {
			/* alloc mem */
			addr = mems.node[i].addr;

			if (size == mems.node[i].size) {
				/* space left to mem alloced */
				mems.node[i].busy = 1;
			} else {
				/* check space */
				if (nodes >= M_NODES) {
					return 0;
				} else {
					mems.node[i].addr += size;
					mems.node[i].size -= size;

					/* move one space to mem alloced */
					for(j = mems.nodes; j > i; j--) {
						mems.node[j] = mems.node[j - 1];
					}
					
					/* fill mem alloced */
					mems.node[i].addr = addr;
					mems.node[i].size = size;
					mems.node[i].busy = 1;
					mems.nodes++;
					break;
				}
			}
		}
	} 

	return addr;
}

/*
return
0 : success
*/
unsigned int mem_free(unsigned int addr)
{
	unsigned int nodes = 0;
	unsigned int i = 0, j = 0;
	
	nodes = mems.nodes;
	/* find node to free */
	for (i = 0; i < nodes; i++) {
		if((mems.node[i].busy == 1) && (addr == mems.node[i].addr)) {
			break;
		}
	}

	if (i == nodes) {
		/* not found, error */
		return 1;
	}

	if (i < nodes - 1) {
		/* joint, check later */
		if((mems.node[i + 1].busy == 0) && (mems.node[i + 1].addr == mems.node[i].addr + mems.node[i].size)) {
			mems.node[i + 1].addr = mems.node[i].addr;
			mems.node[i + 1].size += mems.node[i].size;
			/* busy keep unchange */

			/* move << */
			mems.nodes--;
			for (j = i; j < mems.nodes; j++) {
				mems.node[j] = mems.node[j + 1];
			}
		}
	}

	if (i > 0) {
		/* joint, check former */
		if((mems.node[i - 1].busy == 0) && (mems.node[i].addr == mems.node[i - 1].addr + mems.node[i - 1].size)) {
			mems.node[i].addr = mems.node[i - 1].addr;
			mems.node[i].size += mems.node[i - 1].size;
			mems.node[i].busy = 0;

			/* move << */
			mems.nodes--;
			for (j = i - 1; j < mems.nodes; j++) {
				mems.node[j] = mems.node[j + 1];
			}

			/* joint over, return */
			return 0;
		}
	}

	/* space left to mem free */
	mems.node[i].busy = 0;

	return 0;
}

unsigned int get_mem_free(void)
{
	unsigned int ttfrsize = 0;
	unsigned int nodes = 0;
	unsigned int i = 0;

	nodes = mems.nodes;
	for (i = 0; i < nodes; i++) {
		if (0 == mems.node[i].busy) {
			ttfrsize += mems.node[i].size;
		}
	} 

	return ttfrsize;
}

