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
 * @brief G��wne okno aplikacji GUI opartej na wxWidgets do analizy jako�ci powietrza.
 *
 * Umo�liwia pobieranie, zapisywanie i wizualizacj� danych pomiarowych ze stacji monitoringu powietrza.
 */
class MainFrame : public wxFrame {
public:
    /**
     * @brief Konstruktor klasy MainFrame.
     *
     * @param title Tytu� g��wnego okna aplikacji.
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
     * @param event Zdarzenie klikni�cia przycisku.
     */
    void OnFetch(wxCommandEvent& event);

    /**
     * @brief Handler zapisu danych do lokalnej bazy danych.
     *
     * @param event Zdarzenie klikni�cia przycisku.
     */
    void OnSaveToDb(wxCommandEvent& event);

    /**
     * @brief Handler �adowania danych z lokalnej bazy danych.
     *
     * @param event Zdarzenie klikni�cia przycisku.
     */
    void OnLoadFromDb(wxCommandEvent& event);

    /**
     * @brief Prze��cza aplikacj� w tryb offline (gdy np. brak po��czenia z API).
     */
    void SwitchToOfflineMode();

    bool isOfflineMode;  ///< Flaga okre�laj�ca, czy aplikacja dzia�a w trybie offline.

    // Komponenty GUI
    wxPanel* panel;                   ///< G��wny panel aplikacji.
    wxComboBox* stationCombo;        ///< Lista rozwijana ze stacjami.
    wxComboBox* sensorCombo;         ///< Lista rozwijana z sensorami.
    wxDatePickerCtrl* dateFrom;      ///< Wyb�r daty pocz�tkowej.
    wxDatePickerCtrl* dateTo;        ///< Wyb�r daty ko�cowej.
    wxTextCtrl* dataText;            ///< Pole tekstowe do wy�wietlania danych.
    wxTextCtrl* extraText;           ///< Dodatkowe pole tekstowe (np. informacje statystyczne).
    wxStaticText* infoLabel;         ///< Etykieta informacyjna.
    wxNotebook* notebook;            ///< Notebook (zak�adki) do wy�wietlania danych i wykres�w.
    ChartPanel* chartPanel;          ///< Panel z wykresem.

    wxButton* fetchBtn;              ///< Przycisk do pobierania danych z API.
    wxButton* saveToDbBtn;           ///< Przycisk do zapisywania danych do bazy.
    wxButton* loadFromDbBtn;         ///< Przycisk do �adowania danych z bazy.

    ApiClient api;                   ///< Klient API do pobierania danych online.
    DatabaseManager dbManager;       ///< Manager lokalnej bazy danych.

    std::vector<Station> stations;               ///< Lista dost�pnych stacji.
    std::vector<Sensor> currentSensors;          ///< Lista sensor�w aktualnie wybranej stacji.
    std::vector<Measurement> currentMeasurements;///< Aktualnie pobrane lub za�adowane pomiary.
};