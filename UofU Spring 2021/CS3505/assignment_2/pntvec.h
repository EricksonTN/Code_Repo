/* CS3505
 * Author: Erickson Nguyen
 * 2/4/21
 *
 * Declaration of the pntvec class
 */

#ifndef PNTVEC_H
#define PNTVEC_H

#include <iostream>

class pntvec 
{
 private:

  double x; // private instance variable used to contain the x-coordinate
  double y; // private instance variable used to contain the y-coordinate
  double z; // private instance variable used to contain the z-coordinate



  /* Method used to set the pntvec's x-coordinate value
   *
   * Param: double value of the x-coordinate
   */
  void set_x (double x_point);



  /* Method used to set the pntvec's y-coordinate value
   *
   * Param: double value of the y-coordinate
   */
  void set_y (double y_point);



  /* Method used to set the pntvec's z-coordinate value
   *
   * Param: double value of the z-coordinate
   */
  void set_z (double z_point);

 public:



  /* Default constructor of a pntvec. Set's the pntvec's coordinates to (0, 0, 0)
   */
  pntvec ();



  /* Pntvec constructor that takes in three doubles representing the xyz coordinates of the point.
   *
   * Param: 3 doubles representing the x-coordinate value, y-coordinate value, and z-coordinate value.
   */
  pntvec (double x_coord, double y_coord, double z_coord);



  /* Pntvec copy constructor that takes in a pntvec, and copies the coordinates over.
   *
   * Param: pntvec object used to copy over xyz-coordinates
   */
  pntvec (const pntvec & point);



  /* operator overload of the '=' operator. used to set a pntvec object to the right hand side.
   *
   * Param: a pntvec object
   *
   * Return: a pntvec object
   */
  pntvec & operator= (const pntvec & rhs);



  /* operator overload of the '+' operator. used to add pntvec objects together.
   *
   * Param: a pntvec object
   *
   * Return: a pntvec object
   */
  const pntvec operator+ (const pntvec & rhs)const; 



  /* operator overload of the '-' operator. used to subtract pntvec objects.
   *
   * Param: a pntvec object
   *
   * Return: a pntvec object
   */ 
  const pntvec operator- (const pntvec & rhs)const;



  /* operator overload of the '*' operator. used to scale a pntvec object by a double scale value.
   *
   * Param: double scale value
   *
   * Return: a pntvec object
   */
  const pntvec operator* (const double scale)const;



  /* operator overload of the '-' operator. used to negate a pntvec object.
   *
   * Return: a pntvec object
   */
  const pntvec operator- () const;



  /* operator overload of the '<<' operator.
   *
   * Param: ostream object, pntvect object
   *
   * Return: ostream object
   */
  friend std::ostream & operator<< (std::ostream & stream, const pntvec & point);



  /* operator overload of the '>>' operator.
   *
   * Param: istream object, pntvect object
   *
   * Return: istream object
   */  
  friend std::istream & operator>> (std::istream & stream, pntvec & point);



  /* Gets the x value of the pntvec object.
   *
   * Return: x-coordinate value (double)
   */
  double get_x()const;



  /* Gets the y value of the pntvec object.
   *
   * Return: y-coordinate value (double)
   */
  double get_y()const;



  /* Gets the z value of the pntvec object.
   *
   * Return: z-coordinate value (double)
   */
  double get_z()const;



  /* Determines the distance between two points
   *
   * Param: takes in a pntvec object
   *
   * Return: double value representing the distance between two points.
   */
  double distance_to (pntvec& point);
  
};

#endif
