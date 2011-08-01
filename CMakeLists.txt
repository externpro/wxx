project(wxx)
cmake_minimum_required(VERSION 2.8.5)
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # enables Solution Folders...
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(sdlExtern_DIR ${PROJECT_SOURCE_DIR}/../../sdlExtern)
#######################################
# cmake modules
if(UNIX)
  ###################
  # wxWidgets
  set(wxWidgets_CONFIG_OPTIONS --prefix=${sdlExtern_DIR})
  find_program(wxWidgets_CONFIG_EXECUTABLE wx-config
    PATHS ${sdlExtern_DIR}/bin
  )
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
    include_directories(
      /usr/local/lib/amd64/glib-2.0/include
      /usr/local/lib/amd64/gtk-2.0/include
      /usr/local/lib/amd64/libffi-3.0.9/include
    )
  endif()
  find_package(GTK2 2.6 REQUIRED gtk)
  if(GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIRS})
  else()
    message(SEND_ERROR "GTK2 not found")
  endif()
endif(UNIX)
#######################################
set(wxx_libs
  wxIFM
  wxplotctrl/build
  wxthings/build
  wxTreeListCtrl
  wxTreeMultiCtrl/sdl
)
foreach(lib ${wxx_libs})
  add_subdirectory(${lib})
endforeach()

