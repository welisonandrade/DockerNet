#ifndef SLAVE_NUMEROS_H
#define SLAVE_NUMEROS_H

#include <string>
#include <cctype>

class SlaveNumeros {
public:
    static int countNumbers(const std::string& text);
    static std::string processRequest(const std::string& input);
};

#endif // SLAVE_NUMEROS_H
