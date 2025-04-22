/**
 * @file main.cpp
 * @brief Punkt wejścia do aplikacji wxWidgets - tworzy i pokazuje główne okno (MainFrame).
 */

#include <wx/wx.h>
#include "MainFrame.h"

 /**
  * @class MyApp
  * @brief Główna klasa aplikacji dziedzicząca po wxApp.
  *
  * Odpowiada za inicjalizację lokalizacji oraz uruchomienie głównego okna aplikacji.
  */
class MyApp : public wxApp {
public:
    /**
     * @brief Funkcja inicjalizacyjna aplikacji wxWidgets.
     *
     * Ustawia lokalizację na język polski, tworzy i pokazuje główne okno.
     *
     * @return true jeśli inicjalizacja się powiodła.
     */
    bool OnInit() override {
        wxLocale locale;
        setlocale(LC_ALL, "");
        locale.Init(wxLANGUAGE_POLISH);
        MainFrame* frame = new MainFrame("Jakosc_Powietrza-Piotr Czajkowski");
        frame->Show(true);
        return true;
    }
};

/// Makro wxWidgets, które definiuje główną funkcję main() i uruchamia aplikację.
wxIMPLEMENT_APP(MyApp);