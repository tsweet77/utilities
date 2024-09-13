#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include "picosha2.h"
#include <limits>

bool isFilename(const std::string& input) {
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

std::string truncate_to_64_bit(const std::string& hash256) {
    // Each 64-bit group is 16 hex characters
    unsigned long long group1 = std::stoull(hash256.substr(0, 16), nullptr, 16);
    unsigned long long group2 = std::stoull(hash256.substr(16, 16), nullptr, 16);
    unsigned long long group3 = std::stoull(hash256.substr(32, 16), nullptr, 16);
    unsigned long long group4 = std::stoull(hash256.substr(48, 16), nullptr, 16);

    // Add the four 64-bit groups together
    unsigned long long combined_64_bit = group1 + group2 + group3 + group4;

    // Since we are working with 64-bit integers, the modulo operation is inherent
    // We directly convert the result to a hex string
    std::stringstream ss;
    ss << std::hex << std::uppercase << (combined_64_bit & 0xFFFFFFFFFFFFFFFFULL);
    return ss.str();
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

    std::string truncate_choice;
    std::cout << "Truncate to 64-Bit: (Y/n): ";
    std::getline(std::cin, truncate_choice);

    bool truncate_to_64 = (truncate_choice == "Y" || truncate_choice == "y");

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

            if (truncate_to_64) {
                hashed_text = truncate_to_64_bit(hashed_text);
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

        if (truncate_to_64) {
            hashed_text = truncate_to_64_bit(hashed_text);
        }

        std::cout << "\n[Repeats: " << times_to_repeat << "] (Hash level: " << num_hash_levels << "): " << hashed_text << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Wait for the user to press Enter before quitting
    std::cout << "Press Enter to Quit...";
    std::cin.get();
    
    return 0;
}
