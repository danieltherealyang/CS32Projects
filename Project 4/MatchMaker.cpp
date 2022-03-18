#include "MatchMaker.h"
#include <string>
#include <algorithm>
#include <sstream>
#include <functional>

using namespace std;

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at) {
    m_mdb = &mdb;
    m_at = &at;
}

MatchMaker::~MatchMaker() {}

vector<EmailCount> MatchMaker::IdentifyRankedMatches(string email, int threshold) const {
    vector<EmailCount> sortedEmailCount;
    //key = email, look up number of compatible
    const PersonProfile* memberProfile = m_mdb->GetMemberByEmail(email);
    vector<AttValPair> memberAttributes;
    for (int i = 0; i < memberProfile->GetNumAttValPairs(); i++) {
        AttValPair attribPair;
        memberProfile->GetAttVal(i, attribPair);
        memberAttributes.push_back(attribPair);
    }
    vector<string> compatibleAttributes;
    for (int i = 0; i < memberAttributes.size(); i++) {
        vector<AttValPair> temp = m_at->FindCompatibleAttValPairs(memberAttributes[i]);
        for (int j = 0; j < temp.size(); j++) {
            compatibleAttributes.push_back(temp[j].attribute + "," + temp[j].value);
        }
    }
    //make unique compatible attributes
    sort(compatibleAttributes.begin(), compatibleAttributes.end());
    compatibleAttributes.erase(unique(compatibleAttributes.begin(), compatibleAttributes.end()), compatibleAttributes.end());
    vector<string> matchingMembers;
    for (int i = 0; i < compatibleAttributes.size(); i++) {
        AttValPair tempPair;
        stringToPair(compatibleAttributes[i], tempPair);
        vector<string> tempVector = m_mdb->FindMatchingMembers(tempPair);
        matchingMembers.insert(matchingMembers.end(), tempVector.begin(), tempVector.end());
    }
    sort(matchingMembers.begin(), matchingMembers.end());
    vector<EmailCount*> compAttribCount; //delete after function ends
    //edge cases
    if (matchingMembers.size() == 0 || matchingMembers.size() < threshold)
        return sortedEmailCount;
    if (matchingMembers.size() == 1 && threshold == 1) {
        sortedEmailCount.push_back(EmailCount(matchingMembers[0], 1));
    }

    string currentMember = matchingMembers[0];
    EmailCount* ctPtr;
    ctPtr = new EmailCount(currentMember, 1);
    for (int i = 1; i < matchingMembers.size(); i++) {
        if (matchingMembers[i] == currentMember)
            ctPtr->count++;
        else {
            if (ctPtr->count >= threshold)
                compAttribCount.push_back(ctPtr);
            else
                delete ctPtr;
            currentMember = matchingMembers[i];
            ctPtr = new EmailCount(currentMember, 1);
        }
    }
    if (ctPtr->count >= threshold)
        compAttribCount.push_back(ctPtr);
    else
        delete ctPtr;
    sort(compAttribCount.begin(), compAttribCount.end(), bind(&MatchMaker::ecPredicate, this, placeholders::_1, placeholders::_2));
    for (int i = 0; i < compAttribCount.size(); i++) {
        sortedEmailCount.push_back(*compAttribCount[i]);
        delete compAttribCount[i];
    }
    return sortedEmailCount;
}

void MatchMaker::stringToPair(string raw, AttValPair& pair) const {
    stringstream attribute; stringstream value;
    int index = 0;
    while (raw.at(index) != ',') {
        attribute << raw.at(index);
        index++;
    }
    pair.attribute = attribute.str();
    index++;
    while (index < raw.length()) {
        value << raw.at(index);
        index++;
    }
    pair.value = value.str();
}

bool MatchMaker::ecPredicate(EmailCount*& lhs, EmailCount*& rhs) const {
    if (lhs->count > rhs->count)
        return true;
    if (lhs->count == rhs->count) {
        if (lhs->email < rhs->email)
            return true;
        else
            return false;
    }
    return false;
}