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

// Returns a unsigned integer of the hex value
unsigned int ToDecimal(const std::string& hexString)
{
	unsigned int decimal;
	std::stringstream ss;
	ss << std::hex << hexString;
	ss >> decimal;

	return decimal;
}

// Returns the binary of the string value given in
std::string ToBinary(unsigned int decimal)
{
	std::string binary = std::bitset<32>(decimal).to_string();

	return binary;
}

// Get the Binary string representation of the tag from the original decimal address passed in
std::string GetTag(unsigned int decimal)
{
	unsigned int tag = decimal>>15;

	std::string binaryTag = std::bitset<17>(tag).to_string();

	return binaryTag;
}