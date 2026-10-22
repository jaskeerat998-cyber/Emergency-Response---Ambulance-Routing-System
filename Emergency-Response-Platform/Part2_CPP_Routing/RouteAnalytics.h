#ifndef ROUTE_ANALYTICS_H
#define ROUTE_ANALYTICS_H
#include <string>
#include <vector>
class RouteAnalytics  {
    public:
    static double estimateMinutes(double distanceKm, double averageSpeedKmh =25.0);
    static std::string formatNodePath(const std::vector <int >& nodeIds);
    static std::string classifyDistance(double distanceKm);
};
#endif
