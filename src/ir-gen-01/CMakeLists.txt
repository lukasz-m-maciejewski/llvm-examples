file(GLOB IR-GEN-FILES "*.cpp")
add_executable(ir-gen ${IR-GEN-FILES})
llvm_map_components_to_libnames(IR-GEN-LIBS bitwriter core support)
target_link_libraries(ir-gen ${IR-GEN-LIBS})
