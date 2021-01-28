SET(CMAKE_CXX_FLAGS -fPIC)
ADD_EXECUTABLE(NERecord-demo NERecord-demo.cpp)
TARGET_LINK_LIBRARIES(NERecord-demo nerecord pthread dl)

