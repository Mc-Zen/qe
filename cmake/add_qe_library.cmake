
# Add a QE library by creating a target. 
#
# This function automatically sets the target folder and adds the include directory. 
# ____________________________________
#
# Parameters:
#   [target]        target name
#   sources..	        input source files
# ____________________________________
#
# Call example: 
# 
#	 add_qe_library(math
#        matrix.h 
#        graph.h
#        graph.cpp
#	 )

function(add_qe_library target)
    add_library(${target} ${ARGN})

    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    set_target_properties(${target} PROPERTIES FOLDER ${QE_LIBRARY_IDE_FOLDER})
endfunction()