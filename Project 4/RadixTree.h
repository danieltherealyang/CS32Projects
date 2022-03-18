#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <string>
#include <iostream>
#include <type_traits>

template <typename ValueType>
class RadixTree {
    public:
        RadixTree();
        ~RadixTree();
        void insert(std::string key, const ValueType& value);
        ValueType* search(std::string key) const;
    private:
        struct Node {
            std::string postfix = "";
            Node* character[128] = {nullptr};
            ValueType value;
        };

        Node* m_head;
        
        template <typename U = ValueType>
        typename std::enable_if<std::is_pointer<U>::value, void>::type 
        deleteNode(Node* node) {
            if (node == nullptr)
                return;
            delete node->value;
            for (int i = 0; i < 128; i++) {
                deleteNode(node->character[i]);
            }
            delete node;
        };

        template <typename U = ValueType>
        typename std::enable_if<!std::is_pointer<U>::value, void>::type 
        deleteNode(Node* node) {
            if (node == nullptr)
                return;
            for (int i = 0; i < 128; i++) {
                deleteNode(node->character[i]);
            }
            delete node;
        };
};

template <typename ValueType>
RadixTree<ValueType>::RadixTree() {
    m_head = new Node;
}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree() {
    deleteNode(m_head);
}

template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value) {
    if (key == "")
        return;
    Node* previous = nullptr;
    Node* current = m_head;
    int keyIndex = 0;
    int charIndex = int(key.at(keyIndex));
    while (current->character[charIndex] != nullptr && keyIndex < key.length() - 1) {
        previous = current;
        current = current->character[charIndex];
        keyIndex++;
        charIndex = int(key.at(keyIndex));
    }

    if (current->character[charIndex] != nullptr && keyIndex == key.length() - 1) { //last character doesn't reassign charIndex
        previous = current;
        current = current->character[charIndex];
        keyIndex++;
    }

    if (current->postfix == "" && keyIndex < key.length()) { //rest of key is unique
        current->character[charIndex] = new Node;
        keyIndex++;
        current = current->character[charIndex];
        current->postfix = key.substr(keyIndex, key.length() - keyIndex);
        current->character[0] = new Node;
        current->character[0]->value = value;
        return;
    }

    if (current->postfix != "" && keyIndex < key.length()) { //rest of key might have repeating characters
        int repeating = 0;
        int postfixIndex = 0;
        int repKeyIndex = keyIndex - 1;
        charIndex = int(key.at(repKeyIndex));
        while (postfixIndex < current->postfix.length() && keyIndex < key.length() &&
        current->postfix.at(postfixIndex) == key.at(keyIndex)) {
            repeating++;
            postfixIndex++;
            keyIndex++;
        }

        if (repeating != 0) { //repeating characters
            repKeyIndex = 0;
            while (repKeyIndex < repeating) { //new nodes for repeating characters
                previous->character[charIndex] = new Node;
                previous = previous->character[charIndex];
                charIndex = int(current->postfix.at(repKeyIndex));
                repKeyIndex++;
            } //previous is at last repeating characters, charIndex node not created yet
            if (repKeyIndex == current->postfix.length()) { //current all repeating, charIndex point to current value
                current->postfix = "";
                previous->character[charIndex] = current;
                previous = previous->character[charIndex];
            } else { //have charIndex point to new node first non-repeating char of current
                previous->character[charIndex] = new Node;
                previous = previous->character[charIndex];
                charIndex = int(current->postfix.at(repKeyIndex));
                previous->character[charIndex] = current;
                current->postfix = current->postfix.substr(repKeyIndex + 1, current->postfix.length() - repKeyIndex);
            }
        } else { //no repeating characters
            previous->character[charIndex] = new Node;
            previous = previous->character[charIndex];
            charIndex = current->postfix.at(0);
            previous->character[charIndex] = current;
            current->postfix = current->postfix.substr(1, current->postfix.length() - 1);
        }
        if (keyIndex < key.length()) {
            charIndex = int(key.at(keyIndex));
            previous->character[charIndex] = new Node;
            previous = previous->character[charIndex];
            keyIndex++;
        }
        previous->postfix = key.substr(keyIndex, key.length() - keyIndex);
        previous->character[0] = new Node;
        previous->character[0]->value = value;
        return;
    }

    if (keyIndex == key.length()) {
        if (current->character[0] == nullptr)
            current->character[0] = new Node;
        current->character[0]->value = value;
        return;
    }
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) const {
    if (key == "")
        return nullptr;
    //dummy node, key and char index setup
    Node* current = m_head;
    int keyIndex = 0;
    int charIndex = int(key.at(keyIndex));
    //loop through second to last char of key or until no more nodes to follow
    while (current->character[charIndex] != nullptr && keyIndex < key.length() - 1) {
        current = current->character[charIndex];
        keyIndex++;
        charIndex = int(key.at(keyIndex));
    }
    //follow final character of key if node exists
    if (current->character[charIndex] != nullptr && keyIndex == key.length() - 1) {
        current = current->character[charIndex];
        keyIndex++;
    }

    ValueType* valPointer;
    //if looped through all of the chars in key
    if (keyIndex == key.length()) {
        //check if val exists or just a substring of another key
        valPointer = (current->character[0] != nullptr) ? &(current->character[0]->value) : nullptr;
    } else {
        //if postfix matches the rest of the key, then value must exist
        valPointer = (current->postfix == key.substr(keyIndex, key.length() - keyIndex)) ? &(current->character[0]->value) : nullptr;
    }
    return valPointer;
}

#endif