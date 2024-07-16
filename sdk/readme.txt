多核录制sdk需要配置录制核数,NERecordEngineContext结构体中，parameters配置格式如下，
core_num建议配置成6，配置成其他多路录制中可能出现录制不全和偶现崩溃问题。
{
	core_num: 6,
	auto_subscribe_audio: false,
	auto_subscribe_video: false
}