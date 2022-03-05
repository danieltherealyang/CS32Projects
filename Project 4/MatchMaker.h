#ifndef MATCH_MAKER_H
#define MATCH_MAKER_H

#include "provided.h"
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include <vector>
#include <string>

class MatchMaker {
    public:
        MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
        ~MatchMaker();
        std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
    private:
        const MemberDatabase* m_mdb;
        const AttributeTranslator* m_at;
        void stringToPair(std::string raw, AttValPair& pair) const;
        bool ecPredicate(EmailCount*& lhs, EmailCount*& rhs) const;
};

#endif