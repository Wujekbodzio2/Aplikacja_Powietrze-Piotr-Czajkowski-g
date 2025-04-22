#pragma once

#include <string>

/**
 * @file Sensor.h
 * @brief Klasa reprezentuj�ca sensor (czujnik) pomiarowy przypisany do stacji.
 */
class Sensor {
public:
    /**
     * @brief Konstruktor klasy Sensor.
     *
     * @param id Unikalny identyfikator sensora.
     * @param paramName Nazwa parametru (np. "PM10", "O3").
     * @param formula Wz�r chemiczny parametru (np. "C6H6").
     */
    Sensor(int id, const std::string& paramName, const std::string& formula)
        : id_(id), paramName_(paramName), paramFormula_(formula) {
    }

    /**
     * @brief Zwraca identyfikator sensora.
     *
     * @return Id sensora jako liczba ca�kowita.
     */
    int getId() const { return id_; }

    /**
     * @brief Zwraca nazw� parametru mierzonego przez sensor.
     *
     * @return Nazwa parametru jako std::string.
     */
    std::string getParamName() const { return paramName_; }

    /**
     * @brief Zwraca wz�r chemiczny mierzonego parametru.
     *
     * @return Wz�r chemiczny jako std::string.
     */
    std::string getParamFormula() const { return paramFormula_; }

private:
    int id_;                    ///< Identyfikator sensora.
    std::string paramName_;     ///< Nazwa parametru mierzonego przez sensor.
    std::string paramFormula_;  ///< Wz�r chemiczny mierzonego parametru.
};