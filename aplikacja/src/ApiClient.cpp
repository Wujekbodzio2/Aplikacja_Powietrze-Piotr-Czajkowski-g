#include "ApiClient.h"
#include <curl/curl.h>
#include <iostream>
#include <locale.h>

/**
 * @brief Konstruktor klasy ApiClient
 *
 * Inicjalizuje bibliotekę cURL i ustawia lokalizację na język polski.
 */
ApiClient::ApiClient() {
    setlocale(LC_ALL, "Polish");
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

/**
 * @brief Destruktor klasy ApiClient
 *
 * Czyści globalny stan cURL.
 */
ApiClient::~ApiClient() {
    curl_global_cleanup();
}

/**
 * @brief Pobiera listę wszystkich stacji pomiarowych.
 *
 * @return Wektor obiektów Station reprezentujących stacje.
 */
std::vector<Station> ApiClient::getStations() {
    std::vector<Station> result;
    std::string url = "https://api.gios.gov.pl/pjp-api/rest/station/findAll";
    std::string response;

    performCurlRequest(url, response);

    Json::Value root;
    parseJsonResponse(response, root);

    for (const auto& station : root) {
        int id = station["id"].asInt();
        std::string name = station["stationName"].asString();
        result.emplace_back(id, name);
    }
    return result;
}

/**
 * @brief Pobiera listę czujników dla wybranej stacji.
 *
 * @param stationId Identyfikator stacji.
 * @return Wektor obiektów Sensor przypisanych do stacji.
 */
std::vector<Sensor> ApiClient::getSensors(int stationId) {
    std::vector<Sensor> result;
    std::string url = "https://api.gios.gov.pl/pjp-api/rest/station/sensors/" + std::to_string(stationId);
    std::string response;

    performCurlRequest(url, response);

    Json::Value root;
    if (!parseJsonResponse(response, root))
        throw std::runtime_error("Nie udało się sparsować odpowiedzi JSON dla czujników.");

    for (const auto& sensor : root) {
        int id = sensor["id"].asInt();
        std::string param = sensor["param"]["paramName"].asString();
        std::string formula = sensor["param"]["paramFormula"].asString();
        result.emplace_back(id, param, formula);
    }
    return result;
}

/**
 * @brief Pobiera dane pomiarowe z wybranego czujnika.
 *
 * @param sensorId Identyfikator czujnika.
 * @return Wektor obiektów Measurement zawierających dane pomiarowe.
 */
std::vector<Measurement> ApiClient::getSensorData(int sensorId) {
    std::vector<Measurement> result;
    std::string url = "https://api.gios.gov.pl/pjp-api/rest/data/getData/" + std::to_string(sensorId);
    std::string response;

    performCurlRequest(url, response);

    Json::Value root;
    if (!parseJsonResponse(response, root))
        throw std::runtime_error("Nie udało się sparsować odpowiedzi JSON dla danych pomiarowych.");

    for (const auto& val : root["values"]) {
        std::string date = val["date"].asString();
        double value = val["value"].isNull() ? -1.0 : val["value"].asDouble();
        result.emplace_back(date, value);
    }
    return result;
}

/**
 * @brief Pobiera indeks jakości powietrza dla wybranej stacji.
 *
 * @param stationId Identyfikator stacji.
 * @return Mapa z nazwą parametru jako kluczem i poziomem jakości jako wartością.
 */
std::map<std::string, std::string> ApiClient::getAirQualityIndex(int stationId) {
    std::map<std::string, std::string> result;
    std::string url = "https://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/" + std::to_string(stationId);
    std::string response;

    performCurlRequest(url, response);

    Json::Value root;
    if (!parseJsonResponse(response, root))
        throw std::runtime_error("Nie udało się sparsować odpowiedzi JSON dla indeksu jakości powietrza.");

    if (!root["stIndexLevel"].isNull())
        result["Ogólny"] = root["stIndexLevel"]["indexLevelName"].asString();
    if (!root["pm10IndexLevel"].isNull())
        result["PM10"] = root["pm10IndexLevel"]["indexLevelName"].asString();
    if (!root["pm25IndexLevel"].isNull())
        result["PM2.5"] = root["pm25IndexLevel"]["indexLevelName"].asString();
    if (!root["o3IndexLevel"].isNull())
        result["O3"] = root["o3IndexLevel"]["indexLevelName"].asString();
    if (!root["no2IndexLevel"].isNull())
        result["NO2"] = root["no2IndexLevel"]["indexLevelName"].asString();
    if (!root["so2IndexLevel"].isNull())
        result["SO2"] = root["so2IndexLevel"]["indexLevelName"].asString();
    if (!root["coIndexLevel"].isNull())
        result["CO"] = root["coIndexLevel"]["indexLevelName"].asString();

    return result;
}

/**
 * @brief Funkcja callback dla cURL - zapisuje dane do stringa.
 *
 * @param contents Bufor z danymi
 * @param size Rozmiar bloku
 * @param nmemb Ilość bloków
 * @param userp Wskaźnik do stringa, do którego zapisujemy dane
 * @return Liczba przetworzonych bajtów
 */
size_t ApiClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * @brief Wykonuje zapytanie HTTP GET przy pomocy cURL.
 *
 * @param url Adres URL do zapytania
 * @param response Zmienna do której zostanie zapisany wynik zapytania
 * @return true jeśli zapytanie zakończyło się sukcesem, w przeciwnym wypadku rzuca wyjątek
 */
bool ApiClient::performCurlRequest(const std::string& url, std::string& response) {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("Nie udało się zainicjować CURL-a.");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::string err = curl_easy_strerror(res);
        curl_easy_cleanup(curl);

        if (res == CURLE_COULDNT_RESOLVE_HOST || res == CURLE_COULDNT_CONNECT || res == CURLE_OPERATION_TIMEDOUT) {
            throw std::runtime_error("Brak połączenia z internetem: " + err);
        }

        throw std::runtime_error("Błąd podczas pobierania danych: " + err);
    }

    curl_easy_cleanup(curl);
    return true;
}

/**
 * @brief Parsuje odpowiedź JSON do obiektu Json::Value.
 *
 * @param jsonResponse Surowa odpowiedź w formacie JSON
 * @param parsedRoot Obiekt, do którego zostanie zapisany sparsowany JSON
 * @return true jeśli parsowanie zakończyło się sukcesem, false w przeciwnym wypadku
 */
bool ApiClient::parseJsonResponse(const std::string& jsonResponse, Json::Value& parsedRoot) {
    Json::Reader reader;
    return reader.parse(jsonResponse, parsedRoot);
}