#include "RSRead.h"
#include <string>
#include "Serial.h"

wxDEFINE_EVENT(wxEVT_THR_SERIAL_UPDATE, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_THR_SERIAL_COMPLETED, wxThreadEvent);

// SerialThread::SerialThread(MainFrame *handler, SerialConfigs *config)
//   : wxThread(wxTHREAD_DETACHED)
//   , myParent(handler)
//   , serialConfigs(config)
//{
//     ComPort.SetParent(handler);
//     ComPort.ConfigureSerial(config);
// }

SerialThread::SerialThread(MainFrame                                *handler,
                           std::shared_ptr<void>                     databridgeConfigs,
                           deque_s<std::shared_ptr<::data_element_t>> &databridgeData)
  : wxThread(wxTHREAD_DETACHED)
  , Serial{ std::static_pointer_cast<SerialConfigs2>(databridgeConfigs), databridgeData }
  , myParent(handler)
  , serialConfigs2{ std::static_pointer_cast<SerialConfigs2>(databridgeConfigs) }
{
    // ComPort.SetParent(handler);
    // ComPort.ConfigureSerial(config);
}

wxThread::ExitCode
SerialThread::Entry()
{
    int  err           = 0;
    bool data_obtained = false;

    if (TestDestroy() == 0) {
        if (serialConfigs2->GetSelectedPort() == (serialConfigs2->PORT_UNDEFINED)) {
            wxMessageBox(_T("Port is undefined. Select port in configurations"));

            goto exit;
        }

        err = Open();
    }
    else
        goto exit;

    if (err != 1) {
        wxString err_msg("Serial port opening unsuccessful\nError = ");
        err_msg.Append(std::to_string(err));

        wxMessageDialog(myParent, err_msg, "Serial Port").ShowModal();

        goto exit;
    }

    while (!TestDestroy()) {
        if (Read()) {
            wxQueueEvent(myParent, new wxThreadEvent(wxEVT_THR_SERIAL_UPDATE));
        }
    }

exit:
    wxQueueEvent(myParent, new wxThreadEvent(wxEVT_THR_SERIAL_COMPLETED));

    return (ExitCode)0;
}

SerialThread::~SerialThread()
{
    wxCriticalSectionLocker mutex(myParent->CS);

    if (myParent->serialTask != nullptr) {
        Stop();
        myParent->serialTask = nullptr;
    }
}
