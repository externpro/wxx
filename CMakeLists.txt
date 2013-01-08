cmake_minimum_required(VERSION 2.8.5)
project(wxx)
include(flags OPTIONAL)
set(CMAKE_DEBUG_POSTFIX)
set(CMAKE_RELEASEMT_POSTFIX)
set(CMAKE_RELEASE_POSTFIX)
#######################################
set(externpro ${CMAKE_INSTALL_PREFIX})
if(UNIX)
  ###################
  # wxWidgets
  set(wxWidgets_CONFIG_OPTIONS --prefix=${externpro})
  if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(wxWidgets_CONFIG_OPTIONS --prefix=${externpro} --debug=yes)
  endif()
  # NOTE: NUMBITS from flags.cmake
  find_program(wxWidgets_CONFIG_EXECUTABLE wx-config PATHS ${externpro}/bin${NUMBITS})
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
  execute_process(COMMAND sh "${wxWidgets_CONFIG_EXECUTABLE}" --basename
    OUTPUT_VARIABLE wxbasename OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
    )
  string(REPLACE "wx_" "wxx_" wxbasename ${wxbasename})
  execute_process(COMMAND sh "${wxWidgets_CONFIG_EXECUTABLE}" --version
    OUTPUT_VARIABLE wxversion OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
    )
  ###################
  # GTK2
  if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GTK2_ADDITIONAL_SUFFIXES x86_64-linux-gnu/glib-2.0)
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
    set(CMAKE_PREFIX_PATH /usr/local/lib/amd64 ${CMAKE_PREFIX_PATH})
  endif()
  #set(GTK2_DEBUG TRUE)
  find_package(GTK2 2.6 REQUIRED gtk)
  if(GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIRS})
  else()
    message(SEND_ERROR "GTK2 not found")
  endif()
else()
  set(wxbasename wxxmsw)
  set(wxversion 28)
endif()
include_directories(${externpro}/include) # find wx headers
if(MSVC)
  include_directories(${externpro}/include/wx/msvc) # find MSVC-specific wx/setup.h
endif()
#######################################
function(set_wxxtarget_properties target)
  if(MSVC)
    # toolset
    if(MSVC60)
      set(toolset vc60)
    elseif(MSVC70)
      set(toolset vc70)
    elseif(MSVC71)
      set(toolset vc71)
    elseif(MSVC80)
      set(toolset vc80)
    elseif(MSVC90)
      set(toolset vc90)
    elseif(MSVC10)
      set(toolset vc100)
    else()
      set(toolset vcXX)
    endif()
    set_target_properties(${target} PROPERTIES
      OUTPUT_NAME ${wxbasename}${wxversion}${toolset}x_${target}
      DEBUG_OUTPUT_NAME ${wxbasename}${wxversion}${toolset}d_${target}
      RELEASEMT_OUTPUT_NAME ${wxbasename}${wxversion}${toolset}s_${target}
      RELEASE_OUTPUT_NAME ${wxbasename}${wxversion}${toolset}_${target}
      )
  else()
    set_target_properties(${target} PROPERTIES
      OUTPUT_NAME ${wxbasename}x_${target}-${wxversion}
      DEBUG_OUTPUT_NAME ${wxbasename}d_${target}-${wxversion}
      RELEASE_OUTPUT_NAME ${wxbasename}_${target}-${wxversion}
      )
  endif()
endfunction()
#######################################
set(wxx_libs
  wxIFM
  wxplotctrl/build
  wxthings/build
  wxTLC
  wxTMC/sdlcmake
)
foreach(lib ${wxx_libs})
  add_subdirectory(${lib})
endforeach()
