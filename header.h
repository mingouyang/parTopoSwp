#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <mathimf.h> //Intel math functions

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

#define RIGHTMOST (numLine)

/* main.c
 * From Edelsbrunner and Guibas, 1989
 *
 * E is A[] and B[] -- E for equation
 * M is cut[]
 * N is cutR[]
 * I is stack[]
 *
 * cut[i] contains the i-th segment of the cut
 *
 * cutR[ cut[i] ] is the right endpoint of cut[i]
 *
 * cut[i] and cutR[ cut[i] ] are indices to A[] and B[]
 *
 * cutR[], uhtR[], lhtR[] refer directly to the lines and their
 * ordering in A[] and B[] whereas cut[] uses the order
 * in the current cut
*/
extern uint64_t seed;
extern int verbose, numPt, numLine;
extern double *angle, *length, *X, *Y, *A, *B;
extern int *cut, *cutR;
extern int *uhtR, *lhtR;
extern int *stack, numReady;

//firstCut.c
void firstCut(void);

//genData.c
void genData(void);

//utils.c
void lineIntersection(int L1, int L2, double *x, double *y);
int rightNeighbor(int i, int j, int k);
void dumpData(void);
