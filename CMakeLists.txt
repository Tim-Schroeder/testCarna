cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_DEBUG_POSTFIX d)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

############################################
# Library name
############################################
project( testCarna )
############################################

############################################
# Version declaration
# Target file will be named with version suffix
############################################
set( MAJOR_VERSION		0 )
set( MINOR_VERSION		1 )
set( PATCH_VERSION		0 )

set( FULL_VERSION		${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION} )

set( TARGET_NAME		${PROJECT_NAME}-${FULL_VERSION} )
############################################

############################################
# Options
# Set default options for this build
############################################
option(BUILD_DOC	"Build and install the API documentation"	OFF)
option(BUILD_TEST	"Build the unit tests"				OFF)
############################################

############################################
# Find dependencies and add them later to the linker (see below)
############################################
# NO DEPENDENCIES (sample below)
# include( $ENV{MEDITEC_LIBS}/<LibName>/configure.cmake )
############################################

# Qt
set( QT_USE_QTMAIN		TRUE )
find_package ( Qt5Core )
# sinclude ( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

#GLEW
#include( $ENV{MEDITEC_LIBS}/GLEW/1.7.0/configure.cmake )

#TRTK
#include( $ENV{MEDITEC_LIBS}/TRTK/0.13.1/configure.cmake )

#Eigen
#include( $ENV{MEDITEC_LIBS}/EIGEN/3.2.0/configure.cmake )

#Carna
include( $ENV{MEDITEC_LIBS}/CARNA/2.4.0/configure.cmake )

# Project

include_directories( ${CMAKE_PROJECT_DIR}src/include )
set( PRIVATE_HEADERS
#		src/include/test.h
	)
set( SRC
		src/main.cpp
	)


include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( ${TARGET_NAME}	WIN32
			${SRC}
			${HEADERS}
			${HEADERS_MOC}
			${FORMS_HEADERS}
			${RESOURCES_RCC}
		)

add_definitions( -DNOMINMAX  )
#add_definitions( -D_USE_MATH_DEFINES )

############################################
# Add dependencies to the linker
############################################

target_link_libraries(  ${TARGET_NAME}
			${CARNA_LIBLIST} 
			Qt5::Core	)






#vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
### NO CHANGES EVER NECESSARY BELOW THIS POINT!!
##vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
#vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

string( REGEX REPLACE "\\\\" "/" INSTALL_DIR $ENV{MEDITEC_LIBS}/${PROJECT_NAME}/${FULL_VERSION} )

install(	TARGETS	${TARGET_NAME}
		RUNTIME	DESTINATION	$ENV{MEDITEC_LIBS}/bin
		)
install(	TARGETS	${TARGET_NAME}
		RUNTIME	DESTINATION	${INSTALL_DIR}/bin
		ARCHIVE	DESTINATION	${INSTALL_DIR}/bin
		LIBRARY	DESTINATION	${INSTALL_DIR}/bin
		)
install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/misc/configure.cmake ${CMAKE_CURRENT_BINARY_DIR}/misc/install.bat
		DESTINATION 	${INSTALL_DIR}
		)
install(	DIRECTORY	${CMAKE_PROJECT_DIR}include
		DESTINATION 	${INSTALL_DIR}
		)
if (WIN32)
	install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
			DESTINATION		$ENV{MEDITEC_LIBS}/bin
			OPTIONAL
			)
	install(	FILES	${CMAKE_CURRENT_BINARY_DIR}/bin/${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}.pdb
			DESTINATION		${INSTALL_DIR}/bin
			OPTIONAL
			)
endif()


############################################
# Unit tests
############################################
if(BUILD_TEST)
	enable_testing()
	add_subdirectory( test  )
endif()
############################################

############################################
# Doxygen API documentation
############################################
if(BUILD_DOC)
	find_package(Doxygen REQUIRED)
	if(DOXYGEN_FOUND)
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)
		add_custom_target(	doc ALL
						${DOXYGEN_EXECUTABLE} 
						WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
						COMMENT "Generating API documentation with Doxygen" VERBATIM
					)
		install(	DIRECTORY 	${CMAKE_CURRENT_BINARY_DIR}/doc
				DESTINATION	$ENV{MEDITEC_LIBS}/${PROJECT_NAME}/tags/${MAJOR_VERSION}.${MINOR_VERSION}
			)
	endif(DOXYGEN_FOUND)
endif()
############################################

