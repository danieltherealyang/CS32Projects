#include "AttributeTranslator.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

AttributeTranslator::AttributeTranslator() {
}

AttributeTranslator::~AttributeTranslator() {
}

bool AttributeTranslator::Load(string filename) {
    ifstream file(filename);
    if (file.fail())
        return false;
    string line;
    while (getline(file, line)) {
        if (line == "")
            continue;
        int attribIndex;
        string key = parseKey(line, attribIndex);
        AttValPair nextPair;
        loadPair(line.substr(attribIndex, line.length() - attribIndex), nextPair);
        vector<AttValPair>** pairVector = m_attributeTree.search(key);
        if (pairVector == nullptr) {
            vector<AttValPair>* newVector = new vector<AttValPair>;
            newVector->push_back(nextPair);
            m_attributeTree.insert(key, newVector);
        } else {
            //check duplication
            vector<AttValPair>::iterator it;
            bool repeats = false;
            for (it = (*pairVector)->begin(); it != (*pairVector)->end(); it++) {
                if (nextPair == *it) {
                    repeats = true;
                    break;
                }
            }
            if (!repeats)
                (*pairVector)->push_back(nextPair);
        }
    }
    return true;
}

vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const {
    string key = pairToKey(source);
    vector<AttValPair> pairVector;
    vector<AttValPair>** treePtr = m_attributeTree.search(key);
    
    if (treePtr != nullptr) {
        pairVector = **treePtr;
    }

    return pairVector;
}

string AttributeTranslator::pairToKey(const AttValPair& key) const {
    return key.attribute + "," + key.value;
}

string AttributeTranslator::parseKey(string raw, int& afterComma) const {
    stringstream key;
    int commas = 0;
    int index = 0;
    while (commas < 1) {
        key << raw.at(index);
        if (raw.at(index) == ',')
            commas++;
        index++;
    }

    while (raw.at(index) != ',') {
        key << raw.at(index);
        index++;
    }

    afterComma = index + 1;
    return key.str();
}

void AttributeTranslator::loadPair(std::string attVal, AttValPair& pair) {
    stringstream attribute;
    stringstream value;
    int index = 0;
    while (attVal.at(index) != ',') {
        attribute << attVal.at(index);
        index++;
    }
    index++;
    while (index < attVal.length()) {
        value << attVal.at(index);
        index++;
    }

    pair.attribute = attribute.str();
    pair.value = value.str();
}