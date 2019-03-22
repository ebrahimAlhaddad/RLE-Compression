#include "SrcMain.h"
#include <string>
#include "RleFile.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Process argc and argv, and use to compress/decompress files
    RleFile r;
    if(argc == 2) {
        const std::string fileDirec = argv[1];
        bool isCompressed = fileDirec.find(".rl1");
        if(isCompressed)
            r.ExtractArchive(fileDirec);
        else
            r.CreateArchive(fileDirec);
    }
}

