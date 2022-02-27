void removeBad(vector<Movie*>& v)
{
    vector<Movie*>::iterator it = v.begin();
    while (it != v.end()) {
        if ((*it)->rating() < 50) {
            delete *it;
            it = v.erase(it);
            continue;
        }
        it++;
    }
}