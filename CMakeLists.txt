cmake_minimum_required(VERSION 3.14)

project(Swell VERSION 1.0.0.0 DESCRIPTION "Swell VST 3 Plug-in")

# === Options ===
option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

# === SDK Setup ===
set(vst3sdk_SOURCE_DIR "C:/Users/Derek Wingard/Desktop/Work/Plugins/VST_SDK/vst3sdk")
if(NOT EXISTS "${vst3sdk_SOURCE_DIR}")
    message(FATAL_ERROR "VST3 SDK not found at: ${vst3sdk_SOURCE_DIR}")
endif()

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")
add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

# === Plugin Source ===
smtg_add_vst3plugin(Swell
    source/version.h
    source/cids.h
    source/licenser.h
    source/editor.h
    source/editordelegate.h
    source/compression.h
    source/compression.cpp
    source/processor.h
    source/processor.cpp
    source/controller.h
    source/controller.cpp
    source/factory.cpp
)

# === Optional GUI Support ===
target_sources(Swell PRIVATE resource/editor.uidesc)
target_link_libraries(Swell PRIVATE vstgui_support)
smtg_target_add_plugin_resources(Swell
    RESOURCES "resource/editor.uidesc"
)

smtg_target_add_plugin_snapshots(Swell
    RESOURCES
        resource/8E9537EDF9915C74B33300A75FBCE049_snapshot.png
        resource/8E9537EDF9915C74B33300A75FBCE049_snapshot_2.0x.png
)

# === External Includes & Sources ===
include_directories(
    ../DSPFilters/shared/DSPFilters/include
)
file(GLOB DSPFILTERS_SOURCES
    ../DSPFilters/shared/DSPFilters/source/*.cpp
)
target_sources(Swell PRIVATE ${DSPFILTERS_SOURCES})

# === LicenseSpring ===
set(LICENSESPRING_LIB_PATH "${CMAKE_SOURCE_DIR}/LicenseSpring/bin/x64/dynamic")
set(LICENSESPRING_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/LicenseSpring/include")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LICENSE_SPRING_LIB "${LICENSESPRING_LIB_PATH}/LicenseSpringD.lib")
else()
    set(LICENSE_SPRING_LIB "${LICENSESPRING_LIB_PATH}/LicenseSpring.lib")
endif()

target_include_directories(Swell PRIVATE ${LICENSESPRING_INCLUDE_PATH})
target_link_libraries(Swell PRIVATE ${LICENSE_SPRING_LIB})


# === Link all required Steinberg libraries ===
target_link_libraries(Swell PRIVATE
    sdk
    base
    pluginterfaces
)

# === Versioning ===
smtg_target_configure_version_file(Swell)

# === Platform-specific ===
if(SMTG_MAC)
    smtg_target_set_bundle(Swell
        BUNDLE_IDENTIFIER fromtheVOID
        COMPANY_NAME "Void Audio"
    )
    smtg_target_set_debug_executable(Swell
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(Swell PRIVATE resource/win32resource.rc)
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Swell)
        smtg_target_set_debug_executable(Swell
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif()
