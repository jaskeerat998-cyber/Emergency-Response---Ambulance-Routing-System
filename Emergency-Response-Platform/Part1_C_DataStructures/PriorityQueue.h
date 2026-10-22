#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include "IncidentStore.h"
#define PQ_CAPACITY 2048
#ifdef __cplusplus
extern "C"  {
    #endif
    typedef struct  {
        int ids[PQ_CAPACITY];
        int count;
    }
    IncidentPriorityQueue;
    void pq_init(IncidentPriorityQueue *q);
    int pq_push(IncidentPriorityQueue *q, int incident_id);
    int pq_pop(IncidentPriorityQueue *q);
    int pq_empty(const IncidentPriorityQueue *q);
    void pq_rebuild(IncidentPriorityQueue *q, const IncidentStore *store);
    #ifdef __cplusplus
}
#endif
#endif
