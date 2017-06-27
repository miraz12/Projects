#--------------------------------------------------------------------------
# KonsolLabb project
#--------------------------------------------------------------------------

PROJECT(KonsolLabb)
FILE(GLOB KonsolLabb_headers code/*.h)
FILE(GLOB KonsolLabb_sources code/*.cc)


SET(files_KonsolLabb ${KonsolLabb_headers} ${KonsolLabb_sources})
SOURCE_GROUP("KonsolLabb" FILES ${files_KonsolLabb})

ADD_EXECUTABLE(KonsolLabb ${files_KonsolLabb})
TARGET_LINK_LIBRARIES(KonsolLabb core render)
ADD_DEPENDENCIES(KonsolLabb core render)
