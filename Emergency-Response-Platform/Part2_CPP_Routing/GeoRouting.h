#ifndef GEO_ROUTING_H
#define GEO_ROUTING_H
#include <string>
#include <vector>
struct Coordinate  {
    double lat;
    double lon;
};
struct RoadNode  {
    int id;
    std::string name;
    Coordinate point;
};
struct RouteResult  {
    bool found =false;
    double distanceKm =0;
    double estimatedMinutes =0;
    std::vector <int > nodeIds;
    std::string note;
};
class GeoRouting  {
    public:
    GeoRouting();
    static bool validCoordinate(Coordinate p);
    static double haversineKm(Coordinate a, Coordinate b);
    int nearestNode(Coordinate p) const;
    const RoadNode& node(int id) const;
    int nodeCount() const;
    RouteResult shortestRoute(Coordinate start, Coordinate destination) const;
    std::vector <RoadNode > nearbyNodes(Coordinate p, double radiusKm) const;
    void printNodes() const;
    private:
    std::vector <RoadNode > nodes;
    std::vector <std::vector <std::pair <int, double >>> edges;
    void addRoad(int a, int b, double km);
};
#endif
