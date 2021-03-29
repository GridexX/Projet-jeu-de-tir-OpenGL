project(glengine)

cmake_minimum_required(VERSION 3.0.2)

set(RESOURCES_DIRECTORY "${PROJECT_SOURCE_DIR}/resources/")
#set(RESOURCES_DIRECTORY "resources/")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include/")

#Configure config.hpp.in
configure_file(
	${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/config.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/config.hpp @ONLY
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include BEFORE)

set(SRC
  ${SRC_DIR}/texture.cpp
  ${SRC_DIR}/shader.cpp
  ${SRC_DIR}/mesh.cpp
  ${SRC_DIR}/glmesh.cpp
  ${SRC_DIR}/glengine.cpp
)

set(HEADER
  ${INC_DIR}/${PROJECT_NAME}/config.hpp.in
  ${INC_DIR}/${PROJECT_NAME}/shader.hpp
  ${INC_DIR}/${PROJECT_NAME}/mesh.hpp
  ${INC_DIR}/${PROJECT_NAME}/glmesh.hpp
  ${INC_DIR}/${PROJECT_NAME}/camera.hpp
  ${INC_DIR}/${PROJECT_NAME}/texture.hpp
  ${INC_DIR}/${PROJECT_NAME}/glengine.hpp
  ${INC_DIR}/${PROJECT_NAME}/glew.h
  ${INC_DIR}/${PROJECT_NAME}/freeglut.h
)

set(RESOURCE_FILES
#	${PROJECT_SOURCE_DIR}/resources/truc.txt
)

add_library(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
target_include_directories (${PROJECT_NAME}
  PUBLIC ${INC_DIR}
)

#SET(VS_STARTUP_PROJECT ${EXECUTABLE_NAME})
#set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXECUTABLE_NAME})

install(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

add_subdirectory(doc)