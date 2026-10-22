#include "GeoRouting.h"
#include "RouteAnalytics.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
#include <algorithm>
GeoRouting::GeoRouting() {
    // Demo road-network nodes for Dehradun. Coordinates and edge weights are illustrative,
    // not a live or verified navigation dataset.
    nodes = {
        {
            0, "Clock Tower",  {
                30.3256, 78.0413
            }
        }, 
        {
            1, "Dehradun Railway Station",  {
                30.3165, 78.0322
            }
        }, 
        {
            2, "Graphic Era University",  {
                30.2672, 78.0030
            }
        }, 
        {
            3, "Jolly Grant Airport",  {
                30.1890, 78.1803
            }
        }, 
        {
            4, "Doon Hospital area",  {
                30.3245, 78.0437
            }
        }, 
        {
            5, "ISBT Dehradun",  {
                30.2865, 78.0090
            }
        }, 
        {
            6, "Prem Nagar",  {
                30.3500, 77.9650
            }
        }, 
        {
            7, "Patel Nagar",  {
                30.3160, 78.0120
            }
        }, 
        {
            8, "Pacific Mall area",  {
                30.3670, 78.0750
            }
        }, 
        {
            9, "Max Hospital area",  {
                30.3570, 78.0700
            }
        }
    };
    edges.resize(nodes.size());
    addRoad(0, 1, 2.1);
    addRoad(0, 4, 0.5);
    addRoad(0, 5, 5.2);
    addRoad(1, 5, 4.0);
    addRoad(1, 7, 3.0);
    addRoad(2, 5, 4.4);
    addRoad(2, 6, 6.8);
    addRoad(2, 7, 5.0);
    addRoad(3, 8, 18.0);
    addRoad(3, 9, 20.0);
    addRoad(4, 9, 5.1);
    addRoad(4, 8, 6.2);
    addRoad(5, 7, 2.8);
    addRoad(6, 7, 4.8);
    addRoad(7, 9, 6.7);
    addRoad(0, 8, 4.6);
}
void GeoRouting::addRoad(int a, int b, double km) {
    edges[a].push_back( {
        b, km
    });
    edges[b].push_back( {
        a, km
    });
}
bool GeoRouting::validCoordinate(Coordinate p) {
    return std::isfinite(p.lat)&&std::isfinite(p.lon)&&p.lat >=-90&&p.lat <=90&&p.lon >=-180&&p.lon <=180;
}
double GeoRouting::haversineKm(Coordinate a, Coordinate b) {
    const double rad =3.14159265358979323846 /180.0, R =6371.0;
    double p1 =a.lat *rad, p2 =b.lat *rad, dp =(b.lat -a.lat) *rad, dl =(b.lon -a.lon) *rad;
    double h =std::sin(dp /2) *std::sin(dp /2) +std::cos(p1) *std::cos(p2) *std::sin(dl /2) *std::sin(dl /2);
    h =std::max(0.0, std::min(1.0, h));
    return R *2 *std::atan2(std::sqrt(h), std::sqrt(1 -h));
}
int GeoRouting::nearestNode(Coordinate p)const {
    int best =-1;
    double d =std::numeric_limits <double >::max();
    for(const auto&n:nodes) {
        double x =haversineKm(p, n.point);
        if(x <d) {
            d =x;
            best =n.id;
        }
    }
    return best;
}
const RoadNode& GeoRouting::node(int id)const {
    return nodes.at(id);
}
int GeoRouting::nodeCount()const {
    return (int)nodes.size();
}
RouteResult GeoRouting::shortestRoute(Coordinate start, Coordinate dest)const {
    RouteResult r;
    if( !validCoordinate(start)|| !validCoordinate(dest)) {
        r.note ="Invalid latitude/longitude.";
        return r;
    }
    int s =nearestNode(start), t =nearestNode(dest);
    if(s <0||t <0) {
        r.note ="Could not map coordinates to road network.";
        return r;
    }
    if(haversineKm(start, nodes[s].point) >5.0||haversineKm(dest, nodes[t].point) >5.0) {
        r.note ="Coordinate is more than 5 km from the configured road network; refusing to invent a route.";
        return r;
    }
    std::vector <double >d(nodes.size(), std::numeric_limits <double >::infinity());
    std::vector <int >prev(nodes.size(),  -1);
    using Item =std::pair <double, int >;
    std::priority_queue <Item, std::vector <Item >, std::greater <Item >>q;
    d[s] =0;
    q.push( {
        0, s
    });
    while (!q.empty()) {
    auto current = q.top();
    double cost = current.first;
    int u = current.second;
    q.pop();

    if (cost != d[u]) continue;
    if (u == t) break;

    for (const auto& edge : edges[u]) {
        int v = edge.first;
        double w = edge.second;

        if (d[v] > cost + w) {
            d[v] = cost + w;
            prev[v] = u;
            q.push({d[v], v});
        }
    }
}
    if( !std::isfinite(d[t])) {
        r.note ="No connected road route found in the configured network.";
        return r;
    }
    for(int at =t;at !=-1;at =prev[at])r.nodeIds.push_back(at);
    std::reverse(r.nodeIds.begin(), r.nodeIds.end());
    r.found =true;
    r.distanceKm =d[t];
    // Transparent planning estimate: 25 km/h average, not a live ETA.
    r.estimatedMinutes =RouteAnalytics::estimateMinutes(d[t], 25.0);
    r.note ="Estimated using sample road graph and 25 km/h average; not live traffic.";
    return r;
}
std::vector <RoadNode > GeoRouting::nearbyNodes(Coordinate p, double radius)const {
    std::vector <RoadNode >out;
    for(const auto&n:nodes)if(haversineKm(p, n.point) <=radius)out.push_back(n);
    return out;
}
void GeoRouting::printNodes()const {
    std::cout <<"\nConfigured demo road nodes (illustrative, not live verified data):\n";
    for(const auto&n:nodes)std::cout <<"  " <<n.id <<" | " <<n.name <<" | " <<n.point.lat <<", " <<n.point.lon <<"\n";
}
