# libunistd/libuuid/CMakeLists.txt

project(libuuid)
message("--- Building library ${PROJECT_NAME} ---")

if(WIN32)
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../unistd" 
)
endif(WIN32)

add_definitions(
	-DHAVE_UNISTD_H 
	-DHAVE_ERR
	-DHAVE_ERRX
	-DHAVE_WARN
	-DHAVE_WARNX
	-DHAVE_STDLIB_H
	-DHAVE_USLEEP
	-DHAVE_SYS_TIME_H
	)

set(HEADERS
	uuid.h
	all-io.h
	c.h
	randutils.h
	uuidd.h
	uuidP.h
)
source_group("Header Files" FILES "${HEADERS}")

set(SOURCES
	clear.cpp
	compare.cpp
	copy.cpp
	gen_uuid.cpp
	isnull.cpp
	pack.cpp
	parse.cpp
	unpack.cpp
	unparse.cpp
	uuid_time.cpp
	randutils.cpp
)
source_group("Source Files" FILES "${SOURCES}")

if (BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
	set(lib_type SHARED)
else ()
	set(lib_type STATIC)
endif ()
set(LIBRARY_NAME "${PROJECT_NAME}")
add_library(${LIBRARY_NAME} ${lib_type} ${HEADERS} ${SOURCES} )
if(WITH_TESTS)
    add_subdirectory(test)
endif()

#######################################
# Export / installation configuration #
#######################################

include(GNUInstallDirs)

# setup the version LIBRARY_NAME
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "${${LIBRARY_NAME}_VERSION}")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "${${LIBRARY_NAME}_VERSION_MAJOR}")

# install rules
list(APPEND installable_libs "${LIBRARY_NAME}")
if (TARGET "${DEPENDANT_LIBRARY}")
	list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif (TARGET "${DEPENDANT_LIBRARY}")
install(TARGETS ${installable_libs}
		EXPORT "${LIBRARY_NAME}Targets"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES ${HEADERS}
		TYPE "INCLUDE")
