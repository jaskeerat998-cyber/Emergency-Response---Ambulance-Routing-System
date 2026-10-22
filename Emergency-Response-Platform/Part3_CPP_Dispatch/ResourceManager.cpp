#include "ResourceManager.h"
#include "../Part2_CPP_Routing/GeoRouting.h"
#include <iostream>
#include <limits>
ResourceManager::ResourceManager()  {
    // Demonstration resources. Replace with verified fleet/hospital integrations before deployment.
    ambulanceFleet = {
        {
            201, "AMB-01",  {
                30.3256, 78.0413
            }, 0, true,  -1
        }, 
        {
            202, "AMB-02",  {
                30.2865, 78.0090
            }, 1, true,  -1
        }, 
        {
            203, "AMB-03",  {
                30.3570, 78.0700
            }, 1, true,  -1
        }, 
        {
            204, "AMB-04",  {
                30.2672, 78.0030
            }, 2, true,  -1
        }
    };
    hospitalList = {
        {
            301, "Doon Hospital (demo)",  {
                30.3245, 78.0437
            }, 1, 3, true
        }, 
        {
            302, "Max Hospital (demo)",  {
                30.3570, 78.0700
            }, 2, 2, true
        }, 
        {
            303, "Children's Hospital (demo)",  {
                30.3160, 78.0120
            }, 3, 1, true
        }, 
        {
            304, "General Hospital (demo)",  {
                30.2865, 78.0090
            }, 0, 4, true
        }
    };
}
int ResourceManager::nearestCompatibleAmbulance(const Incident&e)const  {
    int best =-1;
    double bestDistance =std::numeric_limits <double >::max();
    for(size_t i =0;i <ambulanceFleet.size(); ++i) {
        const auto&a =ambulanceFleet[i];
        if( !a.available||a.capability <e.required_ambulance)continue;
        double d =GeoRouting::haversineKm(a.position,  {
            e.latitude, e.longitude
        });
        if(d <bestDistance) {
            bestDistance =d;
            best =(int)i;
        }
    }
    return best;
}
int ResourceManager::nearestEligibleHospital(const Incident&e)const  {
    int best =-1;
    double bestDistance =std::numeric_limits <double >::max();
    for(size_t i =0;i <hospitalList.size(); ++i) {
        const auto&h =hospitalList[i];
        if( !h.accepting||h.availableBeds <=0)continue;
        if(e.required_facility !=0&&h.facility !=0&&h.facility !=e.required_facility)continue;
        double d =GeoRouting::haversineKm( {
            e.latitude, e.longitude
        }, h.position);
        if(d <bestDistance) {
            bestDistance =d;
            best =(int)i;
        }
    }
    return best;
}
void ResourceManager::reserve(int ai, int hi, int incidentId)  {
    if(ai >=0&&ai <(int)ambulanceFleet.size()) {
        auto&a =ambulanceFleet[ai];
        a.available =false;
        a.assignedIncident =incidentId;
    }
    if(hi >=0&&hi <(int)hospitalList.size())hospitalList[hi].availableBeds --;
}
void ResourceManager::releaseForIncident(const Incident&e)  {
    for(auto&a:ambulanceFleet)if(a.id ==e.assigned_ambulance_id) {
        a.available =true;
        a.assignedIncident =-1;
        for(const auto&h:hospitalList)if(h.id ==e.assigned_hospital_id) {
            a.position =h.position;
            break;
        }
    }
    for(auto&h:hospitalList)if(h.id ==e.assigned_hospital_id)h.availableBeds ++;
}
std::vector <Ambulance >& ResourceManager::ambulances() {
    return ambulanceFleet;
}
const std::vector <Ambulance >& ResourceManager::ambulances()const {
    return ambulanceFleet;
}
std::vector <Hospital >& ResourceManager::hospitals() {
    return hospitalList;
}
const std::vector <Hospital >& ResourceManager::hospitals()const {
    return hospitalList;
}
void ResourceManager::printResources()const  {
    std::cout <<"\nAMBULANCES\n";
    for(const auto&a:ambulanceFleet)std::cout <<a.id <<" " <<a.callSign <<" capability=" <<a.capability <<" status=" <<(a.available?"AVAILABLE":"BUSY") <<" at " <<a.position.lat <<"," <<a.position.lon <<"\n";
    std::cout <<"\nHOSPITALS (simulated inventory; verify before operational use)\n";
    for(const auto&h:hospitalList)std::cout <<h.id <<" " <<h.name <<" facility=" <<h.facility <<" beds=" <<h.availableBeds <<" accepting=" <<(h.accepting?"yes":"no") <<"\n";
}
