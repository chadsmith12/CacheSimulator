// Parser.cpp
// Define everything in the Parser Class
// Last Modified: 3-12 By: Chad

#include "Parser.h"
#include "HelperFunctions.h"

Parser::Parser()
{
	totalReads = 0;
	totalWrites = 0;
	cacheType = "";
}

// ReadLines
// Reads in ever line read from the file and stores each line in a vector
void Parser::ReadLines()
{
	// until we reach the end of the file
	for(int i = 0; !std::cin.eof(); i++)
	{
		// Read in a line from the file and then push it onto the vector
		std::string input = ReadLine();
		lines.push_back(input);
	}
}

// ParseMemory
// From every line we have stored in the vector, get the memory references and push it onto the deque
void Parser::ParseMemory()
{
	// from every line in the vector
	for(unsigned int i = 0; i < lines.size(); i++)
	{
		std::string memory = GetMemoryReference(lines[i]);
		// push it onto the deque
		memoryReferences.push_back(memory);
	}
}

// ParseReadWrites
// From every line in the vector, get the read/write value and push it onto the deque
void Parser::ParseReadWrites()
{
	// from every line in the vector
	for(unsigned int i = 0; i < lines.size(); i++)
	{
		int paramter = GetReadWrite(lines[i]);
		readWrites.push_back(paramter);

		// increase the number of reads or writes based on what the parameter is
		// 0 is a write
		if(paramter == 0)
		{
			totalWrites++;
		}
		else
		{
			totalReads++;
		}
	}
}

// ParseArguments
// Takes in how many command line arguments, and the arguments array
// Returns true if the arguments are valid, false if invalid
bool Parser::ParseArguments(int argumentCount, char* arguments[])
{
	// used for the argument for the l1 and l2 cache
	unsigned int l1Cache = 0;
	unsigned int l2Cache = 0;

	std::string temp;

	bool passed;
	
	// get the l1 cache
	temp = ReadArgument(argumentCount, arguments, 2);
	// convert to integer
	std::stringstream(temp) >> l1Cache;
	// return false if the l1Cache is not a power of 2
	if(!IsPowerOfTwo(l1Cache))
	{
		passed = false;
	}
	else
	{
		// add it to the deque
		this->arguments.push_back(l1Cache);
	}

	// get the l2 cache
	temp = ReadArgument(argumentCount, arguments, 4);
	// convert to integer
	std::stringstream(temp) >> l2Cache;

	// must be power of two and larger than l1Cache
	if(!IsPowerOfTwo(l2Cache) && l2Cache <= l1Cache)
	{
		passed = false;
	}
	else
	{
		this->arguments.push_back(l2Cache);
		passed = true;
	}

	cacheType = ReadArgument(argumentCount, arguments, 6);

	return passed;

}

// GetMemoryReference
// Gets the front of the memory deque, pops it from the deque and returns that value
std::string Parser::GetMemory()
{
	std::string reference = memoryReferences.front();

	memoryReferences.pop_front();
	return reference;
}

// GetReadsWrites
// Gets the memory reference read/write that is first on the deque, and returns that value
int Parser::GetReadsWrites()
{
	int temp = readWrites.front();

	readWrites.pop_front();
	return temp;
}

// PrintDebug
// Prints out the debug information that the parser read in
void Parser::PrintDebug()
{
	std::cout << "Input Parameters read:\n";
	std::cout << "\tSizeL1 " << arguments[0] << std::endl;
	std::cout << "\tSizeL2 " << arguments[1] << std::endl;
	std::cout << "\tType " << cacheType << std::endl;
	std::cout << "Memory references read from file\n\t";
	std::cout << (totalReads+totalWrites) << " Total\n\t";
	std::cout << totalReads << " Reads\n\t";
	std::cout << totalWrites << " Writes\n";

}