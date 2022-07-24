/////////////////////////////////////////////////////////////////////////////
// Name:        MainPanel.cpp
// Purpose:
// Author:      scphamster
// Modified by:
// Created:     28/06/2022 19:24:43
// RCS-ID:
// Copyright:   scphamster
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 28/06/2022 19:24:43

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"



#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "MainPanel.h"

IMPLEMENT_DYNAMIC_CLASS(MainPanel, wxPanel)

BEGIN_EVENT_TABLE(MainPanel, wxPanel)
END_EVENT_TABLE()

MainPanel::MainPanel()
{
    Init();
}

MainPanel::MainPanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

bool
MainPanel::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    return true;
}

MainPanel::~MainPanel()
{
    //receiverStop();
}

void
MainPanel::Init()
{ }

void
MainPanel::CreateControls()
{
    this->SetBackgroundColour(wxColour(64, 0, 64));

    textPanelASCII = new wxStyledTextCtrl(this, ID_TEXTPANEL, wxDefaultPosition, wxSize(800, 1000));
    textPanelASCII->SetMinSize(wxSize(300, 200));

    textPanelRAW = new wxStyledTextCtrl(this, ID_TEXTPANEL, wxPoint(500, 0), wxSize(800, 500));

    auto *panelSizer = new wxBoxSizer(wxHORIZONTAL);
    auto *frameSizer = new wxBoxSizer(wxHORIZONTAL);

    panelSizer->Add(textPanelASCII, 1, wxEXPAND);
    panelSizer->Add(textPanelRAW, 1, wxEXPAND);
    SetSizer(panelSizer);

    frameSizer->Add(this, 1, wxEXPAND);
    GetParent()->SetSizerAndFit(frameSizer);
}

bool
MainPanel::ShowToolTips()
{
    return true;
}

wxBitmap
MainPanel::GetBitmapResource(const wxString &name)
{
    wxUnusedVar(name);
    return wxNullBitmap;
}

wxIcon
MainPanel::GetIconResource(const wxString &name)
{
    wxUnusedVar(name);
    return wxNullIcon;
}