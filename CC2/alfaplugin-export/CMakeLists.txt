cmake_minimum_required (VERSION 3.0)

set(PROJECT_NAME alfaplugin)

project(alfaplugin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    # Location of helper object headers.
    ../../str
    ../../ds
)

# Build our alfapluging.
set (ALFAPLUGIN_SOURCES
    alfaplugin_manager.c

    # We have the helper objects here instead of making them into libraries.
    ../ds/list.c
    ../ds/list_str.c
    ../ds/htable.c
    ../str/str_builder.c
    main.c
)
add_executable(alfaplugin ${ALFAPLUGIN_SOURCES})
set_target_properties(alfaplugin PROPERTIES ENABLE_EXPORTS ON)

target_compile_definitions(${PROJECT_NAME} PUBLIC #PRIVATE
        "$<$<CONFIG:Debug>:"
        "LINUX;"
        "BIT64;"
        ""
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "LINUX;"
        "BIT64;"
        ""
        ">"
        )

set(ALFA_SOURCES
        plugins/cplug_alfa_redraw.c
        )
add_library(alfa_redraw MODULE ${ALFA_SOURCES})
target_link_libraries(alfa_redraw alfaplugin -O0)

set(ALFA_SOURCES
        plugins/cplug_alfa_stirrup.c
)
add_library(alfa_stirrup MODULE ${ALFA_SOURCES})
target_link_libraries(alfa_stirrup alfaplugin -lm -O0)

set(ALFA_SOURCES
        plugins/cplug_alfa_rebar.c
        )
add_library(alfa_rebar MODULE ${ALFA_SOURCES})
target_link_libraries(alfa_rebar alfaplugin -lm -O0)

set(ALFA_SOURCES
        plugins/cplug_alfa_fillet_polyline.c
        )
add_library(alfa_fillet_polyline MODULE ${ALFA_SOURCES})
target_link_libraries(alfa_fillet_polyline alfaplugin -O0)

set(ALFA_SOURCES
        plugins/cplug_alfa_coordinates.c
        )
add_library(alfa_coordinates MODULE ${ALFA_SOURCES})
target_link_libraries(alfa_coordinates alfaplugin -O0)
