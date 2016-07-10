/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#include <string>
#include <vector>
#include "xmlobject.h"

class LinkList
{
public:
    std::vector<std::string> sources;
    std::vector<XmlObjectNamed**> targets;

    LinkList() {};

    template <typename T> void add(std::string source, T** target) {
        if (source == "") return;

        XmlObjectNamed** targetxmlobj = reinterpret_cast <XmlObjectNamed**> (target);

        sources.push_back(source);
        targets.push_back(targetxmlobj);
    }

    int size() {
        return sources.size();
    }

    template <typename T> bool get(unsigned int i, std::string* source, T*** target) {
		//Type checking was removed, needs to look into this
        if (i < sources.size()) {
            *source = sources[i];
            *target = reinterpret_cast <T**> (targets[i]);
			return true;
        }
		return false;
    }

	bool append(LinkList &foo) {
		sources.insert(sources.end(), foo.sources.begin(), foo.sources.end());
		targets.insert(targets.end(), foo.targets.begin(), foo.targets.end());
		
		return true;
	}
};

/*
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
*/

#endif // _LINKLIST_H_
