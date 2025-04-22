/**
 * @file MeasurementAnalyzer.h
 * @brief Klasa do analizy pomiarów, takich jak min, max, œrednia i trend.
 */

#include "MeasurementAnalyzer.h"
#include <algorithm>
#include <numeric>

 /**
  * @brief Konstruktor klasy MeasurementAnalyzer.
  *
  * Filtruje tylko prawid³owe pomiary i zapisuje je do wewnêtrznej listy.
  *
  * @param measurements Wektor obiektów Measurement.
  */
MeasurementAnalyzer::MeasurementAnalyzer(const std::vector<Measurement>& measurements) {
    for (const auto& m : measurements) {
        if (m.isValid()) {
            validMeasurements.push_back(m);
        }
    }
}

/**
 * @brief Sprawdza, czy s¹ dostêpne jakiekolwiek dane pomiarowe.
 *
 * @return true jeœli s¹ dostêpne dane, false w przeciwnym razie.
 */
bool MeasurementAnalyzer::hasData() const {
    return !validMeasurements.empty();
}

/**
 * @brief Zwraca minimaln¹ wartoœæ spoœród dostêpnych pomiarów.
 *
 * @return Minimalna wartoœæ lub -1.0, jeœli brak danych.
 */
double MeasurementAnalyzer::getMinValue() const {
    if (!hasData()) return -1.0;
    auto min = std::min_element(validMeasurements.begin(), validMeasurements.end(),
        [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });
    return min->getValue();
}

/**
 * @brief Zwraca datê, w której wyst¹pi³a minimalna wartoœæ.
 *
 * @return Data minimalnej wartoœci lub "brak danych", jeœli brak danych.
 */
std::string MeasurementAnalyzer::getMinDate() const {
    if (!hasData()) return "brak danych";
    auto min = std::min_element(validMeasurements.begin(), validMeasurements.end(),
        [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });
    return min->getDate();
}

/**
 * @brief Zwraca maksymaln¹ wartoœæ spoœród dostêpnych pomiarów.
 *
 * @return Maksymalna wartoœæ lub -1.0, jeœli brak danych.
 */
double MeasurementAnalyzer::getMaxValue() const {
    if (!hasData()) return -1.0;
    auto max = std::max_element(validMeasurements.begin(), validMeasurements.end(),
        [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });
    return max->getValue();
}

/**
 * @brief Zwraca datê, w której wyst¹pi³a maksymalna wartoœæ.
 *
 * @return Data maksymalnej wartoœci lub "brak danych", jeœli brak danych.
 */
std::string MeasurementAnalyzer::getMaxDate() const {
    if (!hasData()) return "brak danych";
    auto max = std::max_element(validMeasurements.begin(), validMeasurements.end(),
        [](const Measurement& a, const Measurement& b) { return a.getValue() < b.getValue(); });
    return max->getDate();
}

/**
 * @brief Oblicza i zwraca œredni¹ z wartoœci pomiarów.
 *
 * @return Œrednia wartoœæ lub -1.0, jeœli brak danych.
 */
double MeasurementAnalyzer::getAverage() const {
    if (!hasData()) return -1.0;
    double sum = std::accumulate(validMeasurements.begin(), validMeasurements.end(), 0.0,
        [](double total, const Measurement& m) { return total + m.getValue(); });
    return sum / validMeasurements.size();
}

/**
 * @brief Okreœla trend na podstawie regresji liniowej.
 *
 * Trend mo¿e byæ wzrostowy, malej¹cy, stabilny lub nieznany.
 *
 * @return Wartoœæ enum Trend opisuj¹ca kierunek zmian.
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
 * @return Opis trendu jako napis ("wzrostowy", "malej¹cy", "stabilny", "nieznany").
 */
std::string MeasurementAnalyzer::getTrendDescription() const {
    switch (getTrend()) {
    case Trend::RISING: return "wzrostowy";
    case Trend::FALLING: return "malej¹cy";
    case Trend::STABLE: return "stabilny";
    case Trend::UNKNOWN: return "nieznany (zbyt ma³o danych)";
    }
    return "nieznany";
}
