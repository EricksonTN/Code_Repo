/* A 'string set' is defined as a set of strings stored
 * in sorted order in a drop list.  See the class video
 * for details.
 *
 * For lists that do not exceed 2 ^ (max_width + 1)
 * elements, the add, remove, and contains functions are 
 * O(lg size) on average.  The operator= and get_elements 
 * functions are O(size).   
 * 
 * Peter Jensen
 * February 12, 2021
 */

#include "string_set.h"
#include <iostream>  // For debugging, if needed.
#include <cstdlib>
#include <time.h>

namespace cs3505
{
  /*******************************************************
   * string_set member function definitions
   ***************************************************** */
  
  /** Constructor:  The parameter indicates the maximum
    * width of the next pointers in drop list nodes.
    */
  string_set::string_set(int max_width)
  {
    srand(time(NULL));

    head = new node("", max_width);

    this->max_width = max_width;
    this->size = 0;

    for (int i = 0; i < max_width; i++)
    {
      head->next[i] = NULL;
    }
  }

  
  /** Copy constructor:  Initialize this set
    * to contain exactly the same elements
    * as another set.
    */
  string_set::string_set (const string_set &other)
  {

    srand(time(NULL));

    head = new node("", other.max_width);
    this->max_width = other.max_width;
    this->size = 0;
    for (int i = 0; i < max_width; i++)
    {
      head->next[i] = NULL;
    }

    copySet(other);
  }


  /** Destructor:  Release any memory allocated
    * for this object.
    */
  string_set::~string_set()
  {
    clean();
  }


  /* Additional public and private helper function definitions needed here */


  /* Adds the new node into the list
   *
   *Param: reference string of data to insert
   *
   */
  void string_set::add (const std::string &data)
  {
    if (contains(data))
    {
      return;
    }
    
    std::vector<node *> location = listSearch(data);
    int coinflip, newWidth;

    newWidth = 1;

    while (true)
    {
      coinflip = rand() % 10 + 1;

      if ((coinflip % 2) == 0)
      {
	break;
      }

      if (newWidth == max_width)
      {
	break;
      }

      newWidth++;
    }

    node *newNode = new node(data, newWidth);
    node *nextNode;

    for (int i = 0; i < newWidth; i++)
    {
      if (location[i]->next[i] == NULL)
      {
	nextNode = NULL;
      }
      else
      {
	nextNode = location[i]->next[i];
      }

      location[i]->next[i] = newNode;
      newNode->next[i] = nextNode;
    }
    
    size++;
    
    std::cout << "this is the width of the node: " << location[0]->next[0]->data << " " << location[0]->next[0]->node_width << std::endl;
  }          


  /* Removes specified data and node from the list.
   *
   *Param: reference string data of what should be removed.
   *
   */
  void string_set::remove (const std::string &data)
  {
    if (!contains(data))
    {
      return;
    }

    std::vector<node *> location = listSearch(data);
    node *deletedNode = location[0]->next[0];
    for (int i = 0; i < deletedNode->node_width; i++)
    {
      node *deletedNext = deletedNode->next[i];
      location[i]->next[i] = deletedNext;
    }

    delete deletedNode;
    size--;
  }         


  /* Checks to see if the list contains a certain string
   *
   *Param: Reference string that is check if list contains
   *
   */
  bool string_set::contains (const std::string &data) const
  {
    std::vector<node *> location = listSearch(data);

    if (location[0]->next[0] == NULL)
    {
      return false;
    }

    if (location[0]->next[0]->data == data)
    {
      return true;
    }

    return false;
  }   

  int  string_set::get_size () const
  {
    return size;
  }                           


  /* Operator overload that sets the lhs to the rhs's data
   *
   * param: Reference string_set that will be copied over.
   *
   */
  string_set & string_set::operator= (const string_set &rhs)
  {
    clean();

    this->max_width = rhs.max_width;
    head = new node("", max_width);

    std::vector<std::string> rhs_contents = rhs.get_elements();
    
    for (int i = 0; i < rhs_contents.size(); i++)
    {
      add(rhs_contents[i]);
    }

    return *this;
  }   



  /* Gets the elements out of the list in ascending order.
   *
   *Return: string vector that contains all the elements of the list in ascending order.
   *
   */
  std::vector<std::string> string_set::get_elements() const
  {
    std::vector<std::string> elementList;
    elementList.resize(size);

    node *current = head->next[0];

    for (int i = 0; i < size; i++)
    {
      elementList[i] = current->data;
      current = current->next[0];
    }

    return elementList;
  }



  /* Used in the destructor to clean the string_set prior to deletion. deletes all
   * of the nodes in the list and set's head's next array values to NULL
   *
   *
   */
  void string_set::clean ()
  {
    node *prior = head->next[0];
    node *current = head->next[0];

    for(int i = 1; i <= size; i++)
    {
      if (prior == NULL)
      {
	break;
      }

      current = current->next[0];
      delete prior;
      
      prior = current;
    }

    this->size = 0;

    delete head;
    
    head = NULL;
  }




  /* traverses the list in O(lg N) time. Finds all the positions in the list prior to insertion.
   *
   *Param: string reference of the new data to be inserted
   *
   *Return: Returns a vector of node pointers all pointing to the positions prior to insertion slot.
   *
   */
  std::vector<node *> string_set::listSearch (const std::string & new_data) const
  {
    std::vector<node *> prior;
    prior.resize(max_width);

    node *current;
    current = head;
    
    for (int i = max_width - 1; i >= 0; i--)
    {

      while (true)
      {
	if (current->next[i] == NULL)
	{
	  prior[i] = current;
	  break;
	}
	  
	if (current->next[i]->data >=  new_data)
	{
	  prior[i] = current;
	  break;
	}	  

	if (current->next[i]->data < new_data)
	{
	  current = current->next[i];
	}
      }

    }    
    
    return prior;
  }

  /* Copies the set's data over to this
   *
   *Param: reference to a string set that will be copied.
   *
   */
 void string_set::copySet (const string_set & set)
  { 
    std::vector<std::string> copiedElements = set.get_elements();
    
    for (int i = 0; i < set.size; i++)
    {
      add(copiedElements[i]);
    }
  }

}
