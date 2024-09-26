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
    void OnChat(wxCommandEvent& event);

private:
    wxTextCtrl* inputTextBox;    // Textbox für die Benutzereingabe
    wxTextCtrl* chatResponseTextBox;  // Textbox für die Chat-Antwort
    wxStatusBar* statusBar;

    std::string SendChatRequest(const std::string& message);

    static size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Quit = 1,
    ID_Chat
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Quit, MyFrame::OnQuit)
EVT_BUTTON(ID_Chat, MyFrame::OnChat)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
        wxLogError("curl_global_init() fehlgeschlagen.");
        return false;
    }

    MyFrame* frame = new MyFrame("Chat mit Codestral");
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

    // Textfeld für die Benutzereingabe
    inputTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(400, 100), wxTE_MULTILINE);
    sizer->Add(inputTextBox, 0, wxEXPAND | wxALL, 10);

    // Textfeld für die Chat-Antwort (nur Lesezugriff)
    chatResponseTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(400, 300), wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(chatResponseTextBox, 1, wxEXPAND | wxALL, 10);

    // Button zum Senden der Nachricht
    wxButton* chatButton = new wxButton(this, ID_Chat, "Nachricht senden");
    sizer->Add(chatButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(sizer);
}

void MyFrame::OnQuit(wxCommandEvent&) {
    Close(true);
}

void MyFrame::OnChat(wxCommandEvent&) {
    // Hole die Eingabe aus dem Textfeld
    wxString message = inputTextBox->GetValue();

    // Sende die Nachricht zur API und erhalte die Antwort
    std::string chatResponse = SendChatRequest(std::string(message.mb_str()));

    // Zeige die Antwort des Chats an
    if (!chatResponse.empty()) {
        chatResponseTextBox->AppendText(wxString::FromUTF8(chatResponse) + "\n");
    }

    SetStatusText("Antwort erhalten.");
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

// Funktion für die HTTP-Chat-Anfrage mit libcurl und nlohmann/json
std::string MyFrame::SendChatRequest(const std::string& message) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        wxLogError("curl_easy_init() fehlgeschlagen.");
        return "";
    }

    // Verwende unique_ptr für automatische Bereinigung von curl
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curlHandle(curl, curl_easy_cleanup);

    CURLcode res;
    std::string readBuffer;

    // Setze die URL der Codestral-API für Chat-Anfragen
    res = curl_easy_setopt(curl, CURLOPT_URL, "https://codestral.mistral.ai/v1/chat/completions");
    if (res != CURLE_OK) {
        wxLogError("Fehler beim Setzen der URL: %s", curl_easy_strerror(res));
        return "";
    }

    // Erstelle das JSON-Objekt mit nlohmann/json
    nlohmann::json jsonData;
    jsonData["model"] = "codestral-latest";
    jsonData["messages"] = {
        { {"role", "user"}, {"content", message} }
    };

    // Serialisiere das JSON-Objekt in einen String
    std::string jsonString = jsonData.dump();

    // Setze die POST-Daten
    res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
    if (res != CURLE_OK) {
        wxLogError("Fehler beim Setzen der POST-Daten: %s", curl_easy_strerror(res));
        return "";
    }

    // Setze HTTP-Header
    std::string API_KEY = "YOUR_API_KEY";  // Ersetze "YOUR_API_KEY" durch deinen tatsächlichen API-Schlüssel den du bekommst wenn du dich auf Mistral einloggst mit Google oder Microsoft account und dann auf diese Seite gehst https://console.mistral.ai/codestral
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
        if (http_code != 200) {
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

            // Anpassung an die tatsächliche Struktur der Antwort
            if (responseJson.contains("choices") && responseJson["choices"].is_array() && !responseJson["choices"].empty()) {
                // Überprüfe, ob das Feld "message" vorhanden ist und nicht null ist
                if (responseJson["choices"][0].contains("message") && !responseJson["choices"][0]["message"].is_null()) {
                    // Lese den Text aus dem Feld "content" des "message"-Objekts
                    std::string chatResponse = responseJson["choices"][0]["message"]["content"].get<std::string>();
                    return chatResponse;
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
