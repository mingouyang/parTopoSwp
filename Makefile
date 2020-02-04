CC=icc
CFLAGS=-Wall -O3 -xHost -ipo -qopenmp

ompTS: header.h ompElemStep.c firstCut.c genData.c ompMain.c utils.c
	$(CC) $(CFLAGS) ompElemStep.c firstCut.c genData.c ompMain.c utils.c -o ompTS

serialTS: header.h elemStep.c firstCut.c genData.c serialMain.c utils.c
	$(CC) $(CFLAGS) elemStep.c firstCut.c genData.c serialMain.c utils.c -o serialTS
