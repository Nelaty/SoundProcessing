# Source file utils
function(CM_GetAllSources sources )
    file(GLOB_RECURSE result "*.cpp" "*.c" "*.cxx"
            "*.hpp" "*.h" "*.hxx"
            "*.inl")
    set(${sources} ${result} PARENT_SCOPE)
endfunction()

function(CM_Setup _cxx_standard)
    set(CMAKE_CXX_STANDARD ${_cxx_standard} PARENT_SCOPE)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin PARENT_SCOPE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin PARENT_SCOPE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib PARENT_SCOPE)
endfunction()

function(CM_GetHeaderFiles result)

endfunction()