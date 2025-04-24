# Jakosc_Powietrza - Piotr Czajkowski

Aplikacja do pobierania i analizy danych o jakości powietrza z wykorzystaniem API GIOS. Projekt został stworzony w języku C++.

## Funkcje

- Wybór stacji pomiarowych poprzez listę wszystkich stacji w Polsce uzyskaną z API GIOS.
- Wybór czujnika pomiarowego dla wybranej stacji.
- Pobieranie oraz wyświetlanie danych pomiarowych.
- Wizualizacja danych na wykresach.
- Zapisywanie danych w lokalnej bazie danych (JSON).
- Pobieranie z lokalnej bazy danych w przypadku braku dostępu do API.
- Podstawowa analiza danych.

## Wymagania

- Visual Studio
- vcpkg (menedżer pakietów C++)
- Biblioteki: `curl`, `jsoncpp`,`wxwidgets`

## Instalacja

1. Sklonuj repozytorium vcpkg:

   ```bash
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```

2. Zintegruj vcpkg z Visual Studio:

   ```bash
   .\vcpkg.exe integrate install
   ```

3. Zainstaluj wymagane biblioteki:

   ```bash
   .\vcpkg.exe install curl
   .\vcpkg.exe install jsoncpp
   .\vcpkg.exe install wxwidgets
   ```

## Uruchamianie

1. Otwórz projekt w Visual Studio.
2. Upewnij się, że projekt korzysta z bibliotek zainstalowanych przez vcpkg.
3. Zbuduj i uruchom aplikację.

## Autor

**Piotr Czajkowski**

---

*Projekt edukacyjny służący do nauki pracy z API, przetwarzania danych i analizy w języku C++.*
