#pragma once

#include <vector>
#include <string>
#include "Measurement.h"

/**
 * @file MeasurementAnalyzer.h
 * @brief Klasa s�u��ca do analizy zbioru pomiar�w.
 *
 * Umo�liwia obliczanie warto�ci minimalnej, maksymalnej, �redniej oraz okre�lenie trendu danych.
 */
class MeasurementAnalyzer {
public:
    /**
     * @brief Konstruktor klasy MeasurementAnalyzer.
     *
     * Filtruje tylko prawid�owe pomiary.
     *
     * @param measurements Wektor pomiar�w do analizy.
     */
    MeasurementAnalyzer(const std::vector<Measurement>& measurements);

    /**
     * @brief Sprawdza, czy s� dost�pne jakiekolwiek dane do analizy.
     *
     * @return true je�li dost�pne s� dane, false w przeciwnym razie.
     */
    bool hasData() const;

    /**
     * @brief Zwraca minimaln� warto�� spo�r�d pomiar�w.
     *
     * @return Najni�sza warto�� lub -1.0 je�li brak danych.
     */
    double getMinValue() const;

    /**
     * @brief Zwraca dat� pomiaru o minimalnej warto�ci.
     *
     * @return Data minimalnej warto�ci lub "brak danych" je�li dane nie s� dost�pne.
     */
    std::string getMinDate() const;

    /**
     * @brief Zwraca maksymaln� warto�� spo�r�d pomiar�w.
     *
     * @return Najwy�sza warto�� lub -1.0 je�li brak danych.
     */
    double getMaxValue() const;

    /**
     * @brief Zwraca dat� pomiaru o maksymalnej warto�ci.
     *
     * @return Data maksymalnej warto�ci lub "brak danych" je�li dane nie s� dost�pne.
     */
    std::string getMaxDate() const;

    /**
     * @brief Oblicza �redni� warto�� spo�r�d pomiar�w.
     *
     * @return �rednia warto�� lub -1.0 je�li brak danych.
     */
    double getAverage() const;

    /**
     * @brief Typ wyliczeniowy reprezentuj�cy trend danych.
     */
    enum class Trend {
        RISING,     ///< Trend wzrostowy
        FALLING,    ///< Trend malej�cy
        STABLE,     ///< Trend stabilny
        UNKNOWN     ///< Trend nieznany (np. zbyt ma�o danych)
    };

    /**
     * @brief Oblicza trend danych na podstawie regresji liniowej.
     *
     * @return Warto�� wyliczenia Trend reprezentuj�ca kierunek zmian.
     */
    Trend getTrend() const;

    /**
     * @brief Zwraca opis s�owny trendu.
     *
     * @return Tekstowy opis trendu: "wzrostowy", "malej�cy", "stabilny" lub "nieznany".
     */
    std::string getTrendDescription() const;

private:
    std::vector<Measurement> validMeasurements; ///< Wektor prawid�owych (nieujemnych) pomiar�w.
};