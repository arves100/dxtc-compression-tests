#ifndef _TEST_H_
#define _TEST_H_

enum TestModes
{
	TEST_DXT1,
	TEST_DXT3,
	TEST_DXT5,
};

extern bool CreateTest1(int testMode);
extern bool CreateTest2(int testMode);
extern bool CreateTest3(int testMode);
extern void DeleteTest2Memory(void);
extern void DeleteTest1Memory(void);

#endif
