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
 * @brief Klasa do komunikacji z API zewnêtrznego systemu monitoringu powietrza.
 *
 * Umo¿liwia pobieranie danych o stacjach, sensorach, pomiarach i indeksie jakoœci powietrza.
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
     * @brief Pobiera listê stacji pomiarowych.
     *
     * @return Wektor obiektów typu Station.
     */
    std::vector<Station> getStations();

    /**
     * @brief Pobiera listê sensorów dla danej stacji.
     *
     * @param stationId ID stacji, dla której maj¹ byæ pobrane sensory.
     * @return Wektor obiektów typu Sensor.
     */
    std::vector<Sensor> getSensors(int stationId);

    /**
     * @brief Pobiera dane pomiarowe dla danego sensora.
     *
     * @param sensorId ID sensora, dla którego maj¹ byæ pobrane dane.
     * @return Wektor obiektów typu Measurement.
     */
    std::vector<Measurement> getSensorData(int sensorId);

    /**
     * @brief Pobiera indeks jakoœci powietrza dla danej stacji.
     *
     * @param stationId ID stacji, dla której ma byæ pobrany indeks.
     * @return Mapa par klucz-wartoœæ reprezentuj¹cych indeks jakoœci powietrza.
     */
    std::map<std::string, std::string> getAirQualityIndex(int stationId);

private:
    /**
     * @brief Callback funkcji CURL do zapisywania odpowiedzi.
     *
     * @param contents Dane odpowiedzi.
     * @param size Rozmiar pojedynczego elementu.
     * @param nmemb Liczba elementów.
     * @param userp WskaŸnik do danych u¿ytkownika (bufor odpowiedzi).
     * @return Liczba zapisanych bajtów.
     */
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    /**
     * @brief Wykonuje ¿¹danie HTTP za pomoc¹ CURL.
     *
     * @param url Adres URL do którego ma zostaæ wys³ane ¿¹danie.
     * @param response Zmienna do której zostanie zapisana odpowiedŸ.
     * @return true jeœli ¿¹danie siê powiod³o, false w przeciwnym razie.
     */
    bool performCurlRequest(const std::string& url, std::string& response);

    /**
     * @brief Parsuje odpowiedŸ JSON.
     *
     * @param jsonResponse Surowa odpowiedŸ w formacie JSON.
     * @param parsedRoot Referencja do obiektu JSON, do którego zostanie zapisany wynik.
     * @return true jeœli parsowanie siê powiod³o, false w przeciwnym razie.
     */
    bool parseJsonResponse(const std::string& jsonResponse, Json::Value& parsedRoot);
};

#endif // APICLIENT_H