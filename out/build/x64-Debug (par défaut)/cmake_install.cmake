# Install script for directory: C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP/out/install/x64-Debug (par défaut)")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP/out/build/x64-Debug (par défaut)/stbimage/cmake_install.cmake")
  include("C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP/out/build/x64-Debug (par défaut)/glad/cmake_install.cmake")
  include("C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP/out/build/x64-Debug (par défaut)/glfw-3.3.2/cmake_install.cmake")
  include("C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP/out/build/x64-Debug (par défaut)/glm-0.9.9.9/glm/cmake_install.cmake")
  include("C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP/out/build/x64-Debug (par défaut)/glengine/cmake_install.cmake")
  include("C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP/out/build/x64-Debug (par défaut)/demoGLEngine/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/arsen/OneDrive/Documents/Cours/synthese-images/proto-TP/out/build/x64-Debug (par défaut)/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
