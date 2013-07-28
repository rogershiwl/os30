

#define PIT_CTRL		0x43
#define PIT_COUNT0		0x40


#define VAL_0X34		(0x34)
/* 11932(0x2E9C) set to 100Hz */
#define VAL_CYCLE_LOW	(0x9C)
#define VAL_CYCLE_HIGH	(0x2E)


struct timer {
	unsigned int count;
};

extern struct timer pit_timer;


