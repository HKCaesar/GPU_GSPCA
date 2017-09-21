cmake_minimum_required(VERSION 3.0)

project(kernel_pca)

set(CMAKE_BUILD_TYPE RELEASE)


if (UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
else ()
    set(CMAKE_CXX_FLAGS "/c /Zi /GS- /GL /W3 /Gy /Gm- /Ox /EHsc /W3 /O2 /MD")
    set(CMAKE_EXE_LINKER_FLAGS "/LTCG /RELEASE")
endif ()



# CUDA PACKAGE
find_package(CUDA REQUIRED)
set(CUDA_SEPARABLE_COMPILATION ON)
set(CUDA_PROPAGATE_HOST_FLAGS OFF)


SET(CUDA_COMPILER "${CUDA_TOOLKIT_ROOT_DIR}/bin/nvcc")

set(CUDA_ARCH "sm_50")



# SETUP FOR CPP FILES
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cuda_include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CUDA_INCLUDE_DIRECTORIES}
)

cuda_add_library(kernel_pca
	${CMAKE_CURRENT_SOURCE_DIR}/src/kernel_pca.cu
)


CUDA_ADD_CUBLAS_TO_TARGET(kernel_pca ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel_pca.cu)

CUDA_COMPILE(kernel_pca ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel_pca.cu)

target_link_libraries(kernel_pca
	cuda
)



#### Python wrapper ############################

find_package( PythonLibs 2.7 REQUIRED )

find_package( Boost COMPONENTS python REQUIRED )

execute_process( COMMAND export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/usr/include/python2.7/" )

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${PYTHON_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
)

add_library(kernel_pca_pywrap 
	${CMAKE_CURRENT_SOURCE_DIR}/src/kernel_pca_pywrap.cpp
)

target_link_libraries(kernel_pca_pywrap
	kernel_pca
	${Boost_LIBRARIES}
	${Python_LIBRARIES}
)

# don't prepend wrapper library name with lib
set_target_properties( kernel_pca_pywrap PROPERTIES PREFIX "" )


### Example compilation ################

find_package(GSL REQUIRED)

# COMPILE AND LINK
add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

include_directories(
	${GSL_INCLUDE_DIRS}
)

target_link_libraries(main 
	kernel_pca
	${GSL_LIBRARIES}
)














