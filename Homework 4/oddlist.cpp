void removeOdd(list<int>& li)
{
    list<int>::iterator it = li.begin();
    while (it != li.end()) {
        if (*it % 2 != 0) {
            it = li.erase(it);
            continue;
        }
        it++;
    }
}