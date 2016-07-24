
project("llvm-examples" CXX)
cmake_minimum_required(VERSION 3.5.0)
include(cmake/utils.cmake REQUIRED)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# execute_process(COMMAND "llvm-config" --cppflags
#   OUTPUT_VARIABLE MY_LLVM_CPPFLAGS
#   OUTPUT_STRIP_TRAILING_WHITESPACE)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_LLVM_CPPFLAGS}")

# execute_process(COMMAND "llvm-config" --cxxflags
#   OUTPUT_VARIABLE MY_LLVM_CXXFLAGS
#   OUTPUT_STRIP_TRAILING_WHITESPACE)

# string(REPLACE "-Wno-maybe-uninitialized" "-Wno-uninitialized" MY_LLVM_CXXFLAGS "${MY_LLVM_CXXFLAGS}")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_LLVM_CXXFLAGS}")

# execute_process(COMMAND "llvm-config" --libs bitreader core support
#   OUTPUT_VARIABLE MY_LLVM_LIBS
#   OUTPUT_STRIP_TRAILING_WHITESPACE)

# execute_process(COMMAND "llvm-config" --system-libs
#   OUTPUT_VARIABLE MY_LLVM_SYSTEM_LIBS
#   OUTPUT_STRIP_TRAILING_WHITESPACE)

list(APPEND PROJ_CXX_FLAGS_LIST "-Wall" "-Wextra" "-Werror" "-std=c++14" "-pedantic" "-g")
list(APPEND PROJ_CXX_FLAGS_LIST "-march=native" "-mtune=generic" "-O2" "-pipe")
list(APPEND PROJ_CXX_FLAGS_LIST "-fstack-protector-strong" "-fPIC" "-fvisibility-inlines-hidden")

ListToString(PROJ_CXX_FLAGS " " ${PROJ_CXX_FLAGS_LIST})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROJ_CXX_FLAGS}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
message("LLVM_DEFINITIONS is: ${LLVM_DEFINITIONS}")


list(APPEND SOURCE_FILES_LIST hello.cpp)

add_executable(hello-llvm ${SOURCE_FILES_LIST})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs bitreader support core)
message("llvm_libs is: ${llvm_libs}")

# Link against LLVM libraries
target_link_libraries(hello-llvm ${llvm_libs})

# set(MY_LLVM_LIBRARIES "${MY_LLVM_LIBS} ${MY_LLVM_SYSTEM_LIBS}")
# target_link_libraries(hello-llvm ${MY_LLVM_LIBRARIES})
