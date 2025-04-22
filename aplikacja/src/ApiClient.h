#ifndef APICLIENT_H
#define APICLIENT_H

#include <string>
#include <vector>
#include <map>
#include <json/json.h>
#include "Station.h"
#include "Sensor.h"
#include "Measurement.h"

/**
 * @file ApiClient.h
 * @brief Klasa do komunikacji z API zewn�trznego systemu monitoringu powietrza.
 *
 * Umo�liwia pobieranie danych o stacjach, sensorach, pomiarach i indeksie jako�ci powietrza.
 */
class ApiClient {
public:
    /**
     * @brief Konstruktor klasy ApiClient.
     */
    ApiClient();

    /**
     * @brief Destruktor klasy ApiClient.
     */
    ~ApiClient();

    /**
     * @brief Pobiera list� stacji pomiarowych.
     *
     * @return Wektor obiekt�w typu Station.
     */
    std::vector<Station> getStations();

    /**
     * @brief Pobiera list� sensor�w dla danej stacji.
     *
     * @param stationId ID stacji, dla kt�rej maj� by� pobrane sensory.
     * @return Wektor obiekt�w typu Sensor.
     */
    std::vector<Sensor> getSensors(int stationId);

    /**
     * @brief Pobiera dane pomiarowe dla danego sensora.
     *
     * @param sensorId ID sensora, dla kt�rego maj� by� pobrane dane.
     * @return Wektor obiekt�w typu Measurement.
     */
    std::vector<Measurement> getSensorData(int sensorId);

    /**
     * @brief Pobiera indeks jako�ci powietrza dla danej stacji.
     *
     * @param stationId ID stacji, dla kt�rej ma by� pobrany indeks.
     * @return Mapa par klucz-warto�� reprezentuj�cych indeks jako�ci powietrza.
     */
    std::map<std::string, std::string> getAirQualityIndex(int stationId);

private:
    /**
     * @brief Callback funkcji CURL do zapisywania odpowiedzi.
     *
     * @param contents Dane odpowiedzi.
     * @param size Rozmiar pojedynczego elementu.
     * @param nmemb Liczba element�w.
     * @param userp Wska�nik do danych u�ytkownika (bufor odpowiedzi).
     * @return Liczba zapisanych bajt�w.
     */
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    /**
     * @brief Wykonuje ��danie HTTP za pomoc� CURL.
     *
     * @param url Adres URL do kt�rego ma zosta� wys�ane ��danie.
     * @param response Zmienna do kt�rej zostanie zapisana odpowied�.
     * @return true je�li ��danie si� powiod�o, false w przeciwnym razie.
     */
    bool performCurlRequest(const std::string& url, std::string& response);

    /**
     * @brief Parsuje odpowied� JSON.
     *
     * @param jsonResponse Surowa odpowied� w formacie JSON.
     * @param parsedRoot Referencja do obiektu JSON, do kt�rego zostanie zapisany wynik.
     * @return true je�li parsowanie si� powiod�o, false w przeciwnym razie.
     */
    bool parseJsonResponse(const std::string& jsonResponse, Json::Value& parsedRoot);
};

#endif // APICLIENT_H