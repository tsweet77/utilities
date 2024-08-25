/*
Intention Repeater File Writer v1.3
Created 3/26/2022 by Anthro Teacher aka Thomas Sweet
Updated 8/21/2024
Repeats an intention to a file a specified number of times.
Includes Hashing Option
*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "picosha2.h" // Include the picosha2 library

using namespace std;

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

int main(int argc, char **argv)
{

    std::string intention, str_repetitions, filename, hash;
    unsigned long long int repetitions;

    cout << "Intention Repeater Talisman File Writer v1.3" << endl;
    cout << "By Anthro Teacher (AnthroHeart) aka Thomas Sweet" << endl;
    cout << "The Talisman Hash File is stronger, but it is SHA-256 encoded." << endl
         << endl;

    cout << "Intention: ";
    std::getline(std::cin, intention);

    cout << "Repetitions (Ex. 10000000): ";
    std::getline(std::cin, str_repetitions);

    repetitions = std::stoll(str_repetitions);

    cout << "Filename (Ex. file.txt): ";
    std::getline(std::cin, filename);
    std::string filePath;

    std::string createHashingFile;
    std::cout << "Create Talisman Hash File [Y/n]: ";
    std::getline(std::cin, createHashingFile);
    std::transform(createHashingFile.begin(), createHashingFile.end(), createHashingFile.begin(), ::toupper);
    
    ofstream o;
    o.open(filename);

    cout << "Writing Talisman File...";
    
    for (unsigned long long int i = 0; i < repetitions; i++)
    {
        o << intention << "\n";
    }

    o.close();

    cout << "\nTalisman File Created: " << filename << endl;

   if (createHashingFile == "Y" || createHashingFile == "YES") {
        std::string fileContents = readFile(filename);
        hash = picosha2::hash256_hex_string(fileContents);
        std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);
        o.open("Hashed_" + filename);
        
        cout << "Writing Talisman Hash File...";
        for (unsigned long long int i = 0; i < repetitions; i++)
        {
            o << hash << "\n";
        }

        o.close();

        cout << "\nTalisman Hash File Created: " << "Hashed_" + filename << endl;
    }

    std::cout << "Press Enter to Quit...";
    std::cin.get();

    return 0;

}

