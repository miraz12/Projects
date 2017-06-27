#--------------------------------------------------------------------------
# Labb2 project
#--------------------------------------------------------------------------

PROJECT(Labb2)
FILE(GLOB Labb2_headers code/*.h)
FILE(GLOB Labb2_sources code/*.cc)


SET(files_Labb2 ${Labb2_headers} ${Labb2_sources})
SOURCE_GROUP("Labb2" FILES ${files_Labb2})

ADD_EXECUTABLE(Labb2 ${files_Labb2})
TARGET_LINK_LIBRARIES(Labb2 core render)
ADD_DEPENDENCIES(Labb2 core render)
