#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "project1app.h"

IMPLEMENT_APP(Project1App)
IMPLEMENT_CLASS(Project1App, wxApp)

BEGIN_EVENT_TABLE(Project1App, wxApp)

END_EVENT_TABLE()

Project1App::Project1App()
{
    Init();
}

void
Project1App::Init()
{
}

bool
Project1App::OnInit()
{

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
    mainWindow = new MainFrame(nullptr);
    mainWindow->Show(true);

    return true;
}


int
Project1App::OnExit()
{
        
    
    return wxApp::OnExit();
}
