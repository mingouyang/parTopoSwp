#include "header.h"

#include <omp.h>
extern int numThread, *uhtMemo, *lhtMemo, *numNewReady, *prefixSum;

//see Figure 3.3 of Edelsbrunner and Guibas 1989

void ompElemStep(void) {
  double x, y, nextX, nextY, vX, vY;
  int r, a, i, j, k, memo;

#pragma omp parallel for private(a,i,j,k,memo,x,y,nextX,nextY,vX,vY)
  for (r = 0; r < numReady; r++) {
    a = stack[r];
    i = cut[a];
    j = cut[a + 1];
    lineIntersection(i, j, &vX, &vY);
    //v is the intersection of cut[a] and cut[a + 1]

    memo = RIGHTMOST;
    if (a + 2 < numLine) {
      x = y = DBL_MAX;
      k = cut[a + 2];
      while (k < numLine) { //RIGHTMOST == numLine
	lineIntersection(i, k, &nextX, &nextY);
	if (nextX < x) {
	  if (nextX > vX)
	    x = nextX, y = nextY, memo = k;
	  k = uhtR[k]; //k gets larger
	}
	else
	  break;
      }
    }
    uhtMemo[a] = memo;

    memo = RIGHTMOST;
    if (a - 1 >= 0) {
      x = y = DBL_MAX;
      k = cut[a - 1];
      while (k >= 0 && k < numLine) { //RIGHTMOST == numLine
	lineIntersection(j, k, &nextX, &nextY);
	if (nextX < x) {
	  if (nextX > vX)
	    x = nextX, y = nextY, memo = k;
	  k = lhtR[k]; //k gets smaller
	}
	else
	  break;
      }
    }
    lhtMemo[a] = memo;
  }

#pragma omp parallel for private(a,i,j)
  for (r = 0; r < numReady; r++) {
    a = stack[r];
    i = cut[a];
    j = cut[a + 1];

    uhtR[i] = uhtMemo[a];
    //uhtR[j] stays the same
    lhtR[j] = lhtMemo[a];
    //lhtR[i] stays the same

    cut[a]     = j;
    cut[a + 1] = i;

    cutR[i] = rightNeighbor(i, uhtR[i], lhtR[i]);
    cutR[j] = rightNeighbor(j, uhtR[j], lhtR[j]);
  }

  /*
   * When two ready pairs are adjacent, i.e. a and a + 2 are adjacent
   * in the stack, we should check exactly once whether (a + 1, a + 2)
   * will become a new ready pair.  Otherwise, there will be
   * duplicates in the stack.
   *
   * This means we check (a - 1, a) as long as a - 1 >= 0.  But before
   * we check (a + 1, a + 2), we need to make sure a + 2 is not in
   * the stack
   *
   * To make this work, the numbers in the stack must be kept strictly
   * increasing
   */

#pragma omp parallel for private(a)
  for (r = 0; r < numReady; r++) {
    a = stack[r];

    numNewReady[r] = 0;
    if (a > 0 && cutR[ cut[a] ] == cut[a - 1] &&
	cutR[ cut[a - 1] ] == cut[a]) {
      uhtMemo[r] = a - 1;
      numNewReady[r] = 1;
    }
    /*
     * Read (r == numReady - 1) || (stack[r + 1] != a + 2)
     * like (r < numReady - 1) --> (stack[r + 1] != a + 2)
     */
    if (a + 2 < numLine && (r == numReady - 1 || stack[r + 1] != a + 2) &&
	cutR[ cut[a + 1] ] == cut[a + 2] && cutR[ cut[a + 2] ] == cut[a + 1]) {
      if (numNewReady[r] == 0)
	uhtMemo[r] = a + 1;
      else
	lhtMemo[r] = a + 1;
      numNewReady[r]++;
    }
  }

  prefixSum[0] = 0;
  for (r = 0; r < numReady; r++)
    prefixSum[r + 1] = prefixSum[r] + numNewReady[r];

#pragma omp parallel for
  for (r = 0; r < numReady; r++) {
    if (numNewReady[r])
      stack[ prefixSum[r] ] = uhtMemo[r];
    if (numNewReady[r] == 2)
      stack[ prefixSum[r] + 1 ] = lhtMemo[r];
  }

  numReady = prefixSum[numReady];
}
