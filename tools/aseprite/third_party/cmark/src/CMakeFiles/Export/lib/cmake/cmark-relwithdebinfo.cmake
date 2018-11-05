#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cmark" for configuration "RelWithDebInfo"
set_property(TARGET cmark APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(cmark PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/cmark.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS cmark )
list(APPEND _IMPORT_CHECK_FILES_FOR_cmark "${_IMPORT_PREFIX}/bin/cmark.exe" )

# Import target "libcmark" for configuration "RelWithDebInfo"
set_property(TARGET libcmark APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(libcmark PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/cmark.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/cmark.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libcmark )
list(APPEND _IMPORT_CHECK_FILES_FOR_libcmark "${_IMPORT_PREFIX}/lib/cmark.lib" "${_IMPORT_PREFIX}/bin/cmark.dll" )

# Import target "libcmark_static" for configuration "RelWithDebInfo"
set_property(TARGET libcmark_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(libcmark_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/cmark_static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS libcmark_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_libcmark_static "${_IMPORT_PREFIX}/lib/cmark_static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
