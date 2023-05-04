#include <iostream>
#include "string_set.h"


void testStringSetAdd();
void testStringSetRemove();
void testStringSetContains();
void testMassAdd();
void testMassRemove();
void testGetSize();
void testGet_contents();
void testOperatorEquals();
void testTwoParamConstructor();
void testCopyConstructor();
void MassAddRemoveAssignment ();
void testOperatorEqualsWithPreviouslyFilledSet ();

int main ()
{

  //testTwoParamConstructor();  //DONE AND WORKS
  
  //testStringSetAdd(); // DONE AND WORKS 

  //testStringSetContains(); //DONE AND WORKS
  
  //testStringSetRemove(); //DONE AND WORKS

  //testMassAdd(); //DONE AND WORKS

  //testMassRemove(); //DONE AND WORKS

  //testGet_contents(); //DONE AND WORKS

  //testGetSize(); //DONE AND WORKS

  //testCopyConstructor(); //DONE AND WORKS

  //testOperatorEquals();

  //MassAddRemoveAssignment();

  testOperatorEqualsWithPreviouslyFilledSet ();

  return 0;
}

void testStringSetAdd()
{
  cs3505::string_set set(4);
  set.add("apple");
  std::cout << "currently at the end of the testStringSetAdd method" << std::endl;
}

void testStringSetRemove()
{
  cs3505::string_set set (5);
  set.add("apple");
  if (set.contains("apple"))
  {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "set currently contains apple" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
  }

  set.remove("apple");
  if (!set.contains("apple"))
  {
    std::cout << "set doesn't contain apple." << std::endl;
  }
}

void testStringSetContains()
{
  cs3505::string_set set(4);
  set.add("apple");
  std::cout << "Adding apple is complete" << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
  if (set.contains("apple"))
  {
    std::cout << "this set contains \"apple\"" << std::endl;
  }
  else
  {
    std::cout << "There's an error" << std::endl;
  }
  if (!set.contains("goodbye"))
  {
    std::cout << "this set doesn't contain \"goodbye\"" << std::endl;
  }


}

void testMassAdd()
{
  cs3505::string_set set (4);
  std::cout << "--------------------------------------------------------------------" << std::endl;
  set.add("apple");
  std::cout << "--------------------------------------------------------------" << std::endl;
  set.add("banana");
  std::cout << "-----------------------------------------------------------------" << std::endl;
  set.add("cranberries");
  std::cout << "-----------------------------------------------------------------" << std::endl;
  set.add("kiwano");
  std::cout << "-----------------------------------------------------------------" << std::endl;
  set.add("peach");
  std::cout << "-----------------------------------------------------------------" << std::endl;
  set.add("mango");
  std::cout << "-----------------------------------------------------------------" << std::endl;
  set.add("pineapple");
  std::cout << "-----------------------------------------------------------------" << std::endl;
  set.add("orange");

}

void testMassRemove()
{
  cs3505::string_set set(6);
  set.add("apple");
  set.add("bananas");
  set.add("cranberries");

  if (set.contains("apple"))
  {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set contains apple" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  } 

  if (set.contains("bananas"))
  {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set contains bananas" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  }

  if (set.contains("cranberries"))
  {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set contains cranberries" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  }

  set.remove("bananas");
  if (!set.contains("bananas"))
  {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set no longer contains bananas" << std::endl;
    std::cout << "the current set's size is: " << set.get_size() << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  }

  set.remove("apple");
  if (!set.contains("apple"))
  { 
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set no longer contains apple" << std::endl;
    std::cout << "the current set's size is: " << set.get_size() << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  }

  set.remove("cranberries");
  if (!set.contains("cranberries"))
  {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set no longer contains cranberries" << std::endl;
    std::cout << "the current set's size is: " << set.get_size() << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  }
}

void testGet_contents()
{
cs3505::string_set set (6);
set.add("apple");
set.add("banana");
set.add("cranberries");
set.add("apricot");

std::vector<std::string> returnedContents = set.get_elements();

std::cout << "-----------------------------------------" << std::endl;
std::cout << "These elements are in the list." << std::endl;
for (int i = 0; i < returnedContents.size(); i++)
{
std::cout << returnedContents[i]<< std::endl;
}

std::cout << "----------------------------------------" << std::endl;

}

void testOperatorEquals ()
{
  cs3505::string_set set (4);
  set.add("apple");
  set.add("bananas");
  set.add("cranberries");

  cs3505::string_set set2 = set;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "set2's size is: " << set2.get_size() << std::endl;
  std::cout << "-------------------------------------" << std::endl;

  if (set2.contains("apple"))
  {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set2 contains apple" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  }

  if (set2.contains("bananas"))
  {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set2 contains bananas" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  }

  if (set2.contains("cranberries"))
  {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "set2 contains cranberries" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
  }

}

void testTwoParamConstructor ()
{
  cs3505::string_set set(5); 
}

void testCopyConstructor ()
{
  cs3505::string_set set (6);
  set.add("apple");
  set.add("banana");
  set.add("cranberries");
  set.add("apricot");

  std::vector<std::string> returnedContents = set.get_elements();

  std::cout << "-----------------------------------------" << std::endl;
  std::cout << "These elements are in the list." << std::endl;
  for (int i = 0; i < returnedContents.size(); i++)
  {
    std::cout << returnedContents[i]<< std::endl;
  }

  std::cout << "----------------------------------------" << std::endl;

  cs3505::string_set copySet(set);

  std::vector<std::string> returnedContents2 = copySet.get_elements();

  std::cout << "-----------------------------------------" << std::endl;
  std::cout << "These elements are in the copied list." << std::endl;
  for (int i = 0; i < returnedContents2.size(); i++)
  {
    std::cout << returnedContents2[i]<< std::endl;
  }

  std::cout << "----------------------------------------" << std::endl;
}

void testGetSize()
{
cs3505::string_set set (7);

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "The set's current size is: " << set.get_size() << std::endl;
  std::cout << "-------------------------------------" << std::endl;

set.add("apple");

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "The set's current size is: " << set.get_size() << std::endl;
  std::cout << "-------------------------------------" << std::endl;

set.add("banana");

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "The set's current size is: " << set.get_size() << std::endl;
  std::cout << "-------------------------------------" << std::endl;

set.add ("cranberries");

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "The set's current size is: " << set.get_size() << std::endl;
  std::cout << "-------------------------------------" << std::endl;

set.add("apricot");

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "The set's current size is: " << set.get_size() << std::endl;
  std::cout << "-------------------------------------" << std::endl;

}

void MassAddRemoveAssignment ()
{

}

void testOperatorEqualsWithPreviouslyFilledSet ()
{
  cs3505::string_set set(9);
  cs3505::string_set set2(4);

  set.add("apples");
  set.add("apricot");
  set.add("cranberries");

  set2.add("bananas");
  set2.add("kiwano");

  set2 = set;

   
  std::cout << "this is set2's size: " << set2.get_size() << std::endl;

  if (set2.contains("apples"))
  {
    std::cout << "set2 contains apples." << std::endl;
  }

  if (set2.contains("apricot"))
  {
    std::cout << "set2 contains apricot." << std::endl;
  }

  if (set2.contains("cranberries"))
  {
    std::cout << "set2 contains cranberries." << std::endl;
  }
}
