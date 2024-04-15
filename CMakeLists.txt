cmake_minimum_required(VERSION 3.28)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/.devcontainer/cmake)
include(preproject)
project(wxx VERSION 24.00)
set(buildpro_REV 24.05)
include(xpfunmac)
set(externpro_REV 24.03)
find_package(externpro REQUIRED)
include(flags)
include(GNUInstallDirs)
# reset any postfix setting done previously
set(CMAKE_DEBUG_POSTFIX)
set(CMAKE_RELEASE_POSTFIX)
#######################################
xpFindPkg(PKGS wxwidgets)
get_target_property(wxIncDirs wx::base INTERFACE_INCLUDE_DIRECTORIES)
list(FILTER wxIncDirs INCLUDE REGEX "include/wx-")
list(GET wxIncDirs 0 wxIncDir)
string(REGEX REPLACE ".*include/wx-([0-9]*).([0-9]*)$" "\\1.\\2" wxVer ${wxIncDir})
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
elseif(UNIX AND DEFINED wxWidgets_CONFIG_EXECUTABLE) # wxWidgets_CONFIG_EXECUTABLE from wxwidgets-targets
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
if(NOT DEFINED XP_INSTALL_CMAKEDIR)
  set(XP_INSTALL_CMAKEDIR ${CMAKE_INSTALL_DATADIR}/cmake)
endif()
set(targetsFile ${PROJECT_NAME}-targets)
set(wxxlib plotctrl things tlc)
list(TRANSFORM wxxlib PREPEND ${CMAKE_PROJECT_NAME}::)
xpPackageDevel(TARGETS_FILE ${targetsFile} DEPS wxwidgets LIBRARIES ${wxxlib})
foreach(lib
  wxthings/build
  wxplotctrl/build
  wxTLC
  )
  add_subdirectory(${lib})
endforeach()
if(DEFINED XP_NAMESPACE)
  set(nameSpace NAMESPACE ${XP_NAMESPACE}::)
endif()
install(EXPORT ${targetsFile} DESTINATION ${XP_INSTALL_CMAKEDIR} ${nameSpace})
