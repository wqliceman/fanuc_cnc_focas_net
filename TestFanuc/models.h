#pragma once
#include <string>

struct program_info
{
	std::string main_prog;       // 主程序号
	std::string main_prog_msg;   // 主程序备注
	std::string current_prog;    // 当前程序号
	std::string current_prog_msg;// 当前程序号备注
};

struct system_info
{
	std::string cnc_type;  // Kind of CNC (ASCII)
	std::string mt_type;   // Kind of M/T/TT (ASCII)
	std::string series;    // Series number (ASCII)
	std::string version;   // Version number (ASCII)
	int max_axis;          // Max. controlled axes
	std::string axis_num;  // Current controlled axes(ASCII)
};

enum enum_pos_type {
	pos_all = -1,  // all type
	pos_abs = 0,   // absolute position
	pos_mach = 1,  // machine position
	pos_rel = 2,   // relative position
	pos_dist = 3   // distance to go
};

enum enum_device_std_status
{
	device_status_unknown = 0, // 未知
	device_status_stop = 1,    // 停机
	device_status_run = 2,     // 运行
	device_status_idle = 3,    // 空闲
	device_status_offline = 4, // 离线
	device_status_debug = 5    // 调试
};

enum enum_op_mode
{
	op_mode_mdi = 0,           // MDI
	op_mode_auto = 1,          // 自动
	op_mode_none = 2,          // None
	op_mode_edit = 3,          // 编辑
	op_mode_handle = 4,        // 手轮
	op_mode_jog = 5,           // 手动
	op_mode_teachin_jog = 6,   // 手动示教
	op_mode_teachin_handle = 7,// 手轮示教
	op_mode_incfeed = 8,       // 手动增量
	op_mode_reference = 9,     // 回零
	op_mode_remote = 10,       // 在线加工
	op_mode_other = 11         // 其他
};

enum enum_run_status
{
	run_status_reset = 0,  // 复位(接通电源或终止程序执行，自动运行结束)
	run_status_stop = 1,   // 停止(自动运行)
	run_status_hold = 2,   // 暂停(自动运行)
	run_status_start = 3,  // 启动(自动运行)
	run_status_mstr = 4,   // 手动数值指令启动
	run_status_other = 5   // 其他
};

struct status_info
{
	enum_device_std_status status; // 工作状态
	enum_op_mode op_mode;          // 操作模式
	enum_run_status run_status;    // 程序运行状态
	bool emergency;                // 急停信息
	bool alarm;                    // 是否报警
};

struct alarm_message
{
	long alarm_no;       // 报警编号
	short type;          // 报警类型
	short axis;          // 报警轴号
	std::string message; // 报警信息
};
