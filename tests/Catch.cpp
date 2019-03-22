#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RleData.h"
#include "RleFile.h"
#include "md5.h"

// Helper functions
bool BuffersAreSame(const char* expected, const char* actual, size_t size)
{
	if (actual == nullptr)
	{
		FAIL("Actual data is null.");
		return false;
	}
	
	bool retVal = true;
	
	for (size_t i = 0; i < size; i++)
	{
		if (expected[i] != actual[i])
		{
			retVal = false;
			FAIL("Expected 0x" << std::hex << static_cast<unsigned>(expected[i])
				 << " at index " << std::dec << i << " but saw 0x"
				 << std::hex << static_cast<unsigned>(actual[i])
				 << std::dec);
			break;
		}
	}
	
	return retVal;
}

bool RunCompressionTest(char* test, size_t testSize,
						char* expected, size_t expectedSize)
{
	RleData r;
	r.Compress(test, testSize);
	return BuffersAreSame(expected, r.mData, expectedSize);
}

bool RunDecompressionTest(char* test, size_t testSize,
						  char* expected, size_t expectedSize)
{
	RleData r;
	r.Decompress(test, testSize, expectedSize);
	return BuffersAreSame(expected, r.mData, expectedSize);
}

bool CheckFileMD5(const std::string& fileName, const std::string& expected)
{
	MD5 md5;
	std::string hash = md5.digestFile(fileName.c_str());
	return hash == expected;
}
