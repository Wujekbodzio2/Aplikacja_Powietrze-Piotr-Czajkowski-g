#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/notebook.h>
#include "ApiClient.h"
#include "MeasurementAnalyzer.h"
#include "ChartPanel.h"
#include "DatabaseManager.h"

/**
 * @file MainFrame.h
 * @brief G³ówne okno aplikacji GUI opartej na wxWidgets do analizy jakoœci powietrza.
 *
 * Umo¿liwia pobieranie, zapisywanie i wizualizacjê danych pomiarowych ze stacji monitoringu powietrza.
 */
class MainFrame : public wxFrame {
public:
    /**
     * @brief Konstruktor klasy MainFrame.
     *
     * @param title Tytu³ g³ównego okna aplikacji.
     */
    MainFrame(const wxString& title);

private:
    /**
     * @brief Handler wyboru stacji z listy.
     *
     * @param event Zdarzenie wyboru stacji.
     */
    void OnStationSelected(wxCommandEvent& event);

    /**
     * @brief Handler przycisku pobierania danych z API.
     *
     * @param event Zdarzenie klikniêcia przycisku.
     */
    void OnFetch(wxCommandEvent& event);

    /**
     * @brief Handler zapisu danych do lokalnej bazy danych.
     *
     * @param event Zdarzenie klikniêcia przycisku.
     */
    void OnSaveToDb(wxCommandEvent& event);

    /**
     * @brief Handler ³adowania danych z lokalnej bazy danych.
     *
     * @param event Zdarzenie klikniêcia przycisku.
     */
    void OnLoadFromDb(wxCommandEvent& event);

    /**
     * @brief Prze³¹cza aplikacjê w tryb offline (gdy np. brak po³¹czenia z API).
     */
    void SwitchToOfflineMode();

    bool isOfflineMode;  ///< Flaga okreœlaj¹ca, czy aplikacja dzia³a w trybie offline.

    // Komponenty GUI
    wxPanel* panel;                   ///< G³ówny panel aplikacji.
    wxComboBox* stationCombo;        ///< Lista rozwijana ze stacjami.
    wxComboBox* sensorCombo;         ///< Lista rozwijana z sensorami.
    wxDatePickerCtrl* dateFrom;      ///< Wybór daty pocz¹tkowej.
    wxDatePickerCtrl* dateTo;        ///< Wybór daty koñcowej.
    wxTextCtrl* dataText;            ///< Pole tekstowe do wyœwietlania danych.
    wxTextCtrl* extraText;           ///< Dodatkowe pole tekstowe (np. informacje statystyczne).
    wxStaticText* infoLabel;         ///< Etykieta informacyjna.
    wxNotebook* notebook;            ///< Notebook (zak³adki) do wyœwietlania danych i wykresów.
    ChartPanel* chartPanel;          ///< Panel z wykresem.

    wxButton* fetchBtn;              ///< Przycisk do pobierania danych z API.
    wxButton* saveToDbBtn;           ///< Przycisk do zapisywania danych do bazy.
    wxButton* loadFromDbBtn;         ///< Przycisk do ³adowania danych z bazy.

    ApiClient api;                   ///< Klient API do pobierania danych online.
    DatabaseManager dbManager;       ///< Manager lokalnej bazy danych.

    std::vector<Station> stations;               ///< Lista dostêpnych stacji.
    std::vector<Sensor> currentSensors;          ///< Lista sensorów aktualnie wybranej stacji.
    std::vector<Measurement> currentMeasurements;///< Aktualnie pobrane lub za³adowane pomiary.
};