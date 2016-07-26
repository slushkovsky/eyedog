cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project("libpyboost_opencv")

#----------------------------CMAKE & GLOBAL PROPERTIES-------------------------#
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

###============= C++11 support====================================
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#=============== Find Packages 
find_package(OpenCV COMPONENTS core highgui imgproc videoio video)
find_package(Boost COMPONENTS python3)
include("DetectPython")

#================
SET(PYTHON_INCLUDE_DIRS ${PYTHON3_INCLUDE_DIR} ${PYTHON3_INCLUDE_DIR2} ${PYTHON3_NUMPY_INCLUDE_DIRS})

#==========

file(GLOB project_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${project_sources} ${CMAKE_CURRENT_SOURCE_DIR}/include/pyboostcvconverter.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
	${Boost_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS} 
	${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} 
	${Boost_LIBRARIES} 
	${OpenCV_LIBRARIES} 
)

#---------------------------   INSTALLATION    -----------------------------------------------------
#-get proper extension for python binary shared object on this platform
set_target_properties(${PROJECT_NAME} PROPERTIES
                      PREFIX ""
                      OUTPUT_NAME ${PROJECT_NAME} 
                      SUFFIX .so)

install(TARGETS ${PROJECT_NAME}
      RUNTIME DESTINATION ./
      LIBRARY DESTINATION ./
      ARCHIVE DESTINATION ./
 )