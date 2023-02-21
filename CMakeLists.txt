cmake_minimum_required(VERSION 3.23)

project(ChatServer VERSION 0.1.0 LANGUAGES CXX)

option(ENABLE_CONAN "Use Conan for dependency management" OFF)
option(BUILD_TEST "Build the tests" ON)
option(BUILD_DOCS "Generate documentation" ON)

include(CTest)
include(util/utilities.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(${PROJECT_NAME}_PROPERTIES "")
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
GET_FILES(${PROJECT_NAME}_SOURCES ${CMAKE_SOURCE_DIR}/src "*.cpp")

ADD_QT_MODULES(${PROJECT_NAME} Core Network Concurrent)
CONFIGURE_QT(${PROJECT_NAME})

add_subdirectory(lib)
list(APPEND ${PROJECT_NAME}_LIBS ${LIB_LIBRARIES})
list(APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${LIB_INCLUDE_DIRS})

if(${ENABLE_CONAN})
    message(STATUS "Conan is being used for dependency management")
    execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR} --update --build=missing
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

    conan_basic_setup(TARGETS)

    list(APPEND ${PROJECT_NAME}_LIB ${CONAN_TARGETS})
    list(APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS})
else()
    message(STATUS "Depdency management is being handled manually")
endif()

set(SRC_FILES
    src/app/main.cpp
    src/dtos/DataMessage.cpp
    src/server/ChatServer.cpp
    src/client/ChatClient.cpp
    src/worker/SocketReader.cpp
)
list(APPEND ${PROJECT_NAME}_SOURCES ${SRC_FILES})

set(Q_OBJECT_HEADERS
    include/src/server/ChatServer.h
    include/src/client/ChatClient.h
    include/src/worker/SocketReader.h
)
list(APPEND ${PROJECT_NAME}_SOURCES ${Q_OBJECT_HEADERS})

qt_add_resources(${PROJECT_NAME}_SOURCES
        resource/collections/ui.qrc
        )

qt_add_executable(${PROJECT_NAME})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        AUTOMOC ON
        WIN32_EXECUTABLE TRUE
        ANDROID TRUE
        )
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        AUTOMOC ON
        ANDROID TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_NAME}.crowell.dev
        MACOSX_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLESHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        )
endif()

target_sources(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${${PROJECT_NAME}_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${${PROJECT_NAME}_LIBS}
        )

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

# TODO: replace with qt_generate_deploy_app_script in else statement
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND windeployqt ${CMAKE_CURRENT_BINARY_DIR}/bin/${PROJECT_NAME}.exe
            )
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND linuxdeploy --plugin qt -e ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME} --appdir /app/${CMAKE_BINARY_DIR}
            )
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND macdeployqt ${CMAKE_CURRENT_BINARY_DIR}/bin/${PROJECT_NAME}.exe
            )
else()
    qt_generate_deploy_app_script(
            TARGET ${PROJECT_NAME}
            FILENAME_VARIABLE deploy_script
            NO_UNSUPPORTED_PLATFORM_ERROR
    )

    install(SCRIPT ${deploy_script}
            DESTINATION .
            )
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_DOCS)
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/docs)
    execute_process(
        COMMAND qdoc.exe --outputdir ${CMAKE_BINARY_DIR}/docs ${PROJECT_NAME}.qdocconf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
endif()
