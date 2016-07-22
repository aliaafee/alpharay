/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "wxalpharaycanvas.h"


wxBEGIN_EVENT_TABLE(wxAlpharayCanvas, wxScrolledCanvas)
	EVT_PAINT( wxAlpharayCanvas::OnPaint )
wxEND_EVENT_TABLE()


wxAlpharayCanvas::wxAlpharayCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
	: wxScrolledCanvas(parent, id, pos, size, style, name)
{
	;
}


void wxAlpharayCanvas::Setup(wxAlpharayBitmap* bitmap, int width, int height) {
	bitmap_ = bitmap;

	SetVirtualSize(width, height);
	SetScrollRate(10,10);
}


void wxAlpharayCanvas::OnPaint(wxPaintEvent& event) {
	if (bitmap_ == NULL) return;
	if (!bitmap_->IsOk()) return;

	int vbX,vbY;
	GetViewStart(&vbX,&vbY);
	vbX *= 10; vbY *= 10;

	wxRect area;

	wxRegionIterator upd(GetUpdateRegion());
	while (upd)
    {
		area = upd.GetRect();
		area.x += vbX;
		area.y += vbY;
	
		
		bitmap_->RefreshArea(area);

		upd++;
	}
	
	wxClientDC dc(this);
	DoPrepareDC(dc);
	
	dc.DrawBitmap(*bitmap_, 0, 0, false );
}

