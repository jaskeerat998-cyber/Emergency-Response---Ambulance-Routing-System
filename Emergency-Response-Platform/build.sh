#!/usr/bin/env bash
set -euo pipefail
mkdir -p build
gcc -std=c11 -Wall -Wextra -pedantic -c Part1_C_DataStructures/IncidentStore.c -o build/IncidentStore.o
gcc -std=c11 -Wall -Wextra -pedantic -c Part1_C_DataStructures/IncidentRules.c -o build/IncidentRules.o
gcc -std=c11 -Wall -Wextra -pedantic -c Part1_C_DataStructures/PriorityQueue.c -o build/PriorityQueue.o
g++ -std=c++17 -Wall -Wextra -pedantic -c Part2_CPP_Routing/GeoRouting.cpp -o build/GeoRouting.o
g++ -std=c++17 -Wall -Wextra -pedantic -c Part2_CPP_Routing/RouteAnalytics.cpp -o build/RouteAnalytics.o
g++ -std=c++17 -Wall -Wextra -pedantic -c Part2_CPP_Routing/InputValidation.cpp -o build/InputValidation.o
g++ -std=c++17 -Wall -Wextra -pedantic -c Part3_CPP_Dispatch/ResourceManager.cpp -o build/ResourceManager.o
g++ -std=c++17 -Wall -Wextra -pedantic -c Part3_CPP_Dispatch/DispatchAudit.cpp -o build/DispatchAudit.o
g++ -std=c++17 -Wall -Wextra -pedantic -c Part3_CPP_Dispatch/IncidentReporter.cpp -o build/IncidentReporter.o
g++ -std=c++17 -Wall -Wextra -pedantic -c Part3_CPP_Dispatch/DispatchEngine.cpp -o build/DispatchEngine.o
g++ -std=c++17 -Wall -Wextra -pedantic -c Part3_CPP_Dispatch/main.cpp -o build/main.o
g++ build/IncidentStore.o build/IncidentRules.o build/PriorityQueue.o build/GeoRouting.o build/RouteAnalytics.o build/InputValidation.o build/ResourceManager.o build/DispatchAudit.o build/IncidentReporter.o build/DispatchEngine.o build/main.o -o build/emergency-routing
echo "Built: ./build/emergency-routing"
