# Mindestversion von CMake festlegen
cmake_minimum_required(VERSION 3.14)

# Projektname und verwendete Sprache festlegen
project(MyWxWidgetsApp LANGUAGES CXX)

# Vcpkg-Integration hinzufügen (falls nicht automatisch erkannt)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# wxWidgets finden
find_package(wxWidgets CONFIG REQUIRED)

# Quelle für das Projekt hinzufügen (zum Beispiel main.cpp)
add_executable(MyWxWidgetsApp main.cpp)

# wxWidgets-Bibliotheken verlinken
target_link_libraries(MyWxWidgetsApp PRIVATE wx::core wx::base)

# Subsystem auf "Windows" setzen, um GUI-Anwendung zu erstellen
set_target_properties(MyWxWidgetsApp PROPERTIES WIN32_EXECUTABLE TRUE)
