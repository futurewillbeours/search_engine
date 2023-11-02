# Search Engine
Эта программа - локальный консольный поисковый движок по файлам, который позволяет по файлам JSON формировать запросы и выводить результаты поиска.

Для работы программы используются язык С++, библиотеки nlohmann_JSON и Google Test, а также утилита сборки CMake

Чтобы собрать проект и запустить его, потребуется добавить файлы библиотеки nlohmann_JSON в папку проекта. Библиотека Google Test подключается автоматически с помощью файла CMakeLists.txt. После сборки проекта с помощью CMake с целью search_engine появляется исполняемый файл в папке build/Debug/search_engine.exe. Поместив файлы конфирураций в папку с проектом, и запустив исполняемый файл в папке build/Debug/search_engine.exe, можно запустить работу программы. Build находится в той же папке, что и папка проекта

По умолчанию создается тестовая конфигурация с готовыми файлами и congig.json/requeses.json, его можно отключить, закооментировав строку generateTestState(0) в файле main.c

В зависимости от значения в generateTestState, создаются разные начальные конфигурации. Для выбора доступны 0-1

