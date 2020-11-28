#include "utility.hpp"
#include <bitset>


unsigned char *convertHostNameToDNSField(std::string inputString){
    // std::string testString = "www.charity.org";
    // std::string testString = "www.google.io";
    // std::cout << "testing kinda works???: " <<testString.length()<< std::endl;
    int newSize = inputString.length() + 2;
    unsigned char *conversionField = new unsigned char[newSize];

    int index = 0;
    int newIndex = 1;
    uint8_t charCount = 0; //counts the character before '.'
    int dotIndex = 0;//replaces '.' with the byte value
    while(index < inputString.length()){
        if(inputString[index] == '.'){
            conversionField[dotIndex] = charCount;
            dotIndex = newIndex;
            charCount = 0;
        }
        if(inputString[index] != '.'){
            conversionField[newIndex] = inputString[index];
            charCount++;
        }
        newIndex++;
        index++;
    }
    conversionField[dotIndex] = charCount;
    conversionField[newIndex] = '\0';


    //conversion from little edian to big edian
    // unsigned char *returnField = new unsigned char[newSize];
    // int returnIndex = newSize - 1;
    // int iteratingIndex = 0;
    // while(conversionField[iteratingIndex] != '\0'){   
    //     returnField[returnIndex] = conversionField[iteratingIndex];
    //     returnIndex--;
    //     iteratingIndex++;
    // }


    //debugging code
    // int testIndex = 0;
    // while(conversionField[testIndex] != '\0'){
    //     std::cout << "help: " << conversionField[testIndex] <<" " << std::bitset<8>(conversionField[testIndex]) << std::endl;
    //     testIndex++;
    // }
    //end

    return conversionField;
    // return returnField;
};