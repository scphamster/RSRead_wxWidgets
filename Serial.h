#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include "wx/wx.h"   //this include leads to many windows symbols redefinition
#include <deque>
#include <map>
#include "serialib.h"
#include <algorithm>

constexpr int  SERIAL_BUFF_INIT_SZE2 = 32;
constexpr auto SERIAL_PORT_PREFIX    = "\\\\.\\";

template<typename _Key, typename _Tp>
struct myPair : public std::pair<_Key, _Tp> { };

template<typename _Key,
         typename _Tp,
         typename _Compare = std::less<_Key>,
         typename _Alloc   = std::allocator<std::pair<const _Key, _Tp>>>
struct myMap : public std::map<_Key, _Tp, _Compare, _Alloc> {
    template<typename... Args>   // OK
    myMap(Args &&...args)
      : std::map<_Key, _Tp, _Compare, _Alloc>(std::forward<Args>(args)...)
    { }

    int64_t GetIndex(_Key k)
    {
        auto found = this->find(k);
        if (found == this->end())
            return -1;

        return std::distance(this->begin(), found);
    }

    auto ConvToPairByKey(_Key k)
    {
        if (this->find(k) == this->end())
            return std::pair<_Key, _Tp>{};
        else
            return std::pair<_Key, _Tp>{ k, this->operator[](k) };
    }

    auto ConvToPairByIdx(int idx)
    {
        if (idx >= this->size())
            return std::pair<_Key, _Tp>{};
        else {
            int iterator = 0;

            for (auto &val : *this) {
                if (iterator == idx) {
                    return std::pair<_Key, _Tp>{ val.first, val.second };
                }

                iterator++;
            }

            return std::pair<_Key, _Tp>{};
        }
    }

    bool Contains(_Key k)
    {
        auto found = this->find(k);

        if (found == this->end())
            return false;
        else
            return true;
    }

    auto GetValuesWxArrString() const noexcept
    {
        wxArrayString ret;

        for (const auto &pair : *this) {
            ret.Add(pair.second);
        }

        return ret;
    }
};

template<typename _Key,
         typename _Tp,
         typename _Compare = std::less<_Key>,
         typename _Alloc   = std::allocator<std::pair<const _Key, _Tp>>>
struct myMap_OLD : public std::map<_Key, _Tp, _Compare, _Alloc> {
    template<typename... Args>   // OK
    myMap_OLD(Args &&...args)
      : std::map<_Key, _Tp, _Compare, _Alloc>(std::forward<Args>(args)...)
    { }

    int64_t GetIndex(_Key k)
    {
        auto found = this->find(k);
        if (found == this->end())
            return -1;

        return std::distance(this->begin(), found);
    }

    auto ConvToPairByKey(_Key k)
    {
        if (this->find(k) == this->end())
            return std::pair<_Key, _Tp>{};
        else
            return std::pair<_Key, _Tp>{ k, this->operator[](k) };
    }

    auto ConvToPairByIdx(int idx)
    {
        if (idx >= this->size())
            return std::pair<_Key, _Tp>{};
        else {
            int iterator = 0;

            for (auto &val : *this) {
                if (iterator == idx) {
                    return std::pair<_Key, _Tp>{ val.first, val.second };
                }

                iterator++;
            }

            return std::pair<_Key, _Tp>{};
        }
    }

    bool Contains(_Key k)
    {
        auto found = this->find(k);

        if (found == this->end())
            return false;
        else
            return true;
    }

    auto GetValuesWxArrString() const noexcept
    {
        wxArrayString ret;

        for (const auto &pair : *this) {
            ret.Add(pair.second);
        }

        return ret;
    }
};

struct SerialConfigs {
    enum portSelection {
        PORT_NOT_SELECTED = -1
    };

    using config       = std::pair<int, wxString>;
    using _lookupTable = std::map<int, wxString>;
    using lookupTable  = myMap_OLD<int, wxString>;

    const config port_undefined = { PORT_NOT_SELECTED, "Undefined" };
    config       port           = port_undefined;
    config       baudrate       = { 115200, "115200" };
    config       bitsNum        = { SERIAL_DATABITS_8, "8" };
    config       stopBitsNum    = { SERIAL_STOPBITS_1, "1" };
    config       parity         = { SERIAL_PARITY_NONE, "NONE" };

    lookupTable avlblPorts = _lookupTable{ { PORT_NOT_SELECTED, "Undefined" } };

    lookupTable avlblBaudrates =
      _lookupTable{ { 110, "110" },       { 300, "300" },       { 600, "600" },      { 1200, "1200" },
                    { 2400, "2400" },     { 4800, "4800" },     { 9600, "9600" },    { 14400, "14400" },
                    { 19200, "19200" },   { 38400, "38400" },   { 56000, "56000" },  { 57600, "57600" },
                    { 115200, "115200" }, { 128000, "128000" }, { 256000, "256000" } };
    ;
    lookupTable avlblDatabits = _lookupTable{ { SERIAL_DATABITS_5, "5" },
                                              { SERIAL_DATABITS_6, "6" },
                                              { SERIAL_DATABITS_7, "7" },
                                              { SERIAL_DATABITS_8, "8" },
                                              { SERIAL_DATABITS_16, "16" } };
    lookupTable avlblStopbits =
      _lookupTable{ { SERIAL_STOPBITS_1, "1" }, { SERIAL_STOPBITS_1_5, "1_5" }, { SERIAL_STOPBITS_2, "2" } };

    lookupTable avlblParities = _lookupTable{ { SERIAL_PARITY_NONE, "NONE" },
                                              { SERIAL_PARITY_EVEN, "EVEN" },
                                              { SERIAL_PARITY_ODD, "ODD" },
                                              { SERIAL_PARITY_MARK, "MARK" },
                                              { SERIAL_PARITY_SPACE, "SPACE" } };
};

template<typename _Key,
         typename _Tp,
         typename _Compare = std::less<_Key>,
         typename _Alloc   = std::allocator<std::pair<const _Key, _Tp>>>
struct superMap : public std::map<_Key, _Tp, _Compare, _Alloc> {
    template<typename... Args>
    superMap(Args &&...args)
      : std::map<_Key, _Tp, _Compare, _Alloc>(std::forward<Args>(args)...)
    { }

    superMap(const std::initializer_list<typename superMap::value_type> &_Ilist)
      : std::map<_Key, _Tp, _Compare, _Alloc>(std::forward<decltype(_Ilist)>(_Ilist))
    { }

    superMap(std::initializer_list<typename superMap::value_type> &&_Ilist)
      : std::map<_Key, _Tp, _Compare, _Alloc>(std::move(_Ilist))
    { }

    superMap() = default;

    // const std::pair<const int, wxString> const config
    /*friend bool operator<(const std::pair<const int, wxString> &lh,
                          const std::pair<const int, wxString> &rh) noexcept
    {
        return (lh.first < rh.first);
    }
    friend bool operator>(const std::pair<const int, wxString> &lh,
                          const std::pair<const int, wxString> &rh) noexcept
    {
        return (rh < lh);
    }
    friend bool operator>=(const std::pair<const int, wxString> &lh,
                           const std::pair<const int, wxString> &rh) noexcept
    {
        return !(rh < lh);
    }
    friend bool operator<=(const std::pair<const int, wxString> &lh,
                           const std::pair<const int, wxString> &rh) noexcept
    {
        return !(lh > rh);
    }
    friend bool operator==(const std::pair<const int, wxString> &lh,
                           const std::pair<const int, wxString> &rh) noexcept
    {
        return (lh.first == rh.first);
    }
    friend bool operator!=(const std::pair<const int, wxString> &lh,
                           const std::pair<const int, wxString> &rh) noexcept
    {
        return !(lh == rh);
    }*/

    // bool operator==(const std::pair<_Key, _Tp> &rh);

    // using Value = second;
    // using Key = first;

    size_t GetIndexByKeyIfAny(_Key k) const
    {
        auto found = this->find(k);
        if (found == this->end())
            return CB_ERR;   //-1

        return std::distance(this->begin(), found);
    }

    bool GetKeyByValueIfAny(_Tp value_tofind, _Key &foundKey) const
    {
        for (auto &[key, value_here] : *this) {
            if (value_tofind == value_here) {
                foundKey = key;
                return true;
            }
        }

        return false;
    }

    bool ConvToPairByKeyIfAny(_Key k, std::pair<_Key, _Tp> &dest) const
    {
        if (this->find(k) == this->end())
            return false;
        else {
            dest = std::move(std::pair{ k, this->operator[](k) });
            return true;
        }
    }

    bool ConvToPairByValueIfAny(const _Tp &value_tofind, std::pair<_Key, _Tp> &dest) const
    {
        for (const auto [key, value] : *this) {
            if (value == value_tofind) {
                dest = std::move(std::pair{ key, value });

                return true;
            }
        }

        return false;
    }

    bool ConvToPairByIdxIfAny(int idx, std::pair<_Key, _Tp> &dest) const
    {
        if (idx >= this->size()) {
            return false;
        }
        else {
            int iterator = 0;

            for (const auto &val : *this) {
                if (iterator == idx) {
                    dest = std::move(std::pair{ val.first, val.second });
                    return true;
                }

                iterator++;
            }

            return false;
        }
    }

    bool ContainsKey(_Key k) const noexcept
    {
        auto found = this->find(k);
        if (found == this->end())
            return false;
        else
            return true;
    }

    bool ContainsValue(_Tp value_tofind) const noexcept
    {
        for (const auto &[key, value] : *this) {
            if (value == value_tofind)
                return true;
        }
        return false;
    }

    std::vector<_Tp> GetAllValues() const noexcept
    {
        std::vector<_Tp> retvect;
        for (const auto &pair : *this) {
            retvect.push_back(this->second);
        }
    }

    bool testmember(_Tp val);
};

// template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
// bool
// superMap<_Key, _Tp, _Compare, _Alloc>::operator==(const std::pair<_Key, _Tp> &rh)
//{
//     return (this->first == rh.first);
// }
template<typename Pt, typename Mt, typename Vt>
struct _helper {
    bool SetByValue(Pt &dest, const Mt &dataset, const Vt &value)
    {
        auto resultIT = std::find_if(dataset.begin(), dataset.end(), [&value](auto &pair) {
            return (pair.second == value);
        });

        if (resultIT == dataset.end()) {
            wxASSERT_MSG(false, _T("Value is: ") + value);
            return false;
        }

        dest = *resultIT;
        return true;
    }
};
// TODO: make template
class SerialConfigs2 : protected _helper<std::pair<int, wxString>, superMap<int, wxString>, wxString> {
  public:
    using config    = std::pair<int, wxString>;
    using dataTable = superMap<int, wxString>;

    enum portSelection {
        PORT_NOT_SELECTED = -1
    };

    void                          FindAvailableComPorts();
    const superMap<int, wxString> GetAvlblComPorts() const noexcept { return avlblPorts; }
    const superMap<int, wxString> GetAvlblBauds() const noexcept { return avlblBaudrates; }
    const superMap<int, wxString> GetAvlblDatabits() const noexcept { return avlblDatabits; }
    const superMap<int, wxString> GetAvlblStopbits() const noexcept { return avlblStopbits; }
    const superMap<int, wxString> GetAvlblParities() const noexcept { return avlblParities; }

    const config   GetSelectedPort() const noexcept { return port; }
    const wxString GetSelectedPortName() const noexcept { return port.second; }
    bool           PreviouslySelectedPortIsPresent() const noexcept
    {
        if (avlblPorts.ContainsValue(GetSelectedPortName()))
            return true;
        else
            return false;
    }
    const config GetSelectedBaudrate() const noexcept { return baudrate; }
    const config GetSelectedDatabits() const noexcept { return bitsNum; }
    const config GetSelectedStopbits() const noexcept { return stopBitsNum; }
    const config GetSelectedParity() const noexcept { return parity; }

    const std::vector<config> GetAllConfigs() noexcept(
      std::is_nothrow_constructible_v<std::vector<config>, config>)
    {
        return std::vector<config>{ port, baudrate, bitsNum, stopBitsNum, parity };

        /*retval.push_back(port);
        retval.push_back(baudrate);
        retval.push_back(bitsNum);
        retval.push_back(stopBitsNum);
        retval.push_back(parity);*/
    }

    bool SetPortByName(const wxString &portname) noexcept(
      std::is_nothrow_invocable_v<decltype(&dataTable::GetKeyByValueIfAny), wxString, int>)
    {
        /*int key;

        if (avlblPorts.GetKeyByValueIfAny(portname, key)) {
            port.first  = key;
            port.second = portname;

            return true;
        }
        else {
            port = PORT_UNDEFINED;
            return false;
        }*/

        return SetByValue(port, avlblPorts, portname);
    }
    bool SetPortByKey(const int portKey) noexcept(
      std::is_nothrow_invocable_v<decltype(&dataTable::ContainsKey), decltype(portKey)>)
    {
        if (avlblPorts.ContainsKey(portKey)) {
            port.first  = portKey;
            port.second = avlblPorts[portKey];
            return true;
        }
        else {
            port = PORT_UNDEFINED;
            return false;
        }
    }
    void SetPortToUndefined() noexcept { port = PORT_UNDEFINED; }
    void ResetAvlblPorts() noexcept
    {
        avlblPorts.clear();
        avlblPorts = { { PORT_NOT_SELECTED, _T("Undefined" ) } };
    }

    bool SetBaudrateByValue(const wxString &value) { return SetByValue(baudrate, avlblBaudrates, value); }
    bool SetDatabitsByValue(const wxString &value) { return SetByValue(bitsNum, avlblDatabits, value); }
    bool SetStopbitsByValue(const wxString &value) { return SetByValue(stopBitsNum, avlblStopbits, value); }
    bool SetParityByValue(const wxString &value) { return SetByValue(parity, avlblParities, value); }

    static const inline config PORT_UNDEFINED{ PORT_NOT_SELECTED, _T("Undefined") };

  protected:
    config port        = PORT_UNDEFINED;
    config baudrate    = { 115200, _T("115200" ) };
    config bitsNum     = { SERIAL_DATABITS_8, _T("8" ) };
    config stopBitsNum = { SERIAL_STOPBITS_1, _T("1" ) };
    config parity      = { SERIAL_PARITY_NONE, _T("NONE" ) };

    dataTable avlblPorts{ { PORT_NOT_SELECTED, _T("Undefined" ) } };

    static const inline dataTable avlblBaudrates{
        { 110, _T("110" ) },       { 300, _T("300" ) },       { 600, _T("600" ) },
        { 1200, _T("1200" ) },     { 2400, _T("2400" ) },     { 4800, _T("4800" ) },
        { 9600, _T("9600" ) },     { 14400, _T("14400" ) },   { 19200, _T("19200" ) },
        { 38400, _T("38400" ) },   { 56000, _T("56000" ) },   { 57600, _T("57600" ) },
        { 115200, _T("115200" ) }, { 128000, _T("128000" ) }, { 256000, _T("256000" ) }
    };
    ;
    static const inline dataTable avlblDatabits{ { SERIAL_DATABITS_5, _T("5"  ) },
                                                 { SERIAL_DATABITS_6, _T("6"  ) },
                                                 { SERIAL_DATABITS_7, _T("7"  ) },
                                                 { SERIAL_DATABITS_8, _T("8"  ) },
                                                 { SERIAL_DATABITS_16, _T( "16 ") } };
    static const inline dataTable avlblStopbits{ { SERIAL_STOPBITS_1, _T("1" ) },
                                                 { SERIAL_STOPBITS_1_5, _T("1_5" ) },
                                                 { SERIAL_STOPBITS_2, _T("2" ) } };

    static const inline dataTable avlblParities{ { SERIAL_PARITY_NONE, _T("NONE" ) },
                                                 { SERIAL_PARITY_EVEN, _T("EVEN" ) },
                                                 { SERIAL_PARITY_ODD, _T("ODD" ) },
                                                 { SERIAL_PARITY_MARK, _T("MARK" ) },
                                                 { SERIAL_PARITY_SPACE, _T("SPACE" ) } };
};

class MainFrame;

template<typename T, typename Allocator = std::allocator<T>>
class deque_s {
  public:
    void pop_front_wait(T &buf)
    {
        mutex.Lock();

        while (data.empty()) {
            condition.Wait();
        }

        buf = data.front();
        data.pop_front();

        mutex.Unlock();
    }

    bool empty()
    {
        mutex.Lock();
        bool retval = data.empty();
        mutex.Unlock();

        return retval;
    }

    void pop_all_wait(T &buf) { }

    void push_back(const T &buf)
    {
        mutex.Lock();
        data.push_back(buf);
        mutex.Unlock();

        condition.Signal();
    }

  private:
    std::deque<T, Allocator> data;
    wxMutex                  mutex;
    wxCondition              condition{ mutex };
};

struct data_element_t {
    uint32_t    msg_counter;
    uint32_t    bytes_in_buffer;
    std::time_t msg_arrival_time;
    char        data[SERIAL_BUFF_INIT_SZE2];

    data_element_t() = default;
    data_element_t(uint32_t msgN, std::time_t t)
      : msg_counter(msgN)
      , msg_arrival_time(t)
    { }
};

class Serial : private data_element_t,
               private serialib {
  public:
    Serial(std::shared_ptr<SerialConfigs2>, deque_s<std::shared_ptr<data_element_t>> &);
    ~Serial();

    char Open();
    bool Read();
    bool Stop();

    //void     SetParent(MainFrame *const handler) { myParent = handler; }
    //void     SetComPort(wxString &name) { portName = name; }
    //void     ConfigureSerial(SerialConfigs *config) { portConfigs = config; }
    //wxString GetSystemPortName() { return wxString(SERIAL_PORT_PREFIX).append(portConfigs->port.second); }

    wxString                                ConvRawToString(std::shared_ptr<Serial::data_element_t> data_item);
    std::shared_ptr<Serial::data_element_t> GetData();
    wxString                                ConvRawToPrettyString(std::shared_ptr<data_element_t> data);

  private:
    std::shared_ptr<SerialConfigs2>           serialConfigs;
    deque_s<std::shared_ptr<data_element_t>> &databridgeData;
    uint64_t                                  data_packet_counter = 0;
    int                                       err                 = 0;

    // MainFrame *myParent;
    // wxString   portName;
    // SerialConfigs                  *portConfigs;
};
