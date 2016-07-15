
#include "mainframe.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU	(ID_Open, MainFrame::OnOpen)
	EVT_MENU	(ID_Save, MainFrame::OnSave)
    EVT_MENU	(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU	(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU	(ID_Render, MainFrame::OnRender)
	EVT_TREE_SEL_CHANGED (ID_ProjectTree, MainFrame::OnTreeSelect)
	EVT_CLOSE( MainFrame::OnClose )
wxEND_EVENT_TABLE()


MainFrame::MainFrame(const wxString& title, Project* project, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{	
	project_ = project;

	renderFrame_ = new RenderFrame(this, ID_RenderFrame,"Render");
	
	//Menu Bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Open, "&Open Project\tCtrl-O",
                     "Open project from file");
	menuFile->Append(ID_Save, "&Save Project\tCtrl-S",
                     "Save project to file");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
	
	wxMenu *menuRender = new wxMenu;
	menuRender->Append(ID_Render, "Render");
	/*
	menuRender->Append(ID_RenderFinal, "Render &Final");
	*/
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
	wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuRender, "&Render" );
    menuBar->Append( menuHelp, "&Help" );
    
	SetMenuBar( menuBar );
	
	//Status bar
    CreateStatusBar();
    SetStatusText( "" );
	
	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL);
	
	//Project Tree
	projectTree_ = new wxTreeCtrl(this, ID_ProjectTree,  wxDefaultPosition, wxSize(200,200));
	sizer->Add(projectTree_, 0, wxEXPAND|wxALL,2);

	//Object Props List 
	objectProps_ = new wxListCtrl(this, ID_ObjectProps, wxDefaultPosition, wxSize(200,200), wxLC_REPORT);
	sizer->Add(objectProps_, 1, wxEXPAND|wxALL, 2);
	objectProps_->AppendColumn("Property", wxLIST_FORMAT_LEFT,80);
	objectProps_->AppendColumn("Value", wxLIST_FORMAT_RIGHT,150);

	//Sizer
	SetSizer(sizer);
	
	genProjectTree();
}


void MainFrame::genProjectTree() {
	projectTree_->DeleteAllItems();
	wxTreeItemId root;
	root = projectTree_->AddRoot("Project");
	
	root = projectTree_->AppendItem(root, "Scene");
	addTreeList <Image> (root, "Images", project_->scene.images);
	addTreeList <Map> (root, "Maps", project_->scene.maps);
	addTreeList <Material> (root, "Materials", project_->scene.materials);
	addTreeList <Light> (root, "Lights", project_->scene.lights);
	addTreeList <Object> (root, "Objects", project_->scene.objects);

	//projectTree_->Expand(root);
	projectTree_->ExpandAll(); 
	objectProps_->DeleteAllItems();
}


void MainFrame::showObjectProps(XmlObjectNamed* object) {
	objectProps_->DeleteAllItems();
	
	for (unsigned int i = 0; i < object->editables.size(); i++) {
		BaseEditable* editable = object->editables[i];
		wxListItem item;
        item.SetId(i);
        item.SetText( editable->name() );
		objectProps_->InsertItem( item );
		objectProps_->SetItem(i, 1, editable->str());		
	}
}


void MainFrame::OnTreeSelect(wxTreeEvent& event) {
	wxTreeItemData* treeData = projectTree_->GetItemData(event.GetItem());
	
	if (!treeData) return;
	
	XmlObjectTreeData* data = reinterpret_cast <XmlObjectTreeData*> (treeData);
	XmlObjectNamed* obj = data->GetObject();
	
	showObjectProps(obj);
}



void MainFrame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, "Open Project file", "", "",
                       "Alpharay Project files (*.xml)|*.xml", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	delete project_;
	project_ = new Project();

	wxString path = openFileDialog.GetPath();

	ProjectLoadingDialog loader(this, project_, path);

	loader.ShowModal();

	switch (loader.result()) {
		case LOADING_FAIL:
			wxMessageBox( "The project file appears to be invalid, or resource files not found",
                  "Project not loaded", wxOK | wxICON_ERROR );
		case LOADING_CANCEL:
			delete project_;
			project_ = new Project();
	}

	genProjectTree();

	SetStatusText("Project Opened");
}


void MainFrame::OnSave(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Save Project file"), "", "",
                       "Alpharay Project files (*.xml)|*.xml", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	project_->save(saveFileDialog.GetPath().ToStdString());
}


void MainFrame::OnRender(wxCommandEvent& event)
{
	renderFrame_->Show();
	renderFrame_->Render(project_);
}


void MainFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}


void MainFrame::OnClose(wxCloseEvent& event)
{
	renderFrame_->Close(true);
	event.Skip();
}



void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}

