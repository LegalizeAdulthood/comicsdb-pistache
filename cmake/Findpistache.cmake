include(FindPackageHandleStandardArgs)

find_library(PISTACHE_LIB NAMES pistache)
find_file(PISTACHE_INCLUDE pistache)

find_package_handle_standard_args(pistache DEFAULT_MSG PISTACHE_LIB PISTACHE_INCLUDE)

if(pistache_FOUND)
    get_filename_component(PISTACHE_LIB_DIR ${PISTACHE_LIB} DIRECTORY)
    get_filename_component(PISTACHE_INCLUDE_DIR ${PISTACHE_INCLUDE} DIRECTORY)
    add_library(pistache::pistache SHARED IMPORTED)
    set_property(TARGET pistache::pistache PROPERTY IMPORTED_LOCATION ${PISTACHE_LIB})
    if(WIN32)
	set_property(TARGET pistache::pistache PROPERTY IMPORTED_LIB ${PISTACHE_LIB})
    endif()
    target_include_directories(pistache::pistache INTERFACE ${PISTACHE_INCLUDE_DIR})
endif()
