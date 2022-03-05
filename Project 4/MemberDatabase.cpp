#include "MemberDatabase.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;
MemberDatabase::MemberDatabase() {}

MemberDatabase::~MemberDatabase() {}

bool MemberDatabase::LoadDatabase(string filename) {
    ifstream file(filename);
    if (file.fail())
        return false;
    int recordNum = 1;

    string line; string name; string email; int remainAttrib = 0;
    PersonProfile* memberProfile = nullptr; vector<vector<string>*> mailLists;
    while (getline(file, line)) {
        if (line == "") {
            recordNum = 1;
            memberProfile = nullptr;
            continue;
        }
        
        PersonProfile** profilePtr;
        switch (recordNum) {
            case 1:
                name = line;
                recordNum++;
                continue;
            case 2:
                email = line;
                profilePtr = m_profileTree.search(email);
                if (profilePtr != nullptr)
                    return false;
                recordNum++;
                continue;
            case 3:
                remainAttrib = stoi(line);
                recordNum++;
                continue;
        }

        vector<string>** emailList;
        if (remainAttrib > 0) {
            int valIndex;
            string attribute = parseAttribute(line, valIndex);
            string value = line.substr(valIndex, line.length() - valIndex);
            //add attribute to m_profileTree
            if (memberProfile == nullptr) {
                memberProfile = new PersonProfile(name, email);
                m_profileTree.insert(email, memberProfile);
            }
            AttValPair attribPair; attribPair.attribute = attribute; attribPair.value = value;
            memberProfile->AddAttValPair(attribPair); //automatically filters duplicates
            //add email to m_attributeTree vector
            vector<string>** emailList = m_attributeTree.search(line);
            if (emailList == nullptr) {
                vector<string>* newEmailList = new vector<string>;
                newEmailList->push_back(email);
                m_attributeTree.insert(line, newEmailList);
                mailLists.push_back(newEmailList);
            } else {
                (*emailList)->push_back(email);
            }
            //decrement counter
            remainAttrib--;
        }
    }
    //remove email m_attributeTree duplicates
    for (int i = 0; i < mailLists.size(); i++) {
        sort(mailLists[i]->begin(), mailLists[i]->end());
        mailLists[i]->erase(unique(mailLists[i]->begin(), mailLists[i]->end()), mailLists[i]->end());
    }
    return true;
}

vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
    vector<string> matches;
    string key = input.attribute + "," + input.value;
    vector<string>** treeVector = m_attributeTree.search(key);
    if (treeVector != nullptr) {
        matches = **treeVector;
    }
    return matches;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const {
    PersonProfile** profilePtr = m_profileTree.search(email);
    PersonProfile* profile = (profilePtr == nullptr) ? nullptr : *profilePtr;
    return profile;
}

string MemberDatabase::parseAttribute(string raw, int& afterComma) const {
    stringstream attribute;
    int index = 0;

    while (raw.at(index) != ',') {
        attribute << raw.at(index);
        index++;
    }

    afterComma = index + 1;
    return attribute.str();
}