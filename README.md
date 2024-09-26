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

## Kontakt

Bei Fragen oder Problemen, wende dich gerne an Ralf Krümmel oder erstelle ein Issue im Repository.
```

### Erläuterungen zu den Schritten:

1. **Installation von Git**: Git wird benötigt, um das Projekt und vcpkg zu klonen.
2. **vcpkg installieren**: Schritt für Schritt-Anleitung, wie vcpkg heruntergeladen und installiert wird.
3. **wxWidgets installieren**: Wie wxWidgets über vcpkg installiert wird.
4. **CMake installieren**: CMake ist erforderlich, da das Projekt CMake verwendet.
5. **CMake in Visual Studio einrichten**: Schritt-für-Schritt-Anleitung, wie man ein CMake-Projekt in Visual Studio öffnet und konfiguriert.
6. **Subsystem auf Windows setzen**: Falls erforderlich, wie das Subsystem auf Windows gesetzt wird.
7. **Kompilieren und Ausführen**: Anleitung zur Kompilierung und Ausführung des Projekts in Visual Studio.

