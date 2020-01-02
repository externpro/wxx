cmake_minimum_required(VERSION 3.12)
project(wxx)
include(flags OPTIONAL)
# reset any postfix setting done previously
set(CMAKE_DEBUG_POSTFIX)
set(CMAKE_RELEASE_POSTFIX)
#######################################
find_package(usexp-wxwidgets REQUIRED PATHS ${CMAKE_MODULE_PATH} NO_DEFAULT_PATH)
get_target_property(wxIncDirs wx::base INTERFACE_INCLUDE_DIRECTORIES)
list(FILTER wxIncDirs INCLUDE REGEX "include/wx-")
string(REGEX REPLACE ".*include/wx-([0-9]*).([0-9]*)$" "\\1.\\2" wxVer ${wxIncDirs})
set(wxInc "include/wx-${wxVer}") # wxInc used in subdirectories
message(STATUS "wxVer: ${wxVer}, wxInc: ${wxInc}")
if(MSVC)
  string(REGEX REPLACE "([0-9])\\.([0-9])?" "\\1\\2" wxVer ${wxVer})
  if(COMMAND xpGetCompilerPrefix)
    xpGetCompilerPrefix(toolset)
  else()
    set(toolset vc)
  endif()
  if(XP_BUILD_STATIC_RT) # from flags include, xpCommonFlags, xpopts.cmake
    set(static s)
  endif()
  set(WXX_OUTPUT_NAME wxxmsw${wxVer}${toolset}${static}y_@TARGET@
    DEBUG_OUTPUT_NAME wxxmsw${wxVer}${toolset}${static}d_@TARGET@
    RELEASE_OUTPUT_NAME wxxmsw${wxVer}${toolset}${static}_@TARGET@
    )
elseif(UNIX AND DEFINED wxWidgets_CONFIG_EXECUTABLE) # wxWidgets_CONFIG_EXECUTABLE from usexp-wxwidgets
  execute_process(COMMAND sh "${wxWidgets_CONFIG_EXECUTABLE}" --prefix=${CMAKE_INSTALL_PREFIX}
    --version=${wxVer} --basename --debug=no
    OUTPUT_VARIABLE wxbasename OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
    )
  string(REPLACE "wx_" "wxx_" wxbasename ${wxbasename})
  set(WXX_OUTPUT_NAME ${wxbasename}y_@TARGET@-${wxVer}
    DEBUG_OUTPUT_NAME ${wxbasename}d_@TARGET@-${wxVer}
    RELEASE_OUTPUT_NAME ${wxbasename}_@TARGET@-${wxVer}
    )
else()
  set(WXX_OUTPUT_NAME wxx_unky_@TARGET@-${wxVer}
    DEBUG_OUTPUT_NAME wxx_unkd_@TARGET@-${wxVer}
    RELEASE_OUTPUT_NAME wxx_unk_@TARGET@-${wxVer}
    )
endif()
#######################################
# we want all platforms, at this point, to have
# WX_VER of the form: 30, 31 (not 3.0, 3.1)
# for the naming of the targets.cmake files
string(REGEX REPLACE "([0-9])\\.([0-9])?" "\\1\\2" WX_VER ${wxVer})
set(targetsFile ${PROJECT_NAME}${WX_VER}-targets)
foreach(lib
  wxthings/build
  wxplotctrl/build
  wxTLC
  )
  add_subdirectory(${lib})
endforeach()
