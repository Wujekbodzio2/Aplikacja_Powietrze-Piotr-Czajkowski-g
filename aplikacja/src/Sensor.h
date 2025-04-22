#pragma once

#include <string>

/**
 * @file Sensor.h
 * @brief Klasa reprezentuj¹ca sensor (czujnik) pomiarowy przypisany do stacji.
 */
class Sensor {
public:
    /**
     * @brief Konstruktor klasy Sensor.
     *
     * @param id Unikalny identyfikator sensora.
     * @param paramName Nazwa parametru (np. "PM10", "O3").
     * @param formula Wzór chemiczny parametru (np. "C6H6").
     */
    Sensor(int id, const std::string& paramName, const std::string& formula)
        : id_(id), paramName_(paramName), paramFormula_(formula) {
    }

    /**
     * @brief Zwraca identyfikator sensora.
     *
     * @return Id sensora jako liczba ca³kowita.
     */
    int getId() const { return id_; }

    /**
     * @brief Zwraca nazwê parametru mierzonego przez sensor.
     *
     * @return Nazwa parametru jako std::string.
     */
    std::string getParamName() const { return paramName_; }

    /**
     * @brief Zwraca wzór chemiczny mierzonego parametru.
     *
     * @return Wzór chemiczny jako std::string.
     */
    std::string getParamFormula() const { return paramFormula_; }

private:
    int id_;                    ///< Identyfikator sensora.
    std::string paramName_;     ///< Nazwa parametru mierzonego przez sensor.
    std::string paramFormula_;  ///< Wzór chemiczny mierzonego parametru.
};