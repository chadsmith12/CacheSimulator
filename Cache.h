#ifndef CACHE_H_
#define CACHE_H_

// Standard C++ Includes
#include <iostream>
#include <string>
#include <map>
#include<list>

// Project Includes
#include "Parser.h"

// Defines a Cache class that holds the mapping for the cache memory
class Cache
{
public:
	// Default constructor for the class object
	Cache();

	// InitSimulation - gets the simulation ready to run by parsing the trace file ahead of time
	bool InitSimulation(int argumentCount, char* arguments[]);
	// Run Simulation - Actually runs the cache memory simulation
	void RunSimulation();
	// PrintDebug - actually prints out the simulation results
	void PrintDebug();

private:
	// CacheMemory struct - defines the memory object for the Cache
	// stores the tag and block address
	struct CacheMemory 
	{
		std::string tag;
		unsigned int blockAddress;
		unsigned int readWrite;
	};
	// hold the actual cache memory map
	// use a std::map
	// key: line
	// value: tag
	std::map<int, std::string> l1cacheMap;
	std::map<int, std::string> l2cacheMap;

	// hold the fully associative cache's
	std::list<std::string> l1AssociativeCache;
	std::list<std::string> l2AssociativeCache;

	// Runs a Direct Cache Simulation
	void DirectSimulation();
	// associative cache
	void AssociativeSimulation();


	// Returns a CacheMemory object that was gathered in from the parser
	CacheMemory GetMemoryReference();

	// hold how many hits and misses the simulation had
	int l1Misses;
	int l1Hits;
	int l2Misses;
	int l2Hits;
	int l1TotalTries;
	int l2TotalTries;

	int totalReplacements;

	Parser traceParser;
};

#endif