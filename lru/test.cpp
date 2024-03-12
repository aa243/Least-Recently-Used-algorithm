#include "lru.hpp"
#include <bits/stdc++.h>
using namespace std;
using namespace sjtu;

int main()
{
    double_list<int> dl = double_list<int>();
    dl.delete_head();
    dl.insert_head(10);
    dl.insert_head(20);
    dl.insert_head(30);
    double_list<int> dr(dl);
    cout << *(--dr.end()) << endl;
    // auto it = --dl.end();
    // cout << *it << endl;
    // it = dl.erase(it);
    // --it;
    // cout << *it << endl;
    // cout << *dl.begin() << endl;

    return 0;
}