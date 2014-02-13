/****************************************************************
 *	program: cMatMult.c
 *  EE292M, Stanford, Winter 2013
 *	Andrea Di Blas
 *
 *  Matrix multiplication with same input and output
 *  file format as kMatMult.kasm
 *
 *	the -p (print) option prints an output file
 *	the -c (compare) option prints the difference between
 *		two output files
 *
 *	Revised, January 2014, EE392Q
 *
 *	(but this code is still TERRIBLE!!! do not imitate!!!!)
 *
 ****************************************************************/

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

int
main(int argc, char *argv[]);
/******************************************************************/
int
main(int argc, char *argv[])
{
  FILE    *fpi, *fpo;
  int      i, j, k;
  int      x, y; /* temp */
  int      N, H, M;
  int      N2, M2; /* used for the compare option */
  int     *A, *B, *C;
  int      minA, maxA;  /* min and max value for randomized A */
  int      minB, maxB;  /* min and max value for randomized B */

  if (argc < 3)
  {
    fprintf(stderr, "\nUsage: cMatMult <inputfile> <outputfile> to perform a multiplication\n");
    fprintf(stderr, "   or: cMatMult -g  <inputfile>            to generate an input file\n");
    fprintf(stderr, "   or: cMatMult -p  <outputfile>           to print out an output file\n");
    fprintf(stderr, "   or: cMatMult -c  <outfile1> <outfile2>  to compare two output files\n\n");
    return (1);
  }

  /****************************************************************
   *  option to just print out a formatted output file
   ****************************************************************/
  if (!strcmp(argv[1], "-g"))
  {
    /*-----------------------------------------------------------
     * input operand matrix sizes
     */
    printf("Input N, number of rows of matrix A (0 < N <= 65535):         ");
    scanf("%d", &N);
    while (N <= 0 || N > 65535)
    {
      printf("*** Bad value: N can't be %d! Try again: N =                ", N);
      scanf("%d", &N);
    }
    printf("Input H, number of columns of A and rows of B (0 < H <= 256): ");
    scanf("%d", &H);
    while (H <= 0 || H > 256)
    {
      printf("*** Bad value: N can't be %d! Try again: H =                ", H);
      scanf("%d", &H);
    }
    printf("Input M, number of rows of matrix B (0 < M <= 512):           ");
    scanf("%d", &M);
    while (M <= 0 || M > 512)
    {
      printf("*** Bad value: M can't be %d! Try again: M =                ", M);
      scanf("%d", &M);
    }

    printf("\nN = %d, H = %d, M = %d\n\n", N, H, M);

    /*-----------------------------------------------------------
     * write sizes to file
     */
    if ((fpi = fopen(argv[2], "w")) == NULL)
    {
      fprintf(stderr, "\n*** can't write file %s ***\n\n", argv[2]);
      return (1);
    }
    fprintf(fpi, " %d\n",  N & (int)0xFF);
    fprintf(fpi, " %d\n", (N & (int)0xFF00) >> 8);
    fprintf(fpi, " %d\n",  H & (int)0xFF);
    fprintf(fpi, " %d\n", (H & (int)0xFF00) >> 8);
    fprintf(fpi, " %d\n",  M & (int)0xFF);
    fprintf(fpi, " %d\n", (M & (int)0xFF00) >> 8);

    /*------------------------------------------------
     * input limits for randomized values
     */
    printf("Input minA, minimum value of elements in A (-128 <= minA <= 127): ");
    scanf("%d", &minA);
    while (minA < -128 || minA > 127)
    {
      printf("*** Bad value: minA can't be %d! Try again: minA = ", minA);
      scanf("%d", &minA);
    }
    printf("Input maxA, maximum value of elements in A (%d <= maxA <= 127):   ", minA);
    scanf("%d", &maxA);
    while (maxA < minA || maxA > 127)
    {
      printf("*** Bad value: maxA can't be %d! Try again: maxA", maxA);
      scanf("%d", &maxA);
    }
    printf("Input minB, minimum value of elements in B (-128 <= minB <= 127): ");
    scanf("%d", &minB);
    while (minB < -128 || minB > 127)
    {
      printf("*** Bad value: minB can't be %d! Try again: minb = !", minB);
      scanf("%d", &minB);
    }
    printf("Input maxB, maximum value of elements in B (%d <= maxB <= 127):   ", minB);
    scanf("%d", &maxB);
    while (maxB < minB || maxB > 127)
    {
      printf("*** Bad value: maxB can't be %d! Try again: maxB = ", maxB);
      scanf("%d", &maxB);
    }

    printf("\nminA = %d, maxA = %d, minB = %d, maxB = %d\n\n", minA, maxA, minB, maxB);

    /* generate and write B */
    printf("\n--------------------------------------");
    printf("\n B in reverse column order");
    printf("\n--------------------------------------");
    printf("\n      ");
    for(j = M - 1; j >= 0; --j)
      printf("%5d", j);
    printf("\n");
    for(i = 0; i < H; ++i)
    {
      printf("\n%4d  ", i);
      for(j = M - 1; j >= 0; --j)
      {
        x = minB + (int) (((double)rand()/(double)RAND_MAX) * (maxB - minB));
        fprintf(fpi, "%u\n", (unsigned)(x & 0xFF)); /* kestrel input: 2's complement */
        printf("%5d", x);
      }
    }

    printf("\n");
    printf("\n--------------------------------------");
    printf("\n A in direct column order");
    printf("\n--------------------------------------");
    printf("\n      ");
    for(j = 0; j < H; ++j)
      printf("%5d", j);
    printf("\n");
    for(i = 0; i < N; ++i)
    {
      printf("\n%4d  ", i);
      for(j = 0; j < H; ++j)
      {
        x = minA + (int) (((double)rand()/(double)RAND_MAX) * (maxA - minA));
        fprintf(fpi, "%u\n", (unsigned)(x & 0xFF));  /* kestrel input: 2's complement */
        printf("%5d", x);
      }
    }

    fclose(fpi);
    printf("\n\n");
    return (0);
  }

  /****************************************************************
   *	option to just print out a formatted output file
   ****************************************************************/
  if (argv[1][0] == '-' && argv[1][1] == 'p')
  {
    if ((fpi = fopen(argv[2], "r")) == NULL)
    {
      fprintf(stderr, "\n*** file %s not found ***\n\n", argv[1]);
      return (1);
    }

    fscanf(fpi, " %d", &N);
    fscanf(fpi, " %d", &x);
    N = N | (x << 8);
    fscanf(fpi, " %d", &M);
    fscanf(fpi, " %d", &x);
    M = M | (x << 8);

    if ((C = (int *) malloc(N * M * sizeof(int))) == NULL)
    {
      fprintf(stderr, "\n*** out of memory (array C) ***\n\n");
      return (2);
    }

    /* read C */
    for(i = 0; i < N; ++i)
      for(j = 0; j < M; ++j)
      {
        fscanf(fpi, " %d", &C[i * M + j]);
        fscanf(fpi, " %d", &x);
        C[i * M + j] |= (x << 8);
        fscanf(fpi, " %d", &x);
        C[i * M + j] |= (x << 16);
        fscanf(fpi, " %d", &x);
        C[i * M + j] |= (x << 24);
        if (C[i * M + j] & 0x80000000)
          C[i * M + j] = -C[i * M + j];
      }

    /* print C */
    printf("\n-------------------------\n"
        "C: from file %s, N = %d, M = %d "
        "\n-------------------------", argv[2], N, M);
    for(i = 0; i < N; ++i)
    {
      printf("\n%3d:  ", i);
      for(j = 0; j < M; ++j)
        printf(" %5d", C[i * M + j]);
    }
    printf("\n-------------------------\n");
    return (0);
  }

  /****************************************************************
   *	option to print out the difference of two matrices ("compare")
   ****************************************************************/
  if (argv[1][0] == '-' && argv[1][1] == 'c')
  {
    /*
     *	read first file into C[][]
     */
    if ((fpi = fopen(argv[2], "r")) == NULL)
    {
      fprintf(stderr, "\n*** file %s not found ***\n\n", argv[2]);
      return (1);
    }

    fscanf(fpi, " %d", &N);
    fscanf(fpi, " %d", &x);
    N = N | (x << 8);
    fscanf(fpi, " %d", &M);
    fscanf(fpi, " %d", &x);
    M = M | (x << 8);

    if ((C = (int *) malloc(N * M * sizeof(int))) == NULL)
    {
      fprintf(stderr, "\n*** out of memory (array C) ***\n\n");
      return (2);
    }

    /* read C */
    for(i = 0; i < N; ++i)
      for(j = 0; j < M; ++j)
      {
        fscanf(fpi, " %d", &C[i * M + j]);
        fscanf(fpi, " %d", &x);
        C[i * M + j] |= (x << 8);
        fscanf(fpi, " %d", &x);
        C[i * M + j] |= (x << 16);
        fscanf(fpi, " %d", &x);
        C[i * M + j] |= (x << 24);
        if (C[i * M + j] & 0x80000000)
          C[i * M + j] = -C[i * M + j];
      }
    fclose(fpi);

    /*
     *	read second file and store the diffference
     */
    if ((fpi = fopen(argv[3], "r")) == NULL)
    {
      fprintf(stderr, "\n*** file %s not found ***\n\n", argv[3]);
      return (1);
    }

    fscanf(fpi, " %d", &N2);
    fscanf(fpi, " %d", &x);
    N2 = N2 | (x << 8);
    fscanf(fpi, " %d", &M2);
    fscanf(fpi, " %d", &x);
    M2 = M2 | (x << 8);

    if (N != N2 || M != M2)
    {
      fprintf(stderr, "\n*** sizes don't match! "
          "%s is %dx%d and %s is %dx%d ***\n\n", argv[2], N, M, argv[3], N2,
          M2);
      return (1);
    }

    for(i = 0; i < N; ++i)
      for(j = 0; j < M; ++j)
      {
        fscanf(fpi, " %d", &y);
        fscanf(fpi, " %d", &x);
        y |= (x << 8);
        fscanf(fpi, " %d", &x);
        y |= (x << 16);
        fscanf(fpi, " %d", &x);
        y |= (x << 24);
        if (y & 0x80000000)
          y = -y;
        C[i * M + j] -= y;
      }
    fclose(fpi);

    /* print C */
    printf("\n-------------------------\n"
        "Difference between %s and %s (N = %d, M = %d) "
        "\n-------------------------", argv[2], argv[3], N, M);
    for(i = 0; i < N; ++i)
    {
      printf("\n%3d:  ", i);
      for(j = 0; j < M; ++j)
        printf(" %5d", C[i * M + j]);
    }
    printf("\n-------------------------\n");
    return (0);
  }

  /****************************************************************
   *	regular multiplication
   ****************************************************************/

  if ((fpi = fopen(argv[1], "r")) == NULL)
  {
    fprintf(stderr, "\n*** file %s not found ***\n\n", argv[1]);
    return (1);
  }

  if ((fpo = fopen(argv[2], "w")) == NULL)
  {
    fprintf(stderr, "\n*** can't write file %s ***\n\n", argv[2]);
    return (1);
  }

  fscanf(fpi, " %d", &N);
  fscanf(fpi, " %d", &x);
  N = N | (x << 8);
  fscanf(fpi, " %d", &H);
  fscanf(fpi, " %d", &x);
  H = H | (x << 8);
  fscanf(fpi, " %d", &M);
  fscanf(fpi, " %d", &x);
  M = M | (x << 8);

  printf("\nN = %d, H = %d, M = %d", N, H, M);

  if ((A = (int *) malloc(N * H * sizeof(int))) == NULL)
  {
    fprintf(stderr, "\n*** out of memory (array A) ***\n\n");
    return (2);
  }

  if ((B = (int *) malloc(H * M * sizeof(int))) == NULL)
  {
    fprintf(stderr, "\n*** out of memory (array B) ***\n\n");
    return (2);
  }

  if ((C = (int *) malloc(N * M * sizeof(int))) == NULL)
  {
    fprintf(stderr, "\n*** out of memory (array C) ***\n\n");
    return (2);
  }

  /* read B by rows, reverse col order */
  for(i = 0; i < H; ++i)
    for(j = M - 1; j >= 0; --j)
    {
      fscanf(fpi, " %d", &B[i * M + j]);
      if (B[i * M + j] > 127)
        B[i * M + j] = -(256 - B[i * M + j]);
    }

  /* debug: print B */
  printf("\n-------------------------\n"
      "B:\n-------------------------");
  for(i = 0; i < H; ++i)
  {
    printf("\n%3d:  ", i);
    for(j = 0; j < M; ++j)
      printf(" %3d", B[i * M + j]);
  }

  /* read A by rows, direct col order */
  for(i = 0; i < N; ++i)
    for(j = 0; j < H; ++j)
    {
      fscanf(fpi, " %d", &A[i * H + j]);
      if (A[i * H + j] > 127)
        A[i * H + j] = -(256 - A[i * H + j]);
    }

  /* debug: print A */
  printf("\n-------------------------\n"
      "A:\n-------------------------");
  for(i = 0; i < N; ++i)
  {
    printf("\n%3d:  ", i);
    for(j = 0; j < H; ++j)
      printf(" %3d", A[i * H + j]);
  }

  /* print out N on two bytes, LSB first */
  fprintf(fpo, "%d\n", N & 0xFF);
  fprintf(fpo, "%d\n", (N >> 8) & 0xFF);

  /* print out M on two bytes, LSB first */
  fprintf(fpo, "%d\n", M & 0xFF);
  fprintf(fpo, "%d\n", (M >> 8) & 0xFF);

  /* do the multiplication: */
  for(i = 0; i < N; ++i)
    for(j = 0; j < M; ++j)
    {
      C[i * M + j] = 0;
      for(k = 0; k < H; ++k)
      {
        C[i * M + j] += A[i * H + k] * B[k * M + j];
      }
      fprintf(fpo, "%d\n", C[i * M + j] & 0xFF);
      fprintf(fpo, "%d\n", (C[i * M + j] >> 8) & 0xFF);
      fprintf(fpo, "%d\n", (C[i * M + j] >> 16) & 0xFF);
      fprintf(fpo, "%d\n", (C[i * M + j] >> 24) & 0xFF);
    }

  /* debug: print C */
  printf("\n-------------------------\n"
      "C:\n-------------------------");
  for(i = 0; i < N; ++i)
  {
    printf("\n%3d:  ", i);
    for(j = 0; j < M; ++j)
      printf(" %5d", C[i * M + j]);
  }

  printf("\n-------------------------\n");
  printf("\n\n");
  return (0);
}
/***********************************************************/
/***********************************************************/
/***********************************************************/
