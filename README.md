# Media Searcher

## Функционал

 - CLI
 - Поддержка Linux/Windows
 - режим сервера

## Использование

Программа использует CLI для передачи параметров. Присутствуют два режима: 
 - поиск через указанный интервал и запись в указанный файл (по умолчанию)
 - http сервер на `http://localhost:1234/media_files` (флаг `--server`, порт можно поменять с помощью `--port`)

```
Usage: media_searcher.exe [--help] [--version] [[--server]|[--file]] [--search-dir VAR] [--port VAR] [--interval VAR] [--output-file VAR]

Optional arguments:
  -h, --help         shows help message and exits 
  -v, --version      prints version information and exits 
  -s, --server       starts server 
  -f, --file         fetch data over time and store to file 
  -S, --search-dir   directory where media is searched 
  -p, --port         server port to serve on 
  -i, --interval     interval over which data is stored to file 
  -o, --output-file  file where data is stored 
```

## Сборка

Сборка стандартная, как и для большинства cmake проектов. Зависимости подключаются как submodules, поэтому желательно клонировать с флагом `--recursive`. 
Этого можно не делать, тогда при сборке cmake самостоятельно подтянет нужные зависимости.

```bash
git clone --recursive https://github.com/R3dKar/media-searcher.git
cd media-searcher
mkdir build
cmake -S . -B build
cmake --build build
```
