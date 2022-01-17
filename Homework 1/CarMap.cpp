#include "CarMap.h"
#include <iostream>

using namespace std;

CarMap::CarMap() {

}

bool CarMap::addCar(std::string license) {
    // If a car with the given license plate is not currently in the map, 
    // and there is room in the map, add an entry for that car recording
    // that it has been driven 0 miles, and return true.  Otherwise,
    // make no change to the map and return false.
    return m_map.insert(license, 0);
}

double CarMap::miles(std::string license) const {
    // If a car with the given license plate is in the map, return how
    // many miles it has been driven; otherwise, return -1.
    double milesDriven;
    if (m_map.get(license, milesDriven))
        return milesDriven;
    else
        return -1;        
}

bool CarMap::drive(std::string license, double distance) {
    // If no car with the given license plate is in the map or if
    // distance is negative, make no change to the map and return
    // false.  Otherwise, increase by the distance parameter the number
    // of miles the indicated car has been driven and return true.
    double currentDistance; 
    if (!m_map.get(license, currentDistance) || distance < 0)
        return false;
    else {
        m_map.update(license, currentDistance + distance); 
        return true;
    }
}

int CarMap::fleetSize() const {
    return m_map.size();
}

void CarMap::print() const  {
    // Write to cout one line for every car in the map.  Each line
    // consists of the car's license plate, followed by one space,
    // followed by the number of miles that car has been driven.  Write
    // no other text.  The lines need not be in any particular order.
    for (int i = 0; i < m_map.size(); i++) {
        string license;
        double miles;
        m_map.get(i, license, miles);
        cout << license << " " << miles << endl;
    }
}