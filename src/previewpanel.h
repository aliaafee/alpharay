/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#ifndef PREVIEW_PANEL_H
#define PREVIEW_PANEL_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/panel.h>

#include "wxalpharaybitmap.h"

#include "libalpharay/scene.h"
#include "libalpharay/raytracer.h"


class PreviewPanel : public wxPanel
{
    public:
		void init();
		
        PreviewPanel(Image* object, wxWindow *parent, wxWindowID id=wxID_ANY)
			: wxPanel(parent, id, wxDefaultPosition, wxSize(100,100))
		{ init(); Set(object); }

		PreviewPanel(Map* object, wxWindow *parent, wxWindowID id=wxID_ANY)
			: wxPanel(parent, id, wxDefaultPosition, wxSize(100,100))
		{ init(); Set(object); }

		PreviewPanel(Material* object, wxWindow *parent, wxWindowID id=wxID_ANY)
			: wxPanel(parent, id, wxDefaultPosition, wxSize(100,100))
		{ init(); Set(object); }
		
        ~PreviewPanel() {};
		
		void Render();
    private:
		wxAlpharayBitmap bitmap_;
		Scene scene_;
		Raytracer renderer_;
		Image* image_;
		Map* map_;
		Material* material_;
		float exposure_;

		void Set(Image* image);
		void Set(Map* map);
		void Set(Material* material);

		void OnPaint(wxPaintEvent& event);


	wxDECLARE_EVENT_TABLE();
};


#endif // PREVIEW_PANEL_H
