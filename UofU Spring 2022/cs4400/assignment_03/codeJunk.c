//int current, top_nb, bot_nb, left_nb, right_nb, TL_nb, TR_nb, BL_nb, BR_nb;

//Top Rows Case
  /* for (i = 0; i < 2; i++) */
  /*   for (j = 2; j < border; j++) */
  /*   { */
  /*     current = RIDX(i, j, dim); */
  /*     left_nb = RIDX(i, (j-2), dim); */
  /*     right_nb = RIDX(i, (j+2), dim); */
  /*     BL_nb = RIDX((i+2), (j-2), dim); */
  /*     bot_nb = RIDX((i+2), j, dim); */
  /*     BR_nb = RIDX((i+2), (j+2), dim); */
      
  /*     // left neighbor */
  /*     red += (int) src[left_nb].red; */
  /*     green += (int) src[left_nb].green; */
  /*     blue += (int) src[left_nb].blue; */

  /*     // current neighbor */
  /*     red += (int) src[current].red; */
  /*     green += (int) src[current].green; */
  /*     blue += (int) src[current].blue; */

  /*     // right neighbor */
  /*     red += (int) src[right_nb].red; */
  /*     green += (int) src[right_nb].green; */
  /*     blue += (int) src[right_nb].blue; */

  /*     // bottom left neighbor */
  /*     red += (int) src[BL_nb].red; */
  /*     green += (int) src[BL_nb].green; */
  /*     blue += (int) src[BL_nb].blue; */

  /*     // bottom neighbor */
  /*     red += (int) src[bot_nb].red; */
  /*     green += (int) src[bot_nb].green; */
  /*     blue += (int) src[bot_nb].blue; */

  /*     // bottom right neighbor */
  /*     red += (int) src[BR_nb].red; */
  /*     green += (int) src[BR_nb].green; */
  /*     blue += (int) src[BR_nb].blue; */

  /*     currentPixel.red = (unsigned short) (red / 6); */
  /*     currentPixel.green = (unsigned short) (green / 6); */
  /*     currentPixel.blue = (unsigned short) (blue / 6); */
      
  /*     dst[RIDX(i, j, dim)] =  currentPixel; */
 
  /*   } */

  //Bottom Rows Case
  /* for (i = border; i < dim; i++) */
  /*   for(j = 2; j < border; j++) */
  /*   { */
  /*     current = RIDX(i, j, dim); */
  /*     left_nb = RIDX(i, (j-2), dim); */
  /*     right_nb = RIDX(i, (j+2), dim); */
  /*     TL_nb = RIDX((i-2), (j-2), dim); */
  /*     top_nb = RIDX((i-2), j, dim); */
  /*     TR_nb = RIDX((i-2), (j+2), dim); */

  /*     // left neighbor */
  /*     red += (int) src[left_nb].red; */
  /*     green += (int) src[left_nb].green; */
  /*     blue += (int) src[left_nb].blue; */

  /*     // current neighbor */
  /*     red += (int) src[current].red; */
  /*     green += (int) src[current].green; */
  /*     blue += (int) src[current].blue; */

  /*     // right neighbor */
  /*     red += (int) src[right_nb].red; */
  /*     green += (int) src[right_nb].green; */
  /*     blue += (int) src[right_nb].blue; */

  /*     // top left neighbor */
  /*     red += (int) src[TL_nb].red; */
  /*     green += (int) src[TL_nb].green; */
  /*     blue += (int) src[TL_nb].blue; */

  /*     // top neighbor */
  /*     red += (int) src[top_nb].red; */
  /*     green += (int) src[top_nb].green; */
  /*     blue += (int) src[top_nb].blue; */

  /*     // top right neighbor */
  /*     red += (int) src[TR_nb].red; */
  /*     green += (int) src[TR_nb].green; */
  /*     blue += (int) src[TR_nb].blue; */

  /*     currentPixel.red = (unsigned short) (red / 6); */
  /*     currentPixel.green = (unsigned short) (green / 6); */
  /*     currentPixel.blue = (unsigned short) (blue / 6); */
      
  /*     dst[RIDX(i, j, dim)] =  currentPixel; */
  /*   } */

  //Left Columns Case
  /* for (i = 2; i < border; i++) */
  /*   for(j = 0; j < 2; j++) */
  /*   { */
  /*     current = RIDX(i, j, dim); */
  /*     right_nb = RIDX(i, (j+2), dim); */
  /*     top_nb = RIDX((i-2), j, dim); */
  /*     bot_nb = RIDX((i+2), j, dim); */
  /*     TR_nb = RIDX((i-2), (j+2), dim); */
  /*     BR_nb = RIDX((i+2), (j+2), dim); */

  /*     // current neighbor */
  /*     red += (int) src[current].red; */
  /*     green += (int) src[current].green; */
  /*     blue += (int) src[current].blue; */

  /*     // right neighbor */
  /*     red += (int) src[right_nb].red; */
  /*     green += (int) src[right_nb].green; */
  /*     blue += (int) src[right_nb].blue; */

  /*     // bottom neighbor */
  /*     red += (int) src[bot_nb].red; */
  /*     green += (int) src[bot_nb].green; */
  /*     blue += (int) src[bot_nb].blue; */

  /*     // top neighbor */
  /*     red += (int) src[top_nb].red; */
  /*     green += (int) src[top_nb].green; */
  /*     blue += (int) src[top_nb].blue; */

  /*     // top right neighbor */
  /*     red += (int) src[TR_nb].red; */
  /*     green += (int) src[TR_nb].green; */
  /*     blue += (int) src[TR_nb].blue; */

  /*     // bottom right neighbor */
  /*     red += (int) src[BR_nb].red; */
  /*     green += (int) src[BR_nb].green; */
  /*     blue += (int) src[BR_nb].blue; */

  /*     currentPixel.red = (unsigned short) (red / 6); */
  /*     currentPixel.green = (unsigned short) (green / 6); */
  /*     currentPixel.blue = (unsigned short) (blue / 6); */
      
  /*     dst[RIDX(i, j, dim)] =  currentPixel; */
		       
    /* } */

  //Right Columns Case
  /* for (i = 2; i < border; i++) */
  /*   for (j = border; j < dim; j++) */
  /*   { */
  /*     current = RIDX(i, j, dim); */
  /*     top_nb = RIDX((i-2), j, dim); */
  /*     bot_nb = RIDX((i+2), j, dim); */
  /*     left_nb = RIDX(i, (j-2), dim); */
  /*     TL_nb = RIDX((i-2), (j-2), dim); */
  /*     BL_nb = RIDX((i+2), (j-2), dim); */

  /*     // current neighbor */
  /*     red += (int) src[current].red; */
  /*     green += (int) src[current].green; */
  /*     blue += (int) src[current].blue; */

  /*     // bottom neighbor */
  /*     red += (int) src[bot_nb].red; */
  /*     green += (int) src[bot_nb].green; */
  /*     blue += (int) src[bot_nb].blue; */

  /*     // top neighbor */
  /*     red += (int) src[top_nb].red; */
  /*     green += (int) src[top_nb].green; */
  /*     blue += (int) src[top_nb].blue; */

  /*     // left neighbor */
  /*     red += (int) src[left_nb].red; */
  /*     green += (int) src[left_nb].green; */
  /*     blue += (int) src[left_nb].blue; */
      
  /*     // top left neighbor */
  /*     red += (int) src[TL_nb].red; */
  /*     green += (int) src[TL_nb].green; */
  /*     blue += (int) src[TL_nb].blue; */

  /*     // bottom left neighbor */
  /*     red += (int) src[BL_nb].red; */
  /*     green += (int) src[BL_nb].green; */
  /*     blue += (int) src[BL_nb].blue; */

  /*     currentPixel.red = (unsigned short) (red / 6); */
  /*     currentPixel.green = (unsigned short) (green / 6); */
  /*     currentPixel.blue = (unsigned short) (blue / 6); */
      
  /*     dst[RIDX(i, j, dim)] =  currentPixel; */
    /* } */

static  pixel ver2_weighted_combo(int dim, int i, int j, pixel *src) {

  int ii, jj;
  int red, green, blue;
  red = green = blue = 0;
  pixel currentPixel;
  
  for(ii = -2; ii <= 2; ii += 2)
    for(jj = -2; jj <= 2; jj += 2)
    {
      int blurPixel = RIDX((i + ii), (j+jj), dim);
      
      red += (int) src[blurPixel].red;
      green += (int) src[blurPixel].green;
      blue += (int) src[blurPixel].blue;
    }
  
  currentPixel.red = (unsigned short) (red / 9);
  currentPixel.green = (unsigned short) (green / 9);
  currentPixel.blue = (unsigned short) (blue / 9);

  return currentPixel;    
}

// BLOCKING ATTEMPT
  /* for (i = 2; i < border; i += blockLim) */
  /*   for (j = 2; j < border; j += blockLim) */
  /*     for (ii = i; ii < (i + blockLim); ii++) */
  /* 	for (jj = j; jj < (j + blockLim); jj++) */
  /* 	{ */
  /* 	  red = green = blue = 0; */
  /* 	  for(neighRow = -2; neighRow <= 2; neighRow += 2) */
  /* 	    for(neighCol = -2; neighCol <= 2; neighCol += 2) */
  /* 	    { */
  /* 	      int blurPixel = RIDX((ii + neighRow), (jj + neighCol), dim); */

  /* 	      red += (int) src[blurPixel].red; */
  /* 	      green += (int) src[blurPixel].green; */
  /* 	      blue += (int) src[blurPixel].blue; */
  /* 	    } */
  /* 	} */

/* char ver1_complex_descr[] = "ver1_complex: Version 1 Complex Implementation"; */
/* void ver1_complex(int dim, pixel *src, pixel *dest) */
/* { */
/*   int i, j; */
/*   int reorder_int = dim - 1; */
  
  
/*   for(j = 0; j < dim; j++) */
/*     for(i = 0; i < dim; i++) */
/*     { */
/*       int complexIndex = RIDX(reorder_int - j, reorder_int - i, dim); */
/*       int baseIndex = RIDX(i, j, dim); */
/*       int grey = ((int)src[baseIndex].red + (int)src[baseIndex].green + (int)src[baseIndex].blue) / 3; */

/*       dest[complexIndex].red = grey; */
      
/*       dest[complexIndex].green = grey; */
      
/*       dest[complexIndex].blue = grey; */
/*     } */
/* } */

/* static inline pixel ver1_weighted_combo(int dim, int i, int j, pixel *src)  */
/* { */
/*   int ii, jj; */
/*   pixel current_pixel; */

/*   int red, green, blue; */
/*   red = green = blue = 0; */

/*   int num_neighbors = 0; */
/*   for(ii= -1; ii < 2; ii++) */
/*     for(jj= -1; jj < 2; jj++) */
/*     { */
/*       int boundCheck_i = i + (2 * ii); */
/*       int boundCheck_j = j + (2 * jj); */
      
/*       if ((boundCheck_i < dim) && (boundCheck_i >= 0) && (boundCheck_j < dim) && (boundCheck_j >= 0))  */
/*       { */
/* 	int blurPixel = RIDX(boundCheck_i, boundCheck_j, dim); */
/* 	num_neighbors++; */
/* 	red += (int) src[blurPixel].red; */
/* 	green += (int) src[blurPixel].green; */
/* 	blue += (int) src[blurPixel].blue; */
/*       } */
/*     } */

/*   current_pixel.red = (unsigned short) (red / num_neighbors); */
/*   current_pixel.green = (unsigned short) (green / num_neighbors); */
/*   current_pixel.blue = (unsigned short) (blue / num_neighbors); */
  
/*   return current_pixel; */
/* } */

/* char ver1_dither_blur_desc[] = "ver1_dither_blur: Version 1 dither blur implementation."; */
/* void ver1_dither_blur (int dim, pixel* src, pixel* dst) */
/* { */
/*   int i, j, ii, jj; */

/*   int blockDim = dim / 4; */
  
/*   for (i = 0; i < dim; i += blockDim) */
/*     for (j = 0; j < dim; j += blockDim) */
/*       for (ii = i; ii < i + blockDim; ii++) */
/* 	for (jj = j; jj < j + blockDim; jj++) */
/* 	{ */
/* 	  dst[RIDX(ii, jj, dim)] = ver1_weighted_combo(dim, ii, jj, src); */
/* 	} */
/* } */

/* char ver2_dither_blur_desc[] = "ver2_dither_blur: Version 2 dither blur implementation."; */
/* void ver2_dither_blur (int dim, pixel* src, pixel* dst) */
/* { */
/*   int i, j, ii, jj, neighRow, neighCol; */
/*   int red, green, blue; */
/*   pixel currentPixel; */
  
/*   red = green = blue = 0; */
/*   int border = dim - 2; */
/*   int blockLim = border / 4; */

/*   //Center Block Case */
  
/*   for (i = 2; i < border; i += 2) */
/*     for (j = 2; j < border; j++) */
/*     { */
/*       red = green = blue = 0; */
          
/*       for(ii = -2; ii <= 2; ii += 2) */
/*   	for(jj = -2; jj <= 2; jj += 2) */
/*   	{ */
/*   	  int blurPixel = RIDX((i + ii), (j+jj), dim); */
      
/*   	  red += (int) src[blurPixel].red; */
/*   	  green += (int) src[blurPixel].green; */
/*   	  blue += (int) src[blurPixel].blue; */
/*   	} */
  
/*       currentPixel.red = (unsigned short) (red / 9); */
/*       currentPixel.green = (unsigned short) (green / 9); */
/*       currentPixel.blue = (unsigned short) (blue / 9); */
      
/*       dst[RIDX(i, j, dim)] =  currentPixel; */
/*     } */

/*   for (i = 3; i < border; i += 2) */
/*     for (j = 2; j < border; j++) */
/*     { */
/*       red = green = blue = 0; */
          
/*       for(ii = -2; ii <= 2; ii += 2) */
/*   	for(jj = -2; jj <= 2; jj += 2) */
/*   	{ */
/*   	  int blurPixel = RIDX((i + ii), (j+jj), dim); */
      
/*   	  red += (int) src[blurPixel].red; */
/*   	  green += (int) src[blurPixel].green; */
/*   	  blue += (int) src[blurPixel].blue; */
/*   	} */
  
/*       currentPixel.red = (unsigned short) (red / 9); */
/*       currentPixel.green = (unsigned short) (green / 9); */
/*       currentPixel.blue = (unsigned short) (blue / 9); */
      
/*       dst[RIDX(i, j, dim)] =  currentPixel; */
      
/*     } */

/*   // Outer Ring */
/*   for(i = 0; i < dim; i++) */
/*     for(j = 0; j < dim; j++) */
/*     { */
/*       if((i > 1 && i < (dim - 2)) && (j == 2)) */
/*       { */
/* 	j = (dim - 2); */
/*       } */

/*       dst[RIDX(i, j, dim)] = ver1_weighted_combo(dim, i, j, src); */
/*     } */
  
/* } */
