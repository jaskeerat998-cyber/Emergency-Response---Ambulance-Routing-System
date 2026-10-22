#include "IncidentStore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void incident_store_init(IncidentStore *s) {
    s ->head =NULL;
    s ->count =0;
    s ->next_id =1001;
}
void incident_store_destroy(IncidentStore *s) {
    IncidentNode *n =s ->head;
    while(n) {
        IncidentNode *t =n ->next;
        free(n);
        n =t;
    }
    s ->head =NULL;
    s ->count =0;
}
int incident_store_add(IncidentStore *s, Incident e) {
    IncidentNode *n =(IncidentNode *)malloc(sizeof(IncidentNode));
    if( !n)return -1;
    e.id =s ->next_id ++;
    if(e.created_epoch ==0)e.created_epoch =(long)time(NULL);
    e.status =INCIDENT_WAITING;
    e.assigned_ambulance_id =-1;
    e.assigned_hospital_id =-1;
    n ->value =e;
    n ->next =s ->head;
    s ->head =n;
    s ->count ++;
    return e.id;
}
Incident *incident_store_find(IncidentStore *s, int id) {
    for(IncidentNode *n =s ->head;n;n =n ->next)if(n ->value.id ==id)return &n ->value;
    return NULL;
}
const Incident *incident_store_find_const(const IncidentStore *s, int id) {
    for(const IncidentNode *n =s ->head;n;n =n ->next)if(n ->value.id ==id)return &n ->value;
    return NULL;
}
size_t incident_store_count(const IncidentStore *s) {
    return s ->count;
}
int incident_store_save_csv(const IncidentStore *s, const char *path) {
    FILE *f =fopen(path, "w");
    if( !f)return 0;
    fprintf(f, "id,caller,phone,description,lat,lon,severity,ambulance,facility,status,created,assigned_ambulance,assigned_hospital\n");
    for(const IncidentNode *n =s ->head;n;n =n ->next) {
        const Incident *e =&n ->value;
        /* CSV fields are sanitized at input to avoid embedded commas/newlines. */
        fprintf(f, "%d,%s,%s,%s,%.7f,%.7f,%d,%d,%d,%d,%ld,%d,%d\n", e ->id, e ->caller, e ->phone, e ->description, e ->latitude, e ->longitude, e ->severity, e ->required_ambulance, e ->required_facility, e ->status, e ->created_epoch, e ->assigned_ambulance_id, e ->assigned_hospital_id);
    }
    fclose(f);
    return 1;
}
static void copy_field(char *out, size_t n, const char *in) {
    snprintf(out, n, "%s", in);
}
int incident_store_load_csv(IncidentStore *s, const char *path) {
    FILE *f =fopen(path, "r");
    if( !f)return 0;
    char line[1024];
    fgets(line, sizeof line, f);
    while(fgets(line, sizeof line, f)) {
        Incident e;
        memset(&e, 0, sizeof e);
        char *p =strtok(line, ",");
        if( !p)continue;
        e.id =atoi(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        copy_field(e.caller, sizeof e.caller, p);
        p =strtok(NULL, ",");
        if( !p)continue;
        copy_field(e.phone, sizeof e.phone, p);
        p =strtok(NULL, ",");
        if( !p)continue;
        copy_field(e.description, sizeof e.description, p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.latitude =atof(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.longitude =atof(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.severity =atoi(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.required_ambulance =atoi(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.required_facility =atoi(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.status =atoi(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.created_epoch =atol(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.assigned_ambulance_id =atoi(p);
        p =strtok(NULL, ",");
        if( !p)continue;
        e.assigned_hospital_id =atoi(p);
        IncidentNode *n =(IncidentNode *)malloc(sizeof *n);
        if( !n) {
            fclose(f);
            return 0;
        }
        n ->value =e;
        n ->next =s ->head;
        s ->head =n;
        s ->count ++;
        if(e.id >=s ->next_id)s ->next_id =e.id +1;
    }
    fclose(f);
    return 1;
}
