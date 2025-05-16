cmake_minimum_required(VERSION 3.9)

project(ProgettoPCS LANGUAGES CXX C VERSION 1.0.0)

# Set C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPOSE WARNINGS ON DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# IMPOSE CXX FLAGS FOR WINDOWS
if (WIN32)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif (WIN32)

# Add dependencies
################################################################################

## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND ProgettoPCS_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND ProgettoPCS_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND ProgettoPCS_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND ProgettoPCS_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND ProgettoPCS_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND ProgettoPCS_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Insert Sources
################################################################################
add_subdirectory(src)

list(APPEND ProgettoPCS_SOURCES ${polygonalMesh_sources})
list(APPEND ProgettoPCS_HEADERS ${polygonalMesh_headers})
list(APPEND ProgettoPCS_INCLUDE ${polygonalMesh_includes})

add_subdirectory(src_test)

list(APPEND ProgettoPCS_TEST_SOURCES ${src_test_sources})
list(APPEND ProgettoPCS_TEST_HEADERS ${src_test_headers})
list(APPEND ProgettoPCS_TEST_INCLUDE ${src_test_includes})

add_subdirectory(ExportParaview)

list(APPEND ProgettoPCS_SOURCES ${src_paraview_sources})
list(APPEND ProgettoPCS_HEADERS ${src_paraview_headers})
list(APPEND ProgettoPCS_INCLUDE ${src_paraview_includes})

# Create executable
################################################################################
add_executable(${PROJECT_NAME}
      main.cpp
      ${ProgettoPCS_SOURCES}
      ${ProgettoPCS_HEADERS})

# QUESTI VANNO INCLUSI QUANDO INSERIAMO I FILE DENTRO PolygonalMesh 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell0DTetraedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell0DIcosaedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell0DOttaedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell1DTetraedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell1DIcosaedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell1DOttaedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell2DTetraedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell2DIcosaedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell2DOttaedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell3DTetraedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell3DIcosaedro.csv
${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell3DOttaedro.csv
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} ${ProgettoPCS_LINKED_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${ProgettoPCS_INCLUDE})
target_compile_options(${PROJECT_NAME} PUBLIC -fPIC)

add_executable(${PROJECT_NAME}_TEST
      main_test.cpp
      ${ProgettoPCS_SOURCES}
      ${ProgettoPCS_HEADERS}
      ${src_test_sources}
      ${src_test_headers})

target_link_libraries(${PROJECT_NAME}_TEST ${ProgettoPCS_LINKED_LIBRARIES})
target_include_directories(${PROJECT_NAME}_TEST PRIVATE ${ProgettoPCS_INCLUDE} ${src_test_includes})
target_compile_options(${PROJECT_NAME}_TEST PUBLIC -fPIC)
