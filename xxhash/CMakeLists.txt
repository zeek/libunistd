# libunistd/xxhash/CMakeLists.txt

project(libxxhash)
message("--- Building library ${PROJECT_NAME} ---")

set(Header_Files "xxhash.h")
source_group("Header Files" FILES "${Header_Files}")

set(Source_Files "xxhash.c" "xxhsum.c")
source_group("Source Files" FILES "${Source_Files}")

if (BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
	set(lib_type SHARED)
else ()
	set(lib_type STATIC)
endif ()
set(LIBRARY_NAME "${PROJECT_NAME}")
add_library(${LIBRARY_NAME} ${lib_type} ${Header_Files} ${Source_Files} )

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
install(FILES ${Header_Files}
		TYPE "INCLUDE")
