#include "CarMap.h"
#include <cassert>
#include <iostream>

int main() {
    CarMap carMap;
    assert(carMap.addCar("car1"));
    assert(carMap.fleetSize() == 1);
    assert(!carMap.addCar("car1"));
    assert(carMap.addCar("car2"));
    assert(carMap.fleetSize() == 2);
    assert(!carMap.addCar("car1"));
    assert(!carMap.addCar("car2"));
    assert(carMap.addCar(""));
    assert(carMap.fleetSize() == 3);

    assert(!carMap.drive("asdf", 13));
    assert(carMap.drive("car1", 0));
    assert(!carMap.drive("asdf", -13));
    assert(!carMap.drive("car2", -100));
    assert(!carMap.drive("car1", -100));
    assert(carMap.drive("car1", 100));
    assert(carMap.drive("car2", 19));
    assert(carMap.drive("car2", 15));
    assert(carMap.drive("", 21));
    

    assert(abs(carMap.miles("car1") - 100) < 0.000001);
    assert(abs(carMap.miles("car2") - 34) < 0.000001);
    assert(abs(carMap.miles("") - 21) < 0.000001);
    assert(abs(carMap.miles("asdfdfsa") + 1) < 0.000001);

    assert(carMap.fleetSize() == 3);
    std::cout << "Passed all tests; Check if output below prints all cars" << std::endl;
    carMap.print();
}