# libunistd/portable/CMakeLists.txt

project(libportable)
message("--- Building library ${PROJECT_NAME} ---")

file(STRINGS headers.cmake HEADERS)
file(STRINGS sources.cmake SOURCES)

source_group("Header Files" FILES "${HEADERS}")
source_group("Source Files" FILES "${SOURCES}")

include(GNUInstallDirs)
if (BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS AND BUILD_PORTABLE_SHARED)
	set(lib_type SHARED)
else ()
    set(lib_type STATIC)
endif ()
add_library("${PROJECT_NAME}" ${lib_type} "${HEADERS}" "${SOURCES}")

#######################################
# Export / installation configuration #
#######################################

# setup the version PROJECT_NAME
set_property(TARGET "${PROJECT_NAME}" PROPERTY VERSION "${${PROJECT_NAME}_VERSION}")
set_property(TARGET "${PROJECT_NAME}" PROPERTY SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}")

# install rules
list(APPEND installable_libs "${PROJECT_NAME}")
if (TARGET "${DEPENDANT_LIBRARY}")
	list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif (TARGET "${DEPENDANT_LIBRARY}")
install(TARGETS ${installable_libs}
		EXPORT "${PROJECT_NAME}Targets"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES ${HEADERS}
		TYPE "INCLUDE")
