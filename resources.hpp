#pragma once

#define ID_SERPORTCONFDLG  10000
#define ID_RSREAD_DB       10000
#define ID_PANEL           10001
#define ID_CHOICE_PORT     10002
#define ID_CHOICE_BAUDRATE 10003
#define ID_CHOICE_DATABITS 10004
#define ID_CHOICE_STOPBITS 10005
#define ID_CHOICE_PARITY   10006

#define ID_STATUSBAR        10003
#define ID_TEXTCTRL         10003
#define ID_CONFIGURE_SERIAL 10004
#define ID_START_RCVER      10005
#define ID_CLOSE_SERIAL     10007

#define SYMBOL_SERPORTCONFDLG_STYLE                                                                          \
    wxCAPTION | wxSYSTEM_MENU | wxSTAY_ON_TOP | wxCLOSE_BOX | wxNO_BORDER | wxTAB_TRAVERSAL
#define SYMBOL_SERPORTCONFDLG_TITLE    _("Serial port configuration")
#define SYMBOL_SERPORTCONFDLG_IDNAME   ID_SERPORTCONFDLG
#define SYMBOL_SERPORTCONFDLG_SIZE     wxDefaultSize
#define SYMBOL_SERPORTCONFDLG_POSITION wxDefaultPosition

#define SYMBOL_RSREAD_DB_STYLE                                                                               \
    wxDEFAULT_FRAME_STYLE | wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX |  \
      wxCLOSE_BOX
#define SYMBOL_RSREAD_DB_TITLE    _("RSREAD_DB")
#define SYMBOL_RSREAD_DB_IDNAME   ID_RSREAD_DB
#define SYMBOL_RSREAD_DB_SIZE     wxSize(800, 600)
#define SYMBOL_RSREAD_DB_POSITION wxDefaultPosition
