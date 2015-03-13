#include "HelperFunctions.h"

std::string ReadArgument(int argc, char* argv[], int index)
{
	// index we need is out of range
	if(index > argc - 1)
	{
		// return just an empty string 
		return "";
	}
	else
	{
		std::string argument = argv[index];
		return argument;
	}
}

std::string ReadLine()
{
	std::string input;
	std::getline(std::cin, input);

	return input;
}

int GetReadWrite(std::string line)
{
	int input;
	std::stringstream(line) >> input;

	return input;
}

std::string GetMemoryReference(std::string line)
{
	line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

	std::string reference = line.substr(1);

	return reference;
}

bool IsPowerOfTwo(unsigned int x)
{
	return ((x != 0) && ((x & (~x + 1)) == x));
}