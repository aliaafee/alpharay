/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#include <string>
#include <vector>


template <typename T>
class LinkList
{
public:
    std::vector<std::string> sources;
    std::vector<T**> targets;

    LinkList() {};

    void add(std::string source, T** target) {
        sources.push_back(source);
        targets.push_back(target);
    }

    int size() {
        return sources.size();
    }

    bool get(int i, std::string* source, T*** target) {
        if (i >= 0 && i < sources.size()) {
            *source = sources[i];
            *target = targets[i];
        }
    }
};


#endif // _LINKLIST_H_
