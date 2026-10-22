#ifndef DISPATCH_MODELS_H
#define DISPATCH_MODELS_H
#include <string>
#include <vector>
#include "../Part1_C_DataStructures/IncidentStore.h"
#include "../Part2_CPP_Routing/GeoRouting.h"
struct Ambulance  {
    int id;
    std::string callSign;
    Coordinate position;
    int capability;
    bool available;
    int assignedIncident;
};
struct Hospital  {
    int id;
    std::string name;
    Coordinate position;
    int facility;
    int availableBeds;
    bool accepting;
};
struct DispatchResult  {
    bool success =false;
    int ambulanceId =-1;
    int hospitalId =-1;
    RouteResult ambulanceRoute;
    RouteResult hospitalRoute;
    std::string message;
};
#endif
