/* CS3505
 * Author: Erickson Nguyen
 * 2/4/21
 *
 * Method implementation of the pntvec class.
 * 
 */


#include <iostream>
#include "pntvec.h"
#include <cmath>

/* Default constructor of a pntvec. Set's the pntvec's coordinates to (0, 0, 0)
 */
pntvec::pntvec ()
{
  set_x(0);
  set_y(0);
  set_z(0);
}

/* Pntvec constructor that takes in three doubles representing the xyz coordinates of the point.
 *
 * Param: 3 doubles representing the x-coordinate value, y-coordinate value, and z-coordinate value.
 */
pntvec::pntvec (double x_coord, double y_coord, double z_coord)
{
  set_x(x_coord);
  set_y(y_coord);
  set_z(z_coord);
}

/* Pntvec copy constructor that takes in a pntvec, and copies the coordinates over.
 *
 * Param: pntvec object used to copy over xyz-coordinates
 */
pntvec::pntvec (const pntvec & point)
{
  set_x(point.get_x());
  set_y(point.get_y());
  set_z(point.get_z());
}

/* Method used to set the pntvec's x-coordinate value
 *
 * Param: double value of the x-coordinate
 */
void pntvec::set_x (double x_point)
{
  x = x_point;
}

/* Method used to set the pntvec's y-coordinate value
 *
 * Param: double value of the y-coordinate
 */
void pntvec::set_y (double y_point)
{
  y = y_point;
}

/* Method used to set the pntvec's z-coordinate value
 *
 * Param: double value of the z-coordinate
 */
void pntvec::set_z (double z_point)
{
  z = z_point;
}

/* Gets the x value of the pntvec object.
 *
 * Return: x-coordinate value (double)
 */
double pntvec::get_x () const
{
  return x;
}

/* Gets the y value of the pntvec object.
 *
 * Return: y-coordinate value (double)
 */
double pntvec::get_y () const
{
  return y;
}

/* Gets the z value of the pntvec object.
 *
 * Return: z-coordinate value (double)
 */
double pntvec::get_z () const
{
  return z;
}

/* Determines the distance between two points
 *
 * Param: takes in a pntvec object
 *
 * Return: double value representing the distance between two points.
 */
double pntvec::distance_to (pntvec& point)
{
  double distance;

  double diffX = point.get_x() - x;
  double diffY = point.get_y() - y;
  double diffZ = point.get_z() - z;

  distance = sqrt(pow(diffX, 2) + pow(diffY, 2) + pow(diffZ, 2));
  return distance;
}

/* operator overload of the '=' operator. used to set a pntvec object to the right hand side.
 *
 * Param: a pntvec object
 *
 * Return: a pntvec object
 */
pntvec & pntvec::operator= (const pntvec & rhs)
{
  x = rhs.get_x();
  y = rhs.get_y();
  z = rhs.get_z();
  
  return *this;
}

/* operator overload of the '+' operator. used to add pntvec objects together.
 *
 * Param: a pntvec object
 *
 * Return: a pntvec object
 */
const pntvec pntvec::operator+ (const pntvec & rhs) const
{
  double sumX = x + rhs.get_x();
  double sumY = y + rhs.get_y();
  double sumZ = z + rhs.get_z();

  pntvec result(sumX, sumY, sumZ);
  
  return result;
}    

/* operator overload of the '-' operator. used to subtract pntvec objects.
 *
 * Param: a pntvec object
 *
 * Return: a pntvec object
 */ 
const pntvec pntvec::operator- (const pntvec & rhs) const
{
  double diffX = x - rhs.get_x();
  double diffY = y - rhs.get_y();
  double diffZ = z - rhs.get_z();

  pntvec result(diffX, diffY, diffZ);
  
  return result;
}

/* operator overload of the '*' operator. used to scale a pntvec object by a double scale value.
 *
 * Param: double scale value
 *
 * Return: a pntvec object
 */
const pntvec pntvec::operator* (const double scale) const
{
  double scaledX = x * scale;
  double scaledY = y * scale;
  double scaledZ = z * scale;
  
  pntvec result(scaledX, scaledY, scaledZ);
  
  return result;
}

/* operator overload of the '-' operator. used to negate a pntvec object.
 *
 * Return: a pntvec object
 */
const pntvec pntvec::operator- () const
{
  pntvec result(-x, -y, -z);

  return result;
}

/* operator overload of the '<<' operator.
 *
 * Param: ostream object, pntvect object
 *
 * Return: ostream object
 */
std::ostream & operator<< (std::ostream & stream, const pntvec & point)
{
  stream << "(";
  std::string separator = ", ";

  stream << point.get_x() << separator;
  stream << point.get_y() << separator;
  stream << point.get_z();

  stream << ")";

  return stream;
}

/* operator overload of the '>>' operator.
 *
 * Param: istream object, pntvect object
 *
 * Return: istream object
 */
std::istream & operator>> (std::istream & stream, pntvec & point)
{
  stream >> point.x;
  stream >> point.y;
  stream >> point.z;

  return stream;
}
