#include "header.h"

//see Figure 3.3 of Edelsbrunner and Guibas 1989
static void updateUHT(int a) {
  double x, y, nextX, nextY, vX, vY;
  int i, j, k, memo;

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
  uhtR[i] = memo;

  //uhtR[j] stays the same
}

static void updateLHT(int a) {
  double x, y, nextX, nextY, vX, vY;
  int i, j, k, memo;

  i = cut[a];
  j = cut[a + 1];
  lineIntersection(i, j, &vX, &vY);
  //v is the intersection of cut[a] and cut[a + 1]

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
  lhtR[j] = memo;

  //lhtR[i] stays the same
}

static void updateCut(int a) {
  int i, j;

  i = cut[a];
  j = cut[a + 1];

  cutR[i] = rightNeighbor(i, uhtR[i], lhtR[i]);
  cutR[j] = rightNeighbor(j, uhtR[j], lhtR[j]);

  cut[a] = j;
  cut[a + 1] = i;
}

static void updateStk(int a) {
  if (a > 0)
    if (cutR[ cut[a] ] == cut[a - 1] && cutR[ cut[a - 1] ] == cut[a])
      stack[ numReady++ ] = a - 1;

  if (a + 2 < numLine)
    if (cutR[ cut[a + 1] ] == cut[a + 2] && cutR[ cut[a + 2] ] == cut[a + 1])
      stack[ numReady++ ] = a + 1;
}

//swap cut[a] and cut[a + 1]
void elemStep(int a) {
  updateUHT(a);
  updateLHT(a);
  updateCut(a);
  updateStk(a);
}
