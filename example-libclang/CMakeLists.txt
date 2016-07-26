# find_package(Clang)

add_executable(ex01 ex01.cpp)
target_link_libraries(ex01  -lclang) # -lrt -ldl -lcurses -lpthread -lz -lm)

add_executable(ex02 ex02.cpp)
target_link_libraries(ex02 clang)

add_executable(ex03 ex03.cpp)
target_link_libraries(ex03 clang)

add_executable(ex04 ex04.cpp)
target_link_libraries(ex04 ${MY_CLANG_LIBS})

execute_process(COMMAND "llvm-config" --libs
  OUTPUT_VARIABLE MY_LLVM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

target_link_libraries(ex04 ${MY_LLVM_LIBS})

execute_process(COMMAND "llvm-config" --system-libs
  OUTPUT_VARIABLE MY_LLVM_SYSTEM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

target_link_libraries(ex04 ${MY_LLVM_SYSTEM_LIBS})
