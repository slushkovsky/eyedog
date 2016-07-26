cmake_minimum_required(VERSION 2.8)
project("pyboost_opencv")

#----------------------------CMAKE & GLOBAL PROPERTIES-------------------------------------------


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_compile_options(-std=c++11)

find_package(OpenCV COMPONENTS core highgui imgproc videoio video)
find_package(Boost COMPONENTS python3)

include("DetectPython")
SET(PYTHON_INCLUDE_DIRS ${PYTHON3_INCLUDE_DIR} ${PYTHON3_INCLUDE_DIR2} ${PYTHON3_NUMPY_INCLUDE_DIRS})



#---------------------------   TARGET    --------------------------------------------------------




add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/pyboost_cv3_converter.cpp
                                   ${CMAKE_CURRENT_SOURCE_DIR}/include/pyboostcvconverter.hpp)


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

install(TARGETS ${PROJECT_NAME}
      RUNTIME DESTINATION /usr/local/bin
      LIBRARY DESTINATION /usr/local/lib
      ARCHIVE DESTINATION /dev/null
)