#ifndef INCIDENT_STORE_H
#define INCIDENT_STORE_H
#include <stddef.h>
#ifdef __cplusplus
extern "C"  {
    #endif
    #define ERS_TEXT 120
    typedef enum  {
        INCIDENT_WAITING =0,  INCIDENT_DISPATCHED =1,  INCIDENT_COMPLETED =2,  INCIDENT_CANCELLED =3
    }
    IncidentStatus;
    typedef struct  {
        int id;
        char caller[ERS_TEXT];
        char phone[32];
        char description[ERS_TEXT];
        double latitude,  longitude;
        int severity;
        /* 1 minor .. 5 life-threatening */
        int required_ambulance;
        /* 0 basic, 1 advanced, 2 neonatal */
        int required_facility;
        /* 0 any, 1 trauma, 2 cardiac, 3 pediatric */
        int status;
        long created_epoch;
        int assigned_ambulance_id;
        int assigned_hospital_id;
    }
    Incident;
    typedef struct IncidentNode  {
        Incident value;
        struct IncidentNode *next;
    }
    IncidentNode;
    typedef struct  {
        IncidentNode *head;
        size_t count;
        int next_id;
    }
    IncidentStore;
    void incident_store_init(IncidentStore *store);
    void incident_store_destroy(IncidentStore *store);
    int incident_store_add(IncidentStore *store,  Incident incident);
    Incident *incident_store_find(IncidentStore *store,  int id);
    const Incident *incident_store_find_const(const IncidentStore *store,  int id);
    size_t incident_store_count(const IncidentStore *store);
    int incident_store_save_csv(const IncidentStore *store,  const char *path);
    int incident_store_load_csv(IncidentStore *store,  const char *path);
    #ifdef __cplusplus
}
#endif
#endif
