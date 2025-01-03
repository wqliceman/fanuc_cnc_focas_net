#pragma once
#include <string>

struct program_info
{
	std::string main_prog;       // �������
	std::string main_prog_msg;   // ������ע
	std::string current_prog;    // ��ǰ�����
	std::string current_prog_msg;// ��ǰ����ű�ע
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
	device_status_unknown = 0, // δ֪
	device_status_stop = 1,    // ͣ��
	device_status_run = 2,     // ����
	device_status_idle = 3,    // ����
	device_status_offline = 4, // ����
	device_status_debug = 5    // ����
};

enum enum_op_mode
{
	op_mode_mdi = 0,           // MDI
	op_mode_auto = 1,          // �Զ�
	op_mode_none = 2,          // None
	op_mode_edit = 3,          // �༭
	op_mode_handle = 4,        // ����
	op_mode_jog = 5,           // �ֶ�
	op_mode_teachin_jog = 6,   // �ֶ�ʾ��
	op_mode_teachin_handle = 7,// ����ʾ��
	op_mode_incfeed = 8,       // �ֶ�����
	op_mode_reference = 9,     // ����
	op_mode_remote = 10,       // ���߼ӹ�
	op_mode_other = 11         // ����
};

enum enum_run_status
{
	run_status_reset = 0,  // ��λ(��ͨ��Դ����ֹ����ִ�У��Զ����н���)
	run_status_stop = 1,   // ֹͣ(�Զ�����)
	run_status_hold = 2,   // ��ͣ(�Զ�����)
	run_status_start = 3,  // ����(�Զ�����)
	run_status_mstr = 4,   // �ֶ���ֵָ������
	run_status_other = 5   // ����
};

struct status_info
{
	enum_device_std_status status; // ����״̬
	enum_op_mode op_mode;          // ����ģʽ
	enum_run_status run_status;    // ��������״̬
	bool emergency;                // ��ͣ��Ϣ
	bool alarm;                    // �Ƿ񱨾�
};

struct alarm_message
{
	long alarm_no;       // �������
	short type;          // ��������
	short axis;          // �������
	std::string message; // ������Ϣ
};
