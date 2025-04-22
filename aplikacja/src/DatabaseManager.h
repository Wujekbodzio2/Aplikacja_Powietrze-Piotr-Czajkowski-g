#pragma once

#include <string>
#include <vector>
#include "Measurement.h"
#include "Sensor.h"
#include "Station.h"
#include <json/json.h>
#include <fstream>

/**
 * @file DatabaseManager.h
 * @brief Klasa odpowiedzialna za zarz�dzanie lokaln� baz� danych w formacie JSON.
 *
 * Umo�liwia zapisywanie i wczytywanie danych pomiarowych, stacji oraz sensor�w.
 */
class DatabaseManager {
public:
    /**
     * @brief Konstruktor klasy DatabaseManager.
     *
     * @param dbFilePath �cie�ka do pliku bazy danych JSON (domy�lnie "air_qualitydata.json").
     */
    DatabaseManager(const std::string& dbFilePath = "air_qualitydata.json");

    /**
 * @brief Zapisuje pomiary i opcjonalnie indeksy jako�ci powietrza do lokalnej bazy (JSON).
 *
 * @param stationId    ID stacji.
 * @param stationName  Nazwa stacji.
 * @param sensorId     ID sensora.
 * @param sensorName   Nazwa sensora.
 * @param measurements Wektor pomiar�w do zapisania.
 * @param indexValues  (opcjonalnie) Mapa indeks�w jako�ci powietrza, gdzie kluczem jest
 *                     nazwa indeksu (np. "Og�lny"), a warto�ci� opis (np. "Dobry").
 * @return true je�li zapis danych i indeks�w zako�czy� si� sukcesem, false w przeciwnym razie.
 */
    bool saveData(int stationId, const std::string& stationName,
        int sensorId, const std::string& sensorName,
        const std::vector<Measurement>& measurements,
        const std::map<std::string, std::string>& indexValues = std::map<std::string, std::string>());

    /**
     * @brief Wczytuje dane pomiarowe z lokalnej bazy danych.
     *
     * @param stationId ID stacji.
     * @param sensorId ID sensora.
     * @param measurements Referencja do wektora, do kt�rego zostan� za�adowane dane.
     * @return true je�li dane zosta�y poprawnie wczytane, false w przeciwnym razie.
     */
    bool loadData(int stationId, int sensorId, std::vector<Measurement>& measurements);

    /**
     * @brief Zwraca list� zapisanych stacji w bazie danych.
     *
     * @return Wektor par <ID stacji, nazwa stacji>.
     */
    std::vector<Station>  getSavedStations();

    /**
     * @brief Zwraca list� zapisanych sensor�w dla danej stacji.
     *
     * @param stationId ID stacji.
     * @return Wektor par <ID sensora, nazwa sensora>.
     */
    std::vector<Sensor> getSavedSensors(int stationId);
    
   

        /**
         * @brief Wczytuje indeks jako�ci powietrza dla stacji.
         *
         * @param stationId ID stacji.
         * @param indexValues Referencja do mapy, do kt�rej zostan� za�adowane indeksy.
         * @return true je�li dane zosta�y odnalezione i wczytane, false w przeciwnym razie.
         */
        bool loadAirQualityIndex(int stationId, std::map<std::string, std::string>& indexValues);
private:
    std::string dbFilePath_;   ///< �cie�ka do pliku bazy danych JSON.
    Json::Value dbRoot_;       ///< Struktura przechowuj�ca dane bazy w pami�ci.

    /**
     * @brief �aduje baz� danych z pliku.
     *
     * @return true je�li plik zosta� poprawnie za�adowany, false w przeciwnym razie.
     */
    bool loadDatabase();

    /**
     * @brief Zapisuje aktualn� zawarto�� bazy danych do pliku.
     *
     * @return true je�li zapis si� powi�d�, false w przeciwnym razie.
     */
    bool saveDatabase();

    /**
     * @brief Generuje unikalny klucz identyfikuj�cy dane sensora w danej stacji.
     *
     * @param stationId ID stacji.
     * @param sensorId ID sensora.
     * @return Klucz jako string.
     */
    std::string generateKey(int stationId, int sensorId);

   
};