#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

void stringdoubletest()
{
        Map m;
        assert(m.insert("Fred", 2.956));
        assert(m.insert("Ethel", 3.538));
        assert(m.size() == 2);
        ValueType v = 42;
        assert(!m.get("Lucy", v) && v == 42);
        assert(m.get("Fred", v) && v == 2.956);
        v = 42;
        KeyType x = "Lucy";
        assert(m.get(0, x, v) &&
               ((x == "Fred" && v == 2.956) || (x == "Ethel" && v == 3.538)));
        KeyType x2 = "Ricky";
        assert(m.get(1, x2, v) &&
               ((x2 == "Fred" && v == 2.956) || (x2 == "Ethel" && v == 3.538)) &&
               x != x2);
}
/*
void intstringtest()
{
    Map m;
    assert(m.insert(10, "diez"));
    assert(m.insert(20, "veinte"));
    assert(m.size() == 2);
    ValueType v = "cuarenta y dos";
    assert(!m.get(30, v)  &&  v == "cuarenta y dos");
    assert(m.get(10, v)  &&  v == "diez");
    v = "cuarenta y dos";
    KeyType x = 30;
    assert(m.get(0, x, v)  &&
    ((x == 10  &&  v == "diez")  ||  (x == 20  &&  v == "veinte")));
    KeyType x2 = 40;
    assert(m.get(1, x2, v)  &&
    ((x2 == 10  &&  v == "diez")  ||  (x2 == 20  &&  v == "veinte"))  &&
    x != x2);
} */

int main()
{
        stringdoubletest();
        Map m; // maps strings to doubles
        assert(m.empty());
        ValueType v = -1234.5;
        assert(!m.get("abc", v) && v == -1234.5); // v unchanged by get failure
        m.insert("xyz", 9876.5);
        assert(m.size() == 1);
        KeyType k = "hello";
        assert(m.get(0, k, v) && k == "xyz" && v == 9876.5);

        Map n;
        assert(n.insert("a", 1));
        assert(!n.insert("a", 1));
        assert(!n.insert("a", 2));
        assert(n.insert("b", 2));
        assert(n.insert("c", 3));
        assert(n.insert("", 0));
        KeyType key = "asdf";
        ValueType value = 3;
        assert(n.get(0, key, value) && key == "" && value == 0);
        assert(n.get(1, key, value) && key == "a" && value == 1);
        assert(n.get(2, key, value) && key == "b" && value == 2);
        assert(n.get(3, key, value) && key == "c" && value == 3);

        Map mm;
        mm.insert("Little Ricky", 3.206);
        mm.insert("Ethel", 3.538);
        mm.insert("Ricky", 3.350);
        mm.insert("Lucy", 2.956);
        mm.insert("Ethel", 3.538);
        mm.insert("Fred", 2.956);
        mm.insert("Lucy", 2.956);
        assert(mm.size() == 5);  // duplicate "Ethel" and "Lucy" were not added
        string x;
        double y;
        mm.get(0, x, y);
        assert(x == "Ethel");  // "Ethel" is greater than exactly 0 items in mm
        mm.get(4, x, y);
        assert(x == "Ricky");  // "Ricky" is greater than exactly 4 items in mm
        mm.get(2, x, y);
        assert(x == "Little Ricky");  // "Little Ricky" is greater than exactly 2 items in mm
        assert(y == 3.206);  // the value corresponding to the key "Little Ricky"

        Map m1;
        m1.insert("Fred", 2.956);
        Map m2;
        m2.insert("Ethel", 3.538);
        m2.insert("Lucy", 2.956);
        m1.swap(m2);
        assert(m1.size() == 2  &&  m1.contains("Ethel")  &&  m1.contains("Lucy")  &&
        m2.size() == 1  &&  m2.contains("Fred"));
        Map gpas;
        gpas.insert("Fred", 2.956);
        assert(!gpas.contains(""));
        gpas.insert("Ethel", 3.538);
        gpas.insert("", 4.000);
        gpas.insert("Lucy", 2.956);
        assert(gpas.contains(""));
        gpas.erase("Fred");
        assert(gpas.size() == 3  &&  gpas.contains("Lucy")  &&  gpas.contains("Ethel")  &&
        gpas.contains(""));
        string s;
        double d;
        assert(gpas.get(1, s, d)  &&  s == "Ethel");
        assert(gpas.get(0, s, d)  &&  s == "");

        cout << "Passed all tests" << endl;
}