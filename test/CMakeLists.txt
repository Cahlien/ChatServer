cmake_minimum_required(VERSION 3.23)

project(ChatServer_tests VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

# TODO: determine if I need to use the qmltestrunner executable or if I can do everything with the C++ test harness

# Option 1: Use C++ test harness
GET_TESTS(${CMAKE_CURRENT_SOURCE_DIR})

# Option 2: Use qmltestrunner
#find_program(QMLTESTRUNNER_BIN NAMES qmltestrunner)
#add_test(NAME ui_tests COMMAND ${QMLTESTRUNNER_BIN} -input C:/DCPrototypes/ChatServer/test/ui/pages/HelloWorld/qml/tst_main.qml)
