#include "wx/wxprec.h"
#include <algorithm>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "SerPortConfDlg.h"
#include "RSRead.h"

//IMPLEMENT_DYNAMIC_CLASS(SerPortConfDlg, wxDialog)
//wxIMPLEMENT_DYNAMIC_CLASS(SerPortConfDlg, wxDialog);
BEGIN_EVENT_TABLE(SerPortConfDlg, wxDialog)

EVT_CHOICE(ID_CHOICE_PORT, SerPortConfDlg::OnChoiceSelected_Port)
EVT_CHOICE(ID_CHOICE_BAUDRATE, SerPortConfDlg::OnChoiceSelected_Baudrate)
EVT_CHOICE(ID_CHOICE_DATABITS, SerPortConfDlg::OnChoiceSelected_Databits)
EVT_CHOICE(ID_CHOICE_STOPBITS, SerPortConfDlg::OnChoiceSelected_Stopbits)
EVT_CHOICE(ID_CHOICE_PARITY, SerPortConfDlg::OnChoiceSelected_Parity)

EVT_BUTTON(wxID_APPLY, SerPortConfDlg::OnApplyClick)
EVT_TEXT(ID_TEXTCTRL, SerPortConfDlg::OnTextctrlTextUpdated)
EVT_TEXT_ENTER(ID_TEXTCTRL, SerPortConfDlg::OnTextctrlEnter)

END_EVENT_TABLE()

SerPortConfDlg::SerPortConfDlg(std::shared_ptr<void> &serConfigs,
                               MainFrame             *parent,
                               wxWindowID             id,
                               const wxString        &caption,
                               const wxPoint         &pos,
                               const wxSize          &size,
                               long                   style)
  : serialConfigs2{ std::add_lvalue_reference_t<std::shared_ptr<SerialConfigs2>>(serConfigs) }
{
    if (serialConfigs2.get() == nullptr) {
        serialConfigs2 = std::make_shared<SerialConfigs2>();
    }
    Init();
    Create(parent, id, caption, pos, size, style);
}

void
SerPortConfDlg::Init()
{
    CopyPrevSelectionsToLocal();
}

bool
SerPortConfDlg::Create(MainFrame      *parent,
                       wxWindowID      id,
                       const wxString &caption,
                       const wxPoint  &pos,
                       const wxSize   &size,
                       long            style)
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, caption, pos, size, style);

    CreateControls();
    Centre();
    return true;
}

SerPortConfDlg::~SerPortConfDlg() { }

void
SerPortConfDlg::CreateControls()
{
    wxPanel *panel =
      new wxPanel(this, ID_PANEL, wxDefaultPosition, wxSize(350, 200), wxSUNKEN_BORDER | wxTAB_TRAVERSAL);
    panel->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    panel->SetBackgroundColour(wxColour(240, 240, 240));

    auto avlblPorts = serialConfigs2->GetAvlblComPorts();

    wxArrayString ports;
    std::for_each(avlblPorts.begin(), avlblPorts.end(), [&ports](const auto &pair) {
        ports.Add(pair.second);
    });
    dropDownPORT = new wxChoice(panel, ID_CHOICE_PORT, wxPoint(180, 15), wxDefaultSize, ports, 0);
    dropDownPORT->SetSelection(avlblPorts.GetIndexByKeyIfAny(localSelections[_T("Port")].first));

    wxArrayString bauds;
    auto          avBauds = serialConfigs2->GetAvlblBauds();
    std::for_each(avBauds.begin(), avBauds.end(), [&bauds](const auto &pair) { bauds.Add(pair.second); });
    dropDownBAUDRATE = new wxChoice(panel, ID_CHOICE_BAUDRATE, wxPoint(180, 40), wxDefaultSize, bauds);
    dropDownBAUDRATE->SetSelection(avBauds.GetIndexByKeyIfAny(localSelections[_T("Baudrate")].first));

    wxArrayString databits;
    auto          avDatabits = serialConfigs2->GetAvlblDatabits();
    std::for_each(avDatabits.begin(), avDatabits.end(), [&databits](const auto &pair) {
        databits.Add(pair.second);
    });
    dropDownDATABITS = new wxChoice(panel, ID_CHOICE_DATABITS, wxPoint(180, 65), wxDefaultSize, databits);
    dropDownDATABITS->SetSelection(avDatabits.GetIndexByKeyIfAny(localSelections[_T("Databits")].first));

    wxArrayString stopbits;
    auto          avStopbits = serialConfigs2->GetAvlblStopbits();
    std::for_each(avStopbits.begin(), avStopbits.end(), [&stopbits](const auto &pair) {
        stopbits.Add(pair.second);
    });
    dropDownSTOPBITS = new wxChoice(panel, ID_CHOICE_STOPBITS, wxPoint(180, 90), wxDefaultSize, stopbits);
    dropDownSTOPBITS->SetSelection(avStopbits.GetIndexByKeyIfAny(localSelections[_T("Stopbits")].first));

    wxArrayString parities;
    auto          avParities = serialConfigs2->GetAvlblParities();
    std::for_each(avParities.begin(), avParities.end(), [&parities](const auto &pair) {
        parities.Add(pair.second);
    });
    dropDownPARITY = new wxChoice(panel, ID_CHOICE_PARITY, wxPoint(180, 115), wxDefaultSize, parities);
    dropDownPARITY->SetSelection(avParities.GetIndexByKeyIfAny(localSelections[_T("Pairity")].first));

    new wxStaticText(panel, wxID_STATIC, _("Choose RS232 port number"), wxPoint(30, 20), wxDefaultSize, 0);
    new wxStaticText(panel, wxID_STATIC, _("Choose baudrate"), wxPoint(30, 45));
    new wxStaticText(panel, wxID_STATIC, _("Choose Bits number"), wxPoint(30, 70), wxDefaultSize, 0);
    new wxStaticText(panel, wxID_STATIC, _("Choose Stopbits number"), wxPoint(30, 95), wxDefaultSize, 0);
    new wxStaticText(panel, wxID_STATIC, _("Choose Parity"), wxPoint(30, 120), wxDefaultSize, 0);

    wxButton *applyButton = new wxButton(panel, wxID_APPLY, _("&Apply"), wxPoint(180, 150), wxDefaultSize, 0);
}

bool
SerPortConfDlg::ShowToolTips()
{
    return true;
}

wxBitmap
SerPortConfDlg::GetBitmapResource(const wxString &name)
{
    wxUnusedVar(name);
    return wxNullBitmap;
}

wxIcon
SerPortConfDlg::GetIconResource(const wxString &name)
{
    wxUnusedVar(name);
    return wxNullIcon;
}

void
SerPortConfDlg::OnChoiceSelected_Port(wxCommandEvent &event)
{
    assert(serialConfigs2->GetAvlblComPorts().ConvToPairByValueIfAny(dropDownPORT->GetStringSelection(),
                                                                  localSelections[_T("Port")]));
}

void
SerPortConfDlg::OnChoiceSelected_Baudrate(wxCommandEvent &event)
{
    assert(serialConfigs2->GetAvlblBauds().ConvToPairByValueIfAny(dropDownBAUDRATE->GetStringSelection(),
                                                                  localSelections[_T("Baudrate")]));
}

void
SerPortConfDlg::OnChoiceSelected_Databits(wxCommandEvent &event)
{
    assert(serialConfigs2->GetAvlblDatabits().ConvToPairByValueIfAny(dropDownDATABITS->GetStringSelection(),
                                                                  localSelections[_T("Databits")]));
}

void
SerPortConfDlg::OnChoiceSelected_Stopbits(wxCommandEvent &event)
{ 
    assert(serialConfigs2->GetAvlblStopbits().ConvToPairByValueIfAny(dropDownSTOPBITS->GetStringSelection(),
                                                                     localSelections[_T("Stopbits")]));
}

void
SerPortConfDlg::OnChoiceSelected_Parity(wxCommandEvent &event)
{ 
    assert(serialConfigs2->GetAvlblParities().ConvToPairByValueIfAny(dropDownPARITY->GetStringSelection(),
                                                                     localSelections[_T("Parity")]));
}

void
SerPortConfDlg::OnApplyClick(wxCommandEvent &event)
{
    wxASSERT_MSG(dropDownPORT->GetSelection() < serialConfigs2->GetAvlblComPorts().size(),
                 "Port Sellection number overruns avlblComPorts size");

    assert(serialConfigs2->SetPortByName(localSelections[_T("Port")].second));
    assert(serialConfigs2->SetBaudrateByValue(localSelections[_T("Baudrate")].second));
    assert(serialConfigs2->SetDatabitsByValue(localSelections[_T("Databits")].second));
    assert(serialConfigs2->SetStopbitsByValue(localSelections[_T("Stopbits")].second));
    assert(serialConfigs2->SetParityByValue(localSelections[_T("Parity")].second));

    EndModal(wxID_APPLY);
}

void
SerPortConfDlg::OnTextctrlEnter(wxCommandEvent &event)
{
    // Before editing this code, remove the block markers.
    EndModal(ID_TEXTCTRL);
}

void
SerPortConfDlg::OnTextctrlTextUpdated(wxCommandEvent &event)
{
    // Before editing this code, remove the block markers.
    event.Skip();
}

void
SerPortConfDlg::CopyPrevSelectionsToLocal()
{
    serialConfigs2->FindAvailableComPorts();
    auto avlblPorts2 = serialConfigs2->GetAvlblComPorts();
    if (!serialConfigs2->PreviouslySelectedPortIsPresent()) {
        serialConfigs2->SetPortToUndefined();
    }

    localSelections[_T("Port")]     = serialConfigs2->GetSelectedPort();
    localSelections[_T("Baudrate")] = serialConfigs2->GetSelectedBaudrate();
    localSelections[_T("Databits")] = serialConfigs2->GetSelectedDatabits();
    localSelections[_T("Stopbits")] = serialConfigs2->GetSelectedStopbits();
    localSelections[_T("Parity")]   = serialConfigs2->GetSelectedParity();
}
