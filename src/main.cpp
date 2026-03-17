#include <wx/wx.h>
#include "ui/MainFrame.h"

class ArithmeticCodingApp : public wxApp
{
public:
    bool OnInit() override
    {
        MainFrame *frame = new MainFrame(wxString::FromUTF8("Codificación Aritmética"));
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(ArithmeticCodingApp);
