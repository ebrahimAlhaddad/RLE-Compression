#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

void RleFile::CreateArchive(const std::string& source)
{
	// TODO
    
    // Requires <fstream>
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        memblock = new char[static_cast<unsigned int>(size)]; file.seekg(0, std::ios::beg);
        // Seek back to start of file
        file.read(memblock, size);
        file.close();
        
        // File data is now in memblock array
        // (Do something with it here...)
        mRleData.Compress(memblock, size);
        
        // Make sure to clean up!
        delete[] memblock;
    }
    
    std::string outName = const_cast<std::string&>(source);
    mHeader.mFileName = outName.append(".rl1");//might need to remove append:file name is original only
    mHeader.mFileSize = size;
    mHeader.mFileNameLength = source.length();
    std::ofstream arc(mHeader.mFileName, std::ios::out|std::ios::binary|std::ios::trunc);
    if (arc.is_open())
    {
        // Use arc.write function to write data here
        arc.write(mHeader.mSig,4);
        arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)), 4);
        arc.write(reinterpret_cast<char*>(&(mHeader.mFileNameLength)),1);
        arc.write(mHeader.mFileName.c_str(),mHeader.mFileNameLength);
        arc.write(mRleData.mData,mRleData.mSize);
    }

}

void RleFile::ExtractArchive(const std::string& source)
{
	// TODO
    // Requires <fstream>
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        memblock = new char[static_cast<unsigned int>(size)]; file.seekg(0, std::ios::beg);
        // Seek back to start of file
        file.read(memblock, size);
        file.close();
        
        // File data is now in memblock array
        // (Do something with it here...)
        mHeader.mFileSize = *(reinterpret_cast<int*>(&memblock[4]));
        mHeader.mFileNameLength = memblock[8];
        //identify index at which data begins
        int dataInd = 9 + mHeader.mFileNameLength;
        char* fileName = new char[mHeader.mFileNameLength];
        //read file name into a c-string
        for(unsigned int i = 0; i < mHeader.mFileNameLength; i++){
            fileName[i] = memblock[i+9];
        }
        //calculate size of data
        size_t dataSize = (int)size - dataInd;
        //convert file name c-string into std::string
        mHeader.mFileName.assign(fileName,mHeader.mFileNameLength);
        mRleData.Decompress(&memblock[dataInd], dataSize, mHeader.mFileSize);
        // Make sure to clean up!
        delete[] memblock;
    }
    
    std::ofstream arc(mHeader.mFileName, std::ios::out|std::ios::binary|std::ios::trunc);
    if (arc.is_open())
    {
        // Use arc.write function to write data here
        arc.write(mRleData.mData, mRleData.mSize);
    }

}
