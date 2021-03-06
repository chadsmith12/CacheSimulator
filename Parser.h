#ifndef PARSER_H_
#define PARSER_H_

// Includes
#include <string>
#include <sstream>
#include <vector>
#include <deque>

// Parser Class - Parses the trace file and gets all the information needed
class Parser
{
public:
	// Constructor - Init everything and get the parser setup
	Parser();

	// ParseFile - Parses the file, reads in memory referneces and read/writes
	void ParseFile();

	// ParseArguments - takes in the command line arguments and puts them onto the deque
	bool ParseArguments(int argumentCount, char* arguments[]);

	// GetMemory - returns the front of the memory deque, and then pops it from the deque
	std::string GetMemory();

	// GetReadWrite - gets the front of the readWrites deque, and then pops it from the deque
	int GetReadsWrites();

	// GetCacheSize - returns the cache size. Returns the size of the sspecific cache argument
	unsigned int GetCacheSize(std::string cache);

	// GetCacheType - returns the type of cache simulation that we are going to run
	// direct or indirect
	std::string GetCacheType();

	// IsEmpty - return if the parser is empty
	bool IsEmpty();

	// PrintDebug - prints out the debug information that the parser read in
	void PrintDebug();
private:
	// memory references from the file, holds the hex values
	std::deque<std::string> memoryReferences;
	// tells if the line is a read or a write to a memory reference
	std::deque<int> readWrites;
	// holds all the command line arguments
	std::deque<int> arguments;
	// hold each line in the file in a vector
	std::vector<std::string> lines;
	
	// how many reads their are
	int totalReads;
	// how many writes their are
	int totalWrites;
	// tells if the cacheType is a direct or indirect type
	std::string cacheType;
	int debugHits;
};

#endif