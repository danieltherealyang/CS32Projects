#include "Map.h"
#include <cassert>
#include <iostream>

int main() {
    Map m; //default constructor
    assert(m.empty()); //map initialized to empty
    assert(m.size() == 0); //size returns correct value after initialized
    assert(m.insert("1", 1)); //can insert elements
    assert(m.insert("5", 5));
    assert(m.insert("6", 6));
    assert(m.insert("4", 4));
    assert(m.insert("3", 3));
    assert(m.insert("2", 2));
    assert(!m.insert("1", 1)); //can't insert same element twice
    assert(!m.insert("1", 2)); //can't insert dependant on key not value
    KeyType k = "";
    ValueType v = 0;
    assert(m.get("1", v) && v == 1); //get with 2 parameters returns and sets value
    assert(m.get("2", v) && v == 2);
    assert(m.get("3", v) && v == 3);
    assert(m.get("4", v) && v == 4);
    assert(m.get("5", v) && v == 5);
    assert(m.get("6", v) && v == 6);
    assert(!m.get("7", v) && v == 6); //false and v unchanged if key not in list
    assert(!m.get("0", v) && v == 6);
    assert(m.get(0, k, v) && k == "1" && v == 1); //list in correct order and get with three parameters sets key and value correctly
    assert(m.get(1, k, v) && k == "2" && v == 2);
    assert(m.get(2, k, v) && k == "3" && v == 3);
    assert(m.get(3, k, v) && k == "4" && v == 4);
    assert(m.get(4, k, v) && k == "5" && v == 5);
    assert(m.get(5, k, v) && k == "6" && v == 6);
    assert(!m.empty()); //empty works when list has nodes
    assert(m.size() == 6); //map size counting correctly
    assert(m.update("1", 2)); //can update nodes
    assert(!m.update("18", 12)); //won't update key not in list
    assert(!m.update("", 3)); //doesn't update if key not in list
    assert(m.insert("", 0)); //doesn't treat blank string differently
    assert(m.erase("")); //erase head node
    assert(m.erase("1")); //erase head node
    assert(!m.erase("")); //false if key not in list
    assert(m.get(0, k, v) && k == "2" && v == 2); //list in correct order after erasing
    assert(m.get(1, k, v) && k == "3" && v == 3);
    assert(m.get(2, k, v) && k == "4" && v == 4);
    assert(m.get(3, k, v) && k == "5" && v == 5);
    assert(m.get(4, k, v) && k == "6" && v == 6);
    assert(m.erase("6")); //erase tail node
    assert(m.erase("5")); //erase tail node
    assert(m.erase("3")); //erase middle node
    assert(!m.erase("3")); //false if key not in list
    assert(m.get(0, k, v) && k == "2" && v == 2); //list in correct order after erasing
    assert(m.get(1, k, v) && k == "4" && v == 4);
    assert(m.contains("2")); //contains returns correct value
    assert(m.contains("4"));
    assert(m.size() == 2); //size returns correct value
    assert(!m.contains("")); //return false if key not in list
    assert(!m.contains("3"));
    assert(m.erase("2")); //erase remaining nodes
    assert(m.erase("4"));
    assert(m.empty()); //head node reset to nullptr after erasing all nodes
    assert(m.insertOrUpdate("7", 6)); //insertOrUpdate inserts nodes properly
    assert(m.insertOrUpdate("3", 2));
    assert(m.insertOrUpdate("2", 1));
    assert(m.insertOrUpdate("4", 3));
    assert(m.insertOrUpdate("5", 4));
    assert(m.insertOrUpdate("6", 5));
    assert(m.insertOrUpdate("1", -1)); //value can be negative
    assert(m.get(0, k, v) && k == "1" && v == -1); //list in correct order and get with three parameters sets key and value correctly
    assert(m.get(1, k, v) && k == "2" && v == 1);
    assert(m.get(2, k, v) && k == "3" && v == 2);
    assert(m.get(3, k, v) && k == "4" && v == 3);
    assert(m.get(4, k, v) && k == "5" && v == 4);
    assert(m.get(5, k, v) && k == "6" && v == 5);
    assert(m.get(6, k, v) && k == "7" && v == 6);
    assert(m.get("1", v) && v == -1); //elements can be retrieve correctly by key
    assert(m.get("2", v) && v == 1);
    assert(m.get("3", v) && v == 2);
    assert(m.get("4", v) && v == 3);
    assert(m.get("5", v) && v == 4);
    assert(m.get("6", v) && v == 5);
    assert(m.get("7", v) && v == 6);
    assert(m.insertOrUpdate("6", 6)); //insertOrUpdate updates nodes properly
    assert(m.insertOrUpdate("2", 2));
    assert(m.insertOrUpdate("1", 1));
    assert(m.insertOrUpdate("3", 3));
    assert(m.insertOrUpdate("4", 4));
    assert(m.insertOrUpdate("5", 5));
    assert(m.insertOrUpdate("7", 7));
    assert(m.get(0, k, v) && k == "1" && v == 1); //list in correct order and get with three parameters sets key and value correctly
    assert(m.get(1, k, v) && k == "2" && v == 2);
    assert(m.get(2, k, v) && k == "3" && v == 3);
    assert(m.get(3, k, v) && k == "4" && v == 4);
    assert(m.get(4, k, v) && k == "5" && v == 5);
    assert(m.get(5, k, v) && k == "6" && v == 6);
    assert(m.get(6, k, v) && k == "7" && v == 7);

    Map n = m; //copy constructor
    assert(n.get(0, k, v) && k == "1" && v == 1); //list copied to n correctly
    assert(n.get(1, k, v) && k == "2" && v == 2);
    assert(n.get(2, k, v) && k == "3" && v == 3);
    assert(n.get(3, k, v) && k == "4" && v == 4);
    assert(n.get(4, k, v) && k == "5" && v == 5);
    assert(n.get(5, k, v) && k == "6" && v == 6);
    assert(n.get(6, k, v) && k == "7" && v == 7);
    
    Map o;
    o.insert("1", 1);
    o.insert("5", 5);
    o.insert("6", 6);
    o.insert("4", 4);
    o.insert("3", 3);
    o.insert("2", 2);
    int size = o.size();
    o = o;
    assert(o.size() == size); //make sure assignment operator keeps list the same when assigning self
    o = n; //assignment operator
    assert(o.get(0, k, v) && k == "1" && v == 1); //previous nodes cleared
    assert(o.get(1, k, v) && k == "2" && v == 2); //n list copied correctly
    assert(o.get(2, k, v) && k == "3" && v == 3);
    assert(o.get(3, k, v) && k == "4" && v == 4);
    assert(o.get(4, k, v) && k == "5" && v == 5);
    assert(o.get(5, k, v) && k == "6" && v == 6);
    assert(o.get(6, k, v) && k == "7" && v == 7);
    assert(!o.get(7, k, v) && k == "7" && v == 7); //list is same length as n and parameters unchanged
    o.erase("6");
    o.erase("5");
    o.erase("2");
    o.swap(n); //swap lists
    assert(o.get(0, k, v) && k == "1" && v == 1); //o list set to n
    assert(o.get(1, k, v) && k == "2" && v == 2);
    assert(o.get(2, k, v) && k == "3" && v == 3);
    assert(o.get(3, k, v) && k == "4" && v == 4);
    assert(o.get(4, k, v) && k == "5" && v == 5);
    assert(o.get(5, k, v) && k == "6" && v == 6);
    assert(o.get(6, k, v) && k == "7" && v == 7);

    assert(n.get(0, k, v) && k == "1" && v == 1); //n list set to o
    assert(n.get(1, k, v) && k == "3" && v == 3);
    assert(n.get(2, k, v) && k == "4" && v == 4);
    assert(n.get(3, k, v) && k == "7" && v == 7);
    n.insert("9", 9);
    n.insert("asdf", 10);

    Map result;
    assert(merge(n, o, result)); //merge correctly for empty result list
    assert(result.get(0, k, v) && k == "1" && v == 1); //result list set correctly
    assert(result.get(1, k, v) && k == "2" && v == 2);
    assert(result.get(2, k, v) && k == "3" && v == 3);
    assert(result.get(3, k, v) && k == "4" && v == 4);
    assert(result.get(4, k, v) && k == "5" && v == 5);
    assert(result.get(5, k, v) && k == "6" && v == 6);
    assert(result.get(6, k, v) && k == "7" && v == 7);
    assert(result.get(7, k, v) && k == "9" && v == 9);
    assert(result.get(8, k, v) && k == "asdf" && v == 10);

    o.update("3", 1);
    o.update("4", 1);
    o.update("7", 1);
    n.erase("9");
    n.erase("asdf");

    assert(!merge(n, o, result)); //same keys with different value return false
    assert(result.get(0, k, v) && k == "1" && v == 1); //result list overwritten
    assert(result.get(1, k, v) && k == "2" && v == 2); //duplicate keys not in list
    assert(result.get(2, k, v) && k == "5" && v == 5);
    assert(result.get(3, k, v) && k == "6" && v == 6);

    n.insert("test", 13);

    assert(!merge(n, o, n)); //m1 and result maps are the same and with duplicate keys
    assert(n.get(0, k, v) && k == "1" && v == 1); //n 
    assert(n.get(1, k, v) && k == "2" && v == 2);
    assert(n.get(2, k, v) && k == "5" && v == 5);
    assert(n.get(3, k, v) && k == "6" && v == 6);
    assert(n.get(4, k, v) && k == "test" && v == 13);
    
    n.insert("asdf", 10);

    assert(merge(n, o, o)); //m2 and result maps are the same
    assert(o.get(0, k, v) && k == "1" && v == 1); //o merged correctly
    assert(o.get(1, k, v) && k == "2" && v == 2);
    assert(o.get(2, k, v) && k == "3" && v == 1);
    assert(o.get(3, k, v) && k == "4" && v == 1);
    assert(o.get(4, k, v) && k == "5" && v == 5);
    assert(o.get(5, k, v) && k == "6" && v == 6);
    assert(o.get(6, k, v) && k == "7" && v == 1);
    assert(o.get(7, k, v) && k == "asdf" && v == 10);

    o.update("3", 3);
    o.update("4", 4);
    o.update("7", 7);
    reassign(o, n);
    KeyType result_k;
    ValueType result_v;
    assert(n.get("1", result_v) && result_v != 1); //all nodes in o reassigned
    assert(n.get("2", result_v) && result_v != 2);
    assert(n.get("3", result_v) && result_v != 3);
    assert(n.get("4", result_v) && result_v != 4);
    assert(n.get("5", result_v) && result_v != 5);
    assert(n.get("6", result_v) && result_v != 6);
    assert(n.get("7", result_v) && result_v != 7);
    assert(n.get("asdf", result_v) && result_v != 10);

    reassign(o, o);
    assert(o.get("1", v) && v != 1); //o reassigned
    assert(o.get("2", v) && v != 2);
    assert(o.get("3", v) && v != 3);
    assert(o.get("4", v) && v != 4);
    assert(o.get("5", v) && v != 5);
    assert(o.get("6", v) && v != 6);
    assert(o.get("7", v) && v != 7);
    assert(o.get("asdf", v) && v != 10);

    std::cout << "Passed all tests" << std::endl;
}