project(cameraviewer)

include(CompileWarnings)
include(GenResources)
include(LinkMacOSGUI)

configure_file(src/main/generate/WPILibVersion.cpp.in WPILibVersion.cpp)
generate_resources(src/main/native/resources generated/main/cpp CV cv cameraviewer_resources_src)

file(GLOB cameraviewer_src src/main/native/cpp/*.cpp ${CMAKE_CURRENT_BINARY_DIR}/WPILibVersion.cpp)

if(WIN32)
    set(cameraviewer_rc src/main/native/win/cameraviewer.rc)
elseif(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE cv.icns)
    set(APP_ICON_MACOSX src/main/native/mac/cv.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

add_executable(
    cameraviewer
    ${cameraviewer_src}
    ${cameraviewer_resources_src}
    ${cameraviewer_rc}
    ${APP_ICON_MACOSX}
)
wpilib_link_macos_gui(cameraviewer)
wpilib_target_warnings(cameraviewer)
target_link_libraries(cameraviewer libglassnt libglass)

if(WIN32)
    set_target_properties(cameraviewer PROPERTIES WIN32_EXECUTABLE YES)
elseif(APPLE)
    set_target_properties(cameraviewer PROPERTIES MACOSX_BUNDLE YES OUTPUT_NAME "CameraViewer")
endif()
