#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H
#include <string>
#include "GeoRouting.h"
namespace InputValidation  {
    bool readLine(const std::string& prompt, std::string& out);
    bool readInt(const std::string& prompt, int minValue, int maxValue, int& out);
    bool readDouble(const std::string& prompt, double minValue, double maxValue, double& out);
    std::string cleanField(std::string value);
    bool readCoordinate(const std::string& label, double minValue, double maxValue, double& out);
}
#endif
