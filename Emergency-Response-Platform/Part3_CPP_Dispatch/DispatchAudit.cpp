#include "DispatchAudit.h"
#include <fstream>
#include <ctime>
#include <iomanip>
DispatchAudit::DispatchAudit(std::string path):filePath(std::move(path)) {
}
void DispatchAudit::record(const std::string&event)const {
    std::ofstream f(filePath, std::ios::app);
    if( !f)return;
    std::time_t now =std::time(nullptr);
    f <<std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") <<" | " <<event <<"\n";
}
