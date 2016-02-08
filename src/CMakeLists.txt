PROJECT("HexBlocker")
cmake_minimum_required(VERSION 2.8)

IF(NOT VTK_DIR)
  MESSAGE("VTK_DIR is not set it with export VTK_DIR=/path/to/vtk/build, trying default settings")
  SET(VTK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../build/VTK)
ENDIF(NOT VTK_DIR)
IF(NOT VTK_BINARY_DIR)
    FIND_PACKAGE(VTK REQUIRED NO_MODULE)
    INCLUDE(${VTK_USE_FILE})
ENDIF(NOT VTK_BINARY_DIR)

#SET(CMAKE_AUTOUIC ON)  

# use what QVTK built with
SET(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)
# Set your files and resources here  
SET(HexBlockerSrcs
    main.cpp MainWindow.cpp HexBlock.cpp HexBlocker.cpp
    HexPatch.cpp InteractorStyleVertPick.cpp
    MoveVerticesWidget.cpp CreateBlockWidget.cpp
    RotateVerticesWidget.cpp Geometry.cpp
    HexBC.cpp ToolBoxWidget.cpp
    SetBCsWidget.cpp SetBCsItem.cpp HexExporter.cpp HexEdge.cpp
    HexReader.cpp EdgePropsWidget.cpp
    TEdgeSpace.cpp GradingCalculatorDialog.cpp InteractorStyleActorPick.cpp
    EdgeSetTypeWidget.cpp PointsTableModel.cpp VerticeEditorWidget.cpp
    )
SET(HexBlockerUI
    MainWindow.ui ToolBoxWidget.ui
    MoveVerticesWidget.ui CreateBlockWidget.ui
    RotateVerticesWidget.ui
    SetBCsWidget.ui EdgePropsWidget.ui
    GradingCalculatorDialog.ui EdgeSetTypeWidget.ui
    VerticeEditorWidget.ui
    )
SET(HexBlockerHeaders
    MainWindow.h HexBlock.h HexBlocker.h HexPatch.h
    InteractorStyleVertPick.h MoveVerticesWidget.h
    RotateVerticesWidget.h
    CreateBlockWidget.h HexBC.h ToolBoxWidget.h
    SetBCsWidget.h SetBCsItem.h HexExporter.h HexEdge.h
    HexReader.h EdgePropsWidget.h TEdgeSpace.h
    GradingCalculatorDialog.h InteractorStyleActorPick.h
    EdgeSetTypeWidget.h PointsTableModel.h
    VerticeEditorWidget.h
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

# Add install time link to vtk libraries
SET( CMAKE_INSTALL_RPATH "\$ORIGIN/../VTK/lib/" )
# Change exe name if debug
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE("This is a debug build")
    SET(EXE_NAME HexBlocker-dbg)
ELSE()
    MESSAGE("This is a release build")
    SET(EXE_NAME HexBlocker)
ENDIF()

ADD_EXECUTABLE( ${EXE_NAME} MACOSX_BUNDLE ${HexBlockerSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs})

TARGET_LINK_LIBRARIES(${EXE_NAME}
  ${QT_LIBRARIES}
  ${VTK_LIBRARIES}
  )

get_cmake_property(CacheVars CACHE_VARIABLES)
foreach(CacheVar ${CacheVars})
  get_property(CacheVarHelpString CACHE ${CacheVar} PROPERTY HELPSTRING)
  if(CacheVarHelpString STREQUAL "No help, variable specified on the command line.")
    get_property(CacheVarType CACHE ${CacheVar} PROPERTY TYPE)
    if(CacheVarType STREQUAL "UNINITIALIZED")
      set(CacheVarType)
    else()
      set(CacheVarType :${CacheVarType})
    endif()
    set(CMakeArgs "${CMakeArgs} -D${CacheVar}${CacheVarType}=\"${${CacheVar}}\"")
  endif()
endforeach()
message("CMakeArgs: ${CMakeArgs}")

INSTALL( TARGETS ${EXE_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX} )
