#include "MemberDatabase.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;
int main() {
    MemberDatabase database;
    database.LoadDatabase("members.txt");
    const PersonProfile* profile = database.GetMemberByEmail("BishopPitts3090@juno.com");
    cout << "BishopPitts3090@juno.com: " << endl;
    cout << profile->GetName() << "," << profile->GetEmail() << endl;
    for (int i = 0; i < profile->GetNumAttValPairs(); i++) {
        AttValPair av;
        profile->GetAttVal(i, av);
        std::cout << av.attribute << "," << av.value << std::endl;
    }
    profile = database.GetMemberByEmail("MarceloBrown1@earthlink.net");
    cout << "MarceloBrown1@earthlink.net: " << endl;
    cout << profile->GetName() << "," << profile->GetEmail() << endl;
    for (int i = 0; i < profile->GetNumAttValPairs(); i++) {
        AttValPair av;
        profile->GetAttVal(i, av);
        std::cout << av.attribute << "," << av.value << std::endl;
    }
}