# CacheSimulator
Cache Simulator for CSCE3600

How to compile:
g++ *.cpp -o cacheSimulator

How to use:
./cacheSimulator -sizeL1 xx -sizeL2 yy -type direct < trace_file
 Where xx must be a power of 2, yy must be a power of 2 AND larger than sizeL1
-type can either be direct or full
trace_file is the trace_file to read (i.e: small_trace.trace)

Features:
Trace Parser:
Reads in how many reads/writes their are
reads in all the memory references
Cache Simulator:
Simulates a direct cache mapping

Code Documentation:
HelperFunctions - Functions to help with common things
Parser.h - parser class declaration.  Holds everything that was read/parsed from the trace file
Parser.cpp - Parser class definition. Defines all the functions in the parser class.


Parser::ReadLines:
Returns: Void
Description: Reads every line from he trace file and stores them in a vector for easy refernecing

Parser::ParseMemory:
Returns: Void
Description: Grabs all the memory references from the trace file.  Pushes all of them into a std::deque<string>

Parser::ParseReadWrites
Returns Void
Description: Grabs all the read/writes from the trace file. A 0 is a write, while a 1 is a read
Pushes them all into a std::deque<int>

Parser::ParseArguments:
Parameters: argumentCount - The number of arguments passed into the program
	    arguments - character pointer array, the arguments passed into the program
Returns: Bool - true if the arguments are valid, false if they are invalid
Description: Reads in L1Cache, and L2Cache.  Both must be a power of two, and LSCache must be greater than L1Cache.
Returns false if those conditions are not met
Reads in the type of cache (direct/full)


Parser::PrintDebug:
Returns: Void
Description: Prints out the debug information that the parser read in
Prints out the command line arguments (sizel1, sizel2, type) and the trace file information (numer of reads, number of writes)

