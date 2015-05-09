// Cache.cpp
// Define the actual Cache memory simulation class
// Last Modified: 4-17 By: Chad

#include "Cache.h"
#include "HelperFunctions.h"

Cache::Cache()
{
	l1Misses = 0;
	l1Hits = 0;
	l2Misses = 0;
	l2Hits = 0;
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
	unsigned int blockAddress = decimalReference % (traceParser.GetCacheSize("l1") * traceParser.GetCacheSize("l1"));


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
	if(!traceParser.ParseArguments(argumentCount, arguments))
	{
		std::cout << "ERROR: sizeL1 must be power of two, and sizeL2 must be power of two and larger than sizeL1" << std::endl;
		return false;
	}
	traceParser.ParseFile();

	return true;
}

// Run the actual CacheSimulation
void Cache::RunSimulation()
{
	if (traceParser.GetCacheType() == "direct")
	{
		DirectSimulation();
	}
}

// run a Direct cache simulation
void Cache::DirectSimulation()
{
	// we we still have something to parse
	while (!traceParser.IsEmpty())
	{
		// grab the next memory block
		CacheMemory memoryBlock = GetMemoryReference();

		// this memory block is a write
		if (memoryBlock.readWrite == 0)
		{
			if (traceParser.GetCacheSize("l1") > l1cacheMap.size())
			{
				l1cacheMap[memoryBlock.blockAddress] = memoryBlock.tag;
			}
			else
			{
				l2cacheMap[memoryBlock.blockAddress] = memoryBlock.tag;
			}
			// check to see if we can find it in the l1 cache
			auto l1Find = l1cacheMap.find(memoryBlock.blockAddress);
			// check to see if we can find it in the l2 cache
			auto l2Find = l2cacheMap.find(memoryBlock.blockAddress);

			// wasn't in the l1 cache map, check in l2
			if (l1Find == l1cacheMap.end())
			{
				if (l2Find == l2cacheMap.end())
				{
					l1Misses++;
					l2Misses++;
				}
				else
				{
					l2Hits++;
				}
			}
			// found it, HIT
			else
			{
				l1Hits++;
			}
		}

		// read
		else
		{
			// check to see if we can find it in the l1 cache
			auto l1Find = l1cacheMap.find(memoryBlock.blockAddress);
			// check to see if we can find it in the l2 cache
			auto l2Find = l2cacheMap.find(memoryBlock.blockAddress);
			// couldn't find it in the l1 cache
			if (l1Find == l1cacheMap.end())
			{
				// couldn't find it in l2 cache
				if (l2Find == l2cacheMap.end())
				{
					// missed in both l1 and l2
					l1Misses++;
					l2Misses++;
				}
				// found it in l2 cache, add a hit
				else
				{
					l2Hits++;
				}
			}
			else
			{
				l1Hits++;
			}

		}
	}
}

// Print out the simulation results
void Cache::PrintDebug()
{
	traceParser.PrintDebug();

	// gets the numbers of hits as a percentage
	float l1HitPercentage = (static_cast<float>(l1Misses) / static_cast<float>(l1Hits));
	l1HitPercentage *= 100.0f;
	// get the number of misses as a percentage
	float l1MissPercentage = 100.0f - l1HitPercentage;

	// gets the numbers of hits as a percentage
	float l2HitPercentage = (static_cast<float>(l2Misses) / static_cast<float>(l2Hits));
	l2HitPercentage *= 100.0f;
	// get the number of misses as a percentage
	float l2MissPercentage = 100.0f - l2HitPercentage;
	
	std::cout << "Simulation Execution" << std::endl;
	std::cout << "\t L1: " << l1HitPercentage << "% hits" << std::endl;
	std::cout << "\t L1: " << l1MissPercentage << "% misses" << std::endl;
	std::cout << "\t L2: " << l2HitPercentage << "% hits" << std::endl;
	std::cout << "\t L2: " << l2MissPercentage << "% misses" << std::endl;
}	