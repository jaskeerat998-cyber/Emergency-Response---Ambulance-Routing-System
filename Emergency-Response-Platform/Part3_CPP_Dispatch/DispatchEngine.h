#ifndef DISPATCH_ENGINE_H
#define DISPATCH_ENGINE_H
#include "DispatchModels.h"
#include "ResourceManager.h"
#include "DispatchAudit.h"
#include "../Part1_C_DataStructures/PriorityQueue.h"
#include "../Part2_CPP_Routing/GeoRouting.h"
#include <string>
class DispatchEngine  {
    public:
    DispatchEngine();
    ~DispatchEngine();
    int registerIncident(const Incident& incident);
    void listIncidents() const;
    void showResources() const;
    void showPriorityQueue() const;
    DispatchResult dispatchNext();
    bool updateSeverity(int id, int severity);
    bool cancelIncident(int id);
    bool completeIncident(int id);
    void showRouteForIncident(int id) const;
    void showNetwork() const;
    bool save() const;
    void printSummary() const;
    private:
    IncidentStore store;
    IncidentPriorityQueue queue;
    GeoRouting routing;
    ResourceManager resources;
    DispatchAudit audit;
    std::string dataPath;
    void rebuildQueue();
};
#endif
