cmake_minimum_required(VERSION 3.0.0)
project(FluidSim VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)




# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/include/glad/*.cpp
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/include/glad/*.h
	${CMAKE_SOURCE_DIR}/include/stb_image/*.h
	${CMAKE_SOURCE_DIR}/src/*.h)

# Add shaders
file(GLOB_RECURSE GLSL_FILES 
	${CMAKE_SOURCE_DIR}/src/*.glsl)

# Add .jpg
file(GLOB_RECURSE JPG_FILES 
	${CMAKE_SOURCE_DIR}/src/*.jpg)



# https://stackoverflow.com/questions/46995733/how-to-set-cmake-in-order-to-add-txt-files-into-working-directory-as-resource
file(COPY ${GLSL_FILES} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${JPG_FILES} DESTINATION ${CMAKE_BINARY_DIR})

# include GLAD
add_library(GLAD "include/glad/glad.c")

#include glfw3
#unify libraries under one variable
set(LIBS glfw GLAD OpenGL::GL)

# executable
add_executable(FluidSim ${HEADER_FILES} ${SOURCE_FILES})

# include directories, change to target_include_directories for
# specific include instructions
include_directories("include")

# Link Libraries
target_link_libraries(FluidSim ${LIBS})