/*********************************************************
 * config.h - Configuration data for the driver.c program.
 *********************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* 
 * CPEs for the baseline (naive) version of the complex function that
 * was handed out to the students. Rd is the measured CPE for a dxd
 * image. Run the driver.c program on your system to get these
 * numbers.  
 */
#define R64    5.1
#define R128   5.1
#define R256   5.3
#define R512   10.0
#define R1024  10.7

/* 
 * CPEs for the baseline (naive) version of the blur function that
 * was handed out to the students. Sd is the measure CPE for a dxd
 * image. Run the driver.c program on your system to get these
 * numbers.  
 */
#define S32   50.0
#define S64   50.0
#define S128  50.0
#define S256  50.0
#define S512  50.7


#endif /* _CONFIG_H_ */
