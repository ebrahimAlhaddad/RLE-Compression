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

TEST_CASE("RLE Compression", "[student]")
{
    SECTION("Temp test run")
    {
        char test[] = "\x00" "\x00""\x00""\x00""\x00""\x00""\x00""\x00""\x00""\x00""\x00"
        "\x00""\x00""\x00""\x00""\x00""\x00""\x00""\x00""\x00";
        char expected[] = "\x14" "\x00";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    SECTION("Basic positive run")
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
		"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		bool result = RunCompressionTest(test, sizeof(test) - 1,
										 expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
	
	// TODO: Add more test case sections!!
    SECTION("Long positive run")
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        //96 characters of 'a'
        char expected[] = "\x60" "a";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
   SECTION("Long negative run")
    {
        char test[] = "abcdefghijklmnopqrstuvwxyz";
        //-26 characters
        char expected[] = "\xe6" "abcdefghijklmnopqrstuvwxyz";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    SECTION("Alternating run")
    {
        char test[] = "aaaaaabcdeeeeeefghhhhhhijklmnnnnnn";
        char expected[] = "\x06" "a" "\xfd" "bcd" "\x06" "e"
                        "\xfe" "fg" "\x06" "h" "\xfb" "ijklm" "\x06" "n";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
        
    }
    SECTION("Very long positive run")
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        //170 characters of a over max value of 127
        
        char expected[] = "\x7f" "a" "\x2b" "a";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    SECTION("Very long negative run")
    {
        char test[] = "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz";
        //208 characters of a over max value of 128
        
        char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" "\xaf" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
      SECTION("Very long positive alternating run")
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdeeeeeeeeeeeeee";
        //170 characters of a over max value of 127
        
        char expected[] = "\x7f" "a" "\x2b" "a" "\xfd" "bcd" "\x0e" "e";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    SECTION("Very long negative alternating run")
    {
        char test[] = "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "aaaaaaaaaaaaaaaaaaa";
        //208 characters of a over max value of 128
        
        char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" "\xaf" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
            "\x13" "a";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    SECTION("Very long negative and positive alternating run")
    {
        char test[] = "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        //208 characters of a over max value of 128
        
        char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" "\xaf" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
            "\x7f" "a" "\x2b" "a";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
}

TEST_CASE("RLE Decompression", "[student]")
{
	SECTION("Basic positive run")
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		
		bool result = RunDecompressionTest(test, sizeof(test) - 1,
										   expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
	
	// TODO: Add more test case sections!!
    SECTION("Basic negative run")
    {
        char test[] = "\xfb" "abcde";
        char expected[] = "abcde";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }


SECTION("Alternating run")
{
    char test[] = "\x03" "n" "\xfb" "abcde";
    char expected[] = "nnnabcde";
    
    bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                       expected, sizeof(expected) - 1);
    REQUIRE(result);
}
   
    SECTION("Very long positive run")
    {
        char test[] = "\x7f" "a" "\x2b" "a";
        //170 characters of a over max value of 127
        
        char expected[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    SECTION("Very long negative and positive alternating run")
    {
        char test[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" "\xaf" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "\x7f" "a" "\x2b" "a";
       
        //208 characters of a over max value of 128
        
        char expected[] =  "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    SECTION("Very long negative alternating run")
    {
        char test[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" "\xaf" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "\x13" "a";
        
        //208 characters of a over max value of 128
        
        char expected[] = "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "aaaaaaaaaaaaaaaaaaa";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
}


TEST_CASE("Command line arguments", "[student]")
{
	SECTION("instructions.txt.rl1")
	{
		const char* argv[] = {
			"tests/tests",
			"data/instructions.txt.rl1"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckFileMD5("data/instructions.txt", "5e042d2e4bf014e714dd7cfc4dc25aab");
		REQUIRE(result);
	}
}

TEST_CASE("File compression", "[student]")
{
	RleFile r;
	SECTION("rle.bmp")
	{
		r.CreateArchive("data/rle.bmp");
		bool result = CheckFileMD5("data/rle.bmp.rl1", "f2a9d8425d53c664e45d9eb1b53137b9");
		REQUIRE(result);
	}
	SECTION("pic.jpg")
	{
		r.CreateArchive("data/pic.jpg");
		bool result = CheckFileMD5("data/pic.jpg.rl1", "0bbf2a5109b30d79939d2061ea8c74aa");
		REQUIRE(result);
	}
	SECTION("Conquest.ogg")
	{
		r.CreateArchive("data/Conquest.ogg");
		bool result = CheckFileMD5("data/Conquest.ogg.rl1", "ec29ff368ec5100bfba22635ddc5ba5c");
		REQUIRE(result);
	}
}

TEST_CASE("File decompression", "[student]")
{
	RleFile r;
   
	SECTION("works.bmp.rl1")
	{
		r.ExtractArchive("data/works.bmp.rl1");
		bool result = CheckFileMD5("data/works.bmp", "8baad647acefae2f8c36ee111582a875");
		REQUIRE(result);
	}
	SECTION("xkcd.bmp.rl1")
	{
		r.ExtractArchive("data/xkcd.bmp.rl1");
		bool result = CheckFileMD5("data/xkcd.bmp", "a4d7efa89d47a94a76f8050dd8d60cd0");
		REQUIRE(result);
	}
	SECTION("logo.png.rl1")
	{
		r.ExtractArchive("data/logo.png.rl1");
		bool result = CheckFileMD5("data/logo.png", "95403309460632f60efa647ef59b78fc");
		REQUIRE(result);
	}
}

