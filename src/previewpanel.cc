/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "previewpanel.h"

wxBEGIN_EVENT_TABLE(PreviewPanel, wxPanel)
	EVT_PAINT( PreviewPanel::OnPaint )
wxEND_EVENT_TABLE()


void PreviewPanel::init() 
{
	map_ = NULL;
	image_ = NULL;
	material_ = NULL;
	
	bitmap_.create(100, 100);
	
	Camera* camera = new Camera();
	camera->position_ = Vector(0, 3, 0);
	camera->target_ = Vector(0, 0, 0);
	scene_.camera_ = camera;

	renderer_.outWidth_ = 100;
	renderer_.outHeight_ = 100;
	renderer_.threadCount_ = 1;
	renderer_.subSamplesX_ = 1;
	renderer_.subSamplesY_ = 1;
	renderer_.cellCx = 1;
	renderer_.cellCy = 1;
}


void PreviewPanel::Set(Image* image)
{
	image_ = image;

	Map2d* map = new Map2d("");
	map->image_ = image_;

	scene_.envMap_ = map;
}


void PreviewPanel::Set(Map* map)
{
	map_ = map;
	
	scene_.envMap_ = map_;
}


void PreviewPanel::Set(Material* material)
{
	material_ = material;

	Object* sphere = scene_.add(new Sphere(""));
	sphere->position_ = Vector(0, 0, 0);
	sphere->scale_ = Vector(1, 1, 1);
	sphere->material_ = material_;
	
	Light* light = scene_.add(new Light(""));
	light->position_ = Vector(-6,10,6);
	light->energy_ = 1000;
	light->shadowsOn_ = false;
}


void PreviewPanel::Render()
{
	if (map_) map_->transform(); 
	if (material_) material_->transform(); 
	if (image_) image_->transform(); 
	
	renderer_.render(&scene_, &bitmap_);
	bitmap_.toneMap_exp(-2.0);
}


void PreviewPanel::OnPaint(wxPaintEvent& event)
{
	wxRect area;
	
	wxRegionIterator upd(GetUpdateRegion());
	while (upd)
    {
		area = upd.GetRect();

		bitmap_.RefreshArea(area);

		upd++;
	}
	
	wxClientDC dc(this);
	PrepareDC(dc);
	
	dc.DrawBitmap(bitmap_, 0, 0, false );
}


