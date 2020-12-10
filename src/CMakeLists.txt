cmake_minimum_required(VERSION 3.17)
project(stud-game)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

# <JSON>
FetchContent_Declare(json
        GIT_REPOSITORY "https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent"
        GIT_TAG "v3.7.3"
)
FetchContent_GetProperties(json)

if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# </JSON>

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake_modules" ${CMAKE_MODULE_PATH})

# <SFML>
find_package(SFML 2.4 COMPONENTS graphics audio window system REQUIRED)
# </SFML>

# <pugixml>
FetchContent_Declare(pugixml
        GIT_REPOSITORY "https://github.com/zeux/pugixml"
        GIT_TAG "v1.11.1"
        )
FetchContent_GetProperties(pugixml)

if(NOT pugixml_POPULATED)
    FetchContent_Populate(pugixml)
    add_subdirectory(${pugixml_SOURCE_DIR} ${pugixml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# </pugixml>

add_executable(stud-game main.cpp classes/Object/Object.cpp classes/Object/Object.h)

target_link_libraries(stud-game
            sfml-graphics
            sfml-audio
            sfml-window
            sfml-system
            nlohmann_json::nlohmann_json
            pugixml
)
