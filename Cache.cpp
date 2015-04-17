// Cache.cpp
// Define the actual Cache memory simulation class
// Last Modified: 4-17 By: Chad

#include "Cache.h"
#include "HelperFunctions.h"

Cache::Cache()
{
	misses = 0;
	hits = 0;
}

// Returns an actual Cache memory object from the reference in the parser
Cache::CacheMemory Cache::GetMemoryReference()
{
	// create a temporary memory object that will be returned from the data gathered from the parser
	CacheMemory tempMemory;

	// gets the first memory reference on the deque, pop it off
	std::string memoryReference = traceParser.GetMemory();
	// get if the this is a read or write, pop it off deque
	int readWrite = traceParser.GetReadsWrites();

	// convert the memory reference hex value to decimal
	unsigned int decimalReference = ToDecimal(memoryReference);
	// convert the decimal refernece to the tag
	std::string tag = GetTag(decimalReference);

	// calculate the block address
	unsigned int blockAddress = decimalReference % (traceParser.GetCacheSize() * traceParser.GetCacheSize());

	// store this in a cache memory object
	tempMemory.tag = tag;
	tempMemory.blockAddress = blockAddress;
	tempMemory.readWrite = readWrite;

	return tempMemory;
}

// Starts up the simulation and parses the trace file ahead of time
// returns false if the simulation could not be started
bool Cache::InitSimulation(int argumentCount, char* arguments[])
{
	traceParser.ReadLines();
	if(!traceParser.ParseArguments(argumentCount, arguments))
	{
		std::cout << "ERROR: sizeL1 must be power of two, and sizeL2 must be power of two and larger than sizeL1" << std::endl;
		return false;
	}
	traceParser.ParseReadWrites();
	traceParser.ParseMemory();

	return true;
}

// Run the actual CacheSimulation
void Cache::RunSimulation()
{
	// whilew we still have something to parse
	while(!traceParser.IsEmpty())
	{
		// gets the next memory block from the parser
		CacheMemory memoryBlock = GetMemoryReference();

		// if the cache memory map is empty then we know we need to automatically add a miss
		if(cacheMap.empty())
		{
			++misses;
		}

		// if we have a write put this memory in the map and increase the number of hits
		if(memoryBlock.readWrite == 0)
		{
			cacheMap[memoryBlock.blockAddress] = memoryBlock.tag;
			++hits;
		}

		// if we have a read, check to see if it is in the map
		else if(memoryBlock.readWrite == 1)
		{
			if(cacheMap[memoryBlock.blockAddress] != memoryBlock.tag)
			{
				// not in the map, we have a cold miss
				++misses;
			}
			else
			{
				// we have a hit
				++hits;
			}
		}
	}
}

// Print out the simulation results
void Cache::PrintDebug()
{
	traceParser.PrintDebug();

	// gets the numbers of hits as a percentage
	float hitPercentage = ((float)misses / (float)hits);
	hitPercentage *= 100.0f;
	// get the number of misses as a percentage
	float missPercentage = 100.0f - hitPercentage;
	
	std::cout << "Simulation Execution" << std::endl;
	std::cout << "\t " << hitPercentage << "% hits" << std::endl;
	std::cout << "\t " << missPercentage << "% misses" << std::endl;
}