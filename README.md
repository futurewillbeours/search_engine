# Search Engine
## Описание
Эта программа - многопоточный локальный консольный поисковый движок по файлам, который позволяет по файлам JSON формировать запросы и выводить результаты поиска также в файл JSON.
## Технологии в проекте
Для работы программы используются язык С++, библиотеки [nlohmann/json: JSON for Modern C++](https://github.com/nlohmann/json) и [GoogleTest](https://github.com/google/googletest), а также утилита сборки CMake. Библиотека nlohmann/json: JSON for Modern C++ встрена в каталог проекта в папке nlohmann_json. GoogleTest соединяется с проектом через файл CMakeLists.txt.
## Техническое описание проекта
###  Сборка, запуск и тестирование программы
Чтобы собрать программу, вы можете открыть папку проекта в командной строке Windows PowerShell и выполнить команду:

./build.ps1

Программа будет собрана с помощью утилиты CMake и в директории build/src/Debug/ появятся среди прочих следующие файлы:
* search_engine.exe

  Исполняемый файл программы. Поместив файлы конфигураций:

  * config.json
  * requests.json

   в папку с этим файлом, открыв папку файла в Windows PowerShell и выполнив команду:
  
  ./search_engine.exe

  можно запустить и выполнить программу. Результатом работы будет файл answers.json в папке программы.
* search_engine_test.exe

  Программа для тестирования проекта. Открыв терминал в папке этого файла и запустив его с помощью команды:

  ./search_engine_test.exe

  можно проверить работу программы. При упешном выполнении все тесты в терминале будут подсвечены зеленым цветом:

  ![image](https://github.com/futurewillbeours/search_engine/assets/134860207/d48cceec-298f-420f-826a-bd97ca69d827)

## Использование программы
Чтобы использовать программу, нужно поместить в папку программы 2 файла - config.json и requests.json и запустить программу. Программа создаст файл answers.json с ответами.
Результат выполнения программы в терминале:

![image](https://github.com/futurewillbeours/search_engine/assets/134860207/bdc578a9-5fb3-4359-bee0-bdda98c6a143)

### config.json
В этом файле задаются параметры выполнения программы. Пример файла:

![image](https://github.com/futurewillbeours/search_engine/assets/134860207/36dc058d-c6ce-47ff-b530-c504a622bdaa)

Поле config содержит:
* Название поикового движка: name
* Версию программы: version
* Количество ответов (по умолчанию 5): max_responses

Поле files содержит список файлов, по которым нужно вести поиск.
### requests.json
В этом файле задаются поисковые запросы, по которым нужно провести поиск в файлах, заданных в config.json. Пример файла:

![image](https://github.com/futurewillbeours/search_engine/assets/134860207/c085bb70-5604-4893-a568-37d132a633e2)

requests состоит из не более чем 1000 запросов. Каждый запрос - это не более 10 строчных слов из латинских букв, разделенных пробелами. 
### answers.json
Эот файл создается после выполнения программы и содержит результаты поиска. Пример файла:

![image](https://github.com/futurewillbeours/search_engine/assets/134860207/483fbe56-81f8-4ad0-ac0f-b4ff7629298a)

* answers - базовое поле, которое содержит ответы на запросы
* request001 … 003 - идентификатор запроса, по которому сформирован ответ. Идентификатор запроса формируется автоматически по порядку, в котором находятся запросы в поле requests файла requests.json
* result – результат поиска запроса. Если он принимает значение true, значит по данному запросу найден хотя бы один документ. Если результат имеет значение false, значит ни одного документа не найдено. Тогда других полей в ответе на этот запрос нет
* relevance включается в файл answers.json, если на этот запрос удалось найти более одного документа

Далее идут соответствия рейтинга ответа и названия id документа, в котором осуществлялся поиск:

* <Идентификатор документа>("docid") — идентификатор документа, в котором найден ответ на запрос. Он формируется автоматически при индексации всех документов исходя из порядка, в котором документы расположены в поле files в файле config.json
* <ранг ответа>(“rank”) — ранг или поисковый рейтинг. Это число показывает, насколько документ подходит для заданного запроса. В ответе id документов располагаются в порядке уменьшения поискового рейтинга.
## Характерное для проекта
Программа написана для демонстрации изученных навыков в рамках итогового проекта курса "C++" в Skillbox.
