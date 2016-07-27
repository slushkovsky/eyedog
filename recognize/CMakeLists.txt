cmake_minimum_required(VERSION 2.8)
project(_classifier) # Must be the same with python module name (in py_module.cpp). 

#----------------------------CMAKE & GLOBAL PROPERTIES-------------------------------------------

add_compile_options(-std=c++11)

find_package(OpenCV 3 REQUIRED core imgproc)
find_package(Boost 1.58 REQUIRED python3)

set(Python_ADDITIONAL_VERSIONS 3.4;3.5)
find_package(PythonLibs 3 REQUIRED)


#---------------------------   TARGET    --------------------------------------------------------

add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/py_module.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
	/usr/local/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/../
	${PYTHON_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
	${OpenCV_LIBRARIES}
	${Boost_LIBRARIES} 
	${PYTHON_LIBRARIES}
	pyboost_opencv	
	caffe
	proto
)

#---------------------------   INSTALLATION    -----------------------------------------------------

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
)
