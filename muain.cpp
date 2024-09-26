#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>  // JSON-Bibliothek einbinden

class MyApp : public wxApp {
public:
    virtual bool OnInit();
    virtual int OnExit();
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);

    void OnQuit(wxCommandEvent& event);
    void OnCompleteCode(wxCommandEvent& event);

private:
    wxTextCtrl* leftTextBox;
    wxTextCtrl* rightTextBox;
    wxStatusBar* statusBar;

    std::string SendRequestToAPI(const std::string& code);

    static size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Quit = 1,
    ID_CompleteCode
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Quit, MyFrame::OnQuit)
EVT_BUTTON(ID_CompleteCode, MyFrame::OnCompleteCode)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
        wxLogError("curl_global_init() fehlgeschlagen.");
        return false;
    }

    MyFrame* frame = new MyFrame("Code-Vervollständigung mit Codestral");
    frame->Show(true);
    return true;
}

int MyApp::OnExit() {
    curl_global_cleanup();
    return 0;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {

    // Menüleiste erstellen
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Quit, "Beenden\tAlt-X", "Beendet das Programm");

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&Datei");
    SetMenuBar(menuBar);

    // Statusleiste erstellen
    statusBar = CreateStatusBar();
    SetStatusText("Bereit");

    // Layout-Management mit wxBoxSizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Linkes Textfeld für den Code-Eingang
    leftTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(400, 300), wxTE_MULTILINE);
    sizer->Add(leftTextBox, 1, wxEXPAND | wxALL, 10);

    // Rechtes Textfeld für die Ausgabe (nur Lesezugriff)
    rightTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(400, 300), wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(rightTextBox, 1, wxEXPAND | wxALL, 10);

    // Button zur Code-Vervollständigung
    wxButton* runButton = new wxButton(this, ID_CompleteCode, "Code vervollständigen (Codestral)");
    sizer->Add(runButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(sizer);
}

void MyFrame::OnQuit(wxCommandEvent&) {
    Close(true);
}

void MyFrame::OnCompleteCode(wxCommandEvent&) {
    // Hole den Code aus dem linken Textfeld
    wxString code = leftTextBox->GetValue();

    // Sende den Code zur API und erhalte die Vervollständigung
    std::string completion = SendRequestToAPI(std::string(code.mb_str()));

    // Hole den vorhandenen Text im rechten Textfeld
    wxString existingText = rightTextBox->GetValue();

    // Füge die neue Antwort an den vorhandenen Text an
    wxString newText = existingText + "\n" + completion;

    // Zeige die aktualisierte Antwort im rechten Textfeld an
    rightTextBox->SetValue(newText);

    SetStatusText("Vervollständigung erhalten.");
}


// Statische Callback-Funktion für libcurl
size_t MyFrame::WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    if (userdata == nullptr) {
        return 0;
    }
    size_t realSize = size * nmemb;
    std::string* data = static_cast<std::string*>(userdata);
    data->append(ptr, realSize);
    return realSize;
}

// Funktion für die HTTP-API-Anfrage mit libcurl und nlohmann/json
std::string MyFrame::SendRequestToAPI(const std::string& code) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        wxLogError("curl_easy_init() fehlgeschlagen.");
        return "";
    }

    // Verwende unique_ptr für automatische Bereinigung von curl
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curlHandle(curl, curl_easy_cleanup);

    CURLcode res;
    std::string readBuffer;

    // Setze die URL der Codestral-API
    res = curl_easy_setopt(curl, CURLOPT_URL, "https://codestral.mistral.ai/v1/fim/completions");
    if (res != CURLE_OK) {
        wxLogError("Fehler beim Setzen der URL: %s", curl_easy_strerror(res));
        return "";
    }

    // Erstelle das JSON-Objekt mit nlohmann/json
    nlohmann::json jsonData;
    jsonData["model"] = "codestral-latest";
    jsonData["prompt"] = code;  // Der Code aus dem Editor
    jsonData["max_tokens"] = 100;
    jsonData["temperature"] = 0.7;

    // Serialisiere das JSON-Objekt in einen String
    std::string jsonString = jsonData.dump();

    // Setze die POST-Daten
    res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
    if (res != CURLE_OK) {
        wxLogError("Fehler beim Setzen der POST-Daten: %s", curl_easy_strerror(res));
        return "";
    }

    // Setze HTTP-Header
    std::string API_KEY = "gzJqoKln4xPgTKO7xE15MhjX0fzrSA6i";  // Ersetze "YOUR_API_KEY" durch deinen tatsächlichen API-Schlüssel
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");
    std::string authHeader = "Authorization: Bearer " + API_KEY;
    headers = curl_slist_append(headers, authHeader.c_str());

    if (!headers) {
        wxLogError("Fehler beim Erstellen der HTTP-Header.");
        curl_easy_cleanup(curl);
        return "";
    }

    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    if (res != CURLE_OK) {
        wxLogError("Fehler beim Setzen der HTTP-Header: %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return "";
    }

    // Setze die Callback-Funktion für das Schreiben
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    if (res != CURLE_OK) {
        wxLogError("Fehler beim Setzen der Schreibfunktion: %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return "";
    }

    // Setze den Speicherort der Antwort
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    if (res != CURLE_OK) {
        wxLogError("Fehler beim Setzen des Schreibspeichers: %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return "";
    }

    // Führe die Anfrage aus
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        wxLogError("curl_easy_perform() fehlgeschlagen: %s", curl_easy_strerror(res));
        readBuffer = "";
    }
    else {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code == 200) {
            // Erfolgreiche Anfrage
        }
        else {
            wxLogError("HTTP-Fehlercode erhalten: %ld", http_code);
            wxLogError("Antwort: %s", readBuffer.c_str());
            readBuffer = "";
        }
    }

    // Bereinige
    curl_slist_free_all(headers);

    // Parse die API-Antwort
    if (!readBuffer.empty()) {
        try {
            nlohmann::json responseJson = nlohmann::json::parse(readBuffer);

            // Debug-Ausgabe der gesamten JSON-Antwort
            wxLogMessage("JSON-Antwort: %s", responseJson.dump(4).c_str());

            // Anpassung an die tatsächliche Struktur der Antwort
            if (responseJson.contains("choices") && responseJson["choices"].is_array() && !responseJson["choices"].empty()) {
                // Überprüfe, ob das Feld "message" vorhanden ist und nicht null ist
                if (responseJson["choices"][0].contains("message") && !responseJson["choices"][0]["message"].is_null()) {
                    // Lese den Text aus dem Feld "content" des "message"-Objekts
                    std::string completion = responseJson["choices"][0]["message"]["content"].get<std::string>();
                    return completion;
                }
                else {
                    wxLogError("Die JSON-Antwort enthält keine gültige 'message'.");
                    return "";
                }
            }
            else {
                wxLogError("Die JSON-Antwort enthält keine gültigen 'choices'.");
                return "";
            }
        }
        catch (const std::exception& e) {
            wxLogError("Fehler beim Parsen der JSON-Antwort: %s", e.what());
            wxLogMessage("Rohdaten der JSON-Antwort: %s", readBuffer.c_str());
            return "";
        }
    }
    else {
        wxLogError("Leere Antwort vom Server erhalten.");
        return "";
    }

}
