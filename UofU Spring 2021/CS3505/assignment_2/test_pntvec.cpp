/* CS3505
 * Author: Erickson Nguyen
 * 2/4/21
 *
 * Tests for the pntvec.cpp file
 */

#include <iostream>
#include "pntvec.h"
#include <sstream>


bool Default_Constructor_Test ();
bool Three_Param_Constructor_Test (double x, double y, double z);
bool Copy_Constructor_Test ();
bool negative_coordinate_constructor_Test();
bool zero_constructor_Test();
bool mixture_coordinates_constructor_Test ();
bool Set_X_Test (double x);
bool Set_Y_Test (double y);
bool Set_Z_Test (double z);
bool Get_X_Test ();
bool Get_Y_Test ();
bool Get_Z_Test ();
bool distance_to_Test ();
bool equal_operator_overload_test ();
bool addition_operator_overload_test ();
bool subtraction_operator_overload_test ();
bool multiplication_operator_overload_test ();
bool negation_operator_overload_test ();
bool output_stream_overload_test();
bool input_stream_overload_test();
bool twice_setX_Test();
bool multi_equal_operator_test ();
bool adding_from_negative_test ();
bool adding_from_negative_to_positive_test ();
bool subtracting_from_negative_point_test ();
bool double_negative_subtraction_test ();
bool negative_scalar_multiplication_test ();
bool zero_scalar_multiplication_test ();
bool positive_then_negative_multiplication_test();
bool negating_a_negative_point_test ();
bool constant_addition_test ();
bool constant_subtraction_test ();
bool constant_multiplication_test ();
bool constant_negation_test ();
bool whitespace_input_test ();
bool constant_output_test();
bool zero_output_distanceTo_test ();
bool negative_input_distanceTo_test ();
bool addition_subtraction_chaining_test ();
bool multiplication_addition_subtraction_chaining_test ();
bool operator_chaining_with_distanceTo_test ();
bool output_stream_chaining_test ();

int main()
{
  if (!Default_Constructor_Test())
  {
    return -1;
  }

  if (!Three_Param_Constructor_Test(1.0, 2.0, 3.0))
  {
    return -1;
  }

  if (!Copy_Constructor_Test())
  {
    return -1;
  }
  
  if (!negative_coordinate_constructor_Test())
  {
    return -1;
  }
  
  if (!zero_constructor_Test())
  {
    return -1;
  }
  if (!mixture_coordinates_constructor_Test())
  {
    return -1;
  }
  if (!Set_X_Test(2))
  {
    return -1;
  }

  if (!Set_Y_Test(3))
  {
    return -1;
  }

  if (!Set_Z_Test(4))
  {
    return -1;
  }

  if (!Get_X_Test())
  {
    return -1;
  }

 if (!Get_Y_Test())
  {
    return -1;
  }

 if (!Get_Z_Test())
  {
    return -1;
  }
 
 if (!distance_to_Test())
 {
   return -1;
 }
 
 if (!equal_operator_overload_test())
 {
   return -1;
 }

 if (!addition_operator_overload_test())
 {
   return -1;
 }

 if (!subtraction_operator_overload_test())
 {
   return -1;
 }

 if (!multiplication_operator_overload_test())
 {
   return -1;
 }

 if (!negation_operator_overload_test())
 {
   return -1;
 }

 if (!output_stream_overload_test())
 {
   return -1;
 }

 if (!input_stream_overload_test())
 {
   return -1;
 }

 if (!twice_setX_Test())
 {
   return -1;
 }

 if (!multi_equal_operator_test())
 {
   return -1;
 }

 if (!adding_from_negative_test())
 {
   return -1;
 }

 if (!adding_from_negative_to_positive_test())
 {
   return -1;
 }

 if (!subtracting_from_negative_point_test())
 {
   return -1;
 }

 if (!double_negative_subtraction_test())
 {
   return -1;
 }

 if (!negative_scalar_multiplication_test())
 {
   return -1;
 }

 if (!zero_scalar_multiplication_test())
 {
   return -1;
 }

 if (!positive_then_negative_multiplication_test())
 {
   return -1;
 }

 if (!negating_a_negative_point_test())
 {
   return -1;
 }

 if (!constant_addition_test())
 {
   return -1;
 }

 if (!constant_subtraction_test())
 {
   return -1;
 }

 if (!constant_multiplication_test())
 {
   return -1;
 }

 if (!constant_negation_test())
 {
   return -1;
 }

 if (!whitespace_input_test())
 {
   return -1;
 }

 if (!constant_output_test())
 {
   return -1;
 }

 if (!zero_output_distanceTo_test())
 {
   return -1;
 }

 if (!negative_input_distanceTo_test())
 {
   return -1;
 }

 if (!addition_subtraction_chaining_test())
 {
   return -1;
 }

 if (!multiplication_addition_subtraction_chaining_test())
 {
   return -1;
 }

 if (!operator_chaining_with_distanceTo_test())
 {
   return -1;
 }

 if (!output_stream_chaining_test())
 {
   return -1;
 }

 std::cout << "No Error" << std::endl;
 return 0;
}


/* Tests the default constructor.
 *
 *
 * Param:
 *
 * Return: True if passes, False, if fails
 */
bool Default_Constructor_Test ()
{
  pntvec point;

  if (point.get_x() != 0 || point.get_y() != 0 || point.get_z() != 0)
  { 
    std::cout << "Error in 'Default_Constructor_Test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Tests the three parameter constructor
 *
 *
 * Param: double x-coord, double y-coord, double z-coord
 *
 * Return: True if passes, False, if fails
 */
bool Three_Param_Constructor_Test (double x, double y, double z)
{
  pntvec point(x, y, z);
  if (point.get_x() != x || point.get_y() != y || point.get_z() != z)
  {
    std::cout << "Error in 'Three_Param_Constructor_Test'" << std::endl;
    return false;
  }
  
  return true;
}



/* Tests teh copy contructor.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool Copy_Constructor_Test ()
{
  pntvec point(2, 3, 4);
  pntvec copyPoint(point);
  if (copyPoint.get_x() != point.get_x() || copyPoint.get_y() != point.get_y() || copyPoint.get_z() != point.get_z())
  {
    std::cout << "Error in 'Copy_Constructor_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the three parameter constructor with negative values
 *
 *
 * Param: double x-coord, double y-coord, double z-coord
 *
 * Return: True if passes, False, if fails
 */
bool negative_coordinate_constructor_Test()
{
  pntvec point (-1.0, -7.0, -20.0);
  
  if (point.get_x() != -1.0 || point.get_y() != -7.0 || point.get_z() != -20.0)
  {
    std::cout << "Error in 'negative_coordinate_constructor_Test'" << std::endl;
    return false;
  }

  return true;
}


/*  Tests the three param constructor with zero values 
 *
 *
 * Param: double x-coord, double y-coord, double z-coord
 *
 * Return: True if passes, False, if fails
 */
bool zero_constructor_Test()
{
  pntvec point (0, 0, 0);
  
  if (point.get_x() != 0 || point.get_y() != 0 || point.get_z() != 0)
  {
    std::cout << "Error in 'zero_constructor_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Test the multi param constructor with positive, negative, and zero values
 *
 *
 * Param: double x-coord, double y-coord, double z-coord
 *
 * Return: True if passes, False, if fails
 */
bool mixture_coordinates_constructor_Test ()
{
  pntvec point (10.0, 0, -12.0);
  
  if (point.get_x() != 10.0 || point.get_y() != 0 || point.get_z() != -12.0)
  {
    std::cout << "Error in 'mixture_coordinates_constructor_Test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Tests the private Set_X method
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool Set_X_Test (double x)
{
  pntvec point(x, 0, 0);
  if (point.get_x() != x)
  {
    std::cout << "Error in 'Set_X_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the private Set_X method twice in succession.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool twice_setX_Test()
{
  pntvec point (5, 0, 0);
  
  if (point.get_x() != 5.0)
  {
    std::cout << "Error in 'twice_setX_Test'";
    return false;
  }

  pntvec point2 (6, 0, 0);
  
  point = point2;

  if (point.get_x() != 6.0)
  {
    std::cout << "Error in 'twice_setX_Test'";
    return false;
  }

  return true;
}


/* Tests the private Set_Y method
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool Set_Y_Test (double y)
{
  pntvec point (0, y, 0);
  if (point.get_y() != y)
  {
    std::cout << "Error in 'Set_Y_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the private Set_Z method
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool Set_Z_Test (double z)
{
  pntvec point (0, 0, z);
  if (point.get_z() != z)
  {
    std::cout << "Error in 'Set_Z_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the get_x method and sees if 'this' is unchanged.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool Get_X_Test ()
{
  pntvec point(1, 2, 3);
  if (point.get_x() != 1)
  {
    std::cout << "Error in 'Get_X_Test'" << std::endl;
    return false;
  }

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in Get_X_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the get_y method and sees if 'this' is unchanged.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool Get_Y_Test ()
{
  pntvec point(1, 2, 3);
  if (point.get_y() != 2)
  {
    std::cout << "Error in 'Get_Y_Test'" << std::endl;
    return false;
  }

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in Get_Y_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the get_z method and sees if 'this' is unchanged.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool Get_Z_Test ()
{
  pntvec point(1, 2, 3);
  if (point.get_z() != 3)
  {
    std::cout << "Error in 'Get_Z_Test'" << std::endl;
    return false;
  }

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in Get_Z_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the distance_to method between two points with a margin of error of 0.0001
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool distance_to_Test ()
{
  pntvec point1 (1, 1, 1);
  pntvec point2 (2, 2, 2);
  
  double MarginOfError = 0.0001;

  long expectedResult = 1.732050807568877293527446341;
  double actualResult = point1.distance_to(point2);

  if (!((expectedResult - actualResult) < MarginOfError))
  {
    std::cout << "Error in 'distance_to_Test'" << std::endl;
    return false;
  }

  return true;
}


/* Test the equal operator with pntvec objects
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool equal_operator_overload_test ()
{
  pntvec point (2, 3, 4);
  pntvec copyPoint = point;
  
  if (copyPoint.get_x() != point.get_x() || copyPoint.get_y() != point.get_y() || copyPoint.get_z() != point.get_z())
  {
    std::cout << "Error in 'equal_operator_overload_test'" << std::endl;
    return false;
  }

  return true;
}


/* Test the equal operator with pntvec objects with multiple objects.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool multi_equal_operator_test ()
{
  pntvec point (2, 3, 4);
  pntvec point2;
  pntvec point3;

  point2 = point3 = point;
  
  if (point3.get_x() != point.get_x() || point3.get_y() != point.get_y() || point3.get_z() != point.get_z())
  {
    std::cout << "Error in 'multi_equal_operator_test'" << std::endl;
    return false;
  }

  if (point2.get_x() != point.get_x() || point2.get_y() != point.get_y() || point2.get_z() != point.get_z())
  {
    std::cout << "Error in 'multi_equal_operator_test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Tests the addition operator.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool addition_operator_overload_test ()
{
  pntvec point1 (2, 3, 4);
  pntvec point2 (1, 2, 3);
  pntvec sumofPoints = point1 + point2;

  if (sumofPoints.get_x() != 3 || sumofPoints.get_y() != 5 || sumofPoints.get_z() != 7)
  {
    std::cout << "Error in 'addition_operator_overload_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the additon operator with negative pntvec object
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool adding_from_negative_test ()
{
  pntvec point (-2, -4, -6);
  pntvec point2 (2, 4, 6);
  pntvec sumOfpoints = point + point2;

  if (sumOfpoints.get_x() != 0 || sumOfpoints.get_y() != 0 || sumOfpoints.get_z() != 0)
  {
    std::cout << "Error in 'adding_from_negative_test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Tests the addition operator with negative to positive summation.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool adding_from_negative_to_positive_test ()
{
  pntvec point (-2, -4, -6);
  pntvec point2 (3, 5, 7);
  pntvec sumOfpoints = point + point2;

  if (sumOfpoints.get_x() != 1.0 || sumOfpoints.get_y() != 1.0 || sumOfpoints.get_z() != 1.0)
  {
    std::cout << "adding_from_negative_to_positive_test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Tests the subtraction operator overload for pntvec objects.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool subtraction_operator_overload_test ()
{
  pntvec point1 (2, 3, 4);
  pntvec point2 (1, 2, 3);
  pntvec diffofPoints = point1 - point2;

  if (diffofPoints.get_x() != 1 || diffofPoints.get_y() != 1 || diffofPoints.get_z() != 1)
  {
    std::cout << "Error in 'subtraction_operator_overload_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests subtracting from a negative point.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool subtracting_from_negative_point_test ()
{
  pntvec point1 (-2, -3, -4);
  pntvec point2 (1, 2, 3);
  pntvec diffofPoints = point1 - point2;

  if (diffofPoints.get_x() != -3.0 || diffofPoints.get_y() != -5.0 || diffofPoints.get_z() != -7.0)
  {
    std::cout << "subtracting_from_negative_point_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests subtracting two negative points.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool double_negative_subtraction_test ()
{
  pntvec point1 (-2, -3, -4);
  pntvec point2 (-1, -2, -3);
  pntvec diffofPoints = point1 - point2;

  if (diffofPoints.get_x() != -1.0 || diffofPoints.get_y() != -1.0 || diffofPoints.get_z() != -1.0)
  {
    std::cout << "double_negative_subtraction_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the multiplication operator overload for pntvec objects.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool multiplication_operator_overload_test ()
{
  pntvec point1 (2, 3, 4);
  double scalar = 5;
  pntvec scaledPoint = point1 * scalar;

  if (scaledPoint.get_x() != 10 || scaledPoint.get_y() != 15 || scaledPoint.get_z() != 20)
  {
    std::cout << "Error in 'multiplication_operator_overload_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests negative scalar multiplication
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool negative_scalar_multiplication_test ()
{
  pntvec point1 (2, 3, 4);
  double scalar = -5;
  pntvec scaledPoint = point1 * scalar;

  if (scaledPoint.get_x() != -10.0 || scaledPoint.get_y() != -15.0 || scaledPoint.get_z() != -20.0)
  {
    std::cout << "negative_scalar_multiplication_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests multiplying by zero.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool zero_scalar_multiplication_test ()
{
  pntvec point1 (2, 3, 4);
  double scalar = 0;
  pntvec scaledPoint = point1 * scalar;

  if (scaledPoint.get_x() != 0 || scaledPoint.get_y() != 0 || scaledPoint.get_z() != 0)
  {
    std::cout << "zero_scalar_multiplication_test'" << std::endl;
    return false;
  }

  return true;
}


/*Positive and negative multiplication tested in succession.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool positive_then_negative_multiplication_test()
{
  pntvec point1 (2, 3, 4);
  double scalar = 5;
  double negScale = -1;
  pntvec scaledPoint = point1 * scalar;

  if (scaledPoint.get_x() != 10.0 || scaledPoint.get_y() != 15.0 || scaledPoint.get_z() != 20.0)
  {
    std::cout << "positive_then_negative_multiplication_test'" << std::endl;
    return false;
  }

  scaledPoint = scaledPoint * negScale;
  if (scaledPoint.get_x() != -10.0 || scaledPoint.get_y() != -15.0 || scaledPoint.get_z() != -20.0)
  {
    std::cout << "positive_then_negative_multiplication_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests negating a pntvec object.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool negation_operator_overload_test ()
{
  pntvec point(1, 2, 3);
  pntvec negatedPoint = -point;

  if (negatedPoint.get_x() != -1 || negatedPoint.get_y() != -2 || negatedPoint.get_z() != -3)
  {
    std::cout << "Error in 'negation_operator_overload_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests negating a negative pntvec object.
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool negating_a_negative_point_test ()
{
  pntvec point (-1, -2, -3);
  pntvec negatedPoint = -point;

  if (negatedPoint.get_x() != 1.0 || negatedPoint.get_y() != 2.0 || negatedPoint.get_z() != 3.0)
  {
    std::cout << "negating_a_negative_point_test'" << std::endl;
    return false;
  }

  return true;
}


/* Output stream << operator tested
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool output_stream_overload_test()
{ 
  pntvec point(1, 0, 4);
  
  std::stringstream out;
  out << point;

  if (out.str() != "(1, 0, 4)")
  {
    std::cout << "Error in 'output_stream_overload_test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Input stream >> operator tested
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool input_stream_overload_test()
{
  std::stringstream in;
  pntvec point;

  in.str("1.0 2.0 3.0");

  in >> point;

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in 'input_stream_overload_test'" << std::endl;
    return false;
  }

  return true;
}


/* Checking const variables test
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool constant_addition_test ()
{
  pntvec point (1, 2, 3);
  pntvec point2 (2, 3, 4);
  pntvec summedPoint = point + point2;

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in 'constant_addition_test'" << std::endl;
    return false;
  }

  if (point2.get_x() != 2.0 || point2.get_y() != 3.0 || point2.get_z() != 4.0)
  {
    std::cout << "Error in 'constant_addition_test'" << std::endl;
    return false;
  }

  return true;
}


/* Checking const variables test
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool constant_subtraction_test ()
{
  pntvec point (1, 2, 3);
  pntvec point2 (2, 3, 4);
  pntvec diffPoint = point - point2;

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in 'constant_substraction_test'" << std::endl;
    return false;
  }

  if (point2.get_x() != 2.0 || point2.get_y() != 3.0 || point2.get_z() != 4.0)
  {
    std::cout << "Error in 'constant_substraction_test'" << std::endl;
    return false;
  }

  return true;
}


/* Checking const variables test
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool constant_multiplication_test ()
{
  pntvec point (1, 2, 3);
  double scale = 10;
  pntvec diffPoint = point * scale;

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in 'constant_multiplication_test'" << std::endl;
    return false;
  }

  if (scale != 10.0)
  {
    std::cout << "Error in 'constant_multiplication_test'" << std::endl;
    return false;
  }

  return true;
}


/* Checking const variables test
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool constant_negation_test ()
{
  pntvec point (1, 2, 3);
  pntvec negatedPoint = -point;

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in 'constant_negation_test'" << std::endl;
    return false;
  }

  return true;
}


/* Checks whitespace parsing in the input stream.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool whitespace_input_test ()
{
  std::stringstream in;
  pntvec point;

  in.str("1.0   2.0   3.0");

  in >> point;

  if (point.get_x() != 1.0 || point.get_y() != 2.0 || point.get_z() != 3.0)
  {
    std::cout << "Error in 'whitespace_input_test'" << std::endl;
    return false;
  }

  return true;
}


/* Checking const variables test
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool constant_output_test()
{ 
  pntvec point(1, 0, 4);
  
  std::stringstream out;
  out << point;

  if (out.str() != "(1, 0, 4)")
  {
    std::cout << "Error in 'output_stream_overload_test'" << std::endl;
    return false;
  }

  if (point.get_x() != 1.0 || point.get_y() != 0 || point.get_z() != 4.0)
  {
    std::cout << "Error in 'constant_output_test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Test to see if the outputted zero is zero in the distance_to method.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool zero_output_distanceTo_test ()
{
  pntvec point (1, 2, 3);
  pntvec point2 (1, 2, 3);

  double distance = point.distance_to(point2);

  if (distance != 0.0)
  {
    std::cout << "Error in 'zero_output_distanceTo_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests Negative input distance_to test
 *
 *
 * Param: N/A
 *
 * Return: True if passes, False, if fails
 */
bool negative_input_distanceTo_test ()
{
  pntvec point1 (1, 1, 1);
  pntvec point2 (-2, -2, -2);
  
  double MarginOfError = 0.0001;

  long expectedResult = 5.19615242270663188058233902451;
  double actualResult = point1.distance_to(point2);

  if (!((expectedResult - actualResult) < MarginOfError))
  {
    std::cout << "Error in 'negative_input_distanceTo_test'" << std::endl;
    return false;
  }

  return true;
}


/* Tests the chaining of addition and subtraction operators.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool addition_subtraction_chaining_test ()
{
  pntvec point1 (1, 2, 3);
  pntvec point2 (1, 2, 3);

  pntvec chainPoint = point1 + point2 - point1 -point2;

  if (chainPoint.get_x() != 0 || chainPoint.get_y() != 0 || chainPoint.get_z() != 0)
  {
    std::cout << "Error in 'operator_chaining_test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Tests the chaining of addition, subtraction, and multiplication operators.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool multiplication_addition_subtraction_chaining_test ()
{
  pntvec point1 (1, 2, 3);
  pntvec point2 (1, 2, 3);
  double scale = 5;

  pntvec chainPoint = point1 + point2 - point1 * scale;

  if (chainPoint.get_x() != -3.0 || chainPoint.get_y() != -6.0 || chainPoint.get_z() != -9.0)
  {
    std::cout << "multiplication_addition_subtraction_chaining_test'" << std::endl;
    return false;
  }
  
  return true;
}


/* operator chaining with the distance_to method.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool operator_chaining_with_distanceTo_test ()
{
  pntvec point1 (1, 2, 3);
  pntvec point2 (0, 1, 2);
  double scale = -1;

  double chainPoint = (point1.distance_to(point2)) * -1;

  double marginOfError = 0.0001;
  double actualValue = -1.732050807568877293527446341;

  if (!((actualValue - chainPoint) < marginOfError))
  {
    std::cout << "Error in 'operator_chaining_with_distanceTo_test'" << std::endl;
    return false;
  }
  
  return true;
}


/* Tests the output stream chaining.
 *
 *
 * Param:N/A
 *
 * Return: True if passes, False, if fails
 */
bool output_stream_chaining_test ()
{
  pntvec point1 (1, 2, 3);
  pntvec point2 (4, 5, 6);

  std::stringstream out;

  out << point1 << " " << point2;

  if (out.str() != "(1, 2, 3) (4, 5, 6)")
  {
    std::cout << "Error in 'output_stream_chaining_test'" << std::endl;
    return false;
  }

  return true;
}
