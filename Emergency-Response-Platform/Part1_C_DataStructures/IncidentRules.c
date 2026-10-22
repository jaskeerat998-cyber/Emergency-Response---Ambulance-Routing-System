#include "IncidentRules.h"
#include <math.h>
#include <string.h>
int incident_rules_validate(const Incident *e)  {
    if( !e) return 0;
    if( !isfinite(e ->latitude)|| !isfinite(e ->longitude)) return 0;
    if(e ->latitude < -90.0 || e ->latitude > 90.0 || e ->longitude < -180.0 || e ->longitude > 180.0) return 0;
    if(e ->severity < 1 || e ->severity > 5) return 0;
    if(e ->required_ambulance < 0 || e ->required_ambulance > 2) return 0;
    if(e ->required_facility < 0 || e ->required_facility > 3) return 0;
    if(e ->caller[0] =='\0' || e ->description[0] =='\0') return 0;
    return 1;
}
int incident_rules_priority_compare(const Incident *a, const Incident *b)  {
    if( !a|| !b) return 0;
    if(a ->severity !=b ->severity) return a ->severity >b ->severity ? 1 : -1;
    if(a ->created_epoch ==b ->created_epoch) return 0;
    return a ->created_epoch <b ->created_epoch ? 1 : -1;
}
const char *incident_rules_status_name(int s)  {
    switch(s)  {
        case INCIDENT_WAITING:return "WAITING";
        case INCIDENT_DISPATCHED:return "DISPATCHED";
        case INCIDENT_COMPLETED:return "COMPLETED";
        case INCIDENT_CANCELLED:return "CANCELLED";
        default:return "UNKNOWN";
    }
}
