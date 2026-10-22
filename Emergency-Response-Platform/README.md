# Emergency Response & Ambulance Routing System
## Dispatcher-assisted decision support prototype (C + C++)

### Three top-level source folders only (no nested module folders)
- `Part1_C_DataStructures/`: `IncidentStore.c/.h` (linked-list storage and CSV persistence), `PriorityQueue.c/.h` (severity ordering), and `IncidentRules.c/.h` (input validation and shared priority/status rules).
- `Part2_CPP_Routing/`: `GeoRouting.cpp/.h` (weighted graph and Dijkstra), `InputValidation.cpp/.h` (validated console input), and `RouteAnalytics.cpp/.h` (ETA estimate and route-path formatting).
- `Part3_CPP_Dispatch/`: `DispatchEngine.cpp/.h` (workflow orchestration), `ResourceManager.cpp/.h` (ambulance/hospital eligibility and reservation), `DispatchAudit.cpp/.h` (timestamped audit trail), `IncidentReporter.cpp/.h` (incident, queue and summary reports), `DispatchModels.h` (shared models), and `main.cpp` (console UI).

### Build
**Windows (GCC/G++ via MinGW):** open PowerShell in this folder and run `.\build.ps1`, then `.\build\emergency-routing.exe`.
**Linux/WSL:** `bash build.sh`, then `./build/emergency-routing`.

### Practical workflow
1. Register a call with caller contact, description, latitude/longitude, severity and resource requirements.
2. Review the severity-ordered queue.
3. Dispatch the highest-priority incident for which compatible ambulance and hospital resources are available.
4. Inspect resources, adjust waiting incident severity, cancel waiting incidents, complete dispatched incidents, and view the audit log.
5. Incident records persist to `incidents.csv`; key operational events append to `dispatch_audit.log`.

### Important limitations and safety
This is a functional local prototype, **not an operational emergency service**. The road graph, ambulance locations, hospital facilities and bed counts are demonstration data and must be replaced with verified feeds before field use. The router refuses to produce a route when either coordinate is more than 5 km from the configured graph. The route estimate uses a simplified graph and a fixed 25 km/h average; it does not include live traffic, closures, turn restrictions, one-way streets, dispatch radio, GPS tracking or geocoding. Hospital capacity is only simulated. It does not call emergency services. Do not enter real patient-identifiable or sensitive information into this demo. In a real deployment, add authentication/roles, encrypted transport and storage, privacy controls, reliable database transactions, backups, monitoring, verified medical/dispatch protocols, live map/routing provider, hospital integrations and human dispatcher oversight.



## Dispatch Module

The Dispatch Module coordinates emergency incident handling,
ambulance allocation, hospital selection, and incident completion.

It consists of DispatchEngine, ResourceManager, IncidentReporter,
and DispatchAudit.



## Route Planning

The routing module uses a configured road graph
to calculate routes between locations.

## Resource Management

ResourceManager evaluates ambulance availability
and suitability and helps select an appropriate hospital.



## Route Planning

The routing module uses a configured road graph
to calculate routes between locations.