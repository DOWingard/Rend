cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "C:/Users/Derek Wingard/Desktop/Work/Plugins/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(Swell
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "Swell VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(Swell
    source/version.h
    source/cids.h
    source/view.h
    source/view.cpp
    source/processor.h
    source/processor.cpp
    source/controller.h
    source/controller.cpp
    source/factory.cpp
    source/viewfactory.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(Swell
        PRIVATE
            resource/editor.uidesc
    )
    target_link_libraries(Swell
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(Swell
        RESOURCES
            "resource/editor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (Swell
    RESOURCES
        resource/8E9537EDF9915C74B33300A75FBCE049_snapshot.png
        resource/8E9537EDF9915C74B33300A75FBCE049_snapshot_2.0x.png
)









include_directories(
    ../DSPFilters/shared/DSPFilters/include
)

file(GLOB DSPFILTERS_SOURCES
    ../DSPFilters/shared/DSPFilters/source/*.cpp
)

target_sources(Swell PRIVATE ${DSPFILTERS_SOURCES})






target_link_libraries(Swell
    PRIVATE
        sdk
)

smtg_target_configure_version_file(Swell)

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
    target_sources(Swell PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Swell)

        smtg_target_set_debug_executable(Swell
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
