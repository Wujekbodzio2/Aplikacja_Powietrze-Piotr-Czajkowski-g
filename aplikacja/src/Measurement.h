#pragma once

#include <string>

/**
 * @file Measurement.h
 * @brief Klasa reprezentuj¹ca pojedynczy pomiar z wartoœci¹ i dat¹.
 */
class Measurement {
public:
    /**
     * @brief Konstruktor klasy Measurement.
     *
     * @param date Data wykonania pomiaru (w formacie tekstowym).
     * @param value Wartoœæ pomiaru.
     */
    Measurement(const std::string& date, double value)
        : date_(date), value_(value) {
    }

    /**
     * @brief Zwraca datê pomiaru.
     *
     * @return Data w formacie std::string.
     */
    std::string getDate() const { return date_; }

    /**
     * @brief Zwraca wartoœæ pomiaru.
     *
     * @return Wartoœæ jako liczba zmiennoprzecinkowa.
     */
    double getValue() const { return value_; }

    /**
     * @brief Sprawdza, czy pomiar jest prawid³owy.
     *
     * Pomiar uznaje siê za prawid³owy, jeœli jego wartoœæ jest wiêksza lub równa 0.
     *
     * @return true jeœli wartoœæ jest poprawna, false w przeciwnym razie.
     */
    bool isValid() const { return value_ >= 0; }

private:
    std::string date_;  ///< Data pomiaru.
    double value_;      ///< Wartoœæ pomiaru.
};