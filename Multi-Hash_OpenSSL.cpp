//Compile: g++ -O3 -Wall -static Multi-Hash2.cpp -o Multi-Hash2.exe -lssl -lcrypto -lcrypt32 -ladvapi32 -lws2_32 -I./openssl/include

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>

// Function to compute SHA-256 hash using OpenSSL EVP API
std::string sha256_hash(const std::string& input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        return "";
    }

    if (!EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (!EVP_DigestUpdate(ctx, input.c_str(), input.length())) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (!EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    std::string hash_str;
    for (unsigned int i = 0; i < hash_len; ++i) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        hash_str += buf;
    }

    return hash_str;
}

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
            hashed_text = sha256_hash(repeated_text);
            std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
            for (int level = 1; level <= num_hash_levels; ++level) {
                std::cout << "Hashing Level " << level << "\r";
                repeated_hash = ""; // Reset repeated_hash to an empty string
                for (int i = 0; i < times_to_repeat; ++i) {
                    repeated_hash += hashed_text + "\n";
                }
                
                hashed_text = sha256_hash(repeated_hash);
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
        hashed_text = sha256_hash(repeated_text);
        std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
        
        for (int level = 1; level <= num_hash_levels; ++level) {
            std::cout << "Hashing Level " << level << "\r";
            
            std::string repeated_hash;
            for (int i = 0; i < times_to_repeat; ++i) {
                repeated_hash += hashed_text + "\n";
            }
            
            hashed_text = sha256_hash(repeated_hash);
            repeated_hash = "";
            std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
        }

        std::cout << "\n[Repeats: " << times_to_repeat << "] (Hash level: " << num_hash_levels << "): " << hashed_text << std::endl;
    }

    return 0;
}