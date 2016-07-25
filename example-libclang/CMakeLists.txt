# find_package(Clang)

add_executable(ex01 ex01.cpp)
target_link_libraries(ex01  -lclang) # -lrt -ldl -lcurses -lpthread -lz -lm)

add_executable(ex02 ex02.cpp)
target_link_libraries(ex02 clang)

add_executable(ex03 ex03.cpp)
target_link_libraries(ex03 clang)
