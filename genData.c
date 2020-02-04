#include "header.h"

//ascending X, descending Y
static int cmpPt(const void *a, const void *b) {
  int c, d;

  c = *(int *) a;
  d = *(int *) b;
  if (X[c] < X[d])
    return -1;
  if (X[c] > X[d])
    return 1;
  if (Y[c] > Y[d])
    return -1;
  if (Y[c] < Y[d])
    return 1;
  return 0;
}

void genData(void) {
  int i, j, *idx;

  idx = (int *) malloc(numPt * sizeof(int));

  //generate points inside the unit circle uniformly
  for (i = 0; i < numPt; i++) {
    angle[i] = drand48() * 2 * M_PI;
    length[i] = sqrt(drand48());

    //from polar coordinates to Cartesian coordinates
    X[i] = length[i] * cos(angle[i]);
    Y[i] = length[i] * sin(angle[i]);

    //for sorting
    idx[i] = i;
  }

  //sort by ascending X, descending Y
  qsort((void *)idx, numPt, sizeof(int), cmpPt);

  //the dual line of point (a, b) is the line y = ax + b
  //primal point (X[i], Y[i]), dual line y = X[i] x + Y[i]
  A[0] = X[ idx[0] ], B[0] = Y[ idx[0] ];
  //ascending X[i], increasing slope
  for (i = j = 1; i < numPt; i++) {
    //remove parallel lines
    if (X[ idx[i] ] == A[j - 1])
      continue;

    //remove duplicates
    //when parallel lines are removed -- the if above -- this is redundant
    if (X[ idx[i] ] == A[j - 1] && Y[ idx[i] ] == B[j - 1])
      continue;

    A[j] = X[ idx[i] ], B[j] = Y[ idx[i] ], j++;
  }

  numLine = j;

  free(idx);
}
