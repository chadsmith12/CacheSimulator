
#include <iostream>
#include "Cache.h"
#include "HelperFunctions.h"

int main(int argc, char* argv[])
{
	Cache cacheSimulation;
	
	if(!cacheSimulation.InitSimulation(argc, argv))
	{
		return -1;
	}

	cacheSimulation.RunSimulation();
	cacheSimulation.PrintDebug();

	return 0;
}
