#include "Serial.h"
#include <vector>
#include <ctime>
#include <string.h>

Serial::Serial(std::shared_ptr<SerialConfigs2>           databridgeConfigs,
               deque_s<std::shared_ptr<data_element_t>> &databridgeData)
  : serialConfigs{ databridgeConfigs }
  , databridgeData{ databridgeData }
{ }

Serial::~Serial()
{
    Stop();
}

char
Serial::Open()
{
    return openDevice(serialConfigs->GetSelectedPortName(),
                              serialConfigs->GetSelectedBaudrate().first,
                              SERIAL_DATABITS_8,   // TODO: make all parameters be taken from serialConfigs
                              SERIAL_PARITY_NONE,
                              SERIAL_STOPBITS_1);
}

bool
Serial::Read()
{
    std::time_t msgtime;

    std::time(&msgtime);
    auto newdata = std::make_shared<data_element_t>(data_packet_counter++, msgtime);

    auto ans = readBytes(newdata->data, sizeof(newdata->data), 0);

    if (ans <= 0) {
        err = ans;
        return false;
    }

    newdata->bytes_in_buffer = ans;
    databridgeData.push_back(newdata);

    return true;
}

/** @brief: return true if device was open, otherwise return false */
bool
Serial::Stop()
{
    if (isDeviceOpen()) {
        closeDevice();

        return true;
    }
    else {
        return false;
    }
}

wxString
Serial::ConvRawToString(std::shared_ptr<Serial::data_element_t> data_item)
{
    wxString retString;

    retString.append(data_item->data);

    return retString;
}

std::shared_ptr<Serial::data_element_t>
Serial::GetData()
{
    auto data_item = std::make_shared<data_element_t>();
    if (databridgeData.empty() == false) {
        databridgeData.pop_front_wait(data_item);
    }

    return data_item;
}

 wxString
Serial::ConvRawToPrettyString(std::shared_ptr<data_element_t> data)
{
    char        buffer[100];
    tm         *timeinfo  = new tm;
    std::time_t arvl_time = data->msg_arrival_time;

    localtime_s(timeinfo, &arvl_time);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    wxString retstr = "Data Arival Time: ";
    retstr.append(buffer);

    retstr.append("   Bytes in buffer: ");
    retstr.append(std::to_string(data->bytes_in_buffer));

    retstr.append("RawData: ");

    for (int i = 0; (i < data->bytes_in_buffer) && (i < sizeof(data->data)); i++) {
        retstr.append(std::to_string((data->data[i])));
        retstr.append(" ");
    }
    retstr.append("\n");

    delete timeinfo;
    return retstr;
}

void
SerialConfigs2::FindAvailableComPorts()
{
    ResetAvlblPorts();
    WCHAR pathBuffer[5000];   // buffer to store the path of the COMPORTS

    for (int i = 0; i < 255; i++)   // checking ports from COM0 to COM255
    {
        std::wstring portname = _T("COM") + std::to_wstring(i);

        DWORD portIsPresent = QueryDosDevice(portname.c_str(), pathBuffer, sizeof(pathBuffer));

        if (portIsPresent != 0)   // QueryDosDevice returns zero if it didn't find an object
        {
            avlblPorts[i] = wxString(portname.c_str());
        }

        wxASSERT_MSG(GetLastError() != ERROR_INSUFFICIENT_BUFFER, _T("Not insufficient buffer for path"));
    }
}