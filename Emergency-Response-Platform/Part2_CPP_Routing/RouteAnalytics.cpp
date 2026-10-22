#include "RouteAnalytics.h"
#include <sstream>
#include <iomanip>
double RouteAnalytics::estimateMinutes(double km, double speed)  {
    if(km <0.0||speed <=0.0) return 0.0;
    return (km /speed) *60.0;
}
std::string RouteAnalytics::formatNodePath(const std::vector <int >& ids)  {
    std::ostringstream out;
    for(size_t i =0;i <ids.size(); ++i) {
        if(i)out <<" -> ";
        out <<ids[i];
    }
    return out.str();
}
std::string RouteAnalytics::classifyDistance(double km)  {
    if(km <0)return "invalid";
    if(km <3)return "near";
    if(km <10)return "regional";
    return "long-distance";
}
