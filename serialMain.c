#include "header.h"
#include <getopt.h>
#include <sys/time.h>

int verbose, numPt, numLine;

/* polar coordinates: (length[i], angle[i])
 * Cartesian coordinates: (X[i], Y[i])
 * dual line i: y = X[i] x + Y[i]
 */
double *angle, *length, *X, *Y, *A, *B;

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
 * cutR[ cut[i] ] is the line that delimit the right ends of cut[i]
 *
 * cut[i] and cutR[ cut[i] ] are indices to A[] and B[]
 *
 * cutR[], uhtR[], lhtR[] refer directly to the lines and their
 * ordering in A[] and B[] whereas cut[] uses the ordering of the
 * lines in the current cut
*/
int *cut, *cutR;

//upper and lower horizon trees: right (rho)
int *uhtR, *lhtR;

//the ready stack
int *stack, numReady;

extern void elemStep(int a);

static void init(void) {
  angle  = (double *) malloc(numPt * sizeof(double));
  length = (double *) malloc(numPt * sizeof(double));
  X      = (double *) malloc(numPt * sizeof(double));
  Y      = (double *) malloc(numPt * sizeof(double));
  A      = (double *) malloc(numPt * sizeof(double));
  B      = (double *) malloc(numPt * sizeof(double));
  cut    = (int *) malloc(numPt * sizeof(int));
  cutR   = (int *) malloc(numPt * sizeof(int));
  uhtR   = (int *) malloc(numPt * sizeof(int));
  lhtR   = (int *) malloc(numPt * sizeof(int));
  stack  = (int *) malloc(numPt * sizeof(int));
}

static void cleanup(void) {
  free(angle);
  free(length);
  free(X);
  free(Y);
  free(A);
  free(B);
  free(cut);
  free(cutR);
  free(uhtR);
  free(lhtR);
  free(stack);
}

int main(int argc, char *argv[]) {
  uint64_t seed, numIntersection = 0;
  struct timeval t0, t1;
  int c, i, readFile;
  char *filename;
  float elapsed;
  FILE *fp;

  verbose = 0;
  readFile = 0;
  numPt = 1000;
  gettimeofday(&t0, NULL);
  seed = t0.tv_usec;
  while ((c = getopt(argc, argv, "f:n:s:v")) != -1) {
    switch (c) {
    case 'f':
      fp = fopen(optarg, "r");
      if (fp == NULL) {
	printf("can't open %s\n", optarg);
	return 0;
      }
      fscanf(fp, "%d", &numPt);
      numLine = numPt;
      readFile = 1;
      break;
    case 'n': sscanf(optarg, "%d", &numPt); break; //number of data points
    case 's': sscanf(optarg, "%lu", &seed); break; //seed srand48_r
    case 'v': verbose = 1; break;
    default: break;
    }
  }
  srand48(seed);

  init();

  if (readFile) {
    //assume the points are already sorted by their x-coordinates
    for (i = 0; i < numLine; i++)
      fscanf(fp, "%lf %lf", &A[i], &B[i]);
    fclose(fp);
  }
  else
    genData();

  gettimeofday(&t0, NULL);
  firstCut();

  while (numReady) {
    if (verbose) {
      dumpData();
      printf("swap lines %d %d\n",
	     cut[ stack[numReady - 1] ],  cut[ stack[numReady - 1] + 1 ]);
    }
    elemStep(stack[ --numReady ]);
    numIntersection++;
  }

  gettimeofday(&t1, NULL);
  elapsed = t1.tv_sec - t0.tv_sec + (t1.tv_usec - t0.tv_usec) / 1000000.0;
  printf("serialTS %d lines %.1f sec %lu Xs\n", numLine, elapsed,
	 numIntersection);

  if (verbose) dumpData();

  cleanup();

  return 0;
}
