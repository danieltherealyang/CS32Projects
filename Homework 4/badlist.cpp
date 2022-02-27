void removeBad(list<Movie*>& li)
{
    list<Movie*>::iterator it = li.begin();
    while (it != li.end()) {
        if ((*it)->rating() < 50) {
            delete *it;
            it = li.erase(it);
            continue;
        }
        it++;
    }
}