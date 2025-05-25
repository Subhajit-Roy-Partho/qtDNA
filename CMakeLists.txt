cmake_minimum_required(VERSION 3.16)
project(SphereViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON) # If you use .ui files and want them automatically processed

# --- Find Packages ---
# For Qt6 (recommended)
find_package(Qt6 COMPONENTS Core Gui OpenGL Widgets REQUIRED)
# # For Qt5 (uncomment and comment Qt6 line if using Qt5)
# find_package(Qt5 COMPONENTS Core Gui OpenGL Widgets REQUIRED)

# Find OpenGL and GLU
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED) # This should find GLU::GLU imported target

# --- Define the Executable Target ---
# List all your source files (.cpp) and header files (.h) that contain Q_OBJECT
# or other Qt macros that require processing by MOC.
# While add_executable technically only needs .cpp files for compilation,
# listing headers helps CMAKE_AUTOMOC work correctly.
add_executable(SphereViewer
    main.cpp
    mainwindow.cpp
    openglwidget.cpp
    mainwindow.h    # Important for CMAKE_AUTOMOC if it has Q_OBJECT
    openglwidget.h  # Important for CMAKE_AUTOMOC if it has Q_OBJECT
    # mainwindow.ui # Add this if you have a mainwindow.ui file
)

# --- Link Libraries to the Target ---
# For Qt6
target_link_libraries(SphereViewer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::Widgets
    OpenGL::GL # For OpenGL functions
    GLU::GLU   # For gluSphere
)
# # For Qt5 (uncomment and comment Qt6 block if using Qt5)
# target_link_libraries(SphereViewer PRIVATE
#     Qt5::Core
#     Qt5::Gui
#     Qt5::OpenGL
#     Qt5::Widgets
#     OpenGL::GL
#     GLU::GLU
# )

# If you have a .ui file, and CMAKE_AUTOUIC is ON,
# it should be handled automatically. If not, you might need:
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui")
#     qt6_wrap_ui(SphereViewer_UI_HDRS mainwindow.ui) # For Qt6
#     # qt5_wrap_ui(SphereViewer_UI_HDRS mainwindow.ui) # For Qt5
#     # add_executable(SphereViewer ... ${SphereViewer_UI_HDRS}) # Or ensure it's part of sources
# endif()
# However, with CMAKE_AUTOUIC ON, this is usually not needed if the .ui file
# is in the same directory and follows standard naming conventions related to a .cpp file.