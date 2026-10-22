#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "DispatchModels.h"
#include <vector>
class ResourceManager  {
    public:
    ResourceManager();
    int nearestCompatibleAmbulance(const Incident& incident) const;
    int nearestEligibleHospital(const Incident& incident) const;
    void reserve(int ambulanceIndex, int hospitalIndex, int incidentId);
    void releaseForIncident(const Incident& incident);
    std::vector <Ambulance >& ambulances();
    const std::vector <Ambulance >& ambulances() const;
    std::vector <Hospital >& hospitals();
    const std::vector <Hospital >& hospitals() const;
    void printResources() const;
    private:
    std::vector <Ambulance > ambulanceFleet;
    std::vector <Hospital > hospitalList;
};
#endif
