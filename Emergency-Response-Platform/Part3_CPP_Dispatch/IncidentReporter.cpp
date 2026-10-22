#include "IncidentReporter.h"
#include "../Part1_C_DataStructures/IncidentRules.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
void IncidentReporter::listIncidents(const IncidentStore&s) {
    std::cout <<"\nID     Severity Status      Caller                 Location (lat, lon)       Ambulance Hospital\n";
    for(const IncidentNode *n =s.head;n;n =n ->next) {
        const Incident&e =n ->value;
        std::cout <<std::left <<std::setw(7) <<e.id <<std::setw(9) <<e.severity <<std::setw(12) <<incident_rules_status_name(e.status) <<std::setw(23) <<e.caller
        <<std::fixed <<std::setprecision(4) <<e.latitude <<", " <<e.longitude <<"   " <<e.assigned_ambulance_id <<"       " <<e.assigned_hospital_id <<"\n";
        std::cout <<"       " <<e.description <<"\n";
    }
    if( !s.count)std::cout <<"No incidents recorded yet.\n";
}
void IncidentReporter::showPriorityQueue(const IncidentStore&s, const IncidentPriorityQueue&q) {
    std::cout <<"\nWaiting incident priority (severity first, older incidents break ties):\n";
    for(int i =0;i <q.count;i ++) {
        const Incident *e =incident_store_find_const(&s, q.ids[i]);
        if(e)std::cout <<"#" <<e ->id <<" severity " <<e ->severity <<" — " <<e ->caller <<"\n";
    }
    if( !q.count)std::cout <<"Queue empty.\n";
}
void IncidentReporter::showResources(const ResourceManager&r) {
    r.printResources();
}
void IncidentReporter::showSummary(const IncidentStore&s, const ResourceManager&r) {
    int waiting =0, dispatched =0, completed =0;
    for(const IncidentNode *n =s.head;n;n =n ->next) {
        waiting +=n ->value.status ==INCIDENT_WAITING;
        dispatched +=n ->value.status ==INCIDENT_DISPATCHED;
        completed +=n ->value.status ==INCIDENT_COMPLETED;
    }
    int available =0;
    for(const auto&a:r.ambulances())available +=a.available;
    std::cout <<"\nSYSTEM SUMMARY\nTotal incidents: " <<s.count <<"\nWaiting: " <<waiting <<"\nDispatched: " <<dispatched <<"\nCompleted: " <<completed <<"\nAvailable ambulances: " <<available <<"\n";
}
