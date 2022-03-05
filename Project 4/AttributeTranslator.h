#ifndef ATTRIBUTE_TRANSLATOR_H
#define ATTRIBUTE_TRANSLATOR_H

#include "provided.h"
#include "RadixTree.h"
#include <vector>
#include <string>

class AttributeTranslator {
    public:
        AttributeTranslator();
        ~AttributeTranslator();
        bool Load(std::string filename);
        std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
    private:
        RadixTree<std::vector<AttValPair>*> m_attributeTree;
        std::string pairToKey(const AttValPair& key) const;
        std::string parseKey(std::string raw, int& afterComma) const;
        void loadPair(std::string attVal, AttValPair& pair);
};

#endif