/**
 * @file MeasurementAnalyzer.h
 * @brief Klasa do analizy pomiar�w, takich jak min, max, �rednia i trend.
 */

#include "MeasurementAnalyzer.h"
#include <algorithm>
#include <numeric>

 /**
  * @brief Konstruktor klasy MeasurementAnalyzer.
  *
  * Filtruje tylko prawid�owe pomiary i zapisuje je do wewn�trznej listy.
  *
  * @param measurements Wektor obiekt�w Measurement.
  */
MeasurementAnalyzer::MeasurementAnalyzer(const std::vector<Measurement>& measurements) {
    for (const auto& m : measurements) {
        if (m.isValid()) {
            validMeasurements.push_back(m);
        }
    }
}

/**
 * @brief Sprawdza, czy s� dost�pne jakiekolwiek dane pomiarowe.
 *
 * @return true je�li s� dost�pne dane, false w przeciwnym razie.
 */
bool MeasurementAnalyzer::hasData() const {
    return !validMeasurements.empty();
}

/**
 * @brief Zwraca minimaln� warto�� spo�r�d dost�pnych pomiar�w.
 *
 * @return Minimalna warto�� lub -1.0, je�li brak danych.
 */
double MeasurementAnalyzer::getMinValue() const {
    if (!hasData()) return -1.0;
    auto min = std::min_element(validMeasurements.begin(), validMeasurements.end(),
        [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });
    return min->getValue();
}

/**
 * @brief Zwraca dat�, w kt�rej wyst�pi�a minimalna warto��.
 *
 * @return Data minimalnej warto�ci lub "brak danych", je�li brak danych.
 */
std::string MeasurementAnalyzer::getMinDate() const {
    if (!hasData()) return "brak danych";
    auto min = std::min_element(validMeasurements.begin(), validMeasurements.end(),
        [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });
    return min->getDate();
}

/**
 * @brief Zwraca maksymaln� warto�� spo�r�d dost�pnych pomiar�w.
 *
 * @return Maksymalna warto�� lub -1.0, je�li brak danych.
 */
double MeasurementAnalyzer::getMaxValue() const {
    if (!hasData()) return -1.0;
    auto max = std::max_element(validMeasurements.begin(), validMeasurements.end(),
        [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });
    return max->getValue();
}

/**
 * @brief Zwraca dat�, w kt�rej wyst�pi�a maksymalna warto��.
 *
 * @return Data maksymalnej warto�ci lub "brak danych", je�li brak danych.
 */
std::string MeasurementAnalyzer::getMaxDate() const {
    if (!hasData()) return "brak danych";
    auto max = std::max_element(validMeasurements.begin(), validMeasurements.end(),
        [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });
    return max->getDate();
}

/**
 * @brief Oblicza i zwraca �redni� z warto�ci pomiar�w.
 *
 * @return �rednia warto�� lub -1.0, je�li brak danych.
 */
double MeasurementAnalyzer::getAverage() const {
    if (!hasData()) return -1.0;
    double sum = std::accumulate(validMeasurements.begin(), validMeasurements.end(), 0.0,
        [](double total, const Measurement& m) { return total + m.getValue(); });
    return sum / validMeasurements.size();
}

/**
 * @brief Okre�la trend na podstawie regresji liniowej.
 *
 * Trend mo�e by� wzrostowy, malej�cy, stabilny lub nieznany.
 *
 * @return Warto�� enum Trend opisuj�ca kierunek zmian.
 */
MeasurementAnalyzer::Trend MeasurementAnalyzer::getTrend() const {
    if (validMeasurements.size() < 2) return Trend::UNKNOWN;

    double n = validMeasurements.size();
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;

    for (size_t i = 0; i < validMeasurements.size(); i++) {
        double x = i;
        double y = validMeasurements[i].getValue();

        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    const double THRESHOLD = 0.01;

    if (slope > THRESHOLD) return Trend::RISING;
    if (slope < -THRESHOLD) return Trend::FALLING;
    return Trend::STABLE;
}

/**
 * @brief Zwraca opisowy tekstowy opis trendu.
 *
 * @return Opis trendu jako napis ("wzrostowy", "malej�cy", "stabilny", "nieznany").
 */
std::string MeasurementAnalyzer::getTrendDescription() const {
    switch (getTrend()) {
    case Trend::RISING: return "wzrostowy";
    case Trend::FALLING: return "malej�cy";
    case Trend::STABLE: return "stabilny";
    case Trend::UNKNOWN: return "nieznany (zbyt ma�o danych)";
    }
    return "nieznany";
}
