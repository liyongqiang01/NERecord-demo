message("#############nerecord_demo#############")
project(nerecord_demo)

set(CMAKE_DEBUG_POSTFIX "d")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/demo)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/fjson ${PROJECT_SOURCE_DIR}/curl/include ${PROJECT_SOURCE_DIR}/curl)

LINK_DIRECTORIES (${PROJECT_SOURCE_DIR}/sdk/lib
                ${PROJECT_SOURCE_DIR}/fjson/libs/x64
                ${PROJECT_SOURCE_DIR}/curl/lib)

# INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../../pc/include
#                     ${PROJECT_SOURCE_DIR}/../../../nertc_sdk_wrapper
#                     ${PROJECT_SOURCE_DIR}/../../../../../../thirdparty/cross_platform_net)

FILE(GLOB DEMO_SOURCE 
            main.cpp 
            cmdline.h
            nerecordEventHandlerImpl.h 
            nerecordEventHandlerImpl.cpp
            nerecordPacketObserverImpl.h
            nerecordPacketObserverImpl.cpp
            httpwrapper.h
            httpwrapper.cpp
            )
add_executable(nerecord_demo ${DEMO_SOURCE})

set_target_properties(nerecord_demo PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../sdk/lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

target_link_libraries(nerecord_demo  fjson nerecord pthread curl crypto ssl  dl -static-libstdc++)

FILE(GLOB TEST ${PROJECT_SOURCE_DIR}/*.sh 
               ${PROJECT_SOURCE_DIR}/*.json 
               ${PROJECT_SOURCE_DIR}/nrtc_config.txt 
               ${PROJECT_SOURCE_DIR}/layout.txt
               ${PROJECT_SOURCE_DIR}/layout_sub.txt
               ${PROJECT_SOURCE_DIR}/watermark_1920_1080.png)
FILE(GLOB INCLUDE ${PROJECT_SOURCE_DIR}/sdk/include/*.h)
FILE(GLOB LIBS ${PROJECT_SOURCE_DIR}/sdk/lib/* ${PROJECT_SOURCE_DIR}/sdk/lib/*.*.*)
INSTALL(TARGETS nerecord_demo RUNTIME DESTINATION demo)
INSTALL(FILES ${TEST} DESTINATION demo)
INSTALL(FILES ${INCLUDE} DESTINATION sdk/include)
INSTALL(FILES ${LIBS} DESTINATION sdk/lib)
