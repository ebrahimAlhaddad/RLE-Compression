#include "catch.hpp"
#include "RleData.h"
#include "RleFile.h"
#include "SrcMain.h"

// Helper function declarations (don't change these)
extern bool BuffersAreSame(const char* expected, const char* actual, size_t size);
extern bool RunCompressionTest(char* test, size_t testSize,
						char* expected, size_t expectedSize);
extern bool RunDecompressionTest(char* test, size_t testSize,
						  char* expected, size_t expectedSize);
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
