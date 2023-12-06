#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

int numBitsInFile(string fileName) {

    ifstream inputFile(fileName, ios::binary);

    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    inputFile.seekg(0, ios::end);
    streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    // if (fileSize != -1) {
    //     cout << "Number of bits in the file: " << fileSize * 8 << endl;
    // } else {
    //     cerr << "Error getting file size." << endl;
    //     return 1;
    // }

    inputFile.close();

    int numBits = fileSize * 8;

    return numBits;

}

void compressLZ(const std::string& fileName) {
    std::ifstream inputFile(fileName);
    std::string content((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    std::map<std::string, int> dictionary;
    std::string currentChar;
    std::string compressed = "";
    int dictSize = 0;

    for (char c : content) {
        std::string currentCharPlus = currentChar + c;
        if (dictionary.count(currentCharPlus)) {
            currentChar = currentCharPlus;
        } else {
            if (dictionary.size() == 0) {
                compressed += c;
            } else if (currentChar.size() == 1) {
                compressed += currentChar;
            } else {
                compressed += std::to_string(dictionary[currentChar]) + c;
            }
            dictionary[currentCharPlus] = dictSize++;
            currentChar = "";
        }
    }

    if (!currentChar.empty()) {
        if (currentChar.size() == 1) {
            compressed += currentChar;
        } else {
            compressed += std::to_string(dictionary[currentChar]);
        }
    }

    std::string compressedFileName = "compressed_" + fileName;
    std::ofstream outputFile(compressedFileName);
    outputFile << compressed;

    inputFile.close();
    outputFile.close();
}




int main() {
    
    string fileName = "test1.txt";

    int numBitsBeforeCompression = numBitsInFile("test1.txt");

    cout << "There are " << numBitsBeforeCompression << " bits before compression" << endl;

    compressLZ(fileName);

    int numBitsAfterCompression = numBitsInFile("compressed_test1.txt");

    cout << "There are " << numBitsAfterCompression << " bits after compression" << endl;


    return 0;
}
