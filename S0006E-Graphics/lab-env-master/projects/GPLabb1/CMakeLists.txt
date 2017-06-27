#--------------------------------------------------------------------------
# example project
#--------------------------------------------------------------------------

PROJECT(GPLabb1)
FILE(GLOB GPLabb1_headers code/*.h)
FILE(GLOB GPLabb1_sources code/*.cpp)

SET(files_GPLabb1 ${GPLabb1_headers} ${GPLabb1_sources})
SOURCE_GROUP("GPLabb1" FILES ${files_GPLabb1})

ADD_EXECUTABLE(GPLabb1 ${files_GPLabb1})
TARGET_LINK_LIBRARIES(GPLabb1 core render)
ADD_DEPENDENCIES(GPLabb1 core render)