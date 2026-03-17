#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <memory>
#include "../core/ArithmeticCoder.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);
    ~MainFrame();

private:
    // Event handlers
    void OnEncodeButton(wxCommandEvent &event);
    void OnClearButton(wxCommandEvent &event);

    // UI update methods
    void updateFrequencyTable();
    void updateSubIntervals();
    void updateEncodedValue();
    void updateDecodedValue();
    void clearDisplay();

    // UI Components
    wxTextCtrl *input_text;
    wxListCtrl *frequency_list;
    wxListCtrl *subintervals_list;
    wxStaticText *encoded_value_label;
    wxStaticText *decoded_value_label;

    // Core logic
    std::unique_ptr<ArithmeticCoder> coder;

    wxDECLARE_EVENT_TABLE();
};

#endif // MAIN_FRAME_H
