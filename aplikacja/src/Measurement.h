#pragma once

#include <string>

/**
 * @file Measurement.h
 * @brief Klasa reprezentuj�ca pojedynczy pomiar z warto�ci� i dat�.
 */
class Measurement {
public:
    /**
     * @brief Konstruktor klasy Measurement.
     *
     * @param date Data wykonania pomiaru (w formacie tekstowym).
     * @param value Warto�� pomiaru.
     */
    Measurement(const std::string& date, double value)
        : date_(date), value_(value) {
    }

    /**
     * @brief Zwraca dat� pomiaru.
     *
     * @return Data w formacie std::string.
     */
    std::string getDate() const { return date_; }

    /**
     * @brief Zwraca warto�� pomiaru.
     *
     * @return Warto�� jako liczba zmiennoprzecinkowa.
     */
    double getValue() const { return value_; }

    /**
     * @brief Sprawdza, czy pomiar jest prawid�owy.
     *
     * Pomiar uznaje si� za prawid�owy, je�li jego warto�� jest wi�ksza lub r�wna 0.
     *
     * @return true je�li warto�� jest poprawna, false w przeciwnym razie.
     */
    bool isValid() const { return value_ >= 0; }

private:
    std::string date_;  ///< Data pomiaru.
    double value_;      ///< Warto�� pomiaru.
};