#ifndef INCIDENT_REPORTER_H
#define INCIDENT_REPORTER_H
#include "../Part1_C_DataStructures/IncidentStore.h"
#include "../Part1_C_DataStructures/PriorityQueue.h"
#include "DispatchModels.h"
#include "ResourceManager.h"
class IncidentReporter  {
    public:
    static void listIncidents(const IncidentStore& store);
    static void showPriorityQueue(const IncidentStore& store, const IncidentPriorityQueue& queue);
    static void showResources(const ResourceManager& resources);
    static void showSummary(const IncidentStore& store, const ResourceManager& resources);
};
#endif
