/* This small utility generates test data for assignment #1.  Note that it does
 * not generate solutions -- it only prepares data files for testing student
 * solutions to assignment #1.
 *
 * Command to compile this program (feel free to use a longer executable name):
 *   g++ generate_from_settings.cpp -o gfs
 *
 * Caution:  The -o [filename] is how you specify the name of the executable.
 * Put the output file option as the last option on the line.
 * This helps you avoid overwriting your source code with the output file.
 * (Students often mistype this line and overwrite their source code.)
 * 
 * A quiet compilation is a successful compilation.  Any error messages at all 
 * indicate compilation failure.
 * 
 * Example command to run the executable program (after successful complilation):
 *   ./gfs 
 *
 * This program reads these settings from the settings.txt file:
 *   The number of points to create for the cloud file,
 *   the scale of the resulting point cloud, and
 *   the random number generator seed.
 *
 * In the example settings.txt file, 10 points will be created with each coordinate in
 * the range ([-20..20), [-20..20), [-20..20)), and the point cloud will
 * be the same one every time (as long as the random seed is 12345). 
 *
 * Finally, 'using' statements are expressly forbidden in this assignment.
 *
 * Peter Jensen
 * January 20, 2021
 */


/* Everything in c++ must be declared before it is used.  These declarations
 * are often put into .h files so they can be used in many other programs.
 * Use the 'include' compiler directive to bring those declarations into
 * a program. 
 */

// The c++ iostream library is used when console (or shell) I/O is needed.

#include <iostream>  

// The c++ fstream library is used when file I/O is needed.

#include <fstream> 

// The c++ sstream library is used to treat strings as I/O streams.

#include <sstream>

// The c++ cstdlib library contains a lot of utility functions, including
// functions for generating random numbers.

#include <cstdlib>

// The c++ cmath library contains the sqrt function (not used here).

#include <cmath>

// The vector class works similarly to array lists in other languages.
// Use the 'push_back' function to append items to a vector, then
// access it like an array.  Examples below.

#include <vector>

// I use the same pntvec structure that I expect students to use.  The
// declarations are in a header file (.h file).

#include "pntvec.h"


/* 'using' statements expressly forbidden in this assignment. */

/* I prefer to put my main function at the top of the file.  I also
 * want helper functions.  Since everything must be declared before
 * it is used, the helper functions must be declared before main.
 * These are called forward declarations, and each one is just
 * the complete function header with a semicolon after it.
 */

pntvec generate_point (double scale);

void   output_point   (std::ostream & file, pntvec point);


/* Main -- the application entry point. 
 *
 * Parameters:
 *   argc - a count indicating the number of command line arguments
 *   argv - the command line arguments, as an array of character arrays
 *
 * Return value:
 *    an integer exit code, we return non-zero if an error occured
 */

int main (int argc, char** argv)
{
  // Make sure there are the correct number of command line arguments.

  if (argc != 1)
  {
    std::cout << "Parameters are not needed, settings come from settings.txt." << std::endl;
    std::cout << "Usage:  " << argv[0] << std::endl; 

    return 22;  // Indicates invalid argument -- likely ignored by shell.
  }

  // Read the settings file to get the generator settings.

  int point_count = -1;
  double point_scale = -1;
  unsigned int random_seed = 0;  

  // Open the settings file.
  //
  // Note:  In c++, declaring an object variable creates
  // the object.  The constructor parameter requires a filename.

  std::ifstream settings_file("settings.txt"); 

  // Read all the settings.  Ignore duplicate settings, keep the
  //  last setting for each category.  In the case of an error,
  //  return exit code 22 (invalid argument).

  while (true)
  {
    // Get the name of the next setting.

    std::string setting_name;
    settings_file >> setting_name;

    if (settings_file.fail())
      break;

    // Check for and parse the point count.

    if (setting_name == "point_count:")
    {
      settings_file >> point_count;  // Attempt to convert text
      if (settings_file.fail() || !(point_count>0))
      {
	std::cout << "Setting error:  Point count must be a positive integer." << std::endl;
	settings_file.close();
	return 22;
      }
      continue;
    }

    // Check for and parse the scale.

    if (setting_name == "point_scale:")
    {
      settings_file >> point_scale;  // Attempt to convert text
      if (settings_file.fail() || !(point_scale > 0))
      {
	std::cout << "Setting error:  Point cloud scale must be positive." << std::endl;
	settings_file.close();
	return 22;
      }
      continue;
    }

    // Check for and parse the random seed.

    if (setting_name == "random_seed:")
    {
      settings_file >> random_seed;   // Attempt to convert text
      if (settings_file.fail() || random_seed == 0)
      {
	std::cout << "Setting error:  Random seed must be a non-zero integer in [-2^31..2^31)." << std::endl;
	settings_file.close();
	return 22;
      }
      continue;
    }

    // Setting did not match any expected setting.

    std::cout << "Setting error:  Unexpected setting " << setting_name << "." << std::endl;
    settings_file.close();
    return 22;
  }

  // CLose the file.

  settings_file.close();

  // Make sure all settings were retrieved.

  if (point_count < 0)
  {
    std::cout << "Missing setting:  point_count" << std::endl;
    return 22;
  }

  if (point_scale < 0)
  {
    std::cout << "Missing setting:  point_scale" << std::endl;
    return 22;
  }

  if (random_seed == 0)
  {
    std::cout << "Missing setting:  random_seed" << std::endl;
    return 22;
  }

  // Seed the random number generator

  std::srand(random_seed);

  // Create the candidate file.  Open the file, 
  //   write out the four random points, close the file.

  std::ofstream candidate_file("candidates.txt");

  for (int i = 0; i < 4; i++)
  {
    pntvec p = generate_point (point_scale);
    output_point (candidate_file, p);
  }

  candidate_file.close();

  // Create the point cloud and keep it in a vector.

  std::vector<pntvec> point_cloud;  // Creates the list object

  for (int i = 0; i < point_count; i++)
  {
    pntvec p = generate_point (point_scale);
    point_cloud.push_back(p);
  }

  // Write the point cloud file.  Open the file,
  //   write out the points, close the file.

  std::ofstream point_cloud_file("point_cloud.txt");

  for (int i = 0; i < point_cloud.size(); i++)
    output_point (point_cloud_file, point_cloud[i]);

  point_cloud_file.close();

  // Done, no errors.

  return 0;
}

/* Returns a random point in the range
 *  ([-scale..scale), [-scale..scale), [-scale..scale)).
 *
 * Parameters:
 *   scale - 1/2 the size the of 3D cube to draw from
 *
 * Returns:
 *   a random point within a centered 3D cube (scaled)
 */
pntvec generate_point (double scale)
{
  pntvec result;

  result.x = std::rand() / (double) RAND_MAX * scale * 2 - scale; 
  result.y = std::rand() / (double) RAND_MAX * scale * 2 - scale; 
  result.z = std::rand() / (double) RAND_MAX * scale * 2 - scale; 

  return result;
}

/* Writes the given point to the specified output stream,
 * followed by a newline.
 *
 * Note:  the & in the first parameter indicates pass-by-reference.
 * This avoids copying the output stream (which would likely fail).
 *
 * Parameters:
 *   file - a reference to an output stream (any output stream)
 *   point - a 3D point to write to the file
 *
 * Returns:
 *   nothing
 */
void   output_point   (std::ostream & file, pntvec point)
{
  file << point.x << " ";
  file << point.y << " ";
  file << point.z << std::endl;
}

