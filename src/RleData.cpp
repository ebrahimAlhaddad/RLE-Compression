#include "RleData.h"

void RleData::Compress(const char* input, size_t inSize)
{
	// TODO
    mSize = 0;
    delete[] mData;
    mData = new char[inSize * 2];
    for(unsigned int i = 0; i < inSize; i++){
        //reset charCount
        int char_count = 0;
        unsigned int j = i;
       //positive run reading
       if(input[i] == input[i+1] || i == inSize - 1){
           //edge case is checked in if statement
           //charCount initialized
           char_count = 1;
           //similar pairs checked, max count is checked and over flow case is checked
            while(input[j] == input[j+1] && char_count < 127 && j < inSize){
                //increment count with similar pairs
                char_count += 1;
                j += 1;
                if(j == inSize - 1){
                    //quit while loop once edge case is reached
                    j++;
                    break;
                }
            }
           // update i to read input at the correct index
            i = j;
            mData[mSize] = char_count;
            mSize += 1;
           //edge case is checked for a positive run that ends the input
           if(i == inSize - 1){
               mData[mSize] = input[i];
           }
           else{
               mData[mSize] = input[j-1];
           }
           mSize += 1;
       } else {
           //negative run reading
          while(input[j] != input[j+1] && char_count > -127 && j < inSize)
          {
              char_count -= 1;
              j += 1;
              //negative run edge case check to update i correctly and quit loop
              if(j == inSize - 1){
                  char_count--;
                  j++;
                  break;
              }
          }
           if(char_count == -1){
               char_count = 1;
           }
          mData[mSize] = char_count;
          mSize += 1;
           //update compressed data with the negative run characters
           while(i != j){
               mData[mSize] = input[i];
               i++;
               mSize += 1;
           }
          i -= 1;
        }
    }

    
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	// TODO
    mSize = 0;
    delete[] mData;
    mData = new char[outSize];
    for(unsigned int i = 0; i < inSize; i++){
        int elem;
        //edge case check
        if(i>=inSize-1){
            break;
        }
        else{
        //collect character count in compressed file
        elem  = (int)input[i];
        //positive run processing
        if(elem > 0){
            i++;
            //repetitive copy of positive run character
            for(int j = 0; j < elem; j++){
                mData[mSize] = input[i];
                mSize++;
            }
        //negative run processing
        }else if(elem < 0){
            elem = abs(elem);
            i++;
            //copy negative run characters
            for(int j = 0; j < elem; j++){
                mData[mSize] = input[i];
                mSize++;
                if(!(j == elem - 1)){
                    i++;
                }
            }
        }
        }
    }
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}
