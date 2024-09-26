# Projekt: wxWidgets C++ Anwendung mit vcpkg und CMake

Dieses Projekt nutzt **wxWidgets** als GUI-Framework, **vcpkg** als Paketmanager und **CMake** zur Verwaltung des Build-Systems. In dieser Anleitung erfährst du, wie du das Projekt auf deinem lokalen System einrichtest.

## Voraussetzungen

Bevor du mit dem Projekt startest, stelle sicher, dass folgende Programme auf deinem Computer installiert sind:

- **Git**: Zum Klonen des Repositories.
- **Visual Studio 2022** oder höher: IDE für die Entwicklung.
- **vcpkg**: Paketmanager zum Installieren von Bibliotheken wie wxWidgets.
- **CMake**: Build-System, das im Projekt verwendet wird.

### 1. Installation von Git

Git wird benötigt, um das Projekt und vcpkg zu klonen. Du kannst Git hier herunterladen: [Git herunterladen](https://git-scm.com/downloads).

### 2. vcpkg installieren

1. Öffne die **Eingabeaufforderung** oder **PowerShell**.
2. Klone vcpkg in ein Verzeichnis deiner Wahl:

    ```bash
    git clone https://github.com/microsoft/vcpkg
    cd vcpkg
    ```

3. Führe das Bootstrap-Skript aus, um vcpkg zu kompilieren:

    - Für Windows:
        ```bash
        .\bootstrap-vcpkg.bat
        ```

4. Optional: Führe den folgenden Befehl aus, um vcpkg systemweit in Visual Studio zu integrieren:

    ```bash
    .\vcpkg integrate install
    ```

   Dieser Schritt stellt sicher, dass Visual Studio vcpkg automatisch erkennt.

### 3. wxWidgets mit vcpkg installieren

Um wxWidgets in deinem Projekt zu verwenden, musst du es mit vcpkg installieren:

1. Öffne die **Eingabeaufforderung** oder **PowerShell** und navigiere zu deinem vcpkg-Verzeichnis (wo du vcpkg installiert hast).
2. Installiere wxWidgets:

    ```bash
    .\vcpkg install wxwidgets
    ```

3. Falls du Debug-Builds benötigst, kannst du auch das Debug-Paket installieren:

    ```bash
    .\vcpkg install wxwidgets --triplet x64-windows
    ```

### 4. CMake installieren

**CMake** wird verwendet, um das Projekt zu konfigurieren und zu kompilieren. Lade CMake herunter und installiere es von der offiziellen Website: [CMake herunterladen](https://cmake.org/download/).

### 5. CMake in Visual Studio einrichten

Wenn du CMake in Visual Studio verwendest, kannst du direkt die `CMakeLists.txt` öffnen, die im Projekt enthalten ist:

1. **Projekt aus Git klonen**:
    - Klone das Projekt von GitHub oder deiner Quelle in ein lokales Verzeichnis:

      ```bash
      git clone <REPO-URL>
      ```

2. **Projekt in Visual Studio öffnen**:
    - Öffne Visual Studio und wähle **Projekt oder Lösung öffnen**.
    - Navigiere zu dem Verzeichnis, in dem sich das Projekt befindet, und wähle die Datei **CMakeLists.txt** aus.

3. **CMake konfigurieren**:
    - Stelle sicher, dass du in Visual Studio unter **CMake > Cache löschen und neu konfigurieren** gehst, um sicherzustellen, dass alle Einstellungen korrekt übernommen werden.
    - Visual Studio verwendet automatisch die `CMakeLists.txt`, um die Build-Konfiguration zu laden. Es sollten keine zusätzlichen manuellen Einstellungen erforderlich sein.

### 6. Subsystem auf Windows setzen (Falls erforderlich)

Falls du das Subsystem manuell setzen musst (bei Problemen mit fehlendem `main()`), kannst du das in Visual Studio machen:
- Gehe zu **Projekt > Eigenschaften > Linker > System** und setze das Subsystem auf **Windows (/SUBSYSTEM:WINDOWS)**.

### 7. Projekt kompilieren und ausführen

Nachdem die Konfiguration abgeschlossen ist, kannst du das Projekt in Visual Studio kompilieren:
- Drücke **Strg + Shift + B** oder wähle **Erstellen > Projektmappe neu erstellen**, um das Projekt zu kompilieren.
- Um das Programm zu starten, drücke **F5** oder klicke auf **Debuggen > Starten ohne Debugging**.

## Problembehandlung

### Fehlende Bibliotheken
Falls du Fehlermeldungen bezüglich fehlender Bibliotheken erhältst, stelle sicher, dass:
- vcpkg installiert und korrekt integriert ist.
- Du alle notwendigen Pakete (wie wxWidgets) mit vcpkg installiert hast.

### CMake-Konfiguration
Falls es Probleme mit der CMake-Konfiguration gibt, kannst du den CMake-Cache leeren und neu konfigurieren:
- Gehe zu **CMake > Cache löschen und neu konfigurieren** in Visual Studio.

#### Chat mit Codestral

Dieses C++-Programm ist eine einfache GUI-basierte Anwendung, die es dem Benutzer ermöglicht, Nachrichten an die Codestral-API zu senden und Antworten in einer grafischen Oberfläche anzuzeigen. Die Anwendung nutzt `wxWidgets` für die Benutzeroberfläche, `libcurl` für die HTTP-Kommunikation und `nlohmann/json` zur Verarbeitung von JSON-Daten.

## Funktionen

- **Text-Eingabe:** Der Benutzer kann eine Nachricht in die Textbox eingeben und diese über die API an Codestral senden.
- **Antwort-Anzeige:** Die Antwort von Codestral wird in einem schreibgeschützten Textfeld angezeigt.
- **Statusleiste:** Zeigt den aktuellen Status der Anfrage und des Programms an (z.B. "Bereit", "Antwort erhalten").

## Voraussetzungen

- C++17 oder höher
- wxWidgets (Version 3.0 oder höher)
- libcurl (mit SSL-Unterstützung)
- nlohmann/json (JSON für moderne C++ Bibliothek)

## Installation

1. **wxWidgets installieren:**
   - Linux: Installiere `libwxgtk3.0-dev` über deinen Paketmanager.
   - macOS: Nutze `brew install wxwidgets`.
   - Windows: Lade die neueste Version von der [wxWidgets Website](https://www.wxwidgets.org/downloads/) herunter.

2. **libcurl installieren:**
   - Linux: Nutze `sudo apt install libcurl4-openssl-dev`.
   - macOS: Nutze `brew install curl`.
   - Windows: Lade die neueste Version von der [cURL Website](https://curl.se/download.html) herunter.

3. **nlohmann/json Bibliothek:**
   - Füge die Bibliothek entweder direkt zu deinem Projekt hinzu oder nutze einen Paketmanager wie `vcpkg`:
     ```bash
     vcpkg install nlohmann-json
     ```

4. **Kompilieren:**
   ```bash
   g++ -std=c++17 main.cpp -o chat_app `wx-config --cxxflags --libs` -lcurl
   ```

   Stelle sicher, dass du deinen API-Schlüssel im Code eingibst (ersetze `YOUR_API_KEY` in der `SendChatRequest`-Funktion).

## Nutzung

1. Starte die Anwendung. Es öffnet sich ein Fenster mit einem Textfeld für die Eingabe von Nachrichten.
2. Gib eine Nachricht ein und drücke den "Nachricht senden"-Button.
3. Die Antwort des Codestral-Chatbots wird im unteren Textfeld angezeigt.

## Beispiel

Hier ist ein kurzer Überblick darüber, wie die API-Kommunikation funktioniert:

```cpp
std::string MyFrame::SendChatRequest(const std::string& message) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        wxLogError("curl_easy_init() fehlgeschlagen.");
        return "";
    }
    std::string readBuffer;
    // Setup der Anfrage an die Codestral-API
    nlohmann::json jsonData = {
        {"model", "codestral-latest"},
        {"messages", { { {"role", "user"}, {"content", message} } }}
    };
    std::string jsonString = jsonData.dump();

    // HTTP-Header und Optionen konfigurieren
    curl_easy_setopt(curl, CURLOPT_URL, "https://codestral.mistral.ai/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

    // Empfange die Antwort
    curl_easy_perform(curl);
    return readBuffer;
}
```

## API Schlüssel

Um das Programm nutzen zu können, benötigst du einen API-Schlüssel für die Codestral-API. Registriere dich auf der [Codestral-Website](https://console.mistral.ai/codestral) und erhalte deinen Schlüssel.

## Lizenz

Dieses Projekt ist unter der MIT-Lizenz lizenziert. Siehe die [LICENSE](LICENSE) Datei für Details.


