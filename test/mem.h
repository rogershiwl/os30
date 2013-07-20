

#define M_5A5A		0x5a5a5a5a
#define M_A5A5		0xa5a5a5a5
#define M_MASK		0xffffffff
#define M_NODES		1024


struct mem_con {
	unsigned int addr;
	unsigned int size;
	unsigned int busy; /* 0-free, 1-malloc */
};
struct mem_struct {
	unsigned int nodes;
	struct mem_con node[M_NODES];
};



void mem_init(unsigned int start, unsigned int total_size);
unsigned int mem_test(unsigned int start, unsigned int end);
unsigned int mem_malloc(unsigned int size);
unsigned int mem_free(unsigned int addr);
unsigned int get_mem_free(void);

