#include "header.h"

//see Figure 3.2 of Edelsbrunner and Guibas 1989
static void initUHT(void) {
  double x, y, nextX, nextY;
  int i, j, memo;

  uhtR[numLine - 1] = RIGHTMOST; //the last line has the largest slope

  //looping in decreasing slopes -- traversing the bay of UHT
  for (i = numLine - 2; i >= 0; i--) {
    x = y = DBL_MAX;
    memo = RIGHTMOST;
    j = i + 1;
    while (j < numLine) { //uhtR[numine - 1] == RIGHTMOST == numLine
      lineIntersection(i, j, &nextX, &nextY);
      if (nextX < x)
	x = nextX, y = nextY, memo = j, j = uhtR[j];
      //j gets larger
      else
	break;
    }
    uhtR[i] = memo;
  }
}

static void initLHT(void) {
  double x, y, nextX, nextY;
  int i, j, memo;

  lhtR[0] = RIGHTMOST; //the 0-th line has the smallest slope

  //looping in increasing slopes -- traversing the bay of LHT
  for (i = 1; i < numLine; i++) {
    x = y = DBL_MAX;
    memo = RIGHTMOST;
    j = i - 1;
    while (j >= 0 && j < numLine) { //lhtR[0] == RIGHTMOST == numLine
      lineIntersection(i, j, &nextX, &nextY);
      if (nextX < x)
	x = nextX, y = nextY, memo = j, j = lhtR[j];
      //j gets smaller
      else
	break;
    }
    lhtR[i] = memo;
  }
}

static void initCut(void) {
  int i;

  for (i = 0; i < numLine; i++) {
    //first cut: cut[i] is line i by the ordering in A[] and B[]
    cut[i] = i;

    /* In the next two lines, we can replace cut[i] with just i
     * For consistency, stick to using the indirection of cut[i]
     *
     * In initUHT() and initLHT(), we should also use cut[i] instead of i
     * to access uhtR[], lhtR[]
     * But cut[] is not initialized until now
     */
    cutR[ cut[i] ] = rightNeighbor(cut[i], uhtR[ cut[i] ], lhtR[ cut[i] ]);
  }

}

static void initStk(void) {
  int i;

  /* If cut[i] and cut[i + 1] share a common right endpoint
   * they are ready for an elementary step, Figure 2.2
   *
   * Common right endpoint:
   * cutR[ cut[i] ] == cut[i + 1] && cutR[ cut[i + 1] ] == cut[i] 
   */
  numReady = 0;
  for (i = 0; i < numLine - 1; i++)
    if (cutR[ cut[i] ] == cut[i + 1] && cutR[ cut[i + 1] ] == cut[i])
      stack[ numReady++ ] = i;
}

void firstCut(void) {
  initUHT();
  initLHT();
  initCut();
  initStk();
}
