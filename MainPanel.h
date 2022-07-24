#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "wx/stc/stc.h"
#include "myThread.h"

class MainPanel;

#define ID_PANEL                  10001
#define SYMBOL_PANELMAIN_STYLE    wxSUNKEN_BORDER | wxTAB_TRAVERSAL
#define SYMBOL_PANELMAIN_IDNAME   ID_PANEL
#define SYMBOL_PANELMAIN_SIZE     wxDefaultSize
#define SYMBOL_PANELMAIN_POSITION wxDefaultPosition

#define     ID_TEXTPANEL 10006

class MainPanel : public wxPanel {
    DECLARE_DYNAMIC_CLASS(MainPanel)
    DECLARE_EVENT_TABLE()

  public:
    MainPanel();
    MainPanel(wxWindow      *parent,
              wxWindowID     id    = ID_PANEL,
              const wxPoint &pos   = wxDefaultPosition,
              const wxSize  &size  = wxDefaultSize,
              long           style = wxSUNKEN_BORDER | wxTAB_TRAVERSAL);

    bool Create(wxWindow      *parent,
                wxWindowID     id    = ID_PANEL,
                const wxPoint &pos   = wxDefaultPosition,
                const wxSize  &size  = wxDefaultSize,
                long           style = wxSUNKEN_BORDER | wxTAB_TRAVERSAL);

    ~MainPanel();

    void Init();

    void CreateControls();

    wxBitmap GetBitmapResource(const wxString &name);

    wxIcon GetIconResource(const wxString &name);

    static bool ShowToolTips();

    wxStyledTextCtrl *textPanelASCII = nullptr;
    wxStyledTextCtrl *textPanelRAW   = nullptr;
};
