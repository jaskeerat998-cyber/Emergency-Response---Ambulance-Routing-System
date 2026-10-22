#ifndef DISPATCH_AUDIT_H
#define DISPATCH_AUDIT_H
#include <string>
class DispatchAudit  {
    public:
    explicit DispatchAudit(std::string path ="dispatch_audit.log");
    void record(const std::string& event) const;
    private:
    std::string filePath;
};
#endif
