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
  if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(wxWidgets_CONFIG_OPTIONS --prefix=${externpro} --version=${wxver} --debug=yes)
  else()
    set(wxWidgets_CONFIG_OPTIONS --prefix=${externpro} --version=${wxver} --debug=no)
  endif()
  # NOTE: find_program wasn't always working, so changed to set
  set(wxWidgets_CONFIG_EXECUTABLE ${externpro}/bin/wx-config)
  find_package(wxWidgets REQUIRED)
  include(${wxWidgets_USE_FILE})
  #set(wx_DEBUG TRUE)
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
  string(REPLACE "wx_" "wxx_" wxbasename ${wxbasename})
  ###################
  # GTK2
  if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      set(GTK2_ADDITIONAL_SUFFIXES x86_64-linux-gnu/glib-2.0)
    endif()
    find_package(PkgConfig)
    pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
    if(GTK2_FOUND)
      include_directories(${GTK2_INCLUDE_DIRS})
    else()
      message(SEND_ERROR "GTK2 not found")
    endif()
    # wxplotctrl plotdraw.cpp needs to include wx/gtk/dcclient.h
    include_directories(${WX_SOURCE}/include)
  endif() # NOT Darwin
elseif(MSVC)
  set(wxbasename wxxmsw)
  # find wx headers and custom wx/setup.h
  include_directories(${WX_INCLUDE} ${WX_INCLUDE}/wx/msvc)
endif()
#######################################
function(set_wxxtarget_properties target)
  if(MSVC)
    # toolset
    if(MSVC12)
      set(toolset vc120)
    elseif(MSVC11)
      set(toolset vc110)
    elseif(MSVC10)
      set(toolset vc100)
    elseif(MSVC90)
      set(toolset vc90)
    elseif(MSVC80)
      set(toolset vc80)
    elseif(MSVC71)
      set(toolset vc71)
    elseif(MSVC70)
      set(toolset vc70)
    elseif(MSVC60)
      set(toolset vc60)
    else()
      set(toolset vcXX)
      message(SEND_ERROR "wxx CMakeLists.txt: MSVC compiler support lacking")
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
set(wxx_libs
  wxplotctrl/build
  wxthings/build
  wxTLC
)
foreach(lib ${wxx_libs})
  add_subdirectory(${lib})
endforeach()
