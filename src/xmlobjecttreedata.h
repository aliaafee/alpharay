/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _XML_OBJECT_TREE_DATA_H_
#define _XML_OBJECT_TREE_DATA_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/treectrl.h>


class XmlObjectTreeData : public wxTreeItemData
{
public:
	XmlObjectTreeData(XmlObjectNamed* object) 
		: wxTreeItemData() 
		{ object_ = object; }

	XmlObjectNamed* GetObject()
		{ return object_; }

	//template <typename T>
	//T* GetAs ()
	//	{ return object_; }
private:
	XmlObjectNamed* object_;
};

#endif //_XML_OBJECT_TREE_DATA_H_
