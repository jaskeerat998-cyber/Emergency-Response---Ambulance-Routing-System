#ifndef INCIDENT_RULES_H
#define INCIDENT_RULES_H
#include "IncidentStore.h"
#ifdef __cplusplus
extern "C"  {
    #endif
    int incident_rules_validate(const Incident *incident);
    int incident_rules_priority_compare(const Incident *a, const Incident *b);
    const char *incident_rules_status_name(int status);
    #ifdef __cplusplus
}
#endif
#endif
