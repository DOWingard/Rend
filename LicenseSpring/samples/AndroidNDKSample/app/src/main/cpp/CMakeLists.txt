cmake_minimum_required(VERSION 3.22.1)

project("androidndksample")

set(LIBRARY_LINK_TYPE shared)
set(LIBRARY_EXTENSION "so")

set(LIBRARY_PATH_RELATIVE "bin/${LIBRARY_LINK_TYPE}/${CMAKE_BUILD_TYPE}")
set(LIBRARY_PATH "${CMAKE_SOURCE_DIR}/../../../../../../${LIBRARY_PATH_RELATIVE}")
message ("Library path: ${LIBRARY_PATH}")

string(TOUPPER ${LIBRARY_LINK_TYPE} LIBRARY_LINK_TYPE)

include_directories(${CMAKE_SOURCE_DIR}/../../../../../../include)

add_library(${CMAKE_PROJECT_NAME} ${LIBRARY_LINK_TYPE}
        native-lib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../AppConfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../AppConfig.cpp)

add_library(LicenseSpringLib ${LIBRARY_LINK_TYPE} IMPORTED)
set_target_properties(LicenseSpringLib PROPERTIES
        IMPORTED_LOCATION ${LIBRARY_PATH}/libLicenseSpring.so
)

add_library(CurlLib ${LIBRARY_LINK_TYPE} IMPORTED)
set_target_properties(CurlLib PROPERTIES
        IMPORTED_LOCATION ${LIBRARY_PATH}/libcurl.so
)

add_library(SSLLib ${LIBRARY_LINK_TYPE} IMPORTED)
set_target_properties(SSLLib PROPERTIES
        IMPORTED_LOCATION ${LIBRARY_PATH}/libssl.so
)

add_library(CryptoLib ${LIBRARY_LINK_TYPE} IMPORTED)
set_target_properties(CryptoLib PROPERTIES
        IMPORTED_LOCATION ${LIBRARY_PATH}/libcrypto.so
)

target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        LicenseSpringLib
        CryptoLib
        SSLLib
        CurlLib
        android
        log
)