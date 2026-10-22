#include "DispatchEngine.h"
#include "IncidentReporter.h"
#include "../Part1_C_DataStructures/IncidentRules.h"
#include "../Part2_CPP_Routing/RouteAnalytics.h"
#include <iostream>
DispatchEngine::DispatchEngine():audit("dispatch_audit.log"), dataPath("incidents.csv") {
    incident_store_init(&store);
    pq_init(&queue);
    incident_store_load_csv(&store, dataPath.c_str());
    rebuildQueue();
}
DispatchEngine::~DispatchEngine() {
    save();
    incident_store_destroy(&store);
}
int DispatchEngine::registerIncident(const Incident&e) {
    if( !incident_rules_validate(&e))return -1;
    int id =incident_store_add(&store, e);
    if(id <0)return -1;
    rebuildQueue();
    audit.record("REGISTER incident=" +std::to_string(id) +" severity=" +std::to_string(e.severity));
    save();
    return id;
}
void DispatchEngine::rebuildQueue() {
    pq_rebuild(&queue, &store);
}
void DispatchEngine::listIncidents()const {
    IncidentReporter::listIncidents(store);
}
void DispatchEngine::showResources()const {
    IncidentReporter::showResources(resources);
}
void DispatchEngine::showPriorityQueue()const {
    IncidentReporter::showPriorityQueue(store, queue);
}
DispatchResult DispatchEngine::dispatchNext() {
    DispatchResult result;
    rebuildQueue();
    if(pq_empty(&queue)) {
        result.message ="No waiting incidents.";
        return result;
    }
    for(int i =0;i <queue.count;i ++) {
        int id =queue.ids[i];
        Incident *incident =incident_store_find(&store, id);
        if( !incident)continue;
        int ai =resources.nearestCompatibleAmbulance( *incident);
        int hi =resources.nearestEligibleHospital( *incident);
        if(ai <0||hi <0)continue;
        const Ambulance ambulance =resources.ambulances()[ai];
        const Hospital hospital =resources.hospitals()[hi];
        RouteResult toScene =routing.shortestRoute(ambulance.position,  {
            incident ->latitude, incident ->longitude
        });
        RouteResult toHospital =routing.shortestRoute( {
            incident ->latitude, incident ->longitude
        }, hospital.position);
        if( !toScene.found|| !toHospital.found) {
            result.message ="Route unavailable in configured road graph; incident left waiting.";
            return result;
        }
        incident ->status =INCIDENT_DISPATCHED;
        incident ->assigned_ambulance_id =ambulance.id;
        incident ->assigned_hospital_id =hospital.id;
        resources.reserve(ai, hi, id);
        result.success =true;
        result.ambulanceId =ambulance.id;
        result.hospitalId =hospital.id;
        result.ambulanceRoute =toScene;
        result.hospitalRoute =toHospital;
        result.message ="Dispatch assigned. Confirm details with caller and responders.";
        audit.record("DISPATCH incident=" +std::to_string(id) +" ambulance=" +std::to_string(ambulance.id) +" hospital=" +std::to_string(hospital.id));
        rebuildQueue();
        save();
        return result;
    }
    result.message ="No compatible available ambulance and accepting hospital combination. Escalate to a human dispatcher.";
    return result;
}
bool DispatchEngine::updateSeverity(int id, int severity) {
    Incident *e =incident_store_find(&store, id);
    if( !e||e ->status !=INCIDENT_WAITING||severity <1||severity >5)return false;
    e ->severity =severity;
    rebuildQueue();
    audit.record("SEVERITY_UPDATE incident=" +std::to_string(id) +" severity=" +std::to_string(severity));
    save();
    return true;
}
bool DispatchEngine::cancelIncident(int id) {
    Incident *e =incident_store_find(&store, id);
    if( !e||e ->status !=INCIDENT_WAITING)return false;
    e ->status =INCIDENT_CANCELLED;
    rebuildQueue();
    audit.record("CANCEL incident=" +std::to_string(id));
    save();
    return true;
}
bool DispatchEngine::completeIncident(int id) {
    Incident *e =incident_store_find(&store, id);
    if( !e||e ->status !=INCIDENT_DISPATCHED)return false;
    resources.releaseForIncident( *e);
    e ->status =INCIDENT_COMPLETED;
    audit.record("COMPLETE incident=" +std::to_string(id));
    rebuildQueue();
    save();
    return true;
}
void DispatchEngine::showRouteForIncident(int id)const {
    const Incident *e =incident_store_find_const(&store, id);
    if( !e) {
        std::cout <<"Incident not found.\n";
        return;
    }
    std::cout <<"Incident " <<id <<" status " <<incident_rules_status_name(e ->status) <<"\n";
    auto nearby =routing.nearbyNodes( {
        e ->latitude, e ->longitude
    }, 5.0);
    std::cout <<"Road-network nodes within 5 km straight-line radius: " <<nearby.size() <<"\n";
    for(const auto&n:nearby)std::cout <<"  " <<n.name <<" (" <<GeoRouting::haversineKm( {
        e ->latitude, e ->longitude
    }, n.point) <<" km)\n";
}
void DispatchEngine::showNetwork()const {
    routing.printNodes();
}
bool DispatchEngine::save()const {
    return incident_store_save_csv(&store, dataPath.c_str()) !=0;
}
void DispatchEngine::printSummary()const {
    IncidentReporter::showSummary(store, resources);
}
