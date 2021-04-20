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

        Constants/Constants.cpp
        Constants/Constants.h

        DataTypes/File/File.cpp
        DataTypes/File/File.h
        DataTypes/File/FileWriter/FileWriter.cpp
        DataTypes/File/FileWriter/FileWriter.h
        DataTypes/File/FileReader/FileReader.cpp
        DataTypes/File/FileReader/FileReader.h
        DataTypes/Exception/Exception.cpp
        DataTypes/Exception/Exception.h
        DataTypes/Time/Time.cpp
        DataTypes/Time/Time.h
        DataTypes/Range/Range.cpp
        DataTypes/Range/Range.h

        utils/string/string.cpp
        utils/string/string.h
        utils/array/array.cpp
        utils/array/array.h
        utils/time/time.cpp
        utils/time/time.h
        utils/index.h

        algorithm_tests/find.cpp

        Engine/DataUtils/MousePosition/MousePosition.cpp
        Engine/DataUtils/MousePosition/MousePosition.h

        Engine/Player/Player.cpp
        Engine/Player/Player.h

        Engine/Form/Types/Element/Element.cpp
        Engine/Form/Types/Element/Element.h
        Engine/Form/Types/ProgressBar/ProgressBar.cpp
        Engine/Form/Types/ProgressBar/ProgressBar.h
        Engine/Form/Types/Button/Button.cpp
        Engine/Form/Types/Button/Button.h
        Engine/Form/Types/TextBox/TextBox.cpp
        Engine/Form/Types/TextBox/TextBox.h
        Engine/Form/Types/Text/Text.cpp
        Engine/Form/Types/Text/Text.h

        Engine/Form/ExtendedTypes/FocusElement/FocusElement.h
        Engine/Form/ExtendedTypes/ActionElement/ActionElement.h
        Engine/Form/ExtendedTypes/ActionElement/ActionElement.cpp

        Engine/Form/Objects/FocusObject/FocusObject.cpp
        Engine/Form/Objects/FocusObject/FocusObject.h
        Engine/Form/Objects/ActionObject/ActionObject.cpp
        Engine/Form/Objects/ActionObject/ActionObject.h

        Engine/Form/index.h

        Engine/Controllers/FocusController/FocusController.cpp
        Engine/Controllers/FocusController/FocusController.h
        Engine/Controllers/CallSchedulerController/CallSchedulerController.cpp
        Engine/Controllers/CallSchedulerController/CallSchedulerController.h
        Engine/Controllers/CallSchedulerController/SchedulerPlan/SchedulerPlan.cpp
        Engine/Controllers/CallSchedulerController/SchedulerPlan/SchedulerPlan.h

        Engine/Form/Templates/ErrorForm/ErrorForm.cpp
        Engine/Form/Templates/ErrorForm/ErrorForm.h

        Engine/Form/Form.cpp
        Engine/Form/Form.h

        Engine/Engine.cpp
        Engine/Engine.h

        Engine/index.h
        DataTypes/index.h)


target_link_libraries(stud-game
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
        nlohmann_json::nlohmann_json
        pugixml

        stdc++fs
        )
