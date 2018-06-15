cmake_minimum_required(VERSION 3.0)
project(wxx)
include(flags OPTIONAL)
# reset any postfix setting done previously
set(CMAKE_DEBUG_POSTFIX)
set(CMAKE_RELEASE_POSTFIX)
#######################################
set(externpro ${CMAKE_INSTALL_PREFIX})
if(DEFINED WX_SOURCE)
  message(STATUS "WX_SOURCE: ${WX_SOURCE}")
endif()
if(DEFINED WX_INCLUDE)
  string(REGEX REPLACE ".*include/wx-([0-9]*).([0-9]*)$" "\\1.\\2" wxver ${WX_INCLUDE})
  set(wxinc "wx-${wxver}")
  if(NOT UNIX)
    string(REGEX REPLACE "([0-9])\\.([0-9])?" "\\1\\2" wxver ${wxver})
  endif()
  message(STATUS "wxver: ${wxver}, wxinc: ${wxinc}")
else()
  message(SEND_ERROR "wxx CMakeLists.txt: WX_INCLUDE not defined")
endif()
if(UNIX)
  ###################
  # wxWidgets
  set(wxWidgets_CONFIG_EXECUTABLE ${externpro}/bin/wx-config)
  if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(wxWidgets_CONFIG_OPTIONS --prefix=${externpro} --version=${wxver} --debug=yes)
  else()
    set(wxWidgets_CONFIG_OPTIONS --prefix=${externpro} --version=${wxver} --debug=no)
  endif()
  find_package(wxWidgets REQUIRED)
  include(${wxWidgets_USE_FILE})
  if(wx_DEBUG)
    message(STATUS "wxWidgets_USE_FILE: ${wxWidgets_USE_FILE}")
    message(STATUS "wxWidgets_INCLUDE_DIRS: ${wxWidgets_INCLUDE_DIRS}")
    message(STATUS "wxWidgets_LIBRARIES: ${wxWidgets_LIBRARIES}")
    message(STATUS "wxWidgets_LIBRARY_DIRS: ${wxWidgets_LIBRARY_DIRS}")
    message(STATUS "wxWidgets_DEFINITIONS: ${wxWidgets_DEFINITIONS}")
    message(STATUS "wxWidgets_CXX_FLAGS: ${wxWidgets_CXX_FLAGS}")
  endif()
  ###################
  execute_process(COMMAND sh "${wxWidgets_CONFIG_EXECUTABLE}" --prefix=${externpro}
    --version=${wxver} --basename --debug=no
    OUTPUT_VARIABLE wxbasename OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
    )
  if(wxbasename MATCHES wx_gtk)
    find_package(PkgConfig)
    if(NOT DEFINED GTK_VER)
      message(FATAL_ERROR "GTK_VER should be defined (-DGTK_VER=2 or -DGTK_VER=3)")
    endif()
    pkg_check_modules(GTK REQUIRED gtk+-${GTK_VER}.0)
    if(GTK_FOUND)
      include_directories(${GTK_INCLUDE_DIRS})
    endif()
    if(${GTK_VER} EQUAL 2)
      # wxplotctrl plotdraw.cpp needs to include wx/gtk/dcclient.h when building against gtk2
      include_directories(${WX_SOURCE}/include)
    endif()
  endif()
  string(REPLACE "wx_" "wxx_" wxbasename ${wxbasename})
elseif(MSVC)
  set(wxbasename wxxmsw)
  # find wx headers and custom wx/setup.h
  include_directories(${WX_INCLUDE} ${WX_INCLUDE}/wx/msvc)
endif()
#######################################
function(set_wxxtarget_properties target)
  if(MSVC)
    if(COMMAND xpGetCompilerPrefix)
      xpGetCompilerPrefix(toolset)
    else()
      set(toolset vc)
    endif()
    if(XP_BUILD_STATIC_RT) # from flags include, xpCommonFlags, xpopts.cmake
      set(static s)
    endif()
    set_target_properties(${target} PROPERTIES
      OUTPUT_NAME ${wxbasename}${wxver}${toolset}${static}x_${target}
      DEBUG_OUTPUT_NAME ${wxbasename}${wxver}${toolset}${static}d_${target}
      RELEASE_OUTPUT_NAME ${wxbasename}${wxver}${toolset}${static}_${target}
      )
  else()
    set_target_properties(${target} PROPERTIES
      OUTPUT_NAME ${wxbasename}x_${target}-${wxver}
      DEBUG_OUTPUT_NAME ${wxbasename}d_${target}-${wxver}
      RELEASE_OUTPUT_NAME ${wxbasename}_${target}-${wxver}
      )
  endif()
endfunction()
#######################################
# we want all platforms, at this point, to have
# WX_VER of the form: 30, 31 (not 3.0, 3.1)
# for the naming of the targets.cmake files
string(REGEX REPLACE "([0-9])\\.([0-9])?" "\\1\\2" WX_VER ${wxver})
set(wxx_libs
  wxplotctrl/build
  wxthings/build
  wxTLC
  )
foreach(lib ${wxx_libs})
  add_subdirectory(${lib})
endforeach()
