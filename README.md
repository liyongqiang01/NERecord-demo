# NERecord-demo
集成网易云信 Linux本地服务器录制（NERecord）SDK 的示例代码

开发/运行环境

    Ubuntu 16.04(推荐) 或 Centos 7.3+
    g++ 5.4
    cmake 3.0+
    
    
编译

    获取SDK, 放到代码NERecord-SDK目录下, 通过cmake编译示例代码.
    


获取AppKey / token.
    阅读 快速入门->操作流程.
    
    
运行
    创建普通音视频通话.
    录制SDK加入房间. 

${DIR_OF_SAMPLE}/nerecord_sample \
        -c ${CHANNEL_NAME} \
        -u ${UID} \
        -k ${APP_KEY} \
        -o ${LOG_DIR} \
        -t ${TOKEN} \
        -e ${RTC_ENGINE}
    
