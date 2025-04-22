#pragma once

#include <vector>
#include <string>
#include "Measurement.h"

/**
 * @file MeasurementAnalyzer.h
 * @brief Klasa s³u¿¹ca do analizy zbioru pomiarów.
 *
 * Umo¿liwia obliczanie wartoœci minimalnej, maksymalnej, œredniej oraz okreœlenie trendu danych.
 */
class MeasurementAnalyzer {
public:
    /**
     * @brief Konstruktor klasy MeasurementAnalyzer.
     *
     * Filtruje tylko prawid³owe pomiary.
     *
     * @param measurements Wektor pomiarów do analizy.
     */
    MeasurementAnalyzer(const std::vector<Measurement>& measurements);

    /**
     * @brief Sprawdza, czy s¹ dostêpne jakiekolwiek dane do analizy.
     *
     * @return true jeœli dostêpne s¹ dane, false w przeciwnym razie.
     */
    bool hasData() const;

    /**
     * @brief Zwraca minimaln¹ wartoœæ spoœród pomiarów.
     *
     * @return Najni¿sza wartoœæ lub -1.0 jeœli brak danych.
     */
    double getMinValue() const;

    /**
     * @brief Zwraca datê pomiaru o minimalnej wartoœci.
     *
     * @return Data minimalnej wartoœci lub "brak danych" jeœli dane nie s¹ dostêpne.
     */
    std::string getMinDate() const;

    /**
     * @brief Zwraca maksymaln¹ wartoœæ spoœród pomiarów.
     *
     * @return Najwy¿sza wartoœæ lub -1.0 jeœli brak danych.
     */
    double getMaxValue() const;

    /**
     * @brief Zwraca datê pomiaru o maksymalnej wartoœci.
     *
     * @return Data maksymalnej wartoœci lub "brak danych" jeœli dane nie s¹ dostêpne.
     */
    std::string getMaxDate() const;

    /**
     * @brief Oblicza œredni¹ wartoœæ spoœród pomiarów.
     *
     * @return Œrednia wartoœæ lub -1.0 jeœli brak danych.
     */
    double getAverage() const;

    /**
     * @brief Typ wyliczeniowy reprezentuj¹cy trend danych.
     */
    enum class Trend {
        RISING,     ///< Trend wzrostowy
        FALLING,    ///< Trend malej¹cy
        STABLE,     ///< Trend stabilny
        UNKNOWN     ///< Trend nieznany (np. zbyt ma³o danych)
    };

    /**
     * @brief Oblicza trend danych na podstawie regresji liniowej.
     *
     * @return Wartoœæ wyliczenia Trend reprezentuj¹ca kierunek zmian.
     */
    Trend getTrend() const;

    /**
     * @brief Zwraca opis s³owny trendu.
     *
     * @return Tekstowy opis trendu: "wzrostowy", "malej¹cy", "stabilny" lub "nieznany".
     */
    std::string getTrendDescription() const;

private:
    std::vector<Measurement> validMeasurements; ///< Wektor prawid³owych (nieujemnych) pomiarów.
};