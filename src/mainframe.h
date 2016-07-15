/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _MAIN_FRAME_H_
#define _MAIN_FRAME_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "libalpharay/project.h"

#include "xmlobjecttreedata.h"
#include "projectloadingdialog.h"
#include "renderframe.h"


class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title, Project* project, const wxPoint& pos, const wxSize& size);
private:
	Project* project_;
	RenderFrame* renderFrame_;

	wxTreeCtrl* projectTree_;
	wxListCtrl* objectProps_;

	template <typename T> 
	void addTreeList(wxTreeItemId root, wxString lable, std::vector<T*> &list) {
		root = projectTree_->AppendItem(root, lable);

		for (unsigned int i = 0; i < list.size(); i ++) {
			T* obj = list[i];
			XmlObjectNamed *xmlobj = reinterpret_cast <T*> (obj);
			projectTree_->AppendItem(root, xmlobj->name(), -1, -1, new XmlObjectTreeData(obj));
		}
	};

	void genProjectTree();
	void showObjectProps(XmlObjectNamed* object);
	void OnTreeSelect(wxTreeEvent& event);

	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);

	void OnRender(wxCommandEvent& event);

	void OnClose(wxCloseEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	
    wxDECLARE_EVENT_TABLE();
};


enum {
    ID_Open=1,
	ID_Save,
	ID_Render,
	ID_ProjectTree,
	ID_ObjectProps,
	ID_RenderFrame
};


#endif //_MAIN_FRAME_H_
