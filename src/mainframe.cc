
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

	//renderFrame_ = new RenderFrame(this, ID_RenderFrame,"Render");
	renderFrame_ = NULL;
	
	//Menu Bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Open, "&Open Project\tCtrl-O",
                     "Open project from file");
	menuFile->Append(ID_Save, "&Save Project\tCtrl-S",
                     "Save project to file");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
	
	wxMenu *menuRender = new wxMenu;
	menuRender->Append(ID_Render, "Render\tCtrl-R");
    
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
	
	sizer_ = new wxBoxSizer( wxHORIZONTAL);
	
	//Project Tree
	projectTree_ = new wxTreeCtrl(this, ID_ProjectTree,  wxDefaultPosition);
	sizer_->Add(projectTree_, 1, wxEXPAND|wxALL,2);

	//Sizer
	SetSizer(sizer_);
	
	genProjectTree();	
}


void MainFrame::ResetFrame() {
	//Cancel any active renders
	wxWindow* prevRenderFrame = FindWindowById(renderFrameID_);
	if (prevRenderFrame) {
		prevRenderFrame->Close();
	}

	//Close any project related windows
	wxWindow* prevPropEditor = FindWindowById(propEditorID_);
	if (prevPropEditor) {
		prevPropEditor->Close();
		prevPropEditor->Destroy();
	}

	sizer_->Layout();
}


void MainFrame::genProjectTree() {
	projectTree_->DeleteAllItems();

	if (!project_) return;

	wxTreeItemId prjroot;
	prjroot = projectTree_->AddRoot("Project", -1, -1, new XmlObjectTreeData(project_));

	if (project_->renderer != NULL) {
		projectTree_->AppendItem(prjroot, "Render Settings", -1, -1, new XmlObjectTreeData(project_->renderer));
	}
	
	wxTreeItemId root;
	root = projectTree_->AppendItem(prjroot, "Scene", -1, -1, new XmlObjectTreeData(&(project_->scene)));

	if ( (project_->scene).camera_ != NULL ) {
		projectTree_->AppendItem(root, "Camera", -1, -1, new XmlObjectTreeData((project_->scene).camera_));
	}

	addTreeList <Image> (root, "Images", project_->scene.images);
	addTreeList <Map> (root, "Maps", project_->scene.maps);
	addTreeList <Material> (root, "Materials", project_->scene.materials);
	addTreeList <Light> (root, "Lights", project_->scene.lights);
	addTreeList <Object> (root, "Objects", project_->scene.objects);

	projectTree_->ExpandAll(); 
	projectTree_->SelectItem(prjroot);
	displayPropertyEditor(project_);
}


void MainFrame::displayPropertyEditor(XmlObject* object) {
	wxWindow* prevPropEditor = FindWindowById(propEditorID_);
	if (prevPropEditor) {
		prevPropEditor->Close();
		prevPropEditor->Destroy();
	}
	
	propEditorID_ = wxWindow::NewControlId();
	propEditor_ = new PropertyEditor(&(project_->scene),object, this, propEditorID_, wxDefaultPosition, wxSize(280,200));
	sizer_->Add(propEditor_, 0, wxEXPAND|wxALL,0);
	
	sizer_->Layout();
}


void MainFrame::OnTreeSelect(wxTreeEvent& event) {
	wxTreeItemData* treeData = projectTree_->GetItemData(event.GetItem());
	
	if (!treeData) return;
	
	XmlObjectTreeData* data = static_cast <XmlObjectTreeData*> (treeData);
	XmlObject* obj = data->GetObject();
	
	displayPropertyEditor(obj);
}



void MainFrame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, "Open Project file", "", "",
                       "Alpharay Project files (*.xml)|*.xml", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	ResetFrame();

	if (project_) delete project_;
	
	project_ = new Project();

	wxString path = openFileDialog.GetPath();

	ProjectLoadingDialog loader(this, project_, path);

	loader.ShowModal();

	switch (loader.result()) {
		case LOADING_FAIL:
			wxMessageBox( "The project file appears to be invalid, or resource files not found",
                  "Project not loaded", wxOK | wxICON_ERROR );
		case LOADING_CANCEL:
			SetStatusText("Project Load Cancelled");
			delete project_;
			project_ = NULL;
			genProjectTree();
			break;
		default:
			genProjectTree();
			SetStatusText("Project Opened");
	}
}


void MainFrame::OnSave(wxCommandEvent& event)
{
	if (!project_) {
		wxMessageBox( "Cannot save, current project is empty",
                  "Project Save", wxOK | wxICON_ERROR );
		return;
	}
	wxFileDialog saveFileDialog(this, _("Save Project file"), "", "",
                       "Alpharay Project files (*.xml)|*.xml", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	project_->save(saveFileDialog.GetPath().ToStdString());
}


void MainFrame::OnRender(wxCommandEvent& event)
{
	if (!project_) {
		wxMessageBox( "Cannot render, current project is empty",
                  "Render", wxOK | wxICON_ERROR );
		return;
	}

	wxWindow* prevRenderFrame = FindWindowById(renderFrameID_);
	if (prevRenderFrame) {
		prevRenderFrame->Close();
	}

	renderFrameID_ = wxWindow::NewControlId();
	renderFrame_ = new RenderFrame(project_, this, renderFrameID_,"Render");
	
	renderFrame_->Show();
}


void MainFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}


void MainFrame::OnClose(wxCloseEvent& event)
{
	wxWindow* prevRenderFrame = FindWindowById(renderFrameID_);
	if (prevRenderFrame) {
		prevRenderFrame->Close();
	}
	
	event.Skip();
}



void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "Distributed Raytracer",
                  "Alpharay", wxOK | wxICON_INFORMATION );
}

