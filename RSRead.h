#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include "wx/wx.h"
#include "resources.hpp"
#include "Serial.h"

wxDECLARE_EVENT(wxEVT_THR_SERIAL_UPDATE, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_THR_SERIAL_COMPLETED, wxThreadEvent);

#define MY_THR_EVT(name, func) wx__DECLARE_EVT0(name, wxThreadEventHandler(func))

class MainPanel;
class wxStatusBar;
class SerialThread;

class MainFrame : public wxFrame {
  public:
    MainFrame();
    MainFrame(wxWindow       *parent,
              wxWindowID      id      = SYMBOL_RSREAD_DB_IDNAME,
              const wxString &caption = SYMBOL_RSREAD_DB_TITLE,
              const wxPoint  &pos     = SYMBOL_RSREAD_DB_POSITION,
              const wxSize   &size    = SYMBOL_RSREAD_DB_SIZE,
              long            style   = SYMBOL_RSREAD_DB_STYLE);

    bool Create(wxWindow       *parent,
                wxWindowID      id      = SYMBOL_RSREAD_DB_IDNAME,
                const wxString &caption = SYMBOL_RSREAD_DB_TITLE,
                const wxPoint  &pos     = SYMBOL_RSREAD_DB_POSITION,
                const wxSize   &size    = SYMBOL_RSREAD_DB_SIZE,
                long            style   = SYMBOL_RSREAD_DB_STYLE);

    ~MainFrame();

    void Init();

    void CreateControls();
    void DoStartThread();

    void OnCloseClick(wxCommandEvent &event);
    void OnSaveToFileClick(wxCommandEvent &event);
    void OnConfigureSerialClick(wxCommandEvent &event);
    void OnStartRcverClick(wxCommandEvent &event);
    void OnCloseSerialClick(wxCommandEvent &event);

    void OnThreadUpdate(wxThreadEvent &);
    void OnThreadCompletion(wxThreadEvent &);

    wxBitmap GetBitmapResource(const wxString &name);
    wxIcon   GetIconResource(const wxString &name);

    static bool ShowToolTips();

    std::pair<int, wxString> GetPortSellection();
    std::pair<int, wxString> GetBaudrateSellection();

    void AppendDataFromSerial(const wxString &newdata) { fromSerialToFile.append(newdata); }

    wxStatusBar *statusbar;
    MainPanel   *panel = nullptr;

    wxCriticalSection CS;
    SerialThread     *serialTask = nullptr;

  protected:
    SerialConfigs serialConfigs;

    std::shared_ptr<void>                    databridgeConfigs;
    deque_s<std::shared_ptr<data_element_t>> databridgeData;

    wxString fromSerialToFile;

  private:
    DECLARE_CLASS(MainFrame)
    DECLARE_EVENT_TABLE()
};

class SerialThread : protected Serial,
                     public wxThread {
  public:
    SerialThread() = delete;
    SerialThread(MainFrame                                  *handler,
                 std::shared_ptr<void>                       databridge,
                 deque_s<std::shared_ptr<::data_element_t>> &databridgeData);
    ~SerialThread();
    ExitCode Entry() override;

    bool StopSerial() { return Stop(); }

  protected:
    MainFrame    *myParent;
    wxThreadEvent event;
    // Serial         ComPort;
    SerialConfigs                  *serialConfigs;
    std::shared_ptr<SerialConfigs2> serialConfigs2;

    wxDECLARE_EVENT_TABLE();
};
