#include "slave_numeros.h"
#include <iostream>
#include <algorithm>

int SlaveNumeros::countNumbers(const std::string& text) {
    int count = 0;
    for (char c : text) {
        if (std::isdigit(c)) {
            count++;
        }
    }
    return count;
}

std::string SlaveNumeros::processRequest(const std::string& input) {
    int numberCount = countNumbers(input);
    
    // Return JSON response
    return "{\"numeros\": " + std::to_string(numberCount) + "}";
}
