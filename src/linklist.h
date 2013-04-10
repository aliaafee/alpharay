
#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#include <string>
#include <vector>

using namespace std;

template <typename T>
class LinkList
{
public:
    vector<string> sources;
    vector<T**> targets;

    LinkList() {};

    void add(string source, T** target) {
        sources.push_back(source);
        targets.push_back(target);
    }

    int size() {
        return sources.size();
    }

    bool get(int i, string* source, T*** target) {
        if (i >= 0 && i < sources.size()) {
            *source = sources[i];
            *target = targets[i];
        }
    }
};

#endif // _LINKLIST_H_

