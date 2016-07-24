/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/filepicker.h>

#include <string>
#include <sstream>

#include "libalpharay/vector.h"
#include "libalpharay/scene.h"
#include "libalpharay/xmlobject.h"

#include "previewpanel.h"


class VectorCtrl : public wxNavigationEnabled<wxControl>
{
	public:
		VectorCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize,
				long style=wxSP_ARROW_KEYS|wxTE_RIGHT, double min=0, double max=100, double initial=0, double inc=1);

		virtual void SetValue(const wxString &value);
		wxString GetValue();

	private:
		wxSpinCtrlDouble* spinX_;
		wxSpinCtrlDouble* spinY_;
		wxSpinCtrlDouble* spinZ_;
};


class Vector2Ctrl : public wxNavigationEnabled<wxControl>
{
	public:
		Vector2Ctrl(wxWindow *parent, wxWindowID id, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize,
				long style=wxSP_ARROW_KEYS|wxTE_RIGHT, double min=0, double max=100, double initial=0, double inc=1);

		virtual void SetValue(const wxString &value);
		wxString GetValue();

	private:
		wxSpinCtrlDouble* spinX_;
		wxSpinCtrlDouble* spinY_;
};


template <typename T>
class LinkComboBox : public wxComboBox
{
	public:
		LinkComboBox(Scene* scene, wxWindow *parent, wxWindowID id, 
				const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize) 
			: wxComboBox (parent, id, wxEmptyString, pos, size, 0, NULL, wxCB_READONLY)
		{
			scene_ = scene;

			generate();
		}

		int SetSelection(const wxString &s) {
			int i = FindString(s, true);
			if (i == wxNOT_FOUND) {
				wxComboBox::SetSelection(0);
				return i;
			}
			wxComboBox::SetSelection(i);
			return i;
		}

		int FindString (const wxString &s, bool bCase=false) {
			int result = wxComboBox::FindString(s,bCase);
			if (result == 0) {
				return wxNOT_FOUND;
			}
			return result;
		}
	private:
		Scene* scene_;
		std::vector<T*>* list_;

		void getList() {;}
		void generate() {
			getList();
			
			Append("<None>");
			for (auto it = list_->begin(); it != list_->end(); ++it) {	
				wxString name((*it)->name());
				Append(name);
			}
		}
};

template <> inline void LinkComboBox<Image>::getList() {
	list_ = &(scene_->images);
}

template <> inline void LinkComboBox<Map>::getList() {
	list_ = &(scene_->maps);
}

template <> inline void LinkComboBox<Material>::getList() {
	list_ = &(scene_->materials);
}


class PropertyEditor : public wxScrolledWindow
{
	public:
		PropertyEditor(Scene* scene, XmlObject* object, wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize);
		void Update();
	private:
		PreviewPanel* preview_;
		XmlObject* object_;
		Scene* scene_;
		void OnUpdate(wxCommandEvent& event);

		std::vector<wxControl*> ctrls;
		std::vector<wxComboBox*> linkCtrls;
		
	wxDECLARE_EVENT_TABLE();
};

enum {
	ID_Update = 5000
};


#endif // PROPERTY_EDITOR_H
