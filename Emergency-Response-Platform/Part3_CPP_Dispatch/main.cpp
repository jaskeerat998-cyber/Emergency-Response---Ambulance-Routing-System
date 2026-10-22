#include "DispatchEngine.h"
#include "../Part2_CPP_Routing/InputValidation.h"
#include <iostream>
#include <limits>
#include <string>
#include <cstdio>
static void printMenu() {
    std::cout <<"\n========== EMERGENCY RESPONSE DISPATCH CONSOLE ==========\n"
    "1. Register emergency\n2. List incidents\n3. Show priority queue\n4. Dispatch next eligible incident\n"
    "5. View ambulance/hospital resources\n6. Update waiting incident severity\n7. Cancel waiting incident\n"
    "8. Complete dispatched incident\n9. Inspect incident location/network\n10. Show configured road network\n"
    "11. System summary\n0. Save and exit\nChoose: ";
}
int main() {
    DispatchEngine engine;
    std::cout <<"Emergency Response & Ambulance Routing System\n"
    <<"Prototype for dispatcher-assisted decision support; NOT a replacement for emergency services.\n";
    bool running =true;
    while(running) {
        printMenu();
        std::string choiceLine;
        if( !std::getline(std::cin, choiceLine))break;
        int choice =-1;
        try {
            size_t n;
            choice =std::stoi(choiceLine, &n);
            if(n !=choiceLine.size())choice =-1;
        }
        catch(...) {
            choice =-1;
        }
        if(choice ==1) {
            Incident e {
            };
            std::string s;
            if( !InputValidation::readLine("Caller name: ", s))break;
            std::snprintf(e.caller, sizeof e.caller, "%s", s.c_str());
            if( !InputValidation::readLine("Callback phone (store only if consented): ", s))break;
            std::snprintf(e.phone, sizeof e.phone, "%s", s.c_str());
            if( !InputValidation::readLine("Brief emergency description: ", s))break;
            std::snprintf(e.description, sizeof e.description, "%s", s.c_str());
            if( !InputValidation::readCoordinate("Latitude (-90..90): ",  -90, 90, e.latitude))break;
            if( !InputValidation::readCoordinate("Longitude (-180..180): ",  -180, 180, e.longitude))break;
            if(e.latitude <25||e.latitude >36||e.longitude <70||e.longitude >85)std::cout <<"Warning: coordinates are outside the configured Uttarakhand/North India demo region.\n";
            if( !InputValidation::readInt("Severity (1 minor, 2 moderate, 3 urgent, 4 critical, 5 life-threatening): ", 1, 5, e.severity))break;
            if( !InputValidation::readInt("Ambulance need (0 basic, 1 advanced, 2 neonatal): ", 0, 2, e.required_ambulance))break;
            if( !InputValidation::readInt("Facility need (0 any, 1 trauma, 2 cardiac, 3 pediatric): ", 0, 3, e.required_facility))break;
            int id =engine.registerIncident(e);
            if(id <0)std::cout <<"Registration failed: invalid coordinates or storage error.\n";
            else std::cout <<"Incident registered with ID " <<id <<". Keep this ID for follow-up.\n";
        }
        else if(choice ==2)engine.listIncidents();
        else if(choice ==3)engine.showPriorityQueue();
        else if(choice ==4) {
            auto r =engine.dispatchNext();
            std::cout <<"\n" <<r.message <<"\n";
            if(r.success) {
                std::cout <<"Ambulance ID: " <<r.ambulanceId <<" | Hospital ID: " <<r.hospitalId <<"\nAmbulance-to-scene route: " <<r.ambulanceRoute.distanceKm <<" km, approx " <<r.ambulanceRoute.estimatedMinutes <<" min\nHospital route: " <<r.hospitalRoute.distanceKm <<" km, approx " <<r.hospitalRoute.estimatedMinutes <<" min\n";
                std::cout <<"Route nodes (IDs): ";
                for(size_t i =0;i <r.ambulanceRoute.nodeIds.size(); ++i) {
                    if(i)std::cout <<" -> ";
                    std::cout <<r.ambulanceRoute.nodeIds[i];
                }
                std::cout <<"\n";
            }
        }
        else if(choice ==5)engine.showResources();
        else if(choice ==6) {
            int id, sev;
            if(InputValidation::readInt("Incident ID: ", 1, 1000000, id)&&InputValidation::readInt("New severity (1-5): ", 1, 5, sev))std::cout <<(engine.updateSeverity(id, sev)?"Severity updated.\n":"Update rejected (incident missing or not waiting).\n");
        }
        else if(choice ==7) {
            int id;
            if(InputValidation::readInt("Incident ID to cancel: ", 1, 1000000, id))std::cout <<(engine.cancelIncident(id)?"Incident cancelled.\n":"Cancellation rejected (only waiting incidents can be cancelled).\n");
        }
        else if(choice ==8) {
            int id;
            if(InputValidation::readInt("Incident ID completed: ", 1, 1000000, id))std::cout <<(engine.completeIncident(id)?"Incident marked completed; ambulance released.\n":"Completion rejected (incident must be dispatched).\n");
        }
        else if(choice ==9) {
            int id;
            if(InputValidation::readInt("Incident ID: ", 1, 1000000, id))engine.showRouteForIncident(id);
        }
        else if(choice ==10)engine.showNetwork();
        else if(choice ==11)engine.printSummary();
        else if(choice ==0) {
            engine.save();
            running =false;
            std::cout <<"Saved. Goodbye.\n";
        }
        else std::cout <<"Invalid menu option.\n";
    }
    return 0;
}
