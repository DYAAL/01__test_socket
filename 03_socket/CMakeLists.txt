aux_source_directory(./ SRC_FILE)
include_directories(${HEADPATH})

set(LIBRARY_OUTPUT_PATH ${LIBPATH})
add_library(${SOCKETLIB} STATIC ${SRC_FILE})
