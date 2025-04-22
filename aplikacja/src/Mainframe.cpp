/**
 * @file MainFrame.cpp
 * @brief Implementacja klasy MainFrame – głównego okna aplikacji GUI dla monitorowania jakości powietrza.
 */


#include "MainFrame.h"

 /**
  * @brief Konstruktor klasy MainFrame.
  *
  * Tworzy i konfiguruje główne komponenty GUI: pola wyboru stacji i czujników, daty, przyciski oraz zakładki z danymi i wykresem.
  * Inicjalizuje tryb online lub offline w zależności od dostępności danych z API.
  *
  * @param title Tytuł okna aplikacji.
  */
MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 800)),
    dbManager("data/air_quality_data.json"),
    isOfflineMode(false) {  // inicjalizacja trybu ofline

    // Tworzenie głównego panelu i układu pionowego (sizer)
    panel = new wxPanel(this);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Etykieta: "Wybierz stację"
    wxStaticText* label1 = new wxStaticText(panel, wxID_ANY, wxString::FromUTF8("Wybierz stację:"));
    vbox->Add(label1, 0, wxALL, 10);

    // Lista rozwijana z dostępnymi stacjami
    stationCombo = new wxComboBox(panel, wxID_ANY);
    vbox->Add(stationCombo, 0, wxEXPAND | wxALL, 10);
    // Obsługa zdarzenia wyboru stacji
    stationCombo->Bind(wxEVT_COMBOBOX, &MainFrame::OnStationSelected, this);

    // Etykieta: "Wybierz czujnik"
    wxStaticText* label2 = new wxStaticText(panel, wxID_ANY, "Wybierz czujnik:");
    vbox->Add(label2, 0, wxALL, 10);

    // Lista rozwijana z czujnikami
    sensorCombo = new wxComboBox(panel, wxID_ANY);
    vbox->Add(sensorCombo, 0, wxEXPAND | wxALL, 10);

    // Układ poziomy dla wyboru dat (od - do)
    wxBoxSizer* dateSizer = new wxBoxSizer(wxHORIZONTAL);
    dateFrom = new wxDatePickerCtrl(panel, wxID_ANY); // data początkowa
    dateTo = new wxDatePickerCtrl(panel, wxID_ANY);   // data końcowa

    // Dodanie etykiet i kontrolek wyboru dat do sizer'a
    dateSizer->Add(new wxStaticText(panel, wxID_ANY, "Od:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    dateSizer->Add(dateFrom, 0, wxRIGHT, 10);
    dateSizer->Add(new wxStaticText(panel, wxID_ANY, "Do:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    dateSizer->Add(dateTo, 0);
    vbox->Add(dateSizer, 0, wxALL, 10);

    // Układ poziomy dla przycisków
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Przycisk do pobierania danych
    fetchBtn = new wxButton(panel, wxID_ANY, "Pobierz dane");
    buttonSizer->Add(fetchBtn, 1, wxEXPAND | wxRIGHT, 10);
    fetchBtn->Bind(wxEVT_BUTTON, &MainFrame::OnFetch, this);

    // Przycisk do zapisu danych do bazy – na początku nieaktywny
    saveToDbBtn = new wxButton(panel, wxID_ANY, "Zapisz do bazy danych");
    buttonSizer->Add(saveToDbBtn, 1, wxEXPAND | wxRIGHT, 10);
    saveToDbBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSaveToDb, this);
    saveToDbBtn->Disable();  // Dezaktywacja do czasu pobrania danych

    // Przycisk do wczytywania danych z bazy
    loadFromDbBtn = new wxButton(panel, wxID_ANY, "Wczytaj z bazy danych");
    buttonSizer->Add(loadFromDbBtn, 1, wxEXPAND);
    loadFromDbBtn->Bind(wxEVT_BUTTON, &MainFrame::OnLoadFromDb, this);

    // Dodanie paska przycisków do głównego sizer'a
    vbox->Add(buttonSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Etykieta informacyjna – ukryta do czasu potrzeby pokazania komunikatu
    infoLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    infoLabel->Hide();
    vbox->Add(infoLabel, 0, wxLEFT | wxBOTTOM, 10);

    // Notebook – zakładki do przełączania między widokiem danych tekstowych a wykresem
    notebook = new wxNotebook(panel, wxID_ANY);
    vbox->Add(notebook, 1, wxEXPAND | wxALL, 10);

    // Panel na dane tekstowe i analizę
    wxPanel* dataPanel = new wxPanel(notebook);
    wxBoxSizer* dataPanelSizer = new wxBoxSizer(wxHORIZONTAL);

    // Pole tekstowe do wyświetlania danych pomiarowych
    dataText = new wxTextCtrl(dataPanel, wxID_ANY, "", wxDefaultPosition, wxSize(480, 400),
        wxTE_MULTILINE | wxTE_READONLY);

    // Pole tekstowe do wyświetlania dodatkowych informacji lub analiz
    extraText = new wxTextCtrl(dataPanel, wxID_ANY, "", wxDefaultPosition, wxSize(480, 400),
        wxTE_MULTILINE | wxTE_READONLY);

    // Dodanie obu pól tekstowych do sizer'a
    dataPanelSizer->Add(dataText, 1, wxEXPAND | wxALL, 10);
    dataPanelSizer->Add(extraText, 1, wxEXPAND | wxALL, 10);
    dataPanel->SetSizer(dataPanelSizer);  // Ustawienie layoutu panelu danych

    // Panel z wykresem
    chartPanel = new ChartPanel(notebook);

    // Dodanie zakładek do notesooka
    notebook->AddPage(dataPanel, "Dane i analiza");
    notebook->AddPage(chartPanel, "Wykres");

    panel->SetSizer(vbox);

    // Wczytanie stacji
    try {
        stations = api.getStations();
        for (const auto& station : stations) {
            stationCombo->Append(wxString::FromUTF8(station.getName()));
        }
    }
    catch (const std::exception& e) {
        wxMessageBox(wxString::FromUTF8("Brak internetu - przełączam na tryb offline"), "Informacja", wxOK | wxICON_INFORMATION);
        SwitchToOfflineMode();
    }
}

/**
 * @brief Przełącza aplikację w tryb offline, jeśli nie można pobrać danych z API.
 *
 * Wyłącza przyciski online, wczytuje zapisane stacje i czujniki z lokalnej bazy danych,
 * informuje użytkownika o trybie offline.
 */
void MainFrame::SwitchToOfflineMode() {
    isOfflineMode = true;

	// wyczyść i wyłącz przyciski
    fetchBtn->Disable();
    saveToDbBtn->Disable();

	// wyczyszczenie comboboxow
    stationCombo->Clear();
    sensorCombo->Clear();

    auto dbStations = dbManager.getSavedStations();

    if (dbStations.empty()) {
        stationCombo->Disable();
        sensorCombo->Disable();
        loadFromDbBtn->Disable();
        return;
    }

	// dodanie stacji do comboboxa
    stations.clear();
    for (const auto& station : dbStations) {
        stationCombo->Append(wxString::FromUTF8(station.getName()));
        stations.push_back(station); // Zamiast emplace_back tworzymy już gotową stację
    }

	// włączamy comboboxy
    stationCombo->Enable();
	sensorCombo->Disable(); // bedzie włączone po wyborze stacji
    loadFromDbBtn->Enable();

	// ustawienie etykiety informacyjnej
    infoLabel->SetLabel("TRYB OFFLINE - dane z lokalnej bazy");
    infoLabel->SetForegroundColour(*wxRED);
    infoLabel->Show();
    panel->Layout();
}

/**
 * @brief Obsługuje zdarzenie wyboru stacji z listy.
 *
 * W zależności od trybu (online/offline), ładuje czujniki z API lub z bazy danych dla wybranej stacji.
 *
 * @param event Zdarzenie wxEVT_COMBOBOX związane z wyborem stacji.
 */
void MainFrame::OnStationSelected(wxCommandEvent& event) {
    int sel = stationCombo->GetSelection();
    if (sel == wxNOT_FOUND) return;

    int stationId = stations[sel].getId();

	// Wyczyszanie poprzednich czujników
    sensorCombo->Clear();
    currentSensors.clear();

    if (isOfflineMode) {
        // pobieiebranie czujnikow z bazy        
        auto dbSensors = dbManager.getSavedSensors(stationId);

        if (dbSensors.empty()) {
            sensorCombo->Append("Brak zapisanych czujników dla tej stacji");
            sensorCombo->Disable();
            return;
        }

		// Dodaje dane  do comboboxa
        for (const auto& sensor : dbSensors) {
            sensorCombo->Append(wxString::FromUTF8(sensor.getParamName()));
            currentSensors.push_back(sensor); // Tu bezpośrednio używamy obiektu Sensor
        }

        sensorCombo->Enable();
    }
    else {
		// Pobieranie czujników z API
        try {
            currentSensors = api.getSensors(stationId);
            for (const auto& sensor : currentSensors) {
                sensorCombo->Append(wxString::FromUTF8(sensor.getParamName()));
            }
        }
        catch (const std::exception& e) {
            wxMessageBox("Błąd podczas pobierania czujników: " + std::string(e.what()),
                "Błąd", wxOK | wxICON_ERROR);
            SwitchToOfflineMode();
        }
    }
}

/**
 * @brief Pobiera dane pomiarowe z API i wykonuje analizę.
 *
 * Weryfikuje wybór stacji i czujnika, zakres dat, pobiera dane z API, analizuje je, wyświetla dane tekstowo i w formie wykresu.
 * Aktywuje przycisk zapisu danych do bazy.
 *
 * @param event Zdarzenie kliknięcia przycisku "Pobierz dane".
 */
void MainFrame::OnFetch(wxCommandEvent& event) {
    int selStation = stationCombo->GetSelection();
    int selSensor = sensorCombo->GetSelection();

    if (selStation == wxNOT_FOUND || selSensor == wxNOT_FOUND) {
        wxMessageBox("Wybierz stację i czujnik!", "Błąd", wxOK | wxICON_ERROR);
        return;
    }

    int stationId = stations[selStation].getId();
    const auto& sensor = currentSensors[selSensor];

    std::ostringstream dataOut;
    std::ostringstream analysisOut;

    try {
        wxDateTime fromDate = dateFrom->GetValue();
        wxDateTime toDate = dateTo->GetValue().Add(wxTimeSpan::Days(1));

        if (fromDate > toDate) {
           throw std::runtime_error("Data początkowa nie może być późniejsza od końcowej.");
        }

        // Indeks jakości powietrza
        auto index = api.getAirQualityIndex(stationId);
        if (index.count("Ogólny")) {
            infoLabel->SetLabel(wxString::FromUTF8("Ogólny indeks jakości powietrza: " + index["Ogólny"]));
        }
        else {
            infoLabel->SetLabel("Brak ogólnego indeksu jakości powietrza.");
        }
        infoLabel->Show();
        panel->Layout(); // Odśwież układ

        // Pomiar
        auto measurements = api.getSensorData(sensor.getId());
		currentMeasurements = measurements;  // zapisz dla bazy danych

        std::vector<Measurement> filtered;
        for (const auto& m : measurements) {
            wxDateTime mDate;
            mDate.ParseFormat(m.getDate(), "%Y-%m-%d %H:%M:%S");

            if (!mDate.IsValid()) continue;
            if (mDate >= fromDate && mDate <= toDate) {
                dataOut << mDate.FormatISOCombined(' ') << " - "
                    << (m.isValid() ? std::to_string(m.getValue()) : "brak pomiaru") << "\n";
                filtered.push_back(m);
            }
        }

        dataText->SetValue(dataOut.str());

        // Analiza
        MeasurementAnalyzer analyzer(filtered);
        if (!analyzer.hasData()) {
            throw std::runtime_error("Brak danych pomiarowych.");
        }

        analysisOut << "Analiza dla czujnika: " << sensor.getParamName() << "\n\n";
        analysisOut << "Liczba pomiarów: " << filtered.size() << "\n";
        analysisOut << "Min: " << analyzer.getMinValue() << " (" << analyzer.getMinDate() << ")\n";
        analysisOut << "Max: " << analyzer.getMaxValue() << " (" << analyzer.getMaxDate() << ")\n";
        analysisOut << "Średnia: " << analyzer.getAverage() << "\n";
        analysisOut << "Trend: " << analyzer.getTrendDescription() << "\n";

        extraText->SetValue(analysisOut.str());

        // Utwórz tytuł wykresu
        wxString chartTitle = wxString::Format("Wykres pomiarów %s",
            wxString::FromUTF8(sensor.getParamName()));

        // Ustaw dane na wykresie
        chartPanel->SetData(filtered, chartTitle);

        // Przełącz na zakładkę z wykresem
        notebook->SetSelection(1);

		// umozliwia zapisywanie danych do bazy
        saveToDbBtn->Enable();
    }
    catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Błąd: %s", e.what()), "Błąd", wxOK | wxICON_ERROR);
        
        
    }
}
/**
 * @brief Zapisuje dane pomiarowe do lokalnej bazy danych.
 *
 * Weryfikuje wybór stacji i czujnika, a następnie zapisuje dane do pliku JSON.
 * Informuje użytkownika o sukcesie lub błędzie operacji.
 *
 * @param event Zdarzenie kliknięcia przycisku "Zapisz do bazy danych".
 */
// Implementation of the OnSaveToDb method
void MainFrame::OnSaveToDb(wxCommandEvent& event) {
    int selStation = stationCombo->GetSelection();
    int selSensor = sensorCombo->GetSelection();

    if (selStation == wxNOT_FOUND || selSensor == wxNOT_FOUND || currentMeasurements.empty()) {
        wxMessageBox("Brak danych do zapisania!", "Błąd", wxOK | wxICON_ERROR);
        return;
    }

    int stationId = stations[selStation].getId();
    std::string stationName = stations[selStation].getName();

    int sensorId = currentSensors[selSensor].getId();
    std::string sensorName = currentSensors[selSensor].getParamName();

    try {
        // Indeks jakości powietrza do zapisania
        std::map<std::string, std::string> indexValues;

        // Jeśli jesteśmy w trybie online, pobierz indeks jakości powietrza
        if (!isOfflineMode) {
            try {
                indexValues = api.getAirQualityIndex(stationId);
            }
            catch (const std::exception& e) {
                // Jeśli nie uda się pobrać indeksu, ignorujemy błąd
                // i kontynuujemy z zapisem danych pomiarowych
            }
        }

        // Przekazujemy dane pomiarowe i indeksy jakości powietrza do zapisania
        bool saved = dbManager.saveData(stationId, stationName, sensorId, sensorName, currentMeasurements, indexValues);

        if (saved) {
            wxMessageBox("Dane zostały zapisane do bazy danych!", "Sukces", wxOK | wxICON_INFORMATION);
        }
        else {
            throw std::runtime_error("Nie udało się zapisać danych do bazy danych.");
        }
    }
    catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Błąd: %s", e.what()), "Błąd", wxOK | wxICON_ERROR);
    }
}
/**
 * @brief Wczytuje dane pomiarowe z lokalnej bazy danych i analizuje je.
 *
 * Filtruje dane po wybranym zakresie dat, prezentuje analizę oraz wykres.
 *
 * @param event Zdarzenie kliknięcia przycisku "Wczytaj z bazy danych".
 */
void MainFrame::OnLoadFromDb(wxCommandEvent& event) {
    int selStation = stationCombo->GetSelection();
    int selSensor = sensorCombo->GetSelection();
    if (selStation == wxNOT_FOUND || selSensor == wxNOT_FOUND) {
        wxMessageBox("Wybierz stację i czujnik!", "Błąd", wxOK | wxICON_ERROR);
        return;
    }
    int stationId = stations[selStation].getId();
    int sensorId = currentSensors[selSensor].getId();
    std::string sensorName = currentSensors[selSensor].getParamName();
    std::vector<Measurement> measurements;
    try {
        if (!dbManager.loadData(stationId, sensorId, measurements)) {
            wxMessageBox("Brak danych dla wybranej stacji i czujnika w bazie danych.",
                "Informacja", wxOK | wxICON_INFORMATION);
            return;
        }
        currentMeasurements = measurements;

        // Próba wczytania indeksu jakości powietrza
        std::map<std::string, std::string> airQualityIndex;
        if (dbManager.loadAirQualityIndex(stationId, airQualityIndex) &&
            airQualityIndex.count("Ogólny")) {
            infoLabel->SetLabel(wxString::FromUTF8("Ogólny indeks jakości powietrza: " +
                airQualityIndex["Ogólny"] + " (dane z bazy)"));
        }
        else {
            infoLabel->SetLabel("Dane wczytane z lokalnej bazy");
        }

        // dane flitrowane poprzez date
        wxDateTime fromDate = dateFrom->GetValue();
        wxDateTime toDate = dateTo->GetValue().Add(wxTimeSpan::Days(1));
        std::vector<Measurement> filtered;
        std::ostringstream dataOut;
        for (const auto& m : measurements) {
            wxDateTime mDate;
            mDate.ParseFormat(m.getDate(), "%Y-%m-%d %H:%M:%S");
            if (!mDate.IsValid()) continue;
            if (mDate >= fromDate && mDate <= toDate) {
                dataOut << mDate.FormatISOCombined(' ') << " - "
                    << (m.isValid() ? std::to_string(m.getValue()) : "brak pomiaru") << "\n";
                filtered.push_back(m);
            }
        }
        // Pokazuje dane
        dataText->SetValue(dataOut.str());
        // Analizuje dane
        MeasurementAnalyzer analyzer(filtered);
        std::ostringstream analysisOut;
        if (!analyzer.hasData()) {
            analysisOut << "Brak danych pomiarowych w wybranym zakresie dat.\n";
        }
        else {
            analysisOut << "Analiza dla czujnika: " << sensorName << " (dane z bazy)\n\n";
            analysisOut << wxString::FromUTF8("Liczba pomiarów: ") << filtered.size() << "\n";
            analysisOut << "Min: " << analyzer.getMinValue() << " (" << analyzer.getMinDate() << ")\n";
            analysisOut << "Max: " << analyzer.getMaxValue() << " (" << analyzer.getMaxDate() << ")\n";
            analysisOut << "Średnia: " << analyzer.getAverage() << "\n";
            analysisOut << "Trend: " << analyzer.getTrendDescription() << "\n";
			// ustawienie wykresu
            wxString chartTitle = wxString::Format("Wykres pomiarów %s (dane z bazy)",
                wxString::FromUTF8(sensorName));
            chartPanel->SetData(filtered, chartTitle);
			// Zamiast pokazywać wykres, pokazuje dane
            notebook->SetSelection(1);
        }
        extraText->SetValue(analysisOut.str());

        if (!isOfflineMode) {
            infoLabel->SetForegroundColour(*wxBLUE);
        }
        infoLabel->Show();
        panel->Layout();
    }
    catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Błąd podczas wczytywania danych: %s", e.what()),
            "Błąd", wxOK | wxICON_ERROR);
    }
}