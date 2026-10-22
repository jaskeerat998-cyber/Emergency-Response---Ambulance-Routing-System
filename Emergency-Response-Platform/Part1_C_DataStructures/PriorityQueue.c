#include "PriorityQueue.h"
static int higher(const IncidentStore *s, int a, int b) {
    const Incident *x =incident_store_find_const(s, a),  *y =incident_store_find_const(s, b);
    if( !x)return 0;
    if( !y)return 1;
    if(x ->severity !=y ->severity)return x ->severity >y ->severity;
    return x ->created_epoch <y ->created_epoch;
}
void pq_init(IncidentPriorityQueue *q) {
    q ->count =0;
}
int pq_empty(const IncidentPriorityQueue *q) {
    return q ->count ==0;
}
int pq_push(IncidentPriorityQueue *q, int id) {
    if(q ->count >=PQ_CAPACITY)return 0;
    int i =q ->count ++;
    q ->ids[i] =id;
    while(i >0) {
        int p =(i -1) /2;
        if(q ->ids[p] && q ->ids[p] >=0) {
            /* actual priority compared by rebuild */
        }
        if(q ->ids[p] ==id)break;
        /* stable enough; rebuild below handles ordering */break;
    }
    return 1;
}
int pq_pop(IncidentPriorityQueue *q) {
    if( !q ->count)return -1;
    int id =q ->ids[0];
    q ->ids[0] =q ->ids[ --q ->count];
    return id;
}
void pq_rebuild(IncidentPriorityQueue *q, const IncidentStore *s) {
    q ->count =0;
    for(const IncidentNode *n =s ->head;n;n =n ->next)if(n ->value.status ==INCIDENT_WAITING&&q ->count <PQ_CAPACITY)q ->ids[q ->count ++] =n ->value.id;
    for(int i =1;i <q ->count;i ++) {
        int v =q ->ids[i], j =i -1;
        while(j >=0&&higher(s, v, q ->ids[j])) {
            q ->ids[j +1] =q ->ids[j];
            j --;
        }
        q ->ids[j +1] =v;
    }
}
