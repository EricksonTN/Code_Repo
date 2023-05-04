/* This node class is used to build linked lists for the
 * string_set class.  
 *
 * Peter Jensen
 * February 12, 2021
 */

#include "node.h"

// By default, functions are not in a namespace.  They are not in any
// class.  Symbols defined here are globally available.  We need to
// qualify our function names so that you are definining our 
// cs3505::node class functions.  
//
// Note that you can also use the namespace cs3505 { } block, this 
// would eliminate one level of name qualification.  The 
// 'using' statement will not help in this situation.
// 
// Qualify it as shown here for functions: 
//      cs3505::node::functionname, etc.

/*******************************************************
 * node class - member function definitions
 ***************************************************** */

// Students will decide how to implement the constructor, 
// destructor, and any helper methods.



/* Node constructor that set's the data, node_width, and resizes the next vector to the parameter's max_width
 *
 *Param: string input, and an integer max_width of the node.
 *
 */
cs3505::node::node (const std::string & input, int max_width)
{
  this->data = input;
  this->node_width = max_width;
  this->next.resize(max_width);

  for (int i = 0; i < max_width; i++)
  {
    next[i] = NULL;
  }
}


/* Node destructor, set's the next values all to null and empties the data string.
 *
 *
 *
 */
cs3505::node::~node ()
{
  for (int i = 0; i < node_width; i++)
  {
    next[i] = NULL;
  }

  data = "";
}