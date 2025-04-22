// Plik: ChartPanel.h
#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>
#include "Measurement.h"

/**
 * @file ChartPanel.h
 * @brief Klasa ChartPanel odpowiadaj�ca za wy�wietlanie wykresu pomiar�w w aplikacji wxWidgets.
 */
class ChartPanel : public wxPanel {
public:
    /**
     * @brief Konstruktor klasy ChartPanel.
     *
     * @param parent Wska�nik na okno nadrz�dne.
     */
    ChartPanel(wxWindow* parent);

    /**
     * @brief Ustawia dane do wy�wietlenia na wykresie.
     *
     * @param measurements Wektor pomiar�w do narysowania.
     * @param title Tytu� wykresu.
     */
    void SetData(const std::vector<Measurement>& measurements, const wxString& title);

    /**
     * @brief Czy�ci dane wykresu.
     */
    void ClearData();

private:
    /**
     * @brief Obs�uga zdarzenia rysowania panelu.
     *
     * @param event Obiekt zdarzenia rysowania.
     */
    void OnPaint(wxPaintEvent& event);

    /**
     * @brief Obs�uga zmiany rozmiaru panelu.
     *
     * @param event Obiekt zdarzenia zmiany rozmiaru.
     */
    void OnSize(wxSizeEvent& event);

    std::vector<Measurement> data;     ///< Dane pomiarowe do wy�wietlenia.
    wxString chartTitle;               ///< Tytu� wykresu.
    double minValue;                   ///< Minimalna warto�� na osi Y.
    double maxValue;                   ///< Maksymalna warto�� na osi Y.

    wxDECLARE_EVENT_TABLE();          ///< Makro do deklaracji tablicy zdarze� wxWidgets.
};