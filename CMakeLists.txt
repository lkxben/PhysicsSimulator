cmake_minimum_required(VERSION 3.28)
project(PhysicsSimulator LANGUAGES CXX)
find_package(raylib REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB SOURCES "src/*.cpp")
set(SOURCES_NO_MAIN ${SOURCES})
list(REMOVE_ITEM SOURCES_NO_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(main ${SOURCES})
target_include_directories(main PRIVATE include ${raylib_INCLUDE_DIRS})
target_link_libraries(main PRIVATE SFML::Graphics)
target_link_libraries(main PRIVATE ${raylib_LIBRARIES})

add_executable(pool ${SOURCES_NO_MAIN} scenarios/pool/Pool.cpp)
target_include_directories(pool PRIVATE include ${raylib_INCLUDE_DIRS})
target_link_libraries(pool PRIVATE SFML::Graphics)
target_link_libraries(pool PRIVATE ${raylib_LIBRARIES})

# add_executable(threeBody ${SOURCES_NO_MAIN} scenarios/threeBody/ThreeBody.cpp)
# target_include_directories(threeBody PRIVATE include)
# target_link_libraries(threeBody PRIVATE SFML::Graphics)

add_executable(plasmaCloud ${SOURCES_NO_MAIN} scenarios/plasmaCloud/PlasmaCloud.cpp)
target_include_directories(plasmaCloud PRIVATE include ${raylib_INCLUDE_DIRS})
target_link_libraries(plasmaCloud PRIVATE SFML::Graphics)
target_link_libraries(plasmaCloud PRIVATE ${raylib_LIBRARIES})

add_executable(doublePendulum ${SOURCES_NO_MAIN} scenarios/doublePendulum/DoublePendulum.cpp)
target_include_directories(doublePendulum PRIVATE include ${raylib_INCLUDE_DIRS})
target_link_libraries(doublePendulum PRIVATE SFML::Graphics)
target_link_libraries(doublePendulum PRIVATE ${raylib_LIBRARIES})

add_executable(clothMovement ${SOURCES_NO_MAIN} scenarios/clothMovement/ClothMovement.cpp)
target_include_directories(clothMovement PRIVATE include ${raylib_INCLUDE_DIRS})
target_link_libraries(clothMovement PRIVATE SFML::Graphics)
target_link_libraries(clothMovement PRIVATE ${raylib_LIBRARIES})

add_executable(clothCut ${SOURCES_NO_MAIN} scenarios/clothCut/ClothCut.cpp)
target_include_directories(clothCut PRIVATE include ${raylib_INCLUDE_DIRS})
target_link_libraries(clothCut PRIVATE SFML::Graphics)
target_link_libraries(clothCut PRIVATE ${raylib_LIBRARIES})