# libqdecoder
# Copyright Robin.Rowe@cinepaint.org 2015/7/19
# License MIT or BSD

cmake_minimum_required(VERSION 2.8)
project(CMake_qdecoder)

#version number
set(CMake_qdecoder_VERSION_MAJOR 1)
set(CMake_qdecoder_VERSION_MINOR 0)
set(CMake_qdecoder_VERSION_PATCH 0)

file(GLOB qdecoder_HDR "*.h")
source_group("Header Files" FILES "${qdecoder_HDR}")

file(GLOB qdecoder_SRC "*.cpp")
source_group("Source Files" FILES "${qdecoder_SRC}")

if (BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
    set(lib_type SHARED)
else ()
    set(lib_type STATIC)
endif ()
set(LIBRARY_NAME "qdecoder")
add_library( ${LIBRARY_NAME} ${lib_type} ${qdecoder_HDR} ${qdecoder_SRC} )

#######################################
# Export / installation configuration #
#######################################

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
install(FILES ${qdecoder_HDR}
        TYPE "INCLUDE")
