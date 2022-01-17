#include "newMap.h"

Map::Map() {
    m_pairs = new Pair[DEFAULT_MAX_ITEMS];
    m_maxItems = DEFAULT_MAX_ITEMS;
}

Map::~Map() {
    delete [] m_pairs;
}

Map::Map(int size) {
    m_pairs = new Pair[size];
    m_maxItems = size;
}

Map::Map(const Map& obj) {
    m_pairs = new Pair[obj.m_maxItems];

    for (int i = 0; i < obj.m_size; i++) {
        m_pairs[i].key = obj.m_pairs[i].key;
        m_pairs[i].value = obj.m_pairs[i].value;
    }

    m_size = obj.m_size;
    m_maxItems = obj.m_maxItems;
}

Map& Map::operator=(const Map& obj) {
    delete m_pairs;
    m_pairs = new Pair[obj.m_maxItems];
    for (int i = 0; i < obj.m_size; i++) {
        m_pairs[i].key = obj.m_pairs[i].key;
        m_pairs[i].value = obj.m_pairs[i].value;
    }

    m_size = obj.m_size;
    m_maxItems = obj.m_maxItems;
    return *this;
}

bool Map::empty() const {
    if (m_size == 0)
        return true;
    else
        return false;
}

int Map::size() const {
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    if (m_size >= m_maxItems)
        return false;
    
    int index = 0;
    while (index < m_size) {
        if (m_pairs[index].key == key)
            return false;
        if (m_pairs[index].key > key) {
            break;
        }
        index++;
    }

    for (int i = m_size; i > index; i--) {
        m_pairs[i].key = m_pairs[i - 1].key;
        m_pairs[i].value = m_pairs[i - 1].value;
    }

    m_pairs[index].key = key;
    m_pairs[index].value = value;
    m_size += 1;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    for (int i = 0; i < m_size; i++) {
        if (m_pairs[i].key == key) {
            m_pairs[i].value = value;
            return true;
        }

        if (m_pairs[i].key > key) {
            break;
        }
    }
    
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).
    for (int i = 0; i < m_size; i++) {
        if (m_pairs[i].key == key) {
            m_pairs[i].value = value;
            return true;
        }

        if (m_pairs[i].key > key)
            break;
    }

    return insert(key, value);
}

bool Map::erase(const KeyType& key) {
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    for (int i = 0; i < m_size; i++) {
        if (m_pairs[i].key == key) {
            for (int j = i; j < m_size - 1; j++) {
                m_pairs[j].key = m_pairs[j+1].key;
                m_pairs[j].value = m_pairs[j+1].value;
            }
            m_size -= 1;
            return true;
        }
    }
    return false;
}

bool Map::contains(const KeyType& key) const {
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    for (int i = 0; i < m_size; i++) {
        if (m_pairs[i].key == key)
            return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    // If key is equal to a key currently in the map, set value to the
    // value in the map which that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    for (int i = 0; i < m_size; i++) {
        if (m_pairs[i].key == key) {
            value = m_pairs[i].value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of the key/value pair in the map whose key is strictly
    // greater than exactly i keys in the map and return true.  Otherwise,
    // leave the key and value parameters unchanged and return false.
    if (i < 0 || i >= size())
        return false;
    
    key = m_pairs[i].key;
    value = m_pairs[i].value;
    return true;
}

void Map::swap(Map& other) {
    Pair* temp = m_pairs;
    m_pairs = other.m_pairs;
    other.m_pairs = temp;

    int size = m_size;
    m_size = other.m_size;
    other.m_size = size;

    int max = m_maxItems;
    m_maxItems = other.m_maxItems;
    other.m_maxItems = max;
}

void Map::dump() const {

}