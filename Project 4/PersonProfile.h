#ifndef PERSON_PROFILE_H
#define PERSON_PROFILE_H

#include "provided.h"
#include "RadixTree.h"
#include <vector>

class PersonProfile {
    public:
        PersonProfile(std::string name, std::string email);
        std::string GetName() const;
        std::string GetEmail() const;
        void AddAttValPair(const AttValPair& attval);
        int GetNumAttValPairs() const;
        bool GetAttVal(int attribute_num, AttValPair& attval) const;
    private:
        struct AttribMap {
            std::string uniqueAttrib;
            int values = 1;
        };
        std::string m_name;
        std::string m_email;
        int m_numPairs;
        std::vector<AttribMap> m_attributes;
        RadixTree<std::vector<std::string>*> m_attributeTree;
};

#endif