/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _XML_OBJECT_TREE_DATA_H_
#define _XML_OBJECT_TREE_DATA_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/treectrl.h>

#include "libalpharay/xmlobject.h"


class XmlObjectTreeData : public wxTreeItemData
{
public:
	XmlObjectTreeData(XmlObject* object) 
		: wxTreeItemData() 
		{ object_ = object; }

	XmlObject* GetObject()
		{ return object_; }

	//template <typename T>
	//T* GetAs ()
	//	{ return object_; }
private:
	XmlObject* object_;
};

#endif //_XML_OBJECT_TREE_DATA_H_
