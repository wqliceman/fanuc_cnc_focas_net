#pragma once
#include <string>
#include <list>
#include <vector>
#include "models.h"

/// <summary>
/// ���ӻ���,����socket���
/// </summary>
/// <param name="ip">�豸IP��ַ</param>
/// <param name="port">�˿ڣ�Ĭ��8193</param>
/// <param name="handler">���ؾ��</param>
/// <returns></returns>
short get_alloc_handler(const char* ip, short port, unsigned short* handler);

/// <summary>
/// �ͷž��
/// </summary>
/// <param name="handler">���Ӿ��</param>
/// <returns></returns>
short free_alloc_handler(unsigned short handler);

/// <summary>
/// ��ȡϵͳ��Ϣ
/// </summary>
/// <param name="handler"></param>
/// <param name="info">ϵͳ��Ϣ</param>
/// <returns></returns>
short get_system_info(unsigned short handler, system_info& info);

/// <summary>
/// ��ȡ״̬��Ϣ
/// </summary>
/// <param name="handler"></param>
/// <param name="info">״̬��Ϣ</param>
/// <returns></returns>
short get_status_info(unsigned short handler, status_info& info);

/// <summary>
/// ��ȡ˽�������Ƽ�����
/// </summary>
/// <param name="handler"></param>
/// <param name="axis">������</param>
/// <returns></returns>
short get_axis_names(unsigned short handler, std::vector<std::string>& axis);

/// <summary>
/// ��ȡ�������Ƽ�����
/// </summary>
/// <param name="handler"></param>
/// <param name="axis">������</param>
/// <returns></returns>
short get_spindle_names(unsigned short handler, std::vector<std::string>& sp_axis);

/// <summary>
/// ��ȡ����λ��
/// </summary>
/// <param name="handler"></param>
/// <param name="type">��ȡ��������</param>
/// <param name="poss">��������</param>
/// <returns></returns>
short get_position_info(unsigned short handler, enum_pos_type type, std::vector<double>& poss);

/// <summary>
/// ��ȡ�ӹ�����������Ϣ
/// </summary>
/// <param name="handler"></param>
/// <param name="info">�ӹ�������Ϣ</param>
/// <returns></returns>
short get_program_info(unsigned short handler, program_info& info);

/// <summary>
/// ��ȡ�������
/// </summary>
/// <param name="handler"></param>
/// <param name="current_count">��ǰ����</param>
/// <param name="total_count">�ܲ���</param>
/// <param name="required_count">�������(��ע����)</param>
/// <returns></returns>
short get_part_counts(unsigned short handler, unsigned int* current_count, unsigned int* total_count, unsigned int* required_count);

/// <summary>
/// ��ȡ��������Ϣ
/// </summary>
/// <param name="handler"></param>
/// <param name="act_speed">����ʵ��ת��</param>
/// <param name="set_speed">�����趨ת��</param>
/// <param name="override">��������</param>
/// <returns></returns>
short get_feed_info(unsigned short handler, int* act_speed, int* set_speed, long* override);

/// <summary>
/// ��ȡ������Ϣ
/// </summary>
/// <param name="handler"></param>
/// <param name="act_speed">����ʵ��ת��</param>
/// <param name="set_speed">�����趨�ٶ�</param>
/// <param name="override">���ᱶ��</param>
/// <returns></returns>
short get_spindle_info(unsigned short handler, int* act_speed, int* set_speed, long* override);

/// <summary>
/// ��ȡ��ǰ������Ϣ
/// </summary>
/// <param name="handler"></param>
/// <param name="tool_num"></param>
/// <returns></returns>
short get_tool_info(unsigned short handler, short* tool_num);

/// <summary>
/// ��ȡ˽���Ḻ��
/// </summary>
/// <param name="handler"></param>
/// <param name="loads">˽����������</param>
/// <returns></returns>
short get_servo_axis_load(unsigned short handler, std::vector<double>& loads);

/// <summary>
/// ��ȡ���Ḻ��
/// </summary>
/// <param name="handler"></param>
/// <param name="loads">���Ḻ������</param>
/// <returns></returns>
short get_spindle_axis_load(unsigned short handler, std::vector<double>& loads);

/// <summary>
/// ����ʱ����Ϣ
/// ��λ �� ��
/// </summary>
/// <param name="handler"></param>
/// <param name="cycle_time">�ӹ�����</param>
/// <param name="total_cutting_time">������ʱ��</param>
/// <param name="op_time">������ʱ��</param>
/// <param name="total_power_on_time">�ϵ���ʱ��</param>
/// <returns></returns>
short get_time_info(unsigned short handler, long* cycle_time, long* total_cutting_time, long* op_time, long* total_power_on_time);

/// <summary>
/// ��ȡ������Ϣ
/// </summary>
/// <param name="handler"></param>
/// <param name="alarms">������Ϣ����</param>
/// <returns></returns>
short get_alarm_msg(unsigned short handler, std::vector<alarm_message>& alarms);

/// <summary>
/// ����˽�����¶�
/// </summary>
/// <param name="handler"></param>
/// <param name="temp"></param>
/// <returns></returns>
short get_servo_temperature(unsigned short handler, std::vector<double>& temp);

/// <summary>
/// �����������¶�
/// </summary>
/// <param name="handler"></param>
/// <param name="temp"></param>
/// <returns></returns>
short get_spindle_temperature(unsigned short handler, std::vector<double>& temp);

/// <summary>
/// ��ȡpmc��ַ��fanuc������plc��ַ
/// </summary>
/// <param name="handler"></param>
/// <param name="address"></param>
/// <returns></returns>
short read_pmc_data(unsigned short handler, std::string address);

/// <summary>
/// д��pmc��ַ
/// </summary>
/// <param name="handler"></param>
/// <param name="address"></param>
/// <returns></returns>
short write_pmc_data(unsigned short handler, std::string address);

/// <summary>
/// ��ȡ�����
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <returns></returns>
short read_macro_data(unsigned short handler, int number);

/// <summary>
/// д������
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <returns></returns>
short write_macro_data(unsigned short handler, int number);

/// <summary>
/// ��ȡ��������
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <returns></returns>
short read_paramter_data(unsigned short handler, int number);

/// <summary>
/// д���������
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <returns></returns>
short write_paramter_data(unsigned short handler, int number);

/// <summary>
/// ��ȡ�������
/// </summary>
/// <param name="handler"></param>
/// <param name="number"></param>
/// <param name="axis_no"></param>
/// <returns></returns>
short read_diagloas_data(unsigned short handler, int number, int axis_no);
