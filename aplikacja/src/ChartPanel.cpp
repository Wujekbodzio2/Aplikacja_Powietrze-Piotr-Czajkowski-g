/**
 * @file ChartPanel.cpp
 * @brief Implementacja klasy ChartPanel do wy�wietlania danych pomiarowych w formie wykresu.
 */

#include "ChartPanel.h"
#include <algorithm>

 /// Tablica zdarze� wxWidgets
wxBEGIN_EVENT_TABLE(ChartPanel, wxPanel)
EVT_PAINT(ChartPanel::OnPaint) ///< Obs�uga zdarzenia rysowania panelu
EVT_SIZE(ChartPanel::OnSize)   ///< Obs�uga zdarzenia zmiany rozmiaru
wxEND_EVENT_TABLE()

/**
 * @brief Konstruktor klasy ChartPanel.
 * @param parent Wska�nik na okno nadrz�dne.
 */
    ChartPanel::ChartPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
    minValue(0.0), maxValue(0.0)
{
    SetBackgroundColour(*wxWHITE);
}

/**
 * @brief Ustawia dane do wy�wietlenia na wykresie.
 *
 * Filtruje dane, ustawiaj�c tylko te poprawne, i aktualizuje zakres osi Y.
 *
 * @param measurements Wektor obiekt�w Measurement.
 * @param title Tytu� wykresu.
 */
void ChartPanel::SetData(const std::vector<Measurement>& measurements, const wxString& title)
{
    data.clear();

    // Kopiujemy tylko prawid�owe pomiary
    for (const auto& m : measurements) {
        if (m.isValid()) {
            data.push_back(m);
        }
    }

    chartTitle = title;

    // Znajd� min i max warto�ci
    if (!data.empty()) {
        auto minmax = std::minmax_element(data.begin(), data.end(),
            [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });

        minValue = minmax.first->getValue();
        maxValue = minmax.second->getValue();

        // Dodaj margines
        double range = maxValue - minValue;
        if (range < 0.1) range = 1.0;

        minValue -= range * 0.1;
        maxValue += range * 0.1;

        if (minValue < 0) minValue = 0;
    }

    Refresh();  ///< Od�wie�a panel, wywo�uj�c rysowanie
}

/**
 * @brief Czy�ci dane wykresu i usuwa tytu�.
 */
void ChartPanel::ClearData()
{
    data.clear();
    chartTitle = "";
    Refresh();
}

/**
 * @brief Obs�uguje zmian� rozmiaru panelu.
 * @param event Zdarzenie rozmiaru.
 */
void ChartPanel::OnSize(wxSizeEvent& event)
{
    Refresh();
    event.Skip(); // Przekazuje zdarzenie dalej
}

/**
 * @brief Obs�uguje rysowanie wykresu i element�w interfejsu.
 * @param event Zdarzenie rysowania.
 */
void ChartPanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    wxSize size = GetClientSize();

    // Komunikat, gdy brak danych
    if (data.empty()) {
        wxString message = "Brak danych do wy�wietlenia";
        wxSize textSize = dc.GetTextExtent(message);
        dc.DrawText(message, (size.x - textSize.x) / 2, (size.y - textSize.y) / 2);
        return;
    }

    // Marginesy
    const int leftMargin = 50;
    const int rightMargin = 20;
    const int topMargin = 40;
    const int bottomMargin = 50;

    // Obszar wykresu
    wxRect chartArea(leftMargin, topMargin,
        size.x - leftMargin - rightMargin,
        size.y - topMargin - bottomMargin);

    // Rysowanie tytu�u
    dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    wxSize titleSize = dc.GetTextExtent(chartTitle);
    dc.DrawText(chartTitle, (size.x - titleSize.x) / 2, 10);

    // Osie
    dc.SetPen(wxPen(*wxBLACK, 1));
    dc.DrawLine(chartArea.GetLeft(), chartArea.GetBottom(), chartArea.GetRight(), chartArea.GetBottom());  ///< O� X
    dc.DrawLine(chartArea.GetLeft(), chartArea.GetTop(), chartArea.GetLeft(), chartArea.GetBottom());      ///< O� Y

    // Czcionka do etykiet
    dc.SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Etykiety osi Y
    const int yLabelCount = 5;
    for (int i = 0; i <= yLabelCount; i++) {
        double value = minValue + (maxValue - minValue) * i / yLabelCount;
        int y = chartArea.GetBottom() - chartArea.GetHeight() * i / yLabelCount;

        wxString label = wxString::Format("%.1f", value);
        wxSize labelSize = dc.GetTextExtent(label);

        dc.DrawText(label, chartArea.GetLeft() - labelSize.x - 5, y - labelSize.y / 2);

        // Linie siatki
        dc.SetPen(wxPen(wxColour(200, 200, 200), 1, wxPENSTYLE_DOT));
        dc.DrawLine(chartArea.GetLeft(), y, chartArea.GetRight(), y);
    }

    // Etykiety osi X
    const int xLabelCount = std::min(10, static_cast<int>(data.size()));
    int dataStep = std::max(1, static_cast<int>(data.size()) / xLabelCount);

    for (size_t i = 0; i < data.size(); i += dataStep) {
        int x = chartArea.GetLeft() + chartArea.GetWidth() * i / (data.size() - 1);

        wxString date = wxString::FromUTF8(data[i].getDate().substr(0, 10));
        wxString time = wxString::FromUTF8(data[i].getDate().substr(11, 5));

        wxSize dateSize = dc.GetTextExtent(date);
        wxSize timeSize = dc.GetTextExtent(time);

        dc.DrawText(date, x - dateSize.x / 2, chartArea.GetBottom() + 5);
        dc.DrawText(time, x - timeSize.x / 2, chartArea.GetBottom() + 5 + dateSize.y);

        // Linie siatki pionowej
        dc.SetPen(wxPen(wxColour(200, 200, 200), 1, wxPENSTYLE_DOT));
        dc.DrawLine(x, chartArea.GetTop(), x, chartArea.GetBottom());
    }

    // Rysowanie wykresu
    if (data.size() > 1) {
        dc.SetPen(wxPen(*wxBLUE, 2));

        wxPoint prevPoint;
        bool firstPoint = true;

        for (size_t i = 0; i < data.size(); i++) {
            int x = chartArea.GetLeft() + chartArea.GetWidth() * i / (data.size() - 1);
            int y = chartArea.GetBottom() - chartArea.GetHeight() *
                (data[i].getValue() - minValue) / (maxValue - minValue);

            // Punkt danych
            dc.SetBrush(*wxBLUE_BRUSH);
            dc.DrawCircle(x, y, 3);

            // ��czenie punkt�w
            if (!firstPoint) {
                dc.DrawLine(prevPoint, wxPoint(x, y));
            }

            prevPoint = wxPoint(x, y);
            firstPoint = false;
        }
    }
}