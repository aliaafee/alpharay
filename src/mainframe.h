/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef _MAIN_FRAME_H_
#define _MAIN_FRAME_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/utils.h> 

#include "libalpharay/project.h"

#include "xmlobjecttreedata.h"
#include "projectloadingdialog.h"
#include "renderframe.h"
#include "previewpanel.h"
#include "propertyeditor.h"


class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title, Project* project, const wxPoint& pos, const wxSize& size);
private:
	wxBoxSizer* sizer_;
	Project* project_;
	RenderFrame* renderFrame_;
	wxWindowIDRef renderFrameID_;

	wxTreeCtrl* projectTree_;
	
	template <typename T> 
	void addTreeList(wxTreeItemId root, 
					 wxString lable, 
					 std::vector<T*> &list)
	{
		root = projectTree_->AppendItem(root, lable);

		for (unsigned int i = 0; i < list.size(); i ++) {
			T* obj = list[i];
			XmlObjectNamed *xmlobj = static_cast <T*> (obj);
			projectTree_->AppendItem(root, xmlobj->name(), -1, -1, new XmlObjectTreeData(obj));
		}
	};

	void genProjectTree();
	void OnTreeSelect(wxTreeEvent& event);
	void displayPropertyEditor(XmlObjectNamed* object);

	void ResetFrame();

	//PreviewPanel* preview_;
	//wxWindowIDRef previewID_;

	PropertyEditor* propEditor_;
	wxWindowIDRef propEditorID_;

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
	ID_RenderFrame,
	ID_CloseRenderFrame
};


#endif //_MAIN_FRAME_H_
