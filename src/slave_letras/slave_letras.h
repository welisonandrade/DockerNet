#ifndef SLAVE_LETRAS_H
#define SLAVE_LETRAS_H

#include <string>
#include <cctype>

class SlaveLetras {
public:
    static int countLetters(const std::string& text);
    static std::string processRequest(const std::string& input);
};

#endif // SLAVE_LETRAS_H
