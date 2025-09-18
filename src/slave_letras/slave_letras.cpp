#include "slave_letras.h"
#include <iostream>
#include <algorithm>

int SlaveLetras::countLetters(const std::string& text) {
    int count = 0;
    for (char c : text) {
        if (std::isalpha(c)) {
            count++;
        }
    }
    return count;
}

std::string SlaveLetras::processRequest(const std::string& input) {
    int letterCount = countLetters(input);
    
    // Return JSON response
    return "{\"letras\": " + std::to_string(letterCount) + "}";
}
