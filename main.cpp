
#include <iostream>
#include "Parser.h"

int main(int argc, char* argv[])
{
	Parser traceParser;
	traceParser.ReadLines();
	if(!traceParser.ParseArguments(argc, argv))
	{
		std::cout << "ERROR: sizeL1 must be power of two, sizeL2 must be power of two and larger than sizeL1" << std::endl;
		return -1;
	}
	traceParser.ParseReadWrites();
	traceParser.ParseMemory();
	traceParser.PrintDebug();

	return 0;
}
