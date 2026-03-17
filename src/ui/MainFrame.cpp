#include "MainFrame.h"
#include <sstream>
#include <iomanip>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(wxID_OK, MainFrame::OnEncodeButton)
        EVT_BUTTON(wxID_CLEAR, MainFrame::OnClearButton)
            wxEND_EVENT_TABLE()

                MainFrame::MainFrame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 700))
{
    coder = std::make_unique<ArithmeticCoder>();

    // Create main panel
    wxPanel *main_panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    // Input section
    wxStaticBoxSizer *input_box = new wxStaticBoxSizer(wxVERTICAL, main_panel, "Cadena de Entrada");
    input_text = new wxTextCtrl(main_panel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 50), wxTE_MULTILINE | wxTE_WORDWRAP);
    input_box->Add(input_text, 1, wxALL | wxEXPAND, 5);
    main_sizer->Add(input_box, 0, wxALL | wxEXPAND, 5);

    // Buttons section
    wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *encode_button = new wxButton(main_panel, wxID_OK, "Codificar y Decodificar");
    wxButton *clear_button = new wxButton(main_panel, wxID_CLEAR, "Limpiar");
    button_sizer->Add(encode_button, 0, wxALL, 5);
    button_sizer->Add(clear_button, 0, wxALL, 5);
    main_sizer->Add(button_sizer, 0, wxALL | wxEXPAND, 5);

    // Frequency Table section
    wxStaticBoxSizer *freq_box = new wxStaticBoxSizer(wxVERTICAL, main_panel, "Tabla de Frecuencias");
    frequency_list = new wxListCtrl(main_panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 100), wxLC_REPORT | wxLC_SINGLE_SEL);
    frequency_list->AppendColumn(wxString::FromUTF8("Símbolo (Si)"), wxLIST_FORMAT_CENTER, 80);
    frequency_list->AppendColumn("Frecuencia (Fi)", wxLIST_FORMAT_CENTER, 100);
    frequency_list->AppendColumn("Intervalo (Ii)", wxLIST_FORMAT_CENTER, 200);
    freq_box->Add(frequency_list, 1, wxALL | wxEXPAND, 5);
    main_sizer->Add(freq_box, 1, wxALL | wxEXPAND, 5);

    // Subintervals section
    wxStaticBoxSizer *sub_box = new wxStaticBoxSizer(wxVERTICAL, main_panel, "Subintervalos");
    subintervals_list = new wxListCtrl(main_panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 100), wxLC_REPORT | wxLC_SINGLE_SEL);
    subintervals_list->AppendColumn(wxString::FromUTF8("Símbolo"), wxLIST_FORMAT_CENTER, 80);
    subintervals_list->AppendColumn("Intervalo", wxLIST_FORMAT_CENTER, 300);
    sub_box->Add(subintervals_list, 1, wxALL | wxEXPAND, 5);
    main_sizer->Add(sub_box, 1, wxALL | wxEXPAND, 5);

    // Encoded Value section
    wxStaticBoxSizer *enc_box = new wxStaticBoxSizer(wxVERTICAL, main_panel, wxString::FromUTF8("Resultado de Codificación"));
    wxBoxSizer *enc_sizer = new wxBoxSizer(wxHORIZONTAL);
    enc_sizer->Add(new wxStaticText(main_panel, wxID_ANY, wxString::FromUTF8("Valor Codificado:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    encoded_value_label = new wxStaticText(main_panel, wxID_ANY, "");
    enc_sizer->Add(encoded_value_label, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    enc_box->Add(enc_sizer, 0, wxALL | wxEXPAND, 5);
    main_sizer->Add(enc_box, 0, wxALL | wxEXPAND, 5);

    // Decoded Value section
    wxStaticBoxSizer *dec_box = new wxStaticBoxSizer(wxVERTICAL, main_panel, wxString::FromUTF8("Resultado de Decodificación"));
    wxBoxSizer *dec_sizer = new wxBoxSizer(wxHORIZONTAL);
    dec_sizer->Add(new wxStaticText(main_panel, wxID_ANY, wxString::FromUTF8("Valor Decodificado:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    decoded_value_label = new wxStaticText(main_panel, wxID_ANY, "");
    dec_sizer->Add(decoded_value_label, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    dec_box->Add(dec_sizer, 0, wxALL | wxEXPAND, 5);
    main_sizer->Add(dec_box, 0, wxALL | wxEXPAND, 5);

    main_panel->SetSizer(main_sizer);
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnEncodeButton(wxCommandEvent &event)
{
    wxString input = input_text->GetValue();

    if (input.IsEmpty())
    {
        wxMessageBox("Por favor, ingrese una cadena a codificar", "Error de Entrada", wxOK | wxICON_WARNING);
        return;
    }

    // Encode the string
    coder->encodeString(std::string(input.mb_str()));

    // Update UI
    updateFrequencyTable();
    updateSubIntervals();
    updateEncodedValue();

    // Decode the value
    coder->decodeValue();
    updateDecodedValue();
}

void MainFrame::OnClearButton(wxCommandEvent &event)
{
    input_text->Clear();
    clearDisplay();
}

void MainFrame::updateFrequencyTable()
{
    frequency_list->DeleteAllItems();

    const auto &freq_table = coder->getFrequencyTable();

    for (size_t i = 0; i < freq_table.size(); ++i)
    {
        wxListItem item;
        item.SetId(i);

        // Symbol
        item.SetColumn(0);
        item.SetText(wxString::Format("%c", freq_table[i].symbol));
        frequency_list->InsertItem(item);

        // Frequency
        frequency_list->SetItem(i, 1, wxString::Format("%lld", freq_table[i].frequency));

        // Interval
        wxString interval = wxString::Format("[%.6f, %.6f)", freq_table[i].low_lim, freq_table[i].hi_lim);
        frequency_list->SetItem(i, 2, interval);
    }

    // Auto-resize columns
    frequency_list->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
    frequency_list->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
    frequency_list->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);
}

void MainFrame::updateSubIntervals()
{
    subintervals_list->DeleteAllItems();

    const auto &sub_intervals = coder->getSubIntervals();

    for (size_t i = 0; i < sub_intervals.size(); ++i)
    {
        wxListItem item;
        item.SetId(i);

        // Symbol
        item.SetColumn(0);
        item.SetText(wxString::Format("%c", sub_intervals[i].symbol));
        subintervals_list->InsertItem(item);

        // Interval
        wxString interval = wxString::Format("[%.15f, %.15f)", sub_intervals[i].low_lim, sub_intervals[i].hi_lim);
        subintervals_list->SetItem(i, 1, interval);
    }

    // Auto-resize columns
    subintervals_list->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
    subintervals_list->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
}

void MainFrame::updateEncodedValue()
{
    double encoded_val = coder->getEncodedValue();
    const std::string &original = coder->getOriginalString();

    wxString display = wxString::Format("%s = %.15f", original, encoded_val);
    encoded_value_label->SetLabel(display);
}

void MainFrame::updateDecodedValue()
{
    const std::string &decoded = coder->getDecodedString();
    decoded_value_label->SetLabel(wxString::FromUTF8(decoded));
}

void MainFrame::clearDisplay()
{
    frequency_list->DeleteAllItems();
    subintervals_list->DeleteAllItems();
    encoded_value_label->SetLabel("");
    decoded_value_label->SetLabel("");
}
