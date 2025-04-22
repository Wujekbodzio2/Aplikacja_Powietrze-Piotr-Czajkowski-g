/**
 * @file DatabaseManager.cpp
 * @brief Implementacja klasy DatabaseManager zarz¹dzaj¹cej zapisem i odczytem danych pomiarowych z pliku JSON.
 */

#include "DatabaseManager.h"
#include <iostream>

 /**
  * @brief Konstruktor DatabaseManager.
  *
  * Inicjalizuje œcie¿kê do pliku bazy danych i ³aduje dane.
  *
  * @param dbFilePath Œcie¿ka do pliku JSON z danymi.
  */
DatabaseManager::DatabaseManager(const std::string& dbFilePath)
    : dbFilePath_(dbFilePath) {
    loadDatabase();
}

/**
 * @brief £aduje dane z pliku JSON do pamiêci.
 *
 * Tworzy domyœln¹ strukturê, jeœli plik nie istnieje.
 *
 * @return true jeœli uda³o siê wczytaæ dane lub utworzyæ now¹ strukturê, false w przypadku b³êdu parsowania.
 */
bool DatabaseManager::loadDatabase() {
    std::ifstream file(dbFilePath_);
    if (!file.is_open()) {
        dbRoot_["stations"] = Json::Value(Json::objectValue);
        dbRoot_["data"] = Json::Value(Json::objectValue);
        dbRoot_["indexes"] = Json::Value(Json::objectValue);
        return true;
    }

    Json::Reader reader;
    bool success = reader.parse(file, dbRoot_);
    file.close();

    if (!dbRoot_.isMember("stations")) {
        dbRoot_["stations"] = Json::Value(Json::objectValue);
    }
    if (!dbRoot_.isMember("data")) {
        dbRoot_["data"] = Json::Value(Json::objectValue);
    }
    if (!dbRoot_.isMember("indexes")) {  // Dodana walidacja
        dbRoot_["indexes"] = Json::Value(Json::objectValue);
    }


    return success;
}

/**
 * @brief Zapisuje aktualny stan bazy danych do pliku.
 *
 * @return true jeœli zapis zakoñczy³ siê sukcesem, false w przypadku b³êdu.
 */
bool DatabaseManager::saveDatabase() {
    std::ofstream file(dbFilePath_);
    if (!file.is_open()) {
        return false;
    }

    Json::StyledWriter writer;
    file << writer.write(dbRoot_);
    file.close();

    return true;
}

/**
 * @brief Generuje klucz tekstowy na podstawie ID stacji i ID sensora.
 *
 * @param stationId ID stacji.
 * @param sensorId ID sensora.
 * @return Po³¹czony klucz w postaci "stationId_sensorId".
 */
std::string DatabaseManager::generateKey(int stationId, int sensorId) {
    return std::to_string(stationId) + "_" + std::to_string(sensorId);
}

/**
 * @brief Zapisuje dane pomiarowe do pliku.
 *
 * Aktualizuje informacje o stacjach i sensorach oraz nadpisuje dane pomiarowe.
 *
 * @param stationId ID stacji.
 * @param stationName Nazwa stacji.
 * @param sensorId ID sensora.
 * @param sensorName Nazwa sensora.
 * @param measurements Wektor pomiarów do zapisania.
 * @return true jeœli zapis siê powiód³, false w przeciwnym razie.
 */
bool DatabaseManager::saveData(int stationId, const std::string& stationName,
    int sensorId, const std::string& sensorName,
    const std::vector<Measurement>& measurements,
    const std::map<std::string, std::string>& indexValues) {

    // Zapisz dane stacji
    if (!dbRoot_["stations"].isMember(std::to_string(stationId))) {
        dbRoot_["stations"][std::to_string(stationId)] = Json::Value(Json::objectValue);
        dbRoot_["stations"][std::to_string(stationId)]["name"] = stationName;
        dbRoot_["stations"][std::to_string(stationId)]["sensors"] = Json::Value(Json::objectValue);
    }

    // Zapisz dane sensora
    dbRoot_["stations"][std::to_string(stationId)]["sensors"][std::to_string(sensorId)] = sensorName;

    // Zapisz dane pomiarowe
    std::string key = generateKey(stationId, sensorId);
    if (!dbRoot_["data"].isMember(key)) {
        dbRoot_["data"][key] = Json::Value(Json::arrayValue);
    }
    else {
        dbRoot_["data"][key].clear();
    }

    for (const auto& m : measurements) {
        if (m.isValid()) {
            Json::Value measurement;
            measurement["date"] = m.getDate();
            measurement["value"] = m.getValue();
            dbRoot_["data"][key].append(measurement);
        }
    }

    // Zapisz indeksy jakoœci powietrza (jeœli zosta³y podane)
    if (!indexValues.empty()) {
        std::string indexKey = "index_" + std::to_string(stationId);

        if (!dbRoot_["indexes"].isObject()) {
            dbRoot_["indexes"] = Json::Value(Json::objectValue);
        }

        dbRoot_["indexes"][indexKey] = Json::Value(Json::objectValue);
        for (const auto& pair : indexValues) {
            dbRoot_["indexes"][indexKey][pair.first] = pair.second;
        }
    }

    return saveDatabase();
}

/**
 * @brief Wczytuje dane pomiarowe z pliku.
 *
 * @param stationId ID stacji.
 * @param sensorId ID sensora.
 * @param measurements Referencja do wektora, do którego zostan¹ za³adowane dane.
 * @return true jeœli dane zosta³y odnalezione i wczytane, false w przeciwnym razie.
 */
bool DatabaseManager::loadData(int stationId, int sensorId, std::vector<Measurement>& measurements) {
    std::string key = generateKey(stationId, sensorId);
    if (!dbRoot_["data"].isMember(key)) {
        return false;
    }

    measurements.clear();
    const Json::Value& data = dbRoot_["data"][key];
    for (const auto& m : data) {
        std::string date = m["date"].asString();
        double value = m["value"].asDouble();
        measurements.emplace_back(date, value);
    }

    return true;
}

/**
 * @brief Pobiera zapisane stacje z bazy danych.
 *
 * @return Wektor par: ID stacji i nazwa stacji.
 */
std::vector<Station> DatabaseManager::getSavedStations() {
    std::vector<Station> stations;
    const Json::Value& stationsJson = dbRoot_["stations"];

    for (auto it = stationsJson.begin(); it != stationsJson.end(); ++it) {
        int id = std::stoi(it.name());
        std::string name = (*it)["name"].asString();

        // Tworzymy obiekt Station bezpoœrednio
        stations.emplace_back(id, name);
    }

    return stations;
}

/**
 * @brief Pobiera zapisane sensory przypisane do danej stacji.
 *
 * @param stationId ID stacji.
 * @return Wektor par: ID sensora i nazwa sensora.
 */
std::vector<Sensor> DatabaseManager::getSavedSensors(int stationId) {
    std::vector<Sensor> sensors;
    std::string stationIdStr = std::to_string(stationId);

    if (dbRoot_["stations"].isMember(stationIdStr)) {
        const Json::Value& sensorsJson = dbRoot_["stations"][stationIdStr]["sensors"];

        for (auto it = sensorsJson.begin(); it != sensorsJson.end(); ++it) {
            int id = std::stoi(it.name());
            std::string name = (*it).asString();
            // Trzeci parametr (wzór chemiczny) nie jest przechowywany w bazie,
            // wiêc przekazujemy pusty string
            sensors.emplace_back(id, name, "");
        }
    }

    return sensors;
}




/**
 * @brief Wczytuje indeks jakoœci powietrza dla stacji.
 *
 * @param stationId ID stacji.
 * @param indexValues Referencja do mapy, do której zostan¹ za³adowane indeksy.
 * @return true jeœli dane zosta³y odnalezione i wczytane, false w przeciwnym razie.
 */
bool DatabaseManager::loadAirQualityIndex(int stationId, std::map<std::string, std::string>& indexValues) {
    std::string key = "index_" + std::to_string(stationId);

    if (!dbRoot_["indexes"].isObject() || !dbRoot_["indexes"].isMember(key)) {
        return false;
    }

    const Json::Value& indexJson = dbRoot_["indexes"][key];
    for (auto it = indexJson.begin(); it != indexJson.end(); ++it) {
        indexValues[it.name()] = (*it).asString();
    }

    return !indexValues.empty();
}