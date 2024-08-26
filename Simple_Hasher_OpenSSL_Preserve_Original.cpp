//Compile: g++ -O3 -Wall -static Simple_Hasher_OpenSSL.cpp -o Simple_Hasher_OpenSSL.exe -lssl -lcrypto -lcrypt32 -ladvapi32 -lws2_32 -I./openssl/include

#include <iostream>
#include <string>
#include <csignal>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>
#include <algorithm>
#include <limits>

// Atomic flag to handle Ctrl-C gracefully
std::atomic<bool> keepRunning(true);

// Signal handler to catch Ctrl-C
void signalHandler(int signum) {
    keepRunning = false;
}

// Function to compute SHA3-512 hash using OpenSSL EVP API
std::string sha3_512_hash(const std::string& input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        return "";
    }

    if (!EVP_DigestInit_ex(ctx, EVP_sha3_512(), nullptr)) {
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

    std::ostringstream hash_str;
    for (unsigned int i = 0; i < hash_len; ++i) {
        hash_str << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    // Convert hash to uppercase
    std::string result = hash_str.str();
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    return result;
}

// Function to format large numbers with prefixes
std::string formatWithPrefix(double num) {
    const char* prefixes[] = {"", "k", "M", "B", "T", "q", "Q"};
    int index = 0;
    while (num >= 1000 && index < 6) {
        num /= 1000;
        ++index;
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << num << prefixes[index];
    return oss.str();
}

// Function to format time as H:MM:SS
std::string formatTime(double seconds) {
    int hours = static_cast<int>(seconds) / 3600;
    int minutes = (static_cast<int>(seconds) % 3600) / 60;
    int secs = static_cast<int>(seconds) % 60;

    std::ostringstream oss;
    oss << hours << ":" << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << secs;
    return oss.str();
}

std::string truncate_to_64_bit(const std::string& hash512) {
    // SHA3-512 produces a 512-bit hash, which is 64 bytes or 128 hex characters.
    // We break this into eight 64-bit (16 hex character) chunks.

    unsigned long long combined_64_bit = 0;
    
    for (int i = 0; i < 128; i += 16) {
        unsigned long long group = std::stoull(hash512.substr(i, 16), nullptr, 16);
        combined_64_bit += group;
    }

    // Convert the result to a 64-bit hex string
    std::stringstream ss;
    ss << std::hex << std::uppercase << (combined_64_bit & 0xFFFFFFFFFFFFFFFFULL);
    return ss.str();
}

int main() {
    std::cout << "Simple Hasher SHA3-512 by Anthro Teacher" << std::endl;
    std::cout << "www.intentionrepeater.com" << std::endl;
    std::cout << "Press Ctrl-C to Quit" << std::endl << std::endl;
    // Register signal handler to catch Ctrl-C
    std::signal(SIGINT, signalHandler);

    std::string intention;
    std::cout << "Intention: ";
    std::getline(std::cin, intention);

    std::cout << std::endl;

    unsigned long long iterations = 0;

    // Record the start time
    auto start_time = std::chrono::high_resolution_clock::now();
    auto last_output_time = start_time;
    std::string original_intention = intention;

    while (keepRunning) {
        // Hash the intention using SHA3-512
        intention = sha3_512_hash(intention + ": " + original_intention);
        iterations++;

        // Check if it's time to output the status (once per second)
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = current_time - last_output_time;

        if (elapsed_seconds.count() >= 1.0) {
            auto total_time = std::chrono::duration<double>(current_time - start_time).count();
            std::cout << "\r[" << formatTime(total_time) << "] ("
                      << formatWithPrefix(iterations) << " Hashes): "
                      << intention << " [" << truncate_to_64_bit(intention) << "]     " << std::flush;
            last_output_time = current_time;
        }
    }

    // Final output after Ctrl-C is pressed
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = end_time - start_time;
    std::cout << "\n\n";

    std::cout << "Intention: " << original_intention << std::endl;

    std::cout << "["
              << formatTime(total_time.count()) << "] ("
              << formatWithPrefix(iterations) << " TOTAL HASHES): "
              << intention << " [" << truncate_to_64_bit(intention) << "]     " << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Wait for the user to press Enter before quitting
    std::cout << "Press Enter to Quit...";
    std::cin.get();

    return 0;
}
