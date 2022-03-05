#ifndef MEMBER_DATABASE_H
#define MEMBER_DATABASE_H

#include "provided.h"
#include "PersonProfile.h"
#include "RadixTree.h"
#include <vector>
#include <string>

class MemberDatabase {
    public:
        MemberDatabase();
        ~MemberDatabase();
        bool LoadDatabase(std::string filename);
        std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
        const PersonProfile* GetMemberByEmail(std::string email) const;
    private:
        RadixTree<PersonProfile*> m_profileTree; //search by email
        RadixTree<std::vector<std::string>*> m_attributeTree; //search by AttValPair string
        std::string parseAttribute(std::string raw, int& afterComma) const;
};

#endif