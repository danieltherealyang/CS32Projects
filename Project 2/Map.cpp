#include "Map.h"
#include <iostream>

Map::Map() {
    head = nullptr; //initialize head pointer
}

Map::~Map() {
    Node* current = head; //dummy pointer
    if (current != nullptr) { //loop through entire list
        while (current->next != nullptr) { //move to last node in list
            current = current->next;
        }

        while (current->previous != nullptr) { //backtrack then delete node
            current = current->previous;
            delete current->next;
        }

        delete current; //delete head node
    }
}

Map::Map(const Map& obj) {
    head = nullptr; //initialize head pointer
    Node* objCurrent = obj.head; //obj counter start at obj.head
    Node* mapCurrent = head; //map counter start at head
    while (objCurrent != nullptr) { //loop through entire obj list
        if (mapCurrent == nullptr) { //if copying first obj node
            mapCurrent = new Node; //copy first obj node and only increment obj counter
            mapCurrent->Key = objCurrent->Key;
            mapCurrent->Value = objCurrent->Value;
            head = mapCurrent; //assign head
            objCurrent = objCurrent->next;
            continue;
        }
        //obj counter should be 1 node ahead of map counter
        Node* newNode = new Node;
        newNode->Key = objCurrent->Key; //newNode copy of obj counter
        newNode->Value = objCurrent->Value;
        newNode->previous = mapCurrent; //link newNode with mapCurrent
        mapCurrent->next = newNode;
        mapCurrent = mapCurrent->next; //increment map and obj counter
        objCurrent = objCurrent->next;
    }
}

Map& Map::operator=(const Map& obj) {
    if (&obj == this)
        return *this;
    Node* current = head; //dummy node
    if (current != nullptr) { //delete all nodes of list
        while (current->next != nullptr) {
            current = current->next;
        }

        while (current->previous != nullptr) {
            current = current->previous;
            delete current->next;
        }

        delete current;
    }

    head = nullptr; //reassign head node to null
    Node* objCurrent = obj.head; //obj counter
    Node* mapCurrent = head; //map counter
    while (objCurrent != nullptr) {
        if (mapCurrent == nullptr) { //if first obj node being copied
            mapCurrent = new Node; //copy to mapCurrent then increment only obj counter
            mapCurrent->Key = objCurrent->Key;
            mapCurrent->Value = objCurrent->Value;
            head = mapCurrent; //assign head
            objCurrent = objCurrent->next;
            continue;
        }
        //obj counter should be 1 node ahead of map counter
        Node* newNode = new Node;
        newNode->Key = objCurrent->Key; //newNode copy of obj counter
        newNode->Value = objCurrent->Value;
        newNode->previous = mapCurrent; //link newNode with mapCurrent
        mapCurrent->next = newNode;
        mapCurrent = mapCurrent->next; //increment map and obj counter
        objCurrent = objCurrent->next;
    }
    return *this;
}

bool Map::empty() const {
    return head == nullptr;
}

int Map::size() const {
    Node* current = head; //dummy node
    int count = 0; //counter
    while (current != nullptr) { //increment dummy node until falls off the list
        count++;
        current = current->next;
    }
    return count;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    if (head == nullptr) { //assign key and value to head node if nullptr
        head = new Node;
        head->Key = key;
        head->Value = value;
        return true;
    }

    if (head->Key > key) { //add node before head and reassign head pointer
        Node* newNode = new Node; //ensures that list is ordered from least to greatest
        newNode->Key = key;
        newNode->Value = value;
        newNode->next = head;
        head->previous = newNode;
        head = newNode;
        return true;
    }

    Node* current = head;
    while (current != nullptr) { //loop through all nodes
        if (current->Key == key) //key already in list
            return false;
        if (current->Key > key) { //current key is greater; key not in list
            current = current->previous; //backtrack
            break;
        }

        if (current->next != nullptr) //break if reached the end of list
            current = current->next;
        else
            break;
    }

    if (current->next == nullptr) { //new node added to end of list
        Node* newNode = new Node; //create new node with key and value
        newNode->Key = key;
        newNode->Value = value;
        newNode->previous = current; //link new node to end
        current->next = newNode;
        return true;
    }
    //insert node between two existing nodes
    Node* newNode = new Node; //create node with key and value
    newNode->Key = key;
    newNode->Value = value;
    newNode->previous = current; //link new node pointers
    newNode->next = current->next;
    current->next->previous = newNode; //link surrounding nodes to new node
    current->next = newNode;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
    Node* current = head; //dummy node
    while (current != nullptr) { //loop through list
        if (current->Key == key) { //set value and return if key is equal
            current->Value = value;
            return true;
        }
        
        current = current->next;
    }

    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if (head == nullptr) { //assign key and value to head node if nullptr
        head = new Node;
        head->Key = key;
        head->Value = value;
        return true;
    }

    if (head->Key > key) { //add node before head and reassign head pointer
        Node* newNode = new Node;
        newNode->Key = key;
        newNode->Value = value;
        newNode->next = head;
        head->previous = newNode;
        head = newNode;
        return true;
    }

    Node* current = head;
    while (current != nullptr) { //loop through list
        if (current->Key == key) { //set value and return if key equal
            current->Value = value;
            return true;
        }

        if (current->Key > key) { //current key is greater; key not in list
            current = current->previous; //backtrack
            break;
        }

        if (current->next != nullptr) //break if reached end of list
            current = current->next;
        else
            break;
    }

    if (current->next == nullptr) { //new node added to end of list
        Node* newNode = new Node;
        newNode->Key = key;
        newNode->Value = value;
        newNode->previous = current;
        current->next = newNode;
        return true;
    }

    Node* newNode = new Node; //insert node between two existing nodes
    newNode->Key = key; //create new node with key and value
    newNode->Value = value;
    newNode->previous = current; //link new node pointers
    newNode->next = current->next;
    current->next->previous = newNode; //link surrounding nodes to new node
    current->next = newNode;
    return true;
}

bool Map::erase(const KeyType& key) {
    Node* current = head; //dummy node
    
    while (current != nullptr) { //loop through list
        if (current->Key == key) { //stop when node with key found
            break;
        }

        if (current->Key > key) //if reached node w/ key greater than parameter, key not in list
            return false;
        
        current = current->next;    
    }

    if (current == nullptr) //no nodes in list, return false
        return false;
    
    //current->Key == key
    if (current->previous == nullptr) { //check if deleting first node
        if (current->next != nullptr) { //if more than 1 node in list
            head = current->next; //reassign head to 2nd node
            current->next->previous = nullptr; //remove 2nd node pointer to head
        } else
            head = nullptr; //only 1 node in list
        delete current;
        return true;
    }

    //current->previous != nullptr
    if (current->next == nullptr) { //check if last node in list
        current->previous->next = nullptr; //remove 2nd to last node's pointer to current
        delete current;
        return true;
    }

    //delete node between two surrounding nodes
    current->previous->next = current->next;
    current->next->previous = current->previous;
    delete current;
    return true;
}

bool Map::contains(const KeyType& key) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->Key == key)
            return true;
        current = current->next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    Node* current = head; //dummy node
    while (current != nullptr) { //loop through list
        if (current->Key == key) { //if node with key found, assign value
            value = current->Value;
            return true;
        }

        if (current->Key > key) //node not in list
            return false;

        current = current->next;
    }

    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (i >= size() || i < 0) //check if i falls in bounds
        return false;
    int count = 0;
    Node* current = head;
    while (count < i) { //increment count and current node pointer i times
        current = current->next;
        count++;
    }
    key = current->Key; //set parameters
    value = current->Value;
    return true;
}

void Map::swap(Map& other) {
    Node* temp = head; //exchange head pointers
    head = other.head;
    other.head = temp;
}

bool merge(const Map& m1, const Map& m2, Map& result) {
    const Map* otherMap;

    if (&m1 == &result) { //in case either m1, m2, or both are the same Map object
        otherMap = &m2;
    } else if (&m2 == &result) {
        otherMap = &m1;
    } else {
        result = m1;
        otherMap = &m2;
    }
    
    bool ret_val = true;

    for (int i = 0; i < otherMap->size(); i++) { //loop through elements of otherMapt
        KeyType otherMap_key; ValueType otherMap_value;
        otherMap->get(i, otherMap_key, otherMap_value); //store key and value of otherMap at index i

        ValueType result_value;

        if (result.get(otherMap_key, result_value) && otherMap_value != result_value) { //if otherMap_key is in result and values don't match
            result.erase(otherMap_key); //erase m2_key
            ret_val = false;
            continue;
        }

        result.insert(otherMap_key, otherMap_value); //returns false and does nothing if key already in result
    }

    return ret_val;
}

void reassign(const Map& m, Map& result) {
    KeyType head_key; //store key and value of the head node
    ValueType head_value;
    m.get(0, head_key, head_value);

    result = m;

    KeyType m_key; ValueType m_value;
    KeyType prev_key = head_key;
    for (int i = 1; i < m.size() - 1; i++) { //loop through m by index
        m.get(i, m_key, m_value); //store key and value of m's node
        
        result.update(prev_key, m_value); //shift value back 1 node
        prev_key = m_key;
    }
    result.update(prev_key, head_value); //loop head node's value back around to tail node
}