/*******************************************************************
 * 
 * driver.c - Driver program for CS:APP Performance Lab
 * 
 * In kernels.c, students generate an arbitrary number of complex and
 * blur test functions, which they then register with the driver
 * program using the add_complex_function() and add_blur_function()
 * functions.
 * 
 * The driver program runs and measures the registered test functions
 * and reports their performance.
 * 
 * Copyright (c) 2002, R. Bryant and D. O'Hallaron, All rights
 * reserved.  May not be used, modified, or copied without permission.
 *
 ********************************************************************/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "fcyc.h"
#include "defs.h"
#include "config.h"

/* Student structure that identifies the students */
extern student_t student; 

/* Keep track of a number of different test functions */
#define MAX_BENCHMARKS 100
#define DIM_CNT 5

/* Misc constants */
#define BSIZE 64     /* cache block size in bytes */     
#define MAX_DIM 1280 /* 1024 + 256 */
#define ODD_DIM 96   /* not a power of 2 */

/* fast versions of min and max */
#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b)

/* This struct characterizes the results for one benchmark test */
typedef struct {
  union {
    complex_test_func complex_funct; /* The test function */
    blur_test_func blur_funct; /* The test function */
  };
    double cpes[DIM_CNT]; /* One CPE result for each dimension */
    char *description;    /* ASCII description of the test function */
    unsigned short valid; /* The function is tested if this is non zero */
} bench_t;

/* The range of image dimensions that we will be testing */
static int test_dim_complex[] = {64, 128, 256, 512, 1024};
static int test_dim_blur[] = {32, 64, 128, 256, 512};


/* Baseline CPEs (see config.h) */
static double complex_baseline_cpes[] = {R64, R128, R256, R512, R1024};
static double blur_baseline_cpes[] = {S32, S64, S128, S256, S512};

/* These hold the results for all benchmarks */
static bench_t benchmarks_complex[MAX_BENCHMARKS];
static bench_t benchmarks_blur[MAX_BENCHMARKS];

/* These give the sizes of the above lists */
static int complex_benchmark_count = 0;
static int blur_benchmark_count = 0;

/* 
 * An image is a dimxdim matrix of pixels stored in a 1D array.  The
 * data array holds five images (the input original, complex destination, 
 * complex temporary space, a copy of the original, 
 * and the output result array). 
 * There is also an additional BSIZE bytes
 * of padding for alignment to cache block boundaries.
 */
static pixel data[(5*MAX_DIM*MAX_DIM) + (BSIZE/sizeof(pixel))];

/* Various image pointers */
static pixel *orig = NULL;         /* original image */
static pixel *tmp = NULL;          /* temporary area for checking complex */
static pixel *copy_of_orig = NULL; /* copy of original for checking result */
static pixel *result = NULL;       /* result image */

/* Keep track of the best complex and blur score for grading */
double complex_maxmean = 0.0;
char *complex_maxmean_desc = NULL;

double blur_maxmean = 0.0;
char *blur_maxmean_desc = NULL;

int save_test_image_files;
int save_all_image_files;


/******************** Functions begin *************************/

void add_blur_function(blur_test_func f, char *description) 
{
    benchmarks_blur[blur_benchmark_count].blur_funct = f;
    benchmarks_blur[blur_benchmark_count].description = description;
    benchmarks_blur[blur_benchmark_count].valid = 0;  
    blur_benchmark_count++;
}


void add_complex_function(complex_test_func f, char *description) 
{
    benchmarks_complex[complex_benchmark_count].complex_funct = f;
    benchmarks_complex[complex_benchmark_count].description = description;
    benchmarks_complex[complex_benchmark_count].valid = 0;
    complex_benchmark_count++;
}

/* 
 * random_in_interval - Returns random integer in interval [low, high) 
 */
static int random_in_interval(int low, int high) 
{
    int size = high - low;
    return (rand()% size) + low;
}

static int scale(int i, int from, int to) {
  return (int)(((double)i / from) * to);
}

static void write_image(int dim, char *variant, char *mode, pixel *img)
{
  char buf[64];
  int i, j;
  FILE *f;

  sprintf(buf, "%s_%s_%d.image", variant, mode, dim);
  f = fopen(buf, "w");

  fprintf(f, "%d %d\n", dim, dim);

  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      if (j > 0)
        fprintf(f, " ");
      fprintf(f, "%d %d %d",
              img[RIDX(i,j,dim)].red,
              img[RIDX(i,j,dim)].green,
              img[RIDX(i,j,dim)].blue);
    }
    fprintf(f, "\n");
  }

  fclose(f);
}

#define RANDOM   0
#define GRADIENT 1
#define SQUARES  2
#define LINES    3
static int image_mode = RANDOM;


/*
  Helper functions to set a given pixel using certain image modes
*/
static void set_gradient(pixel* img, int i, int j, int dim)
{
  img[RIDX(i,j,dim)].red = scale(i, dim, 65536);
  img[RIDX(i,j,dim)].green = scale(abs(j-i), dim, 65536);
  img[RIDX(i,j,dim)].blue = scale(i+j, 2*dim, 65536);
}

static void set_squares(pixel* img, int i, int j, int dim)
{
  if (((i >> 4) & 1) && ((j >> 4) & 1)) {
    img[RIDX(i,j,dim)].red = scale((i >> 4), dim >> 4, 65536);
    img[RIDX(i,j,dim)].green = scale((j >> 4), dim >> 4, 65536);
    img[RIDX(i,j,dim)].blue = scale((i + j) >> 4, dim >> 4, 65536);
  } else {
    img[RIDX(i,j,dim)].red = 0;
    img[RIDX(i,j,dim)].green = 0;
    img[RIDX(i,j,dim)].blue = 0;
  }
}

static void set_lines(pixel* img, int i, int j, int dim)
{
  if((j % 32 == (i % 32))) // diagonal line every 32 pixels   
  {
    img[RIDX(i,j,dim)].red = 0xffff;
    img[RIDX(i,j,dim)].green = 0;
    img[RIDX(i,j,dim)].blue = 0;
  }
  else
  {
    img[RIDX(i,j,dim)].red = 0;
    img[RIDX(i,j,dim)].green = 0;
    img[RIDX(i,j,dim)].blue = 0;
  }
}

static void set_random(pixel* img, int i, int j, int dim)
{
  img[RIDX(i,j,dim)].red = random_in_interval(0, 65536);
  img[RIDX(i,j,dim)].green = random_in_interval(0, 65536);
  img[RIDX(i,j,dim)].blue = random_in_interval(0, 65536);
}


/*
 * create - creates a dimxdim image aligned to a BSIZE byte boundary
 */
static void create(int dim)
{
  int i, j;
  
  /* Align the images to BSIZE byte boundaries */
  orig = data;
  while ((long)orig % BSIZE)
    orig = (pixel *)(((char *)orig) + 1);
  tmp = orig + dim*dim;
  result = tmp + dim*dim;
  copy_of_orig = result + dim*dim;
  
  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      
      
      /* Initialize original image */
      switch (image_mode) {
      case GRADIENT:
	set_gradient(orig, i, j, dim);
	break;
      case SQUARES:
	set_squares(orig, i, j, dim);
	break;
      case LINES:
	set_lines(orig, i, j, dim);
	break;
      default:
      case RANDOM:
	set_random(orig, i, j, dim);
	break;
      }
      
      
      /* Copy of original image for checking result */
      copy_of_orig[RIDX(i,j,dim)].red = orig[RIDX(i,j,dim)].red;
      copy_of_orig[RIDX(i,j,dim)].green = orig[RIDX(i,j,dim)].green;
      copy_of_orig[RIDX(i,j,dim)].blue = orig[RIDX(i,j,dim)].blue;
      
      /* Result image initialized to all black */
      result[RIDX(i,j,dim)].red = 0;
      result[RIDX(i,j,dim)].green = 0;
      result[RIDX(i,j,dim)].blue = 0;
    }
  }
  
  return;
}

/* 
 * compare_pixels - Returns 1 if the two arguments don't have same RGB
 *    values, 0 o.w.  
 */
static int compare_pixels(pixel p1, pixel p2) 
{
    return 
	(p1.red != p2.red) || 
	(p1.green != p2.green) || 
	(p1.blue != p2.blue);
}


/* Make sure the orig array is unchanged */
static int check_orig(int dim) 
{
    int i, j;

    for (i = 0; i < dim; i++) 
	for (j = 0; j < dim; j++) 
	    if (compare_pixels(orig[RIDX(i,j,dim)], copy_of_orig[RIDX(i,j,dim)])) {
		printf("\n");
		printf("Error: Original image has been changed!\n");
		return 1;
	    }

    return 0;
}

/* 
 * check_complex - Make sure the complex actually works. 
 */
static int check_complex(int dim, int save_images)
{
    int err = 0;
    int i, j;
    int badi = 0;
    int badj = 0;
    pixel res_bad = { 0, 0, 0}, res_should_be = {0, 0, 0};

    /* return 1 if the original image has been changed */
    if (check_orig(dim)) 
	return 1;

    // Rotate, flip, then grayscale

    for(i = 0; i < dim; i++)
      for(j = 0; j < dim; j++)
      {

	tmp[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)orig[RIDX(i, j, dim)].red +
							(int)orig[RIDX(i, j, dim)].green +
							(int)orig[RIDX(i, j, dim)].blue) / 3;
	
	tmp[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)orig[RIDX(i, j, dim)].red +
							  (int)orig[RIDX(i, j, dim)].green +
							  (int)orig[RIDX(i, j, dim)].blue) / 3;
	
	tmp[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)orig[RIDX(i, j, dim)].red +
							 (int)orig[RIDX(i, j, dim)].green +
							 (int)orig[RIDX(i, j, dim)].blue) / 3;
	
      }


    if (save_images) {
      write_image(dim, "complex", "orig", orig);
      write_image(dim, "complex", "result", result);
      write_image(dim, "complex", "expected", tmp);
    }

    for (j = 0; j < dim; j++)
      for (i = 0; i < dim; i++) {
        if (compare_pixels(tmp[RIDX(i,j,dim)],
                           result[RIDX(i,j,dim)])) {
          err++;
          badi = i;
          badj = j;
          res_bad = result[RIDX(i,j,dim)];
          res_should_be = tmp[RIDX(i,j,dim)];
        }
      }

    if (err) {
	printf("\n");
	printf("ERROR: Dimension=%d, %d errors\n", dim, err);    
	printf("E.g., The following pixel has the wrong value:\n");
	printf("result[%d][%d].{red,green,blue} = {%d,%d,%d}\n",
	       badi, badj, res_bad.red, res_bad.green, res_bad.blue);
	printf("It should be:\n");
	printf("img[%d][%d].{red,green,blue} = {%d,%d,%d}\n",
	       badi, badj, res_should_be.red, res_should_be.green, res_should_be.blue);
    }
    
    return err;
}


static pixel check_weighted_sum(int dim, int i, int j, pixel *src) {
  pixel result;
  int ii, jj;
  int sum0, sum1, sum2;
  
  sum0 = sum1 = sum2 = 0;
  int num_neighbors = 0;
  for(ii=-1; ii < 2; ii++)
    for(jj=-1; jj < 2; jj++) 
      if ((i + 2*ii < dim) && (i + 2*ii >= 0) && (j + 2*jj < dim) && (j + 2*jj >= 0)) 
      {
	num_neighbors++;
	sum0 += (int) src[RIDX(i+2*ii,j+2*jj,dim)].red;
	sum1 += (int) src[RIDX(i+2*ii,j+2*jj,dim)].green;
	sum2 += (int) src[RIDX(i+2*ii,j+2*jj,dim)].blue;
      }
  
  result.red = (unsigned short) (sum0 / num_neighbors);
  result.green = (unsigned short) (sum1 / num_neighbors);
  result.blue = (unsigned short) (sum2 / num_neighbors);
  
  return result;
}


/* 
 * check_blur - Make sure the blur function actually works.  The
 * orig array should not have been tampered with!  
 */
static int check_blur(int dim, int save_images) {
    int err = 0;
    int i, j;
    int badi = 0;
    int badj = 0;
    pixel right = {0, 0, 0}, wrong = {0,0,0};

    /* return 1 if original image has been changed */
    if (check_orig(dim)) 
	return 1;

    for (i = 0; i < dim; i++)
      for (j = 0; j < dim; j++)
        tmp[RIDX(i,j,dim)] = check_weighted_sum(dim, i, j, orig);

    if (save_images) {
      write_image(dim, "blur", "orig", orig);
      write_image(dim, "blur", "result", result);
      write_image(dim, "blur", "expected", tmp);
    }

    for (i = 0; i < dim; i++) {
	for (j = 0; j < dim; j++) {
	    if (compare_pixels(result[RIDX(i,j,dim)],
                               tmp[RIDX(i,j,dim)])) {
              err++;
              badi = i;
              badj = j;
              wrong = result[RIDX(i,j,dim)];
              right = tmp[RIDX(i,j,dim)];
	    }
	}
    }

    if (err) {
	printf("\n");
	printf("ERROR: Dimension=%d, %d errors\n", dim, err);    
	printf("E.g., \n");
	printf("You have dst[%d][%d].{red,green,blue} = {%d,%d,%d}\n",
	       badi, badj, wrong.red, wrong.green, wrong.blue);
	printf("It should be dst[%d][%d].{red,green,blue} = {%d,%d,%d}\n",
	       badi, badj, right.red, right.green, right.blue);
    }

    return err;
}


void complex_wrapper(void *arglist[]) 
{
  pixel *orig, *result;
    int mydim;
    complex_test_func f;

    f = (complex_test_func) arglist[0];
    mydim = *((int *) arglist[1]);
    orig = (pixel *) arglist[2];
    result = (pixel *) arglist[3];

    (*f)(mydim, orig, result);

    return;
}

void blur_wrapper(void *arglist[]) 
{
    pixel *src, *dst;
    int mydim;
    blur_test_func f;

    f = (blur_test_func) arglist[0];
    mydim = *((int *) arglist[1]);
    src = (pixel *) arglist[2];
    dst = (pixel *) arglist[3];

    (*f)(mydim, src, dst);

    return;
}

void run_complex_benchmark(int idx, int dim)
{
  benchmarks_complex[idx].complex_funct(dim, orig, result);
}

void test_complex(int bench_index) 
{
    int i;
    int test_num;
    char *description = benchmarks_complex[bench_index].description;
  
    for (test_num = 0; test_num < DIM_CNT; test_num++) {
      int dim;

	/* Check for odd dimension */
	create(ODD_DIM);
	run_complex_benchmark(bench_index, ODD_DIM);
	if (check_complex(ODD_DIM, save_test_image_files)) {
	    printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
		   benchmarks_complex[bench_index].description, ODD_DIM);
	    return;
	}

	/* Create a test image of the required dimension */
	dim = test_dim_complex[test_num];
	create(dim);
#ifdef DEBUG
	printf("DEBUG: Running benchmark \"%s\"\n", benchmarks_complex[bench_index].description);
#endif

	/* Check that the code works */
	run_complex_benchmark(bench_index, dim);
	if (check_complex(dim, save_all_image_files)) {
	    printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
		   benchmarks_complex[bench_index].description, dim);
	    return;
	}

	/* Measure CPE */
	{
	    double num_cycles, cpe;
	    int tmpdim = dim;
	    void *arglist[4];
	    double dimension = (double) dim;
	    double work = dimension*dimension;
#ifdef DEBUG
	    printf("DEBUG: dimension=%.1f\n",dimension);
	    printf("DEBUG: work=%.1f\n",work);
#endif
            
	    arglist[0] = (void *) benchmarks_complex[bench_index].complex_funct;
	    arglist[1] = (void *) &tmpdim;
	    arglist[2] = (void *) orig;
	    arglist[3] = (void *) result;

	    create(dim);
	    num_cycles = fcyc_v((test_funct_v)&complex_wrapper, arglist); 
	    cpe = num_cycles/work;
	    benchmarks_complex[bench_index].cpes[test_num] = cpe;
	}
    }

    /* 
     * Print results as a table 
     */
    printf("Complex: Version = %s:\n", description);
    printf("Dim\t");
    for (i = 0; i < DIM_CNT; i++)
	printf("\t%d", test_dim_complex[i]);
    printf("\tMean\n");
  
    printf("Your CPEs");
    for (i = 0; i < DIM_CNT; i++) {
	printf("\t%.1f", benchmarks_complex[bench_index].cpes[i]);
    }
    printf("\n");

    printf("Baseline CPEs");
    for (i = 0; i < DIM_CNT; i++) {
	printf("\t%.1f", complex_baseline_cpes[i]);
    }
    printf("\n");

    /* Compute Speedup */
    {
	double prod, ratio, mean;
	prod = 1.0; /* Geometric mean */
	printf("Speedup\t");
	for (i = 0; i < DIM_CNT; i++) {
	    if (benchmarks_complex[bench_index].cpes[i] > 0.0) {
		ratio = complex_baseline_cpes[i]/
		    benchmarks_complex[bench_index].cpes[i];
	    }
	    else {
		printf("Fatal Error: Non-positive CPE value...\n");
		exit(EXIT_FAILURE);
	    }
	    prod *= ratio;
	    printf("\t%.1f", ratio);
	}

	/* Geometric mean */
	mean = pow(prod, 1.0/(double) DIM_CNT);
	printf("\t%.1f", mean);
	printf("\n\n");
	if (mean > complex_maxmean) {
	    complex_maxmean = mean;
	    complex_maxmean_desc = benchmarks_complex[bench_index].description;
	}
    }


#ifdef DEBUG
    fflush(stdout);
#endif
    return;  
}

void run_blur_benchmark(int idx, int dim) 
{
  benchmarks_blur[idx].blur_funct(dim, orig, result);
}

void test_blur(int bench_index) 
{
    int i;
    int test_num;
    char *description = benchmarks_blur[bench_index].description;
  
    for(test_num=0; test_num < DIM_CNT; test_num++) {
	int dim;

	/* Check correctness for odd (non power of two dimensions */
	create(ODD_DIM);
	run_blur_benchmark(bench_index, ODD_DIM);
	if (check_blur(ODD_DIM, save_test_image_files)) {
	    printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
		   benchmarks_blur[bench_index].description, ODD_DIM);
	    return;
	}

	/* Create a test image of the required dimension */
	dim = test_dim_blur[test_num];
	create(dim);

#ifdef DEBUG
	printf("DEBUG: Running benchmark \"%s\"\n", benchmarks_blur[bench_index].description);
#endif
	/* Check that the code works */
	run_blur_benchmark(bench_index, dim);
	if (check_blur(dim, save_all_image_files)) {
	    printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
		   benchmarks_blur[bench_index].description, dim);
	    return;
	}

	/* Measure CPE */
	{
	    double num_cycles, cpe;
	    int tmpdim = dim;
	    void *arglist[4];
	    double dimension = (double) dim;
	    double work = dimension*dimension;
#ifdef DEBUG
	    printf("DEBUG: dimension=%.1f\n",dimension);
	    printf("DEBUG: work=%.1f\n",work);
#endif
	    arglist[0] = (void *) benchmarks_blur[bench_index].blur_funct;
	    arglist[1] = (void *) &tmpdim;
	    arglist[2] = (void *) orig;
	    arglist[3] = (void *) result;
        
	    create(dim);
            num_cycles = fcyc_v((test_funct_v)&blur_wrapper, arglist); 
	    cpe = num_cycles/work;
	    benchmarks_blur[bench_index].cpes[test_num] = cpe;
	}
    }

    /* Print results as a table */
    printf("Blur: Version = %s:\n", description);
    printf("Dim\t");
    for (i = 0; i < DIM_CNT; i++)
	printf("\t%d", test_dim_blur[i]);
    printf("\tMean\n");
  
    printf("Your CPEs");
    for (i = 0; i < DIM_CNT; i++) {
	printf("\t%.1f", benchmarks_blur[bench_index].cpes[i]);
    }
    printf("\n");

    printf("Baseline CPEs");
    for (i = 0; i < DIM_CNT; i++) {
	printf("\t%.1f", blur_baseline_cpes[i]);
    }
    printf("\n");

    /* Compute speedup */
    {
	double prod, ratio, mean;
	prod = 1.0; /* Geometric mean */
	printf("Speedup\t");
	for (i = 0; i < DIM_CNT; i++) {
	    if (benchmarks_blur[bench_index].cpes[i] > 0.0) {
		ratio = blur_baseline_cpes[i]/
		    benchmarks_blur[bench_index].cpes[i];
	    }
	    else {
		printf("Fatal Error: Non-positive CPE value...\n");
		exit(EXIT_FAILURE);
	    }
	    prod *= ratio;
	    printf("\t%.1f", ratio);
	}
	/* Geometric mean */
	mean = pow(prod, 1.0/(double) DIM_CNT);
	printf("\t%.1f", mean);
	printf("\n\n");
	if (mean > blur_maxmean) {
	    blur_maxmean = mean;
	    blur_maxmean_desc = benchmarks_blur[bench_index].description;
	}
    }

    return;  
}


void usage(char *progname) 
{
    fprintf(stderr, "Usage: %s [-hqg] [-f <func_file>] [-d <dump_file>]\n", progname);    
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -h         Print this message\n");
    fprintf(stderr, "  -i         Save test images as \".image\" files\n");
    fprintf(stderr, "  -I         Save all images as \".image\" files\n");
    fprintf(stderr, "  -m <mode>  Pick original image: gradient, squares, lines, or random\n");
    fprintf(stderr, "  -q         Quit after dumping (use with -d )\n");
    fprintf(stderr, "  -g         Autograder mode: checks only complex() and dither_blur()\n");
    fprintf(stderr, "  -f <file>  Get test function names from dump file <file>\n");
    fprintf(stderr, "  -d <file>  Emit a dump file <file> for later use with -f\n");
    exit(EXIT_FAILURE);
}



int main(int argc, char *argv[])
{
    int i;
    int quit_after_dump = 0;
    int skip_studentname_check = 0;
    int autograder = 0;
    int seed = 1729;
    char c = '0';
    char *bench_func_file = NULL;
    char *func_dump_file = NULL;

    /* register all the defined functions */
    register_complex_functions();
    register_blur_functions();

    /* parse command line args */
    while ((c = getopt(argc, argv, "iIm:tgqf:d:s:h")) != -1)
	switch (c) {

        case 'i':
          save_test_image_files = 1;
          break;

        case 'I':
          save_all_image_files = 1;
          break;

        case 'm':
          if (!strcmp(optarg, "gradient")) {
            image_mode = GRADIENT;
          } else if (!strcmp(optarg, "squares")) {
            image_mode = SQUARES;
          } else if (!strcmp(optarg, "lines")) {
            image_mode = LINES;
          } else if (!strcmp(optarg, "random")) {
            image_mode = RANDOM;
          } else {
            fprintf(stderr, "unrecognized image mode: %s\n", optarg);
            exit(1);
          }
          break;
	  
	case 't': /* skip student name check (hidden flag) */
	    skip_studentname_check = 1;
	    break;

	case 's': /* seed for random number generator (hidden flag) */
	    seed = atoi(optarg);
	    break;

	case 'g': /* autograder mode (checks only complex() and blur()) */
	    autograder = 1;
	    break;

	case 'q':
	    quit_after_dump = 1;
	    break;

	case 'f': /* get names of benchmark functions from this file */
	    bench_func_file = strdup(optarg);
	    break;

	case 'd': /* dump names of benchmark functions to this file */
	    func_dump_file = strdup(optarg);
	    {
		int i;
		FILE *fp = fopen(func_dump_file, "w");	

		if (fp == NULL) {
		    printf("Can't open file %s\n",func_dump_file);
		    exit(-5);
		}

		for(i = 0; i < complex_benchmark_count; i++) {
		    fprintf(fp, "R:%s\n", benchmarks_complex[i].description); 
		}
		for(i = 0; i < blur_benchmark_count; i++) {
		    fprintf(fp, "S:%s\n", benchmarks_blur[i].description); 
		}
		fclose(fp);
	    }
	    break;

	case 'h': /* print help message */
	    usage(argv[0]);

	default: /* unrecognized argument */
	    usage(argv[0]);
	}

    if (quit_after_dump) 
	exit(EXIT_SUCCESS);


    /* Print student info */
    if (!skip_studentname_check) {
	if (strcmp("Harry Q. Bovik", student.name) == 0) {
	    printf("%s: Please fill in the student struct in kernels.c.\n", argv[0]);
	    exit(1);
	}
	printf("Student: %s\n", student.name);
	printf("Email: %s\n", student.email);
	printf("\n");
    }

    srand(seed);

    /* 
     * If we are running in autograder mode, we will only test
     * the complex() and bench() functions.
     */
    if (autograder) {
	complex_benchmark_count = 1;
	blur_benchmark_count = 1;

	benchmarks_complex[0].complex_funct = complex;
	benchmarks_complex[0].description = "complex() function";
	benchmarks_complex[0].valid = 1;

	benchmarks_blur[0].blur_funct = dither_blur;
	benchmarks_blur[0].description = "dither_blur() function";
	benchmarks_blur[0].valid = 1;
    }

    /* 
     * If the user specified a file name using -f, then use
     * the file to determine the versions of complex and blur to test
     */
    else if (bench_func_file != NULL) {
	char flag;
	char func_line[256];
	FILE *fp = fopen(bench_func_file, "r");

	if (fp == NULL) {
	    printf("Can't open file %s\n",bench_func_file);
	    exit(-5);
	}
    
	while(func_line == fgets(func_line, 256, fp)) {
	    char *func_name = func_line;
	    char **strptr = &func_name;
	    char *token = strsep(strptr, ":");
	    flag = token[0];
	    func_name = strsep(strptr, "\n");
#ifdef DEBUG
	    printf("Function Description is %s\n",func_name);
#endif

	    if (flag == 'R') {
		for(i=0; i<complex_benchmark_count; i++) {
		    if (strcmp(benchmarks_complex[i].description, func_name) == 0)
			benchmarks_complex[i].valid = 1;
		}
	    }
	    else if (flag == 'S') {
		for(i=0; i<blur_benchmark_count; i++) {
		    if (strcmp(benchmarks_blur[i].description, func_name) == 0)
			benchmarks_blur[i].valid = 1;
		}
	    }      
	}

	fclose(fp);
    }

    /* 
     * If the user didn't specify a dump file using -f, then 
     * test all of the functions
     */
    else { /* set all valid flags to 1 */
	for (i = 0; i < complex_benchmark_count; i++)
	    benchmarks_complex[i].valid = 1;
	for (i = 0; i < blur_benchmark_count; i++)
	    benchmarks_blur[i].valid = 1;
    }

    /* Set measurement (fcyc) parameters */
    set_fcyc_cache_size(1 << 14); /* 16 KB cache size */
    set_fcyc_clear_cache(1); /* clear the cache before each measurement */
#ifdef __linux__
    set_fcyc_compensate(1); /* try to compensate for timer overhead */
#endif

    for (i = 0; i < complex_benchmark_count; i++) {
	if (benchmarks_complex[i].valid)
	    test_complex(i);
    
}
    for (i = 0; i < blur_benchmark_count; i++) {
	if (benchmarks_blur[i].valid)
	    test_blur(i);
    }


    if (autograder) {
	printf("\nbestscores:%.1f:%.1f:\n", complex_maxmean, blur_maxmean);
    }
    else {
	printf("Summary of Your Best Scores:\n");
	printf("  Complex: %3.1f (%s)\n", complex_maxmean, complex_maxmean_desc);
	printf("  Blur: %3.1f (%s)\n", blur_maxmean, blur_maxmean_desc);
    }

    return 0;
}
