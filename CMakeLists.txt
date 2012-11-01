PROJECT("HexBlocker")
cmake_minimum_required(VERSION 2.8)
 
IF(NOT VTK_DIR)
  MESSAGE("VTK_DIR set it with export VTK_DIR=/path/to/vtk/build, trying Nicolas settings")
  SET(VTK_DIR /home/nico/Dropbox/Projekt/myQtApps/VTK/build/)
ENDIF(NOT VTK_DIR)
IF(NOT VTK_BINARY_DIR)
FIND_PACKAGE(VTK)

INCLUDE(${VTK_USE_FILE})
ENDIF(NOT VTK_BINARY_DIR)

# use what QVTK built with
SET(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})


# Use the include path and library for Qt that is used by VTK.
INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)


# Set your files and resources here  
SET(HexBlockerSrcs
    main.cxx MainWindow.cxx HexBlock.cpp HexBlocker.cpp
    hexPatch.cpp InteractorStyleVertPick.cpp
    MoveVerticesWidget.cpp CreateBlockWidget.cpp
    InteractorStylePatchPick.cpp HexBC.cpp ToolBoxWidget.cpp
    SetBCsWidget.cpp
    )
SET(HexBlockerUI
    MainWindow.ui ToolBoxWidget.ui
    MoveVerticesWidget.ui CreateBlockWidget.ui
    SetBCsWidget.ui
    )
SET(HexBlockerHeaders
    MainWindow.h HexBlock.h HexBlocker.h hexPatch.h
    InteractorStyleVertPick.h MoveVerticesWidget.h
    CreateBlockWidget.h InteractorStylePatchPick.h
    HexBC.h ToolBoxWidget.h SetBCsWidget.h
    )
SET(HexBlockerResources Icons/icons.qrc)

# The rest should just work (sure...)
QT4_WRAP_UI(UISrcs ${HexBlockerUI})
QT4_WRAP_CPP(MOCSrcs ${HexBlockerHeaders} )
QT4_ADD_RESOURCES(ResourceSrcs ${HexBlockerResources})

SOURCE_GROUP("Resources" FILES
  ${HexBlockerUI}
  ${HexBlockerResources}
  ${EXE_ICON}
)

SOURCE_GROUP("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs}
  ${RCS_SOURCES}
)

ADD_DEFINITIONS(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)
SET_SOURCE_FILES_PROPERTIES(${HexBlockerSrcs} PROPERTIES
                            OBJECT_DEPENDS "${UISrcs}")

# It's nice to have the ui in the windows project file...just double click on it
# and designer comes up on that ui file :)
IF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
  SET (HexBlockerSrcs ${HexBlockerSrcs} ${HexBlockerUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
IF (${CMAKE_BUILD_TOOL} MATCHES "devenv")
  SET (HexBlockerSrcs ${HexBlockerSrcs} ${HexBlockerUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "devenv")

ADD_EXECUTABLE( HexBlocker MACOSX_BUNDLE ${HexBlockerSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs})

TARGET_LINK_LIBRARIES(HexBlocker
  QVTK
  ${QT_LIBRARIES}
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
  vtkInfovis
  vtkViews
)



