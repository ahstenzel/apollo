include(CMakePrintHelpers)

# Set path variables
if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 17 2022")
	set(APOLLO_GEN "msvc2022")
elseif (${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
	set(APOLLO_GEN "mingw")
	set (APOLLO_ARCH "64")
elseif (${CMAKE_GENERATOR} STREQUAL "Ninja Multi-Config" OR ${CMAKE_GENERATOR} STREQUAL "Ninja")
	set(APOLLO_GEN "gcc")
	set (APOLLO_ARCH "64")
endif()
if (MSVC)
	if (${CMAKE_VS_PLATFORM_NAME} STREQUAL "Win32")
		set (APOLLO_ARCH "32")
	else()
		set (APOLLO_ARCH "64")
	endif()
endif()
set(QT_FULL_PATH "$ENV{QT_PATH}/${APOLLO_GEN}_${APOLLO_ARCH}")
cmake_print_variables(QT_FULL_PATH)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${QT_FULL_PATH}")

# Load Qt support
set(Qt6_DIR "${QT_FULL_PATH}/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Xml)
qt_standard_project_setup()
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Build app
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(APP_SOURCE
	Main.cpp
	Common.cpp
	MainWindow.cpp
	ProjectWidget.cpp
	AssetInfo.cpp
	AssetDialog.cpp
	BrowseLineEdit.cpp
	ImageLabel.cpp
	TextureGroupInfo.cpp
	TextureGroupBuilder.cpp
	ResourceGenerator.cpp
	Resources.cpp
)
set(APP_HEADER
	Common.hpp
	MainWindow.hpp
	ProjectWidget.hpp
	AssetInfo.hpp
	AssetDialog.hpp
	BrowseLineEdit.hpp
	ImageLabel.hpp
	TextureGroupInfo.hpp
	TextureGroupBuilder.hpp
	ResourceGenerator.hpp
)
qt_add_executable(apollo ${APP_SOURCE} ${APP_HEADER})
source_group("Header Files" FILES ${APP_HEADER})

# Build resources
include(resourcecompile)
rcc(apollo "${PROJECT_SOURCE_DIR}/resources/resources.qrc" "${CMAKE_CURRENT_SOURCE_DIR}/Resources.cpp")

# Include directories
target_include_directories(apollo PRIVATE
	"${QT_FULL_PATH}/include/QtCore"
	"${QT_FULL_PATH}/include/QtWidgets"
	"${QT_FULL_PATH}/include/QtGui"
	"${QT_FULL_PATH}/include/QtXml"
	"${PROJECT_SOURCE_DIR}/vendor"
)

# Copy Qt libraries to output folder
if (WIN32 AND MSVC)
	include(windeployqt)
	windeployqt(apollo)
endif()

# Link libraries
target_link_libraries(apollo PRIVATE 
	Qt6::Widgets 
	Qt6::Gui
	Qt6::Xml
	vendor
)

# Set executable properties
set_target_properties(apollo PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Set install rules
install(TARGETS apollo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)