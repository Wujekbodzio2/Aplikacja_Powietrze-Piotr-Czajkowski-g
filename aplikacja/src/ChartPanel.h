// Plik: ChartPanel.h
#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>
#include "Measurement.h"

/**
 * @file ChartPanel.h
 * @brief Klasa ChartPanel odpowiadaj¹ca za wyœwietlanie wykresu pomiarów w aplikacji wxWidgets.
 */
class ChartPanel : public wxPanel {
public:
    /**
     * @brief Konstruktor klasy ChartPanel.
     *
     * @param parent WskaŸnik na okno nadrzêdne.
     */
    ChartPanel(wxWindow* parent);

    /**
     * @brief Ustawia dane do wyœwietlenia na wykresie.
     *
     * @param measurements Wektor pomiarów do narysowania.
     * @param title Tytu³ wykresu.
     */
    void SetData(const std::vector<Measurement>& measurements, const wxString& title);

    /**
     * @brief Czyœci dane wykresu.
     */
    void ClearData();

private:
    /**
     * @brief Obs³uga zdarzenia rysowania panelu.
     *
     * @param event Obiekt zdarzenia rysowania.
     */
    void OnPaint(wxPaintEvent& event);

    /**
     * @brief Obs³uga zmiany rozmiaru panelu.
     *
     * @param event Obiekt zdarzenia zmiany rozmiaru.
     */
    void OnSize(wxSizeEvent& event);

    std::vector<Measurement> data;     ///< Dane pomiarowe do wyœwietlenia.
    wxString chartTitle;               ///< Tytu³ wykresu.
    double minValue;                   ///< Minimalna wartoœæ na osi Y.
    double maxValue;                   ///< Maksymalna wartoœæ na osi Y.

    wxDECLARE_EVENT_TABLE();          ///< Makro do deklaracji tablicy zdarzeñ wxWidgets.
};