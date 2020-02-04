#include "header.h"

/* find the intersection of two lines
 * L1 and L2 are indices for A[] and B[]
 * the equations are y = A[] x + B[]
 * return the intersection as (x, y)
 * assume there are no parallel lines, i.e., A[] are distinct
 */
void lineIntersection(int L1, int L2, double *x, double *y) {

  *x = (B[L2] - B[L1]) / (A[L1] - A[L2]);
  *y = A[L1] * *x + B[L1];
}

/* lines j and k intersect line i on the right
 * the function returns the one that intersects i more to the left
 * that is, making segment i shorter
 */
int rightNeighbor(int i, int j, int k) {
  double jX, jY, kX, kY; //y coordinates are not needed

  if (j == RIGHTMOST) return k;
  if (k == RIGHTMOST) return j;
  lineIntersection(i, j, &jX, &jY);
  lineIntersection(i, k, &kX, &kY);
  return (jX < kX) ? j : k;
}

void dumpData(void) {
  int i;

  printf("----------\n");
  for (i = 0; i < numLine; i++)
    printf("%d %d %d %d\n", cut[i],
	   uhtR[ cut[i] ], lhtR[ cut[i] ], cutR[ cut[i] ]);
  printf("stack:");
  for (i = 0; i < numReady; i++)
    printf(" %d", stack[i]);
  printf("\n");
}
