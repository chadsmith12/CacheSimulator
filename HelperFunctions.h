#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

// ReadArgument - Reads the argument from the command line argument list
// Returns a sring
std::string ReadArgument(int argc, char* argv[], int index);

// ReadLine - Reads the line from the file, and returns line read
std::string ReadLine();

// GetReadWrite - Get whether we need a read/write from the line, returns an int
int GetReadWrite(std::string line);

// GetMemoryReference - Get the memory reference from the line, returns a string
std::string GetMemoryReference(std::string line);

// IsPowerOfTwo - Checks to see if an integer is a power of two
// Returns true if it is, false if it is not
bool IsPowerOfTwo(unsigned int x);


#endif