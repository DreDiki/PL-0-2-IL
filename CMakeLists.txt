#1.cmake verson��ָ��cmake�汾 
cmake_minimum_required(VERSION 3.0)

#2.project name��ָ����Ŀ�����ƣ�һ�����Ŀ���ļ������ƶ�Ӧ
PROJECT(dcc)

# c++11 support
include(CheckCXXCompilerFlag)  
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)  
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)  
if(COMPILER_SUPPORTS_CXX14)  
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")  
elseif(COMPILER_SUPPORTS_CXX0X)  
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")  
else()  
     message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")  
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")  

add_definitions(-DDEBUG)

#3.head file path��ͷ�ļ�Ŀ¼
INCLUDE_DIRECTORIES(
include
)

#4.source directory��Դ�ļ�Ŀ¼
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

#6.add executable file�����Ҫ����Ŀ�ִ���ļ�
ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRCS})

#7.add link library����ӿ�ִ���ļ�����Ҫ�Ŀ⣬���������õ���libm.so����������lib+name+.so��������Ӹÿ������
TARGET_LINK_LIBRARIES(${PROJECT_NAME})