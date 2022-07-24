#pragma once
#include "wx/image.h"
#include "RSRead.h"

class Project1App : public wxApp {
  public:
    Project1App();

    void Init();

    virtual bool OnInit();

    virtual int OnExit();

  private:
    MainFrame *mainWindow = nullptr;

    DECLARE_CLASS(Project1App)
    DECLARE_EVENT_TABLE()
};

DECLARE_APP(Project1App)
