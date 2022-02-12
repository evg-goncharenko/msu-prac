#ifndef _FDTD_2D_H
#define _FDTD_2D_H 
# if !defined(MINI_DATASET) && !defined(SMALL_DATASET) && !defined(MEDIUM_DATASET) && !defined(LARGE_DATASET) && !defined(EXTRALARGE_DATASET)
#define LARGE_DATASET
# endif
# if !defined(TMAX) && !defined(NX) && !defined(NY)
# ifdef MINI_DATASET
#define TMAX 20
#define NX 20
#define NY 30
# endif
# ifdef SMALL_DATASET
#define TMAX 40
#define NX 60
#define NY 80
# endif
# ifdef MEDIUM_DATASET
#define TMAX 100
#define NX 200
#define NY 240
# endif
# ifdef LARGE_DATASET
#define TMAX 500
#define NX 1000
#define NY 1200
# endif
# ifdef EXTRALARGE_DATASET
#define TMAX 1000
#define NX 2000
#define NY 2600
# endif
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#endif
