#pragma once

#include "wx/wx.h"
#include "resources.hpp"
#include "Serial.h"

class MainFrame;

class SerPortConfDlg : public wxDialog {
    //DECLARE_DYNAMIC_CLASS(SerPortConfDlg)
    //wxDECLARE_DYNAMIC_CLASS(SerPortConfDlg);
    DECLARE_EVENT_TABLE()

  public:
    SerPortConfDlg() = default;
    SerPortConfDlg(std::shared_ptr<void> &serConfigs,
                   MainFrame             *parent,
                   wxWindowID             id      = SYMBOL_SERPORTCONFDLG_IDNAME,
                   const wxString        &caption = SYMBOL_SERPORTCONFDLG_TITLE,
                   const wxPoint         &pos     = SYMBOL_SERPORTCONFDLG_POSITION,
                   const wxSize          &size    = SYMBOL_SERPORTCONFDLG_SIZE,
                   long                   style   = SYMBOL_SERPORTCONFDLG_STYLE);

    bool Create(MainFrame      *parent,
                wxWindowID      id      = SYMBOL_SERPORTCONFDLG_IDNAME,
                const wxString &caption = SYMBOL_SERPORTCONFDLG_TITLE,
                const wxPoint  &pos     = SYMBOL_SERPORTCONFDLG_POSITION,
                const wxSize   &size    = SYMBOL_SERPORTCONFDLG_SIZE,
                long            style   = SYMBOL_SERPORTCONFDLG_STYLE);

    ~SerPortConfDlg();

    void Init();
    void CreateControls();

    void OnChoiceSelected_Port(wxCommandEvent &event);
    void OnChoiceSelected_Baudrate(wxCommandEvent &event);
    void OnChoiceSelected_Databits(wxCommandEvent &event);
    void OnChoiceSelected_Stopbits(wxCommandEvent &event);
    void OnChoiceSelected_Parity(wxCommandEvent &event);

    void OnApplyClick(wxCommandEvent &event);
    void OnTextctrlTextUpdated(wxCommandEvent &event);
    void OnTextctrlEnter(wxCommandEvent &event);

    wxBitmap GetBitmapResource(const wxString &name);
    wxIcon   GetIconResource(const wxString &name);

    static bool ShowToolTips();

    void CopyPrevSelectionsToLocal();
    // const std::pair<int, wxString> GetLocalPort() const noexcept { return localSelections[_T("Port")]; }
    const auto GetLocalPort() const noexcept { return localSelections.at(_T("Port")); }
    const auto GetLocalPort_Key() const noexcept { return localSelections.at(_T("Port")).first; }
    const auto GetLocalPort_Name() const noexcept { return localSelections.at(_T("Port")).second; }

    const auto GetLocalBaudrate() const noexcept { return localSelections.at(_T("Baudrate")); }
    const auto GetLocalBaudrate_Key() const noexcept { return localSelections.at(_T("Baudrate")).first; }
    const auto GetLocalBaudrate_Name() const noexcept { return localSelections.at(_T("Baudrate")).second; }

    const auto GetLocalDatabits() const noexcept { return localSelections.at(_T("Databits")); }
    const auto GetLocalDatabits_Key() const noexcept { return localSelections.at(_T("Databits")).first; }
    const auto GetLocalDatabits_Name() const noexcept { return localSelections.at(_T("Databits")).second; }

    const auto GetLocalStopbits() const noexcept { return localSelections.at(_T("Stopbits")); }
    const auto GetLocalStopbits_Key() const noexcept { return localSelections.at(_T("Stopbits")).first; }
    const auto GetLocalStopbits_Name() const noexcept { return localSelections.at(_T("Stopbits")).second; }

    const auto GetLocalParity() const noexcept { return localSelections.at(_T("Parity")); }
    const auto GetLocalParity_Key() const noexcept { return localSelections.at(_T("Parity")).first; }
    const auto GetLocalParity_Name() const noexcept { return localSelections.at(_T("Parity")).second; }

    enum portSellection {
        PORT_UNDEFINED = -1
    };

  protected:
    wxChoice *dropDownPORT;
    wxChoice *dropDownBAUDRATE;
    wxChoice *dropDownDATABITS;
    wxChoice *dropDownSTOPBITS;
    wxChoice *dropDownPARITY;

    SerialConfigs *serialConfigs       = nullptr;
    int            selectedPortNameNum = PORT_UNDEFINED;

    std::shared_ptr<SerialConfigs2>                    &serialConfigs2;
    superMap<const wxString, std::pair<int, wxString>> localSelections{
        { _T("Port"), SerialConfigs2::PORT_UNDEFINED },
        { _T("Baudrate"), { 115200, _T("115200" ) } },
        { _T("Databits"), { SERIAL_DATABITS_8, _T("8" ) } },
        { _T("Stopbits"), { SERIAL_STOPBITS_1, _T("1" ) } },
        { _T("Parity"), { SERIAL_PARITY_NONE, _T("NONE" ) } }
    };
};
