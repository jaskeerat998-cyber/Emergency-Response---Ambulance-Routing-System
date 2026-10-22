#include "InputValidation.h"
#include <iostream>
#include <sstream>
#include <cmath>
namespace InputValidation  {
    bool readLine(const std::string&prompt, std::string&out) {
        std::cout <<prompt;
        if( !std::getline(std::cin, out))return false;
        out =cleanField(out);
        return true;
    }
    std::string cleanField(std::string s) {
        for(char&c:s)if(c ==','||c =='\n'||c =='\r')c =' ';
        while( !s.empty()&&s.front() ==' ')s.erase(s.begin());
        while( !s.empty()&&s.back() ==' ')s.pop_back();
        return s;
    }
    bool readInt(const std::string&p, int lo, int hi, int&out) {
        std::string s;
        if( !readLine(p, s))return false;
        std::stringstream ss(s);
        int v;
        char extra;
        if( !(ss >>v)||(ss >>extra)||v <lo||v >hi) {
            std::cout <<"Enter a whole number from " <<lo <<" to " <<hi <<".\n";
            return readInt(p, lo, hi, out);
        }
        out =v;
        return true;
    }
    bool readDouble(const std::string&p, double lo, double hi, double&out) {
        std::string s;
        if( !readLine(p, s))return false;
        std::stringstream ss(s);
        double v;
        char extra;
        if( !(ss >>v)||(ss >>extra)|| !std::isfinite(v)||v <lo||v >hi) {
            std::cout <<"Enter a number from " <<lo <<" to " <<hi <<".\n";
            return readDouble(p, lo, hi, out);
        }
        out =v;
        return true;
    }
    bool readCoordinate(const std::string&label, double lo, double hi, double&out) {
        return readDouble(label, lo, hi, out);
    }
}
