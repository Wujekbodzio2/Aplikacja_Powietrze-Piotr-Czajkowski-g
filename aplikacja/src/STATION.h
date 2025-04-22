#pragma once

#include <string>

/**
 * @file Station.h
 * @brief Klasa reprezentuj�ca stacj� pomiarow� jako�ci powietrza.
 */
class Station {
public:
    /**
     * @brief Konstruktor klasy Station.
     *
     * @param id Unikalny identyfikator stacji.
     * @param name Nazwa stacji (np. lokalizacja).
     */
    Station(int id, const std::string& name) : id_(id), name_(name) {}

    /**
     * @brief Zwraca identyfikator stacji.
     *
     * @return Id stacji jako liczba ca�kowita.
     */
    int getId() const { return id_; }

    /**
     * @brief Zwraca nazw� stacji.
     *
     * @return Nazwa stacji jako std::string.
     */
    std::string getName() const { return name_; }

private:
    int id_;           ///< Identyfikator stacji pomiarowej.
    std::string name_; ///< Nazwa/lokalizacja stacji.
};