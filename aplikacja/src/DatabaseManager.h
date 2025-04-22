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
 * @brief Klasa odpowiedzialna za zarz¹dzanie lokaln¹ baz¹ danych w formacie JSON.
 *
 * Umo¿liwia zapisywanie i wczytywanie danych pomiarowych, stacji oraz sensorów.
 */
class DatabaseManager {
public:
    /**
     * @brief Konstruktor klasy DatabaseManager.
     *
     * @param dbFilePath Œcie¿ka do pliku bazy danych JSON (domyœlnie "air_qualitydata.json").
     */
    DatabaseManager(const std::string& dbFilePath = "air_qualitydata.json");

    /**
 * @brief Zapisuje pomiary i opcjonalnie indeksy jakoœci powietrza do lokalnej bazy (JSON).
 *
 * @param stationId    ID stacji.
 * @param stationName  Nazwa stacji.
 * @param sensorId     ID sensora.
 * @param sensorName   Nazwa sensora.
 * @param measurements Wektor pomiarów do zapisania.
 * @param indexValues  (opcjonalnie) Mapa indeksów jakoœci powietrza, gdzie kluczem jest
 *                     nazwa indeksu (np. "Ogólny"), a wartoœci¹ opis (np. "Dobry").
 * @return true jeœli zapis danych i indeksów zakoñczy³ siê sukcesem, false w przeciwnym razie.
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
     * @param measurements Referencja do wektora, do którego zostan¹ za³adowane dane.
     * @return true jeœli dane zosta³y poprawnie wczytane, false w przeciwnym razie.
     */
    bool loadData(int stationId, int sensorId, std::vector<Measurement>& measurements);

    /**
     * @brief Zwraca listê zapisanych stacji w bazie danych.
     *
     * @return Wektor par <ID stacji, nazwa stacji>.
     */
    std::vector<Station>  getSavedStations();

    /**
     * @brief Zwraca listê zapisanych sensorów dla danej stacji.
     *
     * @param stationId ID stacji.
     * @return Wektor par <ID sensora, nazwa sensora>.
     */
    std::vector<Sensor> getSavedSensors(int stationId);
    
   

        /**
         * @brief Wczytuje indeks jakoœci powietrza dla stacji.
         *
         * @param stationId ID stacji.
         * @param indexValues Referencja do mapy, do której zostan¹ za³adowane indeksy.
         * @return true jeœli dane zosta³y odnalezione i wczytane, false w przeciwnym razie.
         */
        bool loadAirQualityIndex(int stationId, std::map<std::string, std::string>& indexValues);
private:
    std::string dbFilePath_;   ///< Œcie¿ka do pliku bazy danych JSON.
    Json::Value dbRoot_;       ///< Struktura przechowuj¹ca dane bazy w pamiêci.

    /**
     * @brief £aduje bazê danych z pliku.
     *
     * @return true jeœli plik zosta³ poprawnie za³adowany, false w przeciwnym razie.
     */
    bool loadDatabase();

    /**
     * @brief Zapisuje aktualn¹ zawartoœæ bazy danych do pliku.
     *
     * @return true jeœli zapis siê powiód³, false w przeciwnym razie.
     */
    bool saveDatabase();

    /**
     * @brief Generuje unikalny klucz identyfikuj¹cy dane sensora w danej stacji.
     *
     * @param stationId ID stacji.
     * @param sensorId ID sensora.
     * @return Klucz jako string.
     */
    std::string generateKey(int stationId, int sensorId);

   
};