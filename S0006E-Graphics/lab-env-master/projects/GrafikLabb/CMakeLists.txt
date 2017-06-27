#--------------------------------------------------------------------------
# GrafikLabb project
#--------------------------------------------------------------------------

PROJECT(GrafikLabb)
FILE(GLOB GrafikLabb_headers code/*.h, tinyxml2/tinyxml2.h)
FILE(GLOB GrafikLabb_sources code/*.cc , tinyxml2/tinyxml2.cpp)


SET(files_GrafikLabb ${GrafikLabb_headers} ${GrafikLabb_sources} code/XmlLoader.cc code/XmlLoader.h code/JointsStructure.cc code/JointsStructure.h code/NAXLoader.cc code/NAXLoader.h code/AnimResource.cc code/AnimResource.h code/AnimClip.cc code/AnimClip.h code/AnimCurve.cc code/AnimCurve.h code/AnimKeyBuffer.cc code/AnimKeyBuffer.h code/Nvx2Loader.cc code/Nvx2Loader.h code/PrimitiveGroup.h code/VertexComponent.h)
SOURCE_GROUP("GrafikLabb" FILES ${files_GrafikLabb})

ADD_EXECUTABLE(GrafikLabb ${files_GrafikLabb})
TARGET_LINK_LIBRARIES(GrafikLabb core render)
ADD_DEPENDENCIES(GrafikLabb core render)
