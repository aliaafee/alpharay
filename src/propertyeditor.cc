/* vim: set ts=4 ss=4 sw=4 noet ai cindent : */

#include "propertyeditor.h"


VectorCtrl::VectorCtrl (wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, 
		long style, double min, double max, double initial, double inc)
{
	wxControl::Create(parent, wxID_ANY, pos, size);
	SetId(id);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	spinX_ = new wxSpinCtrlDouble(this, 0, wxEmptyString, wxDefaultPosition,
								wxDefaultSize, wxSP_ARROW_KEYS | wxTE_RIGHT, min, max, initial, inc);
	spinY_ = new wxSpinCtrlDouble(this, 1, wxEmptyString, wxDefaultPosition, 
								wxDefaultSize, wxSP_ARROW_KEYS | wxTE_RIGHT, min, max, initial, inc);
	spinZ_ = new wxSpinCtrlDouble(this, 2, wxEmptyString, wxDefaultPosition, 
								wxDefaultSize, wxSP_ARROW_KEYS | wxTE_RIGHT, min, max, initial, inc);
	
	sizer->Add(spinX_, 1, wxEXPAND | wxALL, 0);
	sizer->Add(spinY_, 1, wxEXPAND | wxALL, 0);
	sizer->Add(spinZ_, 1, wxEXPAND | wxALL, 0);

	SetSizer(sizer);
}


wxString VectorCtrl::GetValue() {
	std::stringstream ss;

	ss << spinX_->GetValue() << "," << spinY_->GetValue() << "," << spinZ_->GetValue();

	return wxString(ss.str());
}


void VectorCtrl::SetValue(const wxString &value)
{
	double x,y,z;
	x = 0; y = 0; z = 0;
	
	std::stringstream ss;

	ss.str(value.ToStdString());

	std::string token;

    token = "";
    std::getline(ss, token, ',');
    x = stringtofloat(token);

    token = "";
    std::getline(ss, token, ',');
    y = stringtofloat(token);

    token = "";
    std::getline(ss, token, ',');
    z = stringtofloat(token);

	spinX_->SetValue(x);
	spinY_->SetValue(y);
	spinZ_->SetValue(z);
}


Vector2Ctrl::Vector2Ctrl (wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, 
		long style, double min, double max, double initial, double inc)
{
	wxControl::Create(parent, wxID_ANY, pos, size);
	SetId(id);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	spinX_ = new wxSpinCtrlDouble(this, 0, wxEmptyString, wxDefaultPosition,
								wxDefaultSize, wxSP_ARROW_KEYS | wxTE_RIGHT, min, max, initial, inc);
	spinY_ = new wxSpinCtrlDouble(this, 1, wxEmptyString, wxDefaultPosition, 
								wxDefaultSize, wxSP_ARROW_KEYS | wxTE_RIGHT, min, max, initial, inc);
	
	sizer->Add(spinX_, 1, wxEXPAND | wxALL, 0);
	sizer->Add(spinY_, 1, wxEXPAND | wxALL, 0);

	SetSizer(sizer);
}


wxString Vector2Ctrl::GetValue() {
	std::stringstream ss;

	ss << spinX_->GetValue() << "," << spinY_->GetValue();

	return wxString(ss.str());
}


void Vector2Ctrl::SetValue(const wxString &value)
{
	double x,y,z;
	x = 0; y = 0; z = 0;
	
	std::stringstream ss;

	ss.str(value.ToStdString());

	std::string token;

    token = "";
    std::getline(ss, token, ',');
    x = stringtofloat(token);

    token = "";
    std::getline(ss, token, ',');
    y = stringtofloat(token);

	spinX_->SetValue(x);
	spinY_->SetValue(y);
}



wxBEGIN_EVENT_TABLE(PropertyEditor, wxScrolledWindow)
    EVT_BUTTON	(ID_Update, PropertyEditor::OnUpdate)
wxEND_EVENT_TABLE()


PropertyEditor::PropertyEditor(Scene* scene, XmlObject* object, wxWindow *parent, wxWindowID id,const wxPoint &pos,const wxSize &size)
	: wxScrolledWindow(parent, id, pos, size, wxVSCROLL)
{
	scene_ = scene;
	object_ = object;
	
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2, 5, 5);

	preview_ = NULL;
	if (object_->type() == "material") {
		Material* material = static_cast <Material*> (object_);

		wxStaticText* lbl = new wxStaticText(this,  wxID_ANY, "Preview", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		sizer->Add(lbl, 1, wxEXPAND);
		preview_ = new PreviewPanel(material, this, wxID_ANY);
		sizer->Add(preview_, 0, wxEXPAND|wxALL,0);

		preview_->Render();
	} else if (object_->type() == "map") {	
		Map* map = dynamic_cast <Map*> (object_);

		wxStaticText* lbl = new wxStaticText(this,  wxID_ANY, "Preview", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		sizer->Add(lbl, 1, wxEXPAND);
		preview_ = new PreviewPanel(map, this, wxID_ANY);
		sizer->Add(preview_, 0, wxEXPAND|wxALL,0);

		preview_->Render();
	} else if (object_->type() == "image") {
		Image* image = dynamic_cast <Image*> (object_);

		wxStaticText* lbl = new wxStaticText(this,  wxID_ANY, "Preview", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		sizer->Add(lbl, 1, wxEXPAND);
		preview_ = new PreviewPanel(image, this, wxID_ANY);
		sizer->Add(preview_, 0, wxEXPAND|wxALL,0);

		preview_->Render();
	}

	for (unsigned int i = 0; i < object_->editables.size(); i++) {
		BaseEditable* editable = object_->editables[i];
		
		wxString name(editable->name());
		name.Replace("-"," ");
		name.MakeCapitalized();
		wxStaticText* lbl = new wxStaticText(this,  1000+i, name, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		sizer->Add(lbl, 1, wxEXPAND);

		wxControl* ctrl;
		if (editable->type() == "float") {
			double min, max, initial, inc;
			min = -1000000;
			max =  1000000;
			initial = 0;
			inc = 0.1;

			wxSpinCtrlDouble* spin = new wxSpinCtrlDouble(this, i, wxEmptyString, 
										wxDefaultPosition, wxDefaultSize, 
										wxSP_ARROW_KEYS | wxTE_RIGHT, 
										min, max, initial, inc);
			spin->SetValue(wxString(editable->str()));
			ctrl = spin;
		} else if (editable->type() == "int") {
			int min, max, initial, inc;
			min = -1000000;
			max =  1000000;
			initial = 0;
			inc = 1;

			wxSpinCtrl* spin = new wxSpinCtrl(this, i, wxEmptyString, 
										wxDefaultPosition, wxDefaultSize, 
										wxSP_ARROW_KEYS | wxTE_RIGHT, 
										min, max, initial);
			spin->SetValue( wxString(editable->str()) );
			ctrl = spin;
		} else if (editable->type() == "vector") {
			double min, max, initial, inc;
			min = -1000000;
			max =  1000000;
			initial = 0;
			inc = 0.1;

			VectorCtrl* vec = new VectorCtrl(this, i,
											wxDefaultPosition, wxDefaultSize, 
											wxSP_ARROW_KEYS | wxTE_RIGHT, 
											min, max, initial, inc);
			vec->SetValue(wxString(editable->str()));
			ctrl = vec;
		} else if (editable->type() == "vector2") {
			double min, max, initial, inc;
			min = -1000000;
			max =  1000000;
			initial = 0;
			inc = 0.1;

			Vector2Ctrl* vec = new Vector2Ctrl(this, i,
											wxDefaultPosition, wxDefaultSize, 
											wxSP_ARROW_KEYS | wxTE_RIGHT, 
											min, max, initial, inc);
			vec->SetValue(wxString(editable->str()));
			ctrl = vec;
		} else if (editable->type() == "bool") {
			wxCheckBox* check = new wxCheckBox(this, i, "");
			if (editable->str() == "true") {
				check->SetValue(true);
			} else {
				check->SetValue(false);
			}
			ctrl = check;
		} else if (editable->type() == "color") {
			//VectorCtrl* vec = new VectorCtrl(this, i);
			//vec->SetValue(wxString(editable->str()));
			Editable<Color>* editableCol = static_cast <Editable<Color>*> (editable);
			Color col = editableCol->getValue();
			wxColour newcol(
					unsigned (char (col.x * 255)),
					unsigned (char (col.y * 255)),
					unsigned (char (col.z * 255)) );
			wxColourPickerCtrl* colpicker = new wxColourPickerCtrl(this, i, newcol);
			ctrl = colpicker;
		} else if (editable->type() == "filename") {
			Editable<FileName>* editableFilename = static_cast <Editable<FileName>*> (editable);
			FileName filename = editableFilename->getValue();
			wxFilePickerCtrl* filePicker = new wxFilePickerCtrl(this, i, filename.absPath());
			ctrl = filePicker;
		} else {
			wxTextCtrl* txt = new wxTextCtrl(this, i);
			txt->SetValue(wxString(editable->str()));
			ctrl = txt;
		}
		ctrls.push_back(ctrl);

		sizer->Add(ctrl, 1, wxEXPAND);
	}

	
	for (unsigned int i = 0; i < object_->editableLinks.size(); i++) {
		BaseEditableLink* editableLink = object_->editableLinks[i];

		wxString name(editableLink->name());
		name.Replace("-"," ");
		name.MakeCapitalized();
		wxStaticText* lbl = new wxStaticText(this,  2000 + i, name, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
		sizer->Add(lbl, 1, wxEXPAND);

		wxString type(editableLink->type());
		
		wxComboBox* ctrl;
		if (editableLink->type() == "image") {
			LinkComboBox<Image>* list = new LinkComboBox<Image> (scene_, this, 3000 + i);
			list->SetSelection(wxString(editableLink->str()));
			ctrl = list;
		} else if (editableLink->type() == "map") {
			LinkComboBox<Map>* list = new LinkComboBox<Map> (scene_, this, 3000 + i);
			list->SetSelection(wxString(editableLink->str()));
			ctrl = list;
		} else if (editableLink->type() == "material") {
			LinkComboBox<Material>* list = new LinkComboBox<Material> (scene_, this, 3000 + i);
			list->SetSelection(wxString(editableLink->str()));
			ctrl = list;
		}
		linkCtrls.push_back(ctrl);

		sizer->Add(ctrl, 1, wxEXPAND);
	}

	
	sizer->AddGrowableCol(1, 1);


	wxString name(object_->xmlTag());
	name.Replace("-"," ");
	name.MakeCapitalized();
	wxStaticText* lbl = new wxStaticText(this,  wxID_ANY, name, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	wxFont font = lbl->GetFont();
	font.MakeBold(); font.MakeLarger();
	lbl->SetFont(font);
	
	wxButton * updateBtn = new wxButton( this, ID_Update, "Save", wxDefaultPosition, wxDefaultSize, 0 );

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(lbl, 0, wxEXPAND|wxALL,10);
	topSizer->Add(updateBtn, 0, wxEXPAND|wxALL, 10);
	topSizer->Add(sizer, 1, wxEXPAND|wxALL,0);
	SetSizer(topSizer);

	FitInside();
	SetScrollRate(10, 10);
}


void PropertyEditor::Update() {
	for (unsigned int i = 0; i < ctrls.size(); i++) {
		wxControl* ctrl = ctrls[i];
		if (ctrl) {
			BaseEditable* editable = object_->editables[i];
			if (editable->type() == "float") {
				wxSpinCtrlDouble* spin = static_cast <wxSpinCtrlDouble*> (ctrl);
				
				std::string strValue = ftos( spin->GetValue() );
				editable->set(strValue);
			} else if (editable->type() == "int") {
				wxSpinCtrl* spin = static_cast <wxSpinCtrl*> (ctrl);
				std::string strValue = ftos( spin->GetValue() );
				editable->set(strValue);
			} else if (editable->type() == "vector") {
				VectorCtrl* vec = static_cast <VectorCtrl*> (ctrl);
				std::string strValue = (vec->GetValue()).ToStdString();
				editable->set(strValue);
			} else if (editable->type() == "vector2") {
				Vector2Ctrl* vec = static_cast <Vector2Ctrl*> (ctrl);
				std::string strValue = (vec->GetValue()).ToStdString();
				editable->set(strValue);
			} else if (editable->type() == "bool") {
				wxCheckBox* check = static_cast <wxCheckBox*> (ctrl);
				std::string strValue;
				if (check->GetValue()) {
					strValue = "true";
				} else {
					strValue = "false";
				}
				editable->set(strValue);
			} else if (editable->type() == "color") {
				wxColourPickerCtrl* colpicker = static_cast <wxColourPickerCtrl*> (ctrl);
				wxColor col = colpicker->GetColour();
				std::stringstream ss;
				ss << float(col.Red())  / 255.0 << ","
				   << float(col.Green())/ 255.0 << ","
				   << float(col.Blue()) / 255.0;
				std::string strValue = ss.str();
				editable->set(strValue);
			} else if (editable->type() == "filename") {
				Editable<FileName>* editableFilename = static_cast <Editable<FileName>*> (editable);
				FileName prevFilename = editableFilename->getValue();
				wxFilePickerCtrl* filePicker = static_cast <wxFilePickerCtrl*> (ctrl);
				std::string strValue = (filePicker->GetPath()).ToStdString();
				editable->set(strValue);
				if (object_->type() == "image") {
					Image* image = dynamic_cast <Image*> (object_);
					if (image->load()) {
						std::cout << "Image loaded" << std::endl;
					} else {
						wxMessageBox( "The image file could not be loaded",
										"Failed to load image", wxOK | wxICON_ERROR );
						editable->set(prevFilename.relPath());
						filePicker->SetPath(prevFilename.absPath());
						image->load();
					}
				}
			} else {
				wxTextCtrl* txt = static_cast <wxTextCtrl*> (ctrl);
				std::string strValue = (txt->GetValue()).ToStdString();
				editable->set(strValue);
			}
		}
	}

	for (unsigned int i = 0; i < linkCtrls.size(); i++) {
		wxComboBox* ctrl = linkCtrls[i];
		BaseEditableLink* editableLink = object_->editableLinks[i];
		std::string strValue = "";
		if (ctrl->GetSelection() != 0) {
			strValue = (ctrl->GetValue()).ToStdString();
		}
		std::cout << "setting to " << strValue << editableLink->type() << std::endl;
		editableLink->setSource(strValue);
		
		if (editableLink->type() == "image") {
			EditableLink<Image>* imageLink = static_cast <EditableLink<Image>*> (editableLink);
			imageLink->updateLinkFromList( &(scene_->images) );
		} else if (editableLink->type() == "map") {
			EditableLink<Map>* mapLink = static_cast <EditableLink<Map>*> (editableLink);
			mapLink->updateLinkFromList( &(scene_->maps) );
		} else if (editableLink->type() == "material") {
			EditableLink<Material>* materialLink = static_cast <EditableLink<Material>*> (editableLink);
			materialLink->updateLinkFromList( &(scene_->materials) );
		}
	}

	if (preview_) {
		preview_->Render();
		preview_->Refresh();
	}
}


void PropertyEditor::OnUpdate(wxCommandEvent& event) {
	Update();
}
