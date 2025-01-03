#pragma once
#include <string>
#include <list>
#include <vector>
#include "models.h"

/// <summary>
/// 连接机床,分配socket句柄
/// </summary>
/// <param name="ip">设备IP地址</param>
/// <param name="port">端口，默认8193</param>
/// <param name="handler">返回句柄</param>
/// <returns></returns>
short get_alloc_handler(const char* ip, short port, unsigned short* handler);

/// <summary>
/// 释放句柄
/// </summary>
/// <param name="handler">连接句柄</param>
/// <returns></returns>
short free_alloc_handler(unsigned short handler);

/// <summary>
/// 获取系统信息
/// </summary>
/// <param name="handler"></param>
/// <param name="info">系统信息</param>
/// <returns></returns>
short get_system_info(unsigned short handler, system_info& info);

/// <summary>
/// 获取状态信息
/// </summary>
/// <param name="handler"></param>
/// <param name="info">状态信息</param>
/// <returns></returns>
short get_status_info(unsigned short handler, status_info& info);

/// <summary>
/// 获取私服轴名称及数量
/// </summary>
/// <param name="handler"></param>
/// <param name="axis">轴数组</param>
/// <returns></returns>
short get_axis_names(unsigned short handler, std::vector<std::string>& axis);

/// <summary>
/// 获取主轴名称及数量
/// </summary>
/// <param name="handler"></param>
/// <param name="axis">轴数组</param>
/// <returns></returns>
short get_spindle_names(unsigned short handler, std::vector<std::string>& sp_axis);

/// <summary>
/// 获取坐标位置
/// </summary>
/// <param name="handler"></param>
/// <param name="type">读取坐标类型</param>
/// <param name="poss">坐标数组</param>
/// <returns></returns>
short get_position_info(unsigned short handler, enum_pos_type type, std::vector<double>& poss);

/// <summary>
/// 获取加工程序号相关信息
/// </summary>
/// <param name="handler"></param>
/// <param name="info">加工程序信息</param>
/// <returns></returns>
short get_program_info(unsigned short handler, program_info& info);

/// <summary>
/// 获取产量相关
/// </summary>
/// <param name="handler"></param>
/// <param name="current_count">当前产量</param>
/// <param name="total_count">总产量</param>
/// <param name="required_count">需求产量(关注较少)</param>
/// <returns></returns>
short get_part_counts(unsigned short handler, unsigned int* current_count, unsigned int* total_count, unsigned int* required_count);

/// <summary>
/// 获取进给轴信息
/// </summary>
/// <param name="handler"></param>
/// <param name="act_speed">进给实际转速</param>
/// <param name="set_speed">进给设定转速</param>
/// <param name="override">切削倍率</param>
/// <returns></returns>
short get_feed_info(unsigned short handler, int* act_speed, int* set_speed, long* override);

/// <summary>
/// 获取主轴信息
/// </summary>
/// <param name="handler"></param>
/// <param name="act_speed">主轴实际转速</param>
/// <param name="set_speed">主轴设定速度</param>
/// <param name="override">主轴倍率</param>
/// <returns></returns>
short get_spindle_info(unsigned short handler, int* act_speed, int* set_speed, long* override);

/// <summary>
/// 获取当前刀具信息
/// </summary>
/// <param name="handler"></param>
/// <param name="tool_num"></param>
/// <returns></returns>
short get_tool_info(unsigned short handler, short* tool_num);

/// <summary>
/// 获取私服轴负载
/// </summary>
/// <param name="handler"></param>
/// <param name="loads">私服负载数组</param>
/// <returns></returns>
short get_servo_axis_load(unsigned short handler, std::vector<double>& loads);

/// <summary>
/// 获取主轴负载
/// </summary>
/// <param name="handler"></param>
/// <param name="loads">主轴负载数组</param>
/// <returns></returns>
short get_spindle_axis_load(unsigned short handler, std::vector<double>& loads);

/// <summary>
/// 返回时间信息
/// 单位 ： 秒
/// </summary>
/// <param name="handler"></param>
/// <param name="cycle_time">加工节拍</param>
/// <param name="total_cutting_time">切削总时间</param>
/// <param name="op_time">操作总时间</param>
/// <param name="total_power_on_time">上电总时间</param>
/// <returns></returns>
short get_time_info(unsigned short handler, long* cycle_time, long* total_cutting_time, long* op_time, long* total_power_on_time);

/// <summary>
/// 获取报警信息
/// </summary>
/// <param name="handler"></param>
/// <param name="alarms">报警信息数组</param>
/// <returns></returns>
short get_alarm_msg(unsigned short handler, std::vector<alarm_message>& alarms);

/// <summary>
/// 返回私服轴温度
/// </summary>
/// <param name="handler"></param>
/// <param name="temp"></param>
/// <returns></returns>
short get_servo_temperature(unsigned short handler, std::vector<double>& temp);

/// <summary>
/// 返回主轴轴温度
/// </summary>
/// <param name="handler"></param>
/// <param name="temp"></param>
/// <returns></returns>
short get_spindle_temperature(unsigned short handler, std::vector<double>& temp);

/// <summary>
/// 读取pmc地址，fanuc机床中plc地址
/// </summary>
/// <param name="handler"></param>
/// <param name="address"></param>
/// <returns></returns>
short read_pmc_data(unsigned short handler, std::string address);

/// <summary>
/// 写入pmc地址
/// </summary>
/// <param name="handler"></param>
/// <param name="address"></param>
/// <returns></returns>
short write_pmc_data(unsigned short handler, std::string address);

/// <summary>
/// 读取宏变量
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <returns></returns>
short read_macro_data(unsigned short handler, int number);

/// <summary>
/// 写入宏变量
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <returns></returns>
short write_macro_data(unsigned short handler, int number);

/// <summary>
/// 读取参数变量
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <returns></returns>
short read_paramter_data(unsigned short handler, int number);

/// <summary>
/// 写入参数变量
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <returns></returns>
short write_paramter_data(unsigned short handler, int number);

/// <summary>
/// 读取诊断数据
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <param name="axis_no"></param>
/// <returns></returns>
short read_diagloas_data(unsigned short handler, int number, int axis_no);
