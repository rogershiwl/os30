
#include <stdio.h>
#include "mem.h"

/*32MB*/
#define MEM_TOTAL_SIZE			(0x2000000)
#define MEM_INIT_ADDRESS		(0x40000)

int mem_mgr_testcase01(void)
{
	unsigned int memaddr = 0;
	unsigned int memaddr2 = 0;
	unsigned int memaddr3 = 0;
	unsigned int memfree = 0;

	/* init */
	mem_init(MEM_INIT_ADDRESS, MEM_TOTAL_SIZE);

	/* check */
	memfree = get_mem_free();
	printf("init: memfree=0x%x\n", memfree);

	/* malloc */
	memaddr = mem_malloc(0x1000);
	printf("malloc: memaddr=0x%x\n", memaddr);
	memaddr2 = mem_malloc(0x2000);
	printf("malloc: memaddr2=0x%x\n", memaddr2);
	memaddr3 = mem_malloc(0x4000);
	printf("malloc: memaddr3=0x%x\n", memaddr3);

	/* free */
	mem_free(memaddr);

	/* malloc */
	memaddr = mem_malloc(0x8000);
	printf("malloc: memaddr=0x%x\n", memaddr);

	/* free */
	mem_free(memaddr2);
	mem_free(memaddr3);
	mem_free(memaddr);

	/* check */
	memfree = get_mem_free();
	printf("free: memfree=0x%x\n", memfree);

	return (MEM_TOTAL_SIZE == memfree);
}

void ut_mem_mgr(void)
{
	int ret = 0;

	printf(">>>>> mem_mgr_UT begin !\n\n");

	printf(">> mem_mgr_testcase01 begin\n");
	ret = mem_mgr_testcase01();
	if (1 == ret) {
		printf(">> mem_mgr_testcase01 success\n");
	} else {
		printf(">> mem_mgr_testcase01 fail\n");
	}

	printf("\n>>>>> mem_mgr_UT end !\n");
}

