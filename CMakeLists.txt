# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.20)

# Nom du projet et langage
project(NanoDB CXX)

# Standard C++ (11, 14, 17, 20, 23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Récupérer automatiquement tous les fichiers .cpp et .h dans src/
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Créer l'exécutable
add_executable(NanoDB ${SOURCES})

# Ajouter le dossier src/ aux chemins d'inclusion des entêtes (#include "Table.h")
target_include_directories(NanoDB PRIVATE 
    src
    src/storage
    src/catalog
    src/types
    src/common
)