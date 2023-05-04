/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Erickson Tat Nguyen",     /* Full name */
  "u1117938@umail.utah.edu",  /* Email address */
};

/***************
 * COMPLEX KERNEL
 ***************/

/******************************************************
 * Your different versions of the complex kernel go here
 ******************************************************/

char ver2_complex_descr[] = "ver2_complex: Version 2 Complex Implementation with Blocking";
void ver2_complex(int dim, pixel *src, pixel *dest)
{
  int i, j, ii, jj;
  int reorder_int = dim - 1;
  
  int blockDim = dim / 4;

  for (j = 0; j < dim; j += blockDim)
    for (i = 0; i < dim; i += blockDim)
      for (jj = j; jj < (j + blockDim); jj++)
	for (ii = i; ii < (i + blockDim); ii++)
	{
	  int complexIndex = RIDX(reorder_int - jj, reorder_int - ii, dim);
	  int baseIndex = RIDX(ii, jj, dim);
	  int grey = ((int)src[baseIndex].red + (int)src[baseIndex].green + (int)src[baseIndex].blue) / 3;

	  dest[complexIndex].red = grey;

	  dest[complexIndex].green = grey;

	  dest[complexIndex].blue = grey;
	  						 
	}
}

/* 
 * naive_complex - The naive baseline version of complex 
 */
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;

  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						      (int)src[RIDX(i, j, dim)].green +
						      (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;

    }
}


/* 
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working optimized version Erickson Nguyen's Version";
void complex(int dim, pixel *src, pixel *dest)
{
  ver2_complex(dim, src, dest);
}

/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&complex, complex_descr);
  add_complex_function(&naive_complex, naive_complex_descr);
}


/***************
 * BLUR KERNEL
 **************/

/***************************************************************
 * Various helper functions for the blur kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/


/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(ii=-1; ii < 2; ii++)
    for(jj=-1; jj < 2; jj++) 
      if ((i + 2*ii < dim) && (i + 2*ii >= 0) && (j + 2*jj < dim) && (j + 2*jj >= 0)) 
      {
	num_neighbors++;
	red += (int) src[RIDX(i+2*ii,j+2*jj,dim)].red;
	green += (int) src[RIDX(i+2*ii,j+2*jj,dim)].green;
	blue += (int) src[RIDX(i+2*ii,j+2*jj,dim)].blue;
      }
  
  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}

__attribute__ ((always_inline)) static pixel TR_Handler(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  for (ii = 0; ii <= 2; ii += 2)
    for (jj = -2; jj <= 2; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j + jj), dim);
      pixel neighPixel = src[blurPixel];
      red += (int) neighPixel.red;
      green += (int) neighPixel.green;
      blue += (int) neighPixel.blue;
    }

  currentPixel.red = (unsigned short) (red / 6);
  currentPixel.green = (unsigned short) (green / 6);
  currentPixel.blue = (unsigned short) (blue / 6);

  return currentPixel;
}

__attribute__ ((always_inline)) static pixel BR_Handler(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  for (ii = -2; ii <= 0; ii += 2)
    for (jj = -2; jj <= 2; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j + jj), dim);
      
      pixel neighPixel = src[blurPixel];
      red += (int) neighPixel.red;
      green += (int) neighPixel.green;
      blue += (int) neighPixel.blue;
    }

  currentPixel.red = (unsigned short) (red / 6);
  currentPixel.green = (unsigned short) (green / 6);
  currentPixel.blue = (unsigned short) (blue / 6);

  return currentPixel;
}

__attribute__ ((always_inline)) static pixel LC_Handler(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  for (ii = -2; ii <= 2; ii += 2)
    for (jj = 0; jj <= 2; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j + jj), dim);
      
      pixel neighPixel = src[blurPixel];
      red += (int) neighPixel.red;
      green += (int) neighPixel.green;
      blue += (int) neighPixel.blue;
    }

  currentPixel.red = (unsigned short) (red / 6);
  currentPixel.green = (unsigned short) (green / 6);
  currentPixel.blue = (unsigned short) (blue / 6);

  return currentPixel;
}

__attribute__ ((always_inline)) static pixel RC_Handler(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  for (ii = -2; ii <= 2; ii += 2)
    for (jj = -2; jj <= 0; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j + jj), dim);
      
      pixel neighPixel = src[blurPixel];
      red += (int) neighPixel.red;
      green += (int) neighPixel.green;
      blue += (int) neighPixel.blue;
    }

  currentPixel.red = (unsigned short) (red / 6);
  currentPixel.green = (unsigned short) (green / 6);
  currentPixel.blue = (unsigned short) (blue / 6);

  return currentPixel;
}

__attribute__ ((always_inline)) static pixel TLC_Handler(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  for (ii = 0; ii <= 2; ii += 2)
    for (jj = 0; jj <= 2; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j + jj), dim);
      
      pixel neighPixel = src[blurPixel];
      red += (int) neighPixel.red;
      green += (int) neighPixel.green;
      blue += (int) neighPixel.blue;
    }

  currentPixel.red = (unsigned short) (red / 4);
  currentPixel.green = (unsigned short) (green / 4);
  currentPixel.blue = (unsigned short) (blue / 4);

  return currentPixel;
}

__attribute__ ((always_inline)) static pixel TRC_Handler(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  for (ii = 0; ii <= 2; ii += 2)
    for (jj = -2; jj <= 0; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j + jj), dim);
      
      pixel neighPixel = src[blurPixel];
      red += (int) neighPixel.red;
      green += (int) neighPixel.green;
      blue += (int) neighPixel.blue;
    }

  currentPixel.red = (unsigned short) (red / 4);
  currentPixel.green = (unsigned short) (green / 4);
  currentPixel.blue = (unsigned short) (blue / 4);

  return currentPixel;
}

__attribute__ ((always_inline)) static pixel BLC_Handler(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  for (ii = -2; ii <= 0; ii += 2)
    for (jj = 0; jj <= 2; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j + jj), dim);
      
      pixel neighPixel = src[blurPixel];
      red += (int) neighPixel.red;
      green += (int) neighPixel.green;
      blue += (int) neighPixel.blue;
    }

  currentPixel.red = (unsigned short) (red / 4);
  currentPixel.green = (unsigned short) (green / 4);
  currentPixel.blue = (unsigned short) (blue / 4);

  return currentPixel;
}

__attribute__ ((always_inline)) static pixel BRC_Handler(int dim, int i, int j, pixel *src)
{
  int ii, jj;
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  for (ii = -2; ii <= 0; ii += 2)
    for (jj = -2; jj <= 0; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j + jj), dim);
      
      pixel neighPixel = src[blurPixel];
      red += (int) neighPixel.red;
      green += (int) neighPixel.green;
      blue += (int) neighPixel.blue;
    }

  currentPixel.red = (unsigned short) (red / 4);
  currentPixel.green = (unsigned short) (green / 4);
  currentPixel.blue = (unsigned short) (blue / 4);

  return currentPixel;
}

__attribute__ ((always_inline)) static pixel Center_Handler(int dim, int i, int j, pixel *src)
{
  pixel currentPixel;
  int red, green, blue;

  red = green = blue = 0;

  /////
  pixel TL_nb = src[RIDX((i - 2), (j - 2), dim)];
  pixel Top_nb = src[RIDX((i - 2), j, dim)];
  pixel TR_nb = src[RIDX((i - 2), (j + 2), dim)];
  pixel Left_nb = src[RIDX(i, (j - 2), dim)];
  pixel current = src[RIDX(i, j, dim)];
  pixel Right_nb = src[RIDX(i, (j + 2), dim)];
  pixel BL_nb = src[RIDX((i + 2), (j - 2), dim)];
  pixel Bot_nb = src[RIDX((i + 2), j, dim)];
  pixel BR_nb = src[RIDX((i + 2), (j + 2), dim)];

  red = (int)TL_nb.red + (int)Top_nb.red + (int)TR_nb.red + (int)Left_nb.red + (int)current.red + (int)Right_nb.red + (int)BL_nb.red + (int)Bot_nb.red + (int)BR_nb.red;
  green = (int)TL_nb.green + (int)Top_nb.green + (int)TR_nb.green + (int)Left_nb.green + (int)current.green + (int)Right_nb.green + (int)BL_nb.green + (int)Bot_nb.green + (int)BR_nb.green;
  blue = (int)TL_nb.blue + (int)Top_nb.blue + (int)TR_nb.blue + (int)Left_nb.blue + (int)current.blue + (int)Right_nb.blue + (int)BL_nb.blue + (int)Bot_nb.blue + (int)BR_nb.blue;
  

  ////
  currentPixel.red = (unsigned short) (red / 9);
  currentPixel.green = (unsigned short) (green / 9);
  currentPixel.blue = (unsigned short) (blue / 9);

  return currentPixel;
}
/******************************************************
 * Your different versions of the blur kernel go here
 ******************************************************/

char ver3_dither_blur_desc[] = "ver3_dither_blur: Version 3 dither blur implementation.";
void ver3_dither_blur (int dim, pixel* src, pixel* dst)
{
  int i, j;
  int border = dim - 2;
  
  // Top Rows Case
  for (i = 0; i < 2; i++)
    for (j = 2; j < border; j++)
    {
      dst[RIDX(i, j, dim)] = TR_Handler(dim, i, j, src);
    }

  // Bottom Rows Case
  for (i = border; i < dim; i++)
    for (j = 2; j < border; j++)
    {
      dst[RIDX(i, j, dim)] = BR_Handler(dim, i, j, src);
    }
  
  // Left Columns Case
  for (i = 2; i < border; i++)
    for (j = 0; j < 2; j++)
    {
      dst[RIDX(i, j, dim)] = LC_Handler(dim, i, j, src);
    }
  
  // Right Columns Case
  for (i = 2; i < border; i++)
    for (j = border; j < dim; j++)
    {
      dst[RIDX(i, j, dim)] = RC_Handler(dim, i, j, src);
    }
  
  // Top Left Corner Case
  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
    {
      dst[RIDX(i, j, dim)] = TLC_Handler(dim, i, j, src);
    }
  
  // Top Right Corner Case
  for (i = 0; i < 2; i++)
    for (j = border; j < dim; j++)
    {
      dst[RIDX(i, j, dim)] = TRC_Handler(dim, i, j, src);
    }
  
  // Bottom Left Corner Case
  for (i = border; i < dim; i++)
    for (j = 0; j < 2; j++)
    {
      dst[RIDX(i, j, dim)] = BLC_Handler(dim, i, j, src);
    }
  
  // Bottom Right Corner Case
  for (i = border; i < dim; i++)
    for (j = border; j < dim; j++)
    {
      dst[RIDX(i, j, dim)] = BRC_Handler(dim, i, j, src);
    }
  
  // Center Block Case
  for (i = 2; i < border; i++)
    for (j = 2; j < border; j++)
    {
      dst[RIDX(i, j, dim)] = Center_Handler(dim, i, j, src);
    }
}

/*
 * naive_dither_blur - The naive baseline version of blur 
 */
char naive_dither_descr[] = "naive dither blur: Naive baseline implementation";
void naive_dither_blur(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}


/*
 * dither_blur - Your current working version of blur. 
 * IMPORTANT: This is the version you will be graded on
 */
char dither_blur_descr[] = "dither_blur: Current working optimized version, Erickson Nguyen's Version";
void dither_blur(int dim, pixel *src, pixel *dst) 
{
  ver3_dither_blur(dim, src, dst);
}

/********************************************************************* 
 * register_blur_functions - Register all of your different versions
 *     of the blur kernel with the driver by calling the
 *     add_blur_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_blur_functions() {
  add_blur_function(&dither_blur, dither_blur_descr);
  add_blur_function(&naive_dither_blur, naive_dither_descr);
}
