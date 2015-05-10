// Cache.cpp
// Define the actual Cache memory simulation class
// Last Modified: 4-17 By: Chad

#include <algorithm>

#include "Cache.h"
#include "HelperFunctions.h"

Cache::Cache()
{
	l1Misses = 0;
	l1Hits = 0;
	l2Misses = 0;
	l2Hits = 0;
	l1TotalTries = 0;
	l2TotalTries = 0;
	totalReplacements = 0;
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
	if (!traceParser.ParseArguments(argumentCount, arguments))
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
	else
	{
		AssociativeSimulation();
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

		l1TotalTries++;
		// this memory block is a write
		if (memoryBlock.readWrite == 0)
		{
			// check which cache we need to write it in
			if (traceParser.GetCacheSize("l1") > l1cacheMap.size())
			{
				l1cacheMap.insert(std::pair<int, std::string>(memoryBlock.blockAddress, memoryBlock.tag));
				l1Hits++;
			}
			else
			{
				l2cacheMap.insert(std::pair<int, std::string>(memoryBlock.blockAddress, memoryBlock.tag));
				l2Hits++;
				l2TotalTries++;
			}
		}

		// read
		else
		{
			// check to see if this tag is in the l1 cache
			//auto l1Find = l1cacheMap.find(memoryBlock.blockAddress); // CSCE machines g++ compiler doesn't support auto keyword
			std::map<int, std::string>::const_iterator l1Find = l1cacheMap.find(memoryBlock.blockAddress);
			if (l1Find != l1cacheMap.end())
			{
				// found it, increase l1 hits and continue onto next iteration of loop
				l1Hits++;
			}
			// not in l1, check l2 now
			else
			{
				l1Misses++;
				//auto l2Find = l2cacheMap.find(memoryBlock.blockAddress); // CSCE machines do not support auto keyword
				std::map<int, std::string>::const_iterator l2Find = l2cacheMap.find(memoryBlock.blockAddress);
				if (l2Find != l2cacheMap.end())
				{
					// found it in l2, 
					l2Hits++;
					l2TotalTries++;
				}
				else
				{
					l2Misses++;
					l2TotalTries++;
				}
			}
		}
	}
}

void Cache::AssociativeSimulation()
{
	// if the last recently used cache was l1
	// used for replacement algorithim
	bool l1RecentlyUsed = false;

	// something to parse
	while (!traceParser.IsEmpty())
	{
		// grab next memory block
		CacheMemory tempMemory = GetMemoryReference();

		// we always try l1 first, so increase number of tries for it
		l1TotalTries++;

		if (tempMemory.readWrite == 0)
		{
			if (l1AssociativeCache.size() < traceParser.GetCacheSize("l1"))
			{
				l1AssociativeCache.push_front(tempMemory.tag);
				l1Hits++;
			}
			else if (l2AssociativeCache.size() < traceParser.GetCacheSize("l2"))
			{
				l2AssociativeCache.push_front(tempMemory.tag);
				l2Hits++;
				l2TotalTries++;
			}
			// both caches full, replace
			// pop from back of l2, push to front of l2
			else
			{
				totalReplacements++;
				// replace in l1
				if (!l1RecentlyUsed)
				{
					l1AssociativeCache.pop_back();
					l1AssociativeCache.push_front(tempMemory.tag);
					// use l2 next time
					l1RecentlyUsed = true;
				}
				// replace in l2
				else
				{
					l2AssociativeCache.pop_back();
					l2AssociativeCache.push_front(tempMemory.tag);
					// use l1 next time
					l1RecentlyUsed = false;
				}
			}
		}
		// we have a read
		else
		{
			// see if it is in the l1 cache
			bool l1Found = std::find(l1AssociativeCache.begin(), l1AssociativeCache.end(), tempMemory.tag) != l1AssociativeCache.end();
			if (!l1Found)
			{
				l1Misses++;
				// check l2
				bool l2Found = std::find(l2AssociativeCache.begin(), l2AssociativeCache.end(), tempMemory.tag) != l2AssociativeCache.end();
				if (!l2Found)
				{
					l2Misses++;
					l2TotalTries++;
				}
				else
				{
					l2Hits++;
					l2TotalTries++;
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
	float l1HitPercentage = 0.0f;
	float l1MissPercentage = 0.0f;
	float l2HitPercentage = 0.0f;
	float l2MissPercentage = 0.0f;

	// calculate l1 percentages
	l1HitPercentage = (static_cast<float>(l1Hits) / static_cast<float>(l1TotalTries));
	l1HitPercentage *= 100.0f;
	l1MissPercentage = (static_cast<float>(l1Misses) / static_cast<float>(l1TotalTries));
	l1MissPercentage *= 100.0f;
	// calculate l2 percentages
	l2HitPercentage = (static_cast<float>(l2Hits) / static_cast<float>(l2TotalTries));
	l2HitPercentage *= 100.0f;
	l2MissPercentage = (static_cast<float>(l2Misses) / static_cast<float>(l2TotalTries));
	l2MissPercentage *= 100.0f;
	
	std::cout << "DEBUG ONLY: " << std::endl;
	std::cout << "L1 Hits: " << l1Hits << std::endl;
	std::cout << "L2 Hits: " << l2Hits << std::endl;
	std::cout << "L1 Misses: " << l1Misses << std::endl;
	std::cout << "L2 Misses: " << l2Misses << std::endl;
	std::cout << "L1 Total Tries: " << l1TotalTries << std::endl;
	std::cout << "L2 Total Tries: " << l2TotalTries << std::endl;

	std::cout << "Simulation Execution" << std::endl;
	std::cout << "\t L1: " << l1HitPercentage << "% hits" << std::endl;
	std::cout << "\t L1: " << l1MissPercentage << "% misses" << std::endl;
	std::cout << "\t L2: " << l2HitPercentage << "% hits" << std::endl;
	std::cout << "\t L2: " << l2MissPercentage << "% misses" << std::endl;
	std::cout << "\n\t Replacements: " << totalReplacements << std::endl;
}