#pragma once
#include <string>

struct program_info
{
	/// <summary>
	/// 主程序号
	/// </summary>
	std::string main_prog;

	/// <summary>
	/// 主程序备注
	/// </summary>
	std::string main_prog_msg;

	/// <summary>
	/// 当前程序号
	/// </summary>
	std::string current_prog;

	/// <summary>
	/// 当前程序号备注
	/// </summary>
	std::string current_prog_msg;
};

struct system_info
{
	/// <summary>
	/// Kind of CNC (ASCII)
	/// '15' : Series 15/15i
	/// '16' : Series 16 / 16i
	/// '18' : Series 18 / 18i
	/// '21' : Series 21 / 21i
	/// '30' : Series 30i
	/// '31' : Series 31i
	/// '32' : Series 32i
	/// '35' : Series 35i
	/// ' 0' : Series 0i
	/// 'PD' : Power Mate i - D
	/// 'PH' : Power Mate i - H
	/// 'PM' : Power Motion i
	/// </summary>
	std::string cnc_type;

	/// <summary>
	/// Kind of M/T/TT (ASCII)
	/// ' M' : Machining center
	/// ' T' : Lathe
	/// 'MM' : M series with 2 path control
	/// 'TT' : T series with 2 / 3 path control
	/// 'MT' : T series with compound machining function
	/// ' P' : Punch press
	/// ' L' : Laser
	/// ' W' : Wire cut
	/// </summary>
	std::string mt_type;

	/// <summary>
	/// Series number (ASCII) The character string of 4 digits is stored.
	/// </summary>
	std::string series;

	/// <summary>
	/// Version number (ASCII) The character string of 4 digits is stored.
	/// </summary>
	std::string version;

	/// <summary>
	/// Max. controlled axes
	/// </summary>
	int max_axis;

	/// <summary>
	/// Current controlled axes(ASCII)
	/// </summary>
	std::string axis_num;
};

enum enum_pos_type {
	/// <summary>
	///  all type
	/// </summary>
	pos_all = -1,

	/// <summary>
	/// absolute position
	/// </summary>
	pos_abs = 0,

	/// <summary>
	/// machine position
	/// </summary>
	pos_mach = 1,

	/// <summary>
	/// relative position
	/// </summary>
	pos_rel = 2,

	/// <summary>
	/// distance to go
	/// </summary>
	pos_dist = 3
};

enum enum_device_std_status
{
	/// <summary>
	/// 未知
	/// </summary>
	device_status_unknown = 0,

	/// <summary>
	/// 停机
	/// </summary>
	device_status_stop = 1,

	/// <summary>
	/// 运行
	/// </summary>
	device_status_run = 2,

	/// <summary>
	/// 空闲
	/// </summary>
	device_status_idle = 3,

	/// <summary>
	/// 离线
	/// </summary>
	device_status_offline = 4,

	/// <summary>
	/// 调试
	/// </summary>
	device_status_debug = 5,
};

enum enum_op_mode
{
	/// <summary>
	/// MDI
	/// </summary>
	op_mode_mdi = 0,

	/// <summary>
	/// 自动
	/// </summary>
	op_mode_auto = 1,

	/// <summary>
	/// None
	/// </summary>
	op_mode_none = 2,

	/// <summary>
	/// 编辑
	/// </summary>
	op_mode_edit = 3,

	/// <summary>
	/// 手轮
	/// </summary>
	op_mode_handle = 4,

	/// <summary>
	/// 手动
	/// </summary>
	op_mode_jog = 5,

	/// <summary>
	/// 手动示教
	/// </summary>
	op_mode_teachin_jog = 6,

	/// <summary>
	/// 手轮示教
	/// </summary>
	op_mode_teachin_handle = 7,

	/// <summary>
	/// 手动增量
	/// </summary>
	op_mode_incfeed = 8,

	/// <summary>
	/// 回零
	/// </summary>
	op_mode_reference = 9,

	/// <summary>
	/// 在线加工
	/// </summary>
	op_mode_remote = 10,

	/// <summary>
	/// 其他
	/// </summary>
	op_mode_other = 11
};

enum enum_run_status
{
	/// <summary>
	/// 复位(接通电源或终止程序执行，自动运行结束)
	/// </summary>
	run_status_reset = 0,

	/// <summary>
	/// 停止(自动运行)
	/// </summary>
	run_status_stop = 1,

	/// <summary>
	/// 暂停(自动运行)
	/// </summary>
	run_status_hold = 2,

	/// <summary>
	/// 启动(自动运行)
	/// </summary>
	run_status_start = 3,

	/// <summary>
	/// 手动数值指令启动
	/// </summary>
	run_status_mstr = 4,

	/// <summary>
	///
	/// </summary>
	run_status_other = 5
};

struct status_info
{
	/// <summary>
	/// 工作状态
	/// </summary>
	enum_device_std_status status;

	/// <summary>
	/// 操作模式
	/// </summary>
	enum_op_mode op_mode;

	/// <summary>
	/// 程序运行状态
	/// </summary>
	enum_run_status run_status;

	/// <summary>
	/// 急停信息
	/// </summary>
	bool emergency;

	/// <summary>
	/// 是否报警
	/// </summary>
	bool alarm;
};

struct alarm_message
{
	/// <summary>
	/// 报警编号
	/// </summary>
	long alarm_no;

	/// <summary>
	/// 报警类型
	/// </summary>
	short type;

	/// <summary>
	/// 报警轴号
	/// </summary>
	short axis;

	/// <summary>
	/// 报警信息
	/// </summary>
	std::string message;
};
