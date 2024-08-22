#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include "picosha2.h"

bool isFilename(const std::string& input) {
    // Check if the 3rd or 4th character from the end is a "."
    size_t len = input.length();
    return (len >= 4 && (input[len - 4] == '.' || input[len - 3] == '.'));
}

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return "";
    }
    
    std::string fileContents;
    file.seekg(0, std::ios::end);
    fileContents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&fileContents[0], fileContents.size());
    file.close();
    
    return fileContents;
}

int main() {
    std::cout << "Multi-Hashing SHA-256 by Anthro Teacher." << std::endl;
    std::cout << "This will repeat your Text a specified number of times." << std::endl;
    std::cout << "It will hash this repeated text repeatedly a specified number of times." << std::endl;
    std::cout << "Hashing more times may make the text even more potent." << std::endl << std::endl;

    std::string original;
    std::cout << "Enter Text or Filename: ";
    std::getline(std::cin, original);

    int times_to_repeat = 1;    
    std::cout << "Number of times to Repeat [1-10000000]: ";
    std::cin >> times_to_repeat;
    std::cin.ignore(); // Ignore the newline character

    int num_hash_levels;
    std::cout << "Number of Hash Levels [1-100]: ";
    std::cin >> num_hash_levels;
    std::cin.ignore(); // Ignore the newline character
    
    std::string line;
    int linenum = 0;
    std::string repeated_text = "";
    std::string hashed_text = "";
    std::string line2 = "";

    if (isFilename(original)) {
        std::ifstream file(original);
        std::ofstream outFile("codes.txt");
        std::string repeated_hash = "";
        while (std::getline(file, line)) {
            line2 = line;
            repeated_text = ""; // Reset repeated_text to an empty string
            for (int i = 0; i < times_to_repeat; ++i) {
                repeated_text += line2 + "\n";
            }
            hashed_text = picosha2::hash256_hex_string(repeated_text);
            std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
            for (int level = 1; level <= num_hash_levels; ++level) {
                std::cout << "Hashing Level " << level << "\r";
                repeated_hash = ""; // Reset repeated_hash to an empty string
                for (int i = 0; i < times_to_repeat; ++i) {
                    repeated_hash += hashed_text + "\n";
                }
                
                hashed_text = picosha2::hash256_hex_string(repeated_hash);
                std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
            }
            outFile << line << ": " << hashed_text << std::endl;
            linenum += 1;
            std::cout << "\nLine #" << linenum << " written." << std::endl;
        }
        file.close();
        outFile.close();
        std::cout << "Codes written to codes.txt" << std::endl;
    } else {
        std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
        for (int i = 0; i < times_to_repeat; ++i) {
            repeated_text += original + "\n";
        }
        hashed_text = picosha2::hash256_hex_string(repeated_text);
        std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
        
        for (int level = 1; level <= num_hash_levels; ++level) {
            std::cout << "Hashing Level " << level << "\r";
            
            std::string repeated_hash;
            for (int i = 0; i < times_to_repeat; ++i) {
                repeated_hash += hashed_text + "\n";
            }
            
            hashed_text = picosha2::hash256_hex_string(repeated_hash);
            repeated_hash = "";
            std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
        }

        std::cout << "\n[Repeats: " << times_to_repeat << "] (Hash level: " << num_hash_levels << "): " << hashed_text << std::endl;
    }

    return 0;
}
