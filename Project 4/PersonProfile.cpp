#include "PersonProfile.h"

using namespace std;

PersonProfile::PersonProfile(string name, string email) {
    m_name = name;
    m_email = email;
    m_numPairs = 0;
}

string PersonProfile::GetName() const {
    return m_name;
}

string PersonProfile::GetEmail() const {
    return m_email;
}

void PersonProfile::AddAttValPair(const AttValPair& attval) {
    vector<string>** valPtr = m_attributeTree.search(attval.attribute);
    if (valPtr == nullptr) {
        vector<string>* newVector = new vector<string>;
        newVector->push_back(attval.value);
        m_attributeTree.insert(attval.attribute, newVector);
        m_numPairs++;
        AttribMap attribute; attribute.uniqueAttrib = attval.attribute;
        m_attributes.push_back(attribute);
        return;
    }

    vector<string>::iterator it;
    for (it = (*valPtr)->begin(); it != (*valPtr)->end(); it++) {
        if (*it == attval.value)
            return;
    }

    (*valPtr)->push_back(attval.value);
    m_numPairs++;
    vector<AttribMap>::iterator iter;
    for (iter = m_attributes.begin(); iter != m_attributes.end(); iter++) {
        if (iter->uniqueAttrib == attval.attribute) {
            iter->values++;
            break;
        }
    }
}

int PersonProfile::GetNumAttValPairs() const {
    return m_numPairs;
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const {
    vector<AttribMap>::const_iterator it = m_attributes.begin();
    while (it != m_attributes.end() && it->values <= attribute_num) {
        attribute_num -= it->values;
        it++;
    }

    if (it == m_attributes.end())
        return false;
    string key = it->uniqueAttrib;
    vector<string>** valPtr = m_attributeTree.search(key);
    string value = (*valPtr)->at(attribute_num);
    attval.attribute = key;
    attval.value = value;
    return true;
}