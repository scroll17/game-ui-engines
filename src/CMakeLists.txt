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

add_executable(stud-game
        main.cpp

        Game/Game.cpp
        Game/Game.h

        Exception/Exception.cpp
        Exception/Exception.h

        Constants/Constants.cpp
        Constants/Constants.h

        File/File.cpp
        File/File.h

        File/FileWriter/FileWriter.cpp
        File/FileWriter/FileWriter.h

        File/FileReader/FileReader.cpp
        File/FileReader/FileReader.h

        utils/string.cpp
        utils/string.h
        utils/array.cpp
        utils/array.h

        Time/Time.cpp
        Time/Time.h

        algorithm_tests/find.cpp

        Game/Interface/Interface.h
        Game/Interface/Interface.cpp

        Game/User/User.cpp
        Game/User/User.h

        Game/Interface/Types/ProgressBar/ProgressBar.cpp
        Game/Interface/Types/ProgressBar/ProgressBar.h

        Game/Interface/Types/Element/Element.cpp
        Game/Interface/Types/Element/Element.h
        Game/Interface/Types/Button/Button.cpp
        Game/Interface/Types/Button/Button.h
        Game/Interface/Types/Text/Text.cpp
        Game/Interface/Types/Text/Text.h

        Game/Forms/Error/ErrorForm.cpp
        Game/Forms/Error/ErrorForm.h
  )

target_link_libraries(stud-game
            sfml-graphics
            sfml-audio
            sfml-window
            sfml-system
            nlohmann_json::nlohmann_json
            pugixml

            stdc++fs
)
