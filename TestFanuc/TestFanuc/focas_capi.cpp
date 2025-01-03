#include "focas_capi.h"
#include "Fwlib32.h"
#include "common.h"

using namespace std;

/// <summary>
/// 连接机床,分配socket句柄
/// </summary>
/// <param name="ip"></param>
/// <param name="port"></param>
/// <param name="handler"></param>
/// <returns></returns>
short get_alloc_handler(const char* ip, short port, unsigned short* handler)
{
	if (ip == NULL || port <= 0)
		return -1;

	if (strlen(ip) <= 0)
		return -1;

	return cnc_allclibhndl3(ip, port, 5, handler);
}

/// <summary>
/// 释放句柄
/// </summary>
/// <param name="handler"></param>
/// <returns></returns>
short free_alloc_handler(unsigned short handler)
{
	if (handler <= 0)
		return -1;

	return cnc_freelibhndl(handler);
}

/// <summary>
/// 获取系统信息
/// </summary>
/// <param name="handler"></param>
/// <param name="info"></param>
/// <returns></returns>
short get_system_info(unsigned short handler, system_info& info)
{
	if (handler <= 0)
		return -1;

	ODBSYS	sysinfo = { 0 };
	short ret = cnc_sysinfo(handler, &sysinfo);
	if (EW_OK == ret)
	{
		const int STR_MAX = 32;
		char cnc_type[STR_MAX] = { 0 };
		char mt_type[STR_MAX] = { 0 };
		char series[STR_MAX] = { 0 };
		char version[STR_MAX] = { 0 };
		char axis_number[STR_MAX] = { 0 };

		memcpy(cnc_type, sysinfo.cnc_type, 2 * sizeof(char));
		memcpy(mt_type, sysinfo.mt_type, 2 * sizeof(char));
		memcpy(series, sysinfo.series, 4 * sizeof(char));
		memcpy(version, sysinfo.version, 4 * sizeof(char));
		memcpy(axis_number, sysinfo.axes, 2 * sizeof(char));

		info.cnc_type = string(cnc_type);
		info.mt_type = string(mt_type);
		info.series = string(series);
		info.version = string(version);
		info.axis_num = string(axis_number);
		info.max_axis = sysinfo.max_axis;
	}

	return ret;
}

/// <summary>
/// 获取轴名称及数量
/// </summary>
/// <param name="handler"></param>
/// <param name="axis"></param>
/// <returns></returns>
short get_axis_names(unsigned short handler, std::vector<string>& axis)
{
	if (handler <= 0)
		return -1;

	short axis_count = MAX_AXIS;
	ODBAXISNAME odbaxisname[MAX_AXIS];
	short ret = cnc_rdaxisname(handler, &axis_count, odbaxisname);
	if (ret == EW_OK)
	{
		for (int i = 0; i < axis_count; i++)
		{
			char temp[10] = { 0 };
			sprintf_s(temp, "%c", odbaxisname[i].name);
			axis.push_back(temp);
		}
	}

	return ret;
}

short get_spindle_names(unsigned short handler, std::vector<std::string>& sp_axis)
{
	if (handler <= 0)
		return -1;

	short axis_count = MAX_AXIS;
	ODBSPDLNAME odbaxisname[MAX_AXIS];
	short ret = cnc_rdspdlname(handler, &axis_count, odbaxisname);
	if (ret == EW_OK)
	{
		for (int i = 0; i < axis_count; i++)
		{
			char temp[10] = { 0 };
			sprintf_s(temp, "%c%c", odbaxisname[i].name, odbaxisname[i].suff1);
			sp_axis.push_back(temp);
		}
	}
	return ret;
}

/// <summary>
/// 获取坐标位置
/// </summary>
/// <param name="handler"></param>
/// <param name="type"></param>
/// <param name="poss"></param>
/// <returns></returns>
short get_position_info(unsigned short handler, enum_pos_type type, std::vector<double>& poss)
{
	if (handler <= 0)
		return -1;

	ODBPOS odbpos[MAX_AXIS];
	short num = MAX_AXIS;
	short ret = cnc_rdposition(handler, (short)type, &num, odbpos);
	if (ret == EW_OK)
	{
		for (int i = 0; i < num; i++)
		{
			POSELM temp = odbpos[i].abs;
			switch (type)
			{
			case pos_mach:
				temp = odbpos[i].mach;
				break;
			case pos_rel:
				temp = odbpos[i].rel;
				break;
			case pos_dist:
				temp = odbpos[i].dist;
				break;
			default:
			case pos_abs:
				temp = odbpos[i].abs;
				break;
			}

			double temp_pos = temp.data / pow(10.0, temp.dec);
			poss.push_back(temp_pos);
		}
	}

	return 0;
}

/// <summary>
/// 获取状态信息
/// </summary>
/// <param name="handler"></param>
/// <param name="info"></param>
/// <returns></returns>
short get_status_info(unsigned short handler, status_info& info)
{
	if (handler <= 0)
		return -1;

	ODBST odbst = { 0 };
	short ret = cnc_statinfo(handler, &odbst);
	if (ret == EW_OK)
	{
		system_info sys_info;
		get_system_info(handler, sys_info);
		info.emergency = odbst.emergency == 1;

		bool temp_alarm = false;
		if (odbst.alarm == 1 || info.emergency)
		{
			temp_alarm = true;

			if (odbst.alarm == 1)
				info.alarm = true;
		}

		trim(sys_info.mt_type);
		trim(sys_info.cnc_type);

		if (sys_info.mt_type == "W" && (sys_info.cnc_type == "16" || sys_info.cnc_type == "18")) //线切割
		{
			switch (odbst.aut)
			{
			case 0: // MDI
				info.op_mode = op_mode_mdi;
				break;

			case 1: // MEM
				info.op_mode = op_mode_auto;
				break;

			case 2:
				info.op_mode = op_mode_none;
				break;

			case 3: // EDT
				info.op_mode = op_mode_edit;
				break;

			case 4: // HAND
				info.op_mode = op_mode_handle;
				break;

			case 5: // JOG
				info.op_mode = op_mode_jog;
				break;

			case 10: // TAPE
				info.op_mode = op_mode_remote;
				break;

			default:
				info.op_mode = op_mode_other;
				break;
			}

			switch (odbst.run)
			{
			case 0: // NOT READ
			case 1: // M-READY
				info.run_status = run_status_reset;
				break;

			case 2: // C-START
				info.run_status = run_status_start;
				break;

			case 3: // F-HOLD
				info.run_status = run_status_hold;
				break;

			case 4: // B-STOP
				info.run_status = run_status_stop;
				break;

			default:
				info.run_status = run_status_other;
				break;
			}

			if (odbst.aut == 1) // MEM
				info.status = (odbst.run == 2) ? device_status_run : device_status_idle;
			else if (odbst.aut == 0 || odbst.aut == 3 || odbst.aut == 4 || odbst.aut == 5 || odbst.aut == 10)
				info.status = device_status_debug;
			else
				info.status = device_status_idle;
		}
		else
		{
			switch (odbst.aut)
			{
			case 0: // MDI
				info.op_mode = op_mode_mdi;
				break;

			case 1: // MEMory
				info.op_mode = op_mode_auto;
				break;

			case 2:
				info.op_mode = op_mode_none;
				break;

			case 3: // EDIT
				info.op_mode = op_mode_edit;
				break;

			case 4: // HaNDle
				info.op_mode = op_mode_handle;
				break;

			case 7: // Teach in HaNDle
				info.op_mode = op_mode_teachin_handle;
				break;

			case 5: // JOG
				info.op_mode = op_mode_jog;
				break;
			case 6: // Teach in JOG
				info.op_mode = op_mode_teachin_jog;
				break;

			case 8:
				info.op_mode = op_mode_incfeed;
				break;

			case 9:
				info.op_mode = op_mode_reference;
				break;

			case 10: // ReMoTe
				info.op_mode = op_mode_remote;
				break;

			default:
				info.op_mode = op_mode_other;
				break;
			}

			switch (odbst.run)
			{
			case 0: // ****(reset)
				info.run_status = run_status_reset;
				break;

			case 1: // STOP
				info.run_status = run_status_stop;
				break;

			case 2: // HOLD
				info.run_status = run_status_hold;
				break;

			case 3: // STaRT
				info.run_status = run_status_start;
				break;

			case 4: // MSTR
				info.run_status = run_status_mstr;
				break;

			default:
				info.run_status = run_status_other;
				break;
			}

			if (odbst.aut == 1 || odbst.aut == 2 || odbst.aut == 10)
				info.status = (odbst.run == 3) ? device_status_run : device_status_idle;
			else if (odbst.aut == 0 || odbst.aut == 3 || odbst.aut == 4 || odbst.aut == 5
				|| odbst.aut == 6 || odbst.aut == 7 || odbst.aut == 8 || odbst.aut == 9)
				info.status = device_status_debug;
			else
				info.status = device_status_idle;
		}
		if (temp_alarm)
			info.status = device_status_stop;
	}

	return 0;
}

string get_prog_num_comment(unsigned short handler, short oname)
{
	string comment = "";
	PRGDIR prgdir = { 0 };
	if (cnc_rdprogdir(handler, 1, oname, oname, 256, &prgdir) == EW_OK)
	{
		string temp_buffer = std::string(prgdir.prg_data);
		if (!temp_buffer.empty())
			trim(temp_buffer);

		if (!temp_buffer.empty() && temp_buffer.length() >= 2)
		{
			size_t first = temp_buffer.find_first_of('(');
			size_t last = temp_buffer.find_first_of(')');
			if (first != -1 && last != -1)
				temp_buffer = temp_buffer.substr(first + 1, last - first - 1);
		}
		comment = temp_buffer;
	}

	return comment;
}

string get_prog_name_comment(unsigned short handler, string full_name)
{
	string comment = "";
	char buffer[MAX_PATH] = { 0 };
	unsigned long line_len = 1;
	unsigned long data_len = MAX_PATH;
	if (cnc_rdpdf_line(handler, (char*)full_name.c_str(), 0, buffer, &line_len, &data_len) == EW_OK)
	{
		string temp_buffer = string(buffer);
		trim(temp_buffer);
		if (!temp_buffer.empty())
		{
			size_t first = temp_buffer.find_first_of('(');
			size_t last = temp_buffer.find_first_of(')');
			if (first != -1 && last != -1)
				comment = temp_buffer.substr(first + 1, last - first - 1);
		}
	}

	return comment;
}

/// <summary>
/// 获取加工程序号相关信息
/// </summary>
/// <param name="handler"></param>
/// <param name="info"></param>
/// <returns></returns>
short get_program_info(unsigned short handler, program_info& info)
{
	if (handler <= 0)
		return -1;

	bool is_ok = false;

	// 优先查询数字程序号
	ODBPRO odbpro = { 0 };
	short ret = cnc_rdprgnum(handler, &odbpro);
	if (ret == EW_OK)
	{
		if (0 != odbpro.mdata)
		{
			info.main_prog = std::to_string(odbpro.mdata);
			info.current_prog = std::to_string(odbpro.data);

			info.main_prog_msg = get_prog_num_comment(handler, odbpro.mdata);
			info.current_prog_msg = get_prog_num_comment(handler, odbpro.data);

			is_ok = true;
		}
	}

	// 如果数字程序号查询失败，就换成字符串程序号查询
	if (!is_ok || info.main_prog.empty() || info.main_prog == "0")
	{
		char prog_path[MAX_PATH] = { 0 };
		ret = cnc_exeprgname2(handler, prog_path);
		if (ret == EW_OK)
		{
			string full_name(prog_path);
			string file_path = full_name;
			string file_name = full_name;
			size_t pos = full_name.find_last_of('/');
			if (pos != -1)
			{
				file_path = full_name.substr(0, pos + 1);
				file_name = full_name.substr(pos + 1, full_name.size() - pos);
			}

			info.main_prog = info.current_prog = file_name;
			if (!info.current_prog.empty())
			{
				info.main_prog_msg = info.current_prog_msg = get_prog_name_comment(handler, full_name);
				is_ok = true;
			}
		}
	}

	return ret;
}

/// <summary>
/// 获取产量相关
/// </summary>
/// <param name="handler"></param>
/// <param name="current_count"></param>
/// <param name="total_count"></param>
/// <param name="required_count"></param>
/// <returns></returns>
short get_part_counts(unsigned short handler, unsigned int* current_count, unsigned int* total_count, unsigned int* required_count)
{
	if (handler <= 0)
		return -1;

	short address = 6711;

	IODBPSD param = { 0 };
	short ret = cnc_rdparam(handler, address, 0, 8, &param);
	if (EW_OK == ret)
	{
		*current_count = param.u.ldata;

		address = 6712;
		ret = cnc_rdparam(handler, address, 0, 8, &param);
		if (EW_OK == ret)
			*total_count = param.u.ldata;

		address = 6713;
		ret = cnc_rdparam(handler, address, 0, 8, &param);
		if (EW_OK == ret)
			*required_count = param.u.ldata;
	}

	return ret;
}

/// <summary>
/// 获取进给轴信息
/// </summary>
/// <param name="handler"></param>
/// <param name="act_speed"></param>
/// <param name="set_speed"></param>
/// <param name="override"></param>
/// <returns></returns>
short get_feed_info(unsigned short handler, int* act_speed, int* set_speed, long* override)
{
	if (handler <= 0)
		return -1;

	//读取进给速速
	ODBACT actf = { 0 };
	short ret = cnc_actf(handler, &actf);
	if (EW_OK == ret)
		*act_speed = actf.data;

	// 进给设定值：宏变量  4109   （配置：M4109）
	ODBM odbm;
	ret = cnc_rdmacro(handler, 4109, sizeof(odbm), &odbm);
	if (EW_OK == ret)
	{
		*set_speed = (long)(odbm.mcr_val / pow(10.0, odbm.dec_val));
	}

	//读取进给倍率参数No.3002#4 判断进给倍率信号正负逻辑
	bool overrideLogic = false;
	IODBPSD param = { 0 };
	ret = cnc_rdparam(handler, 3002, 0, 8, &param);
	if (EW_OK == ret)
	{
		char bit_s = 0x01 << 4;
		if ((param.u.cdata & bit_s) == 0)
			overrideLogic = false;
		else
			overrideLogic = true;
	}

	//读取进给倍率信号G12信号
	IODBPMC buf = { 0 };
	ret = pmc_rdpmcrng(handler, 0, 0, 12, 12, 9, &buf);
	if (EW_OK == ret)
	{
		if (overrideLogic == false)//3002#4==0
			*override = (long)(255 - (unsigned char)buf.u.cdata[0]);
		else
			*override = (long)(unsigned char)(buf.u.cdata[0]);
	}

	return ret;
}

/// <summary>
/// 获取主轴信息
/// </summary>
/// <param name="handler"></param>
/// <param name="act_speed"></param>
/// <param name="set_speed"></param>
/// <param name="override"></param>
/// <returns></returns>
short get_spindle_info(unsigned short handler, int* act_speed, int* set_speed, long* override)
{
	if (handler <= 0)
		return -1;

	ODBACT buf = { 0 };
	short ret = cnc_acts(handler, &buf);
	if (EW_OK == ret)
	{
		*act_speed = buf.data;
	}

	// 主轴转速设定值：宏变量 4119  （配置：M4119）
	ODBM odbm = { 0 };
	ret = cnc_rdmacro(handler, 4119, sizeof(odbm), &odbm);
	if (EW_OK == ret)
	{
		*set_speed = (long)(odbm.mcr_val / pow(10.0, odbm.dec_val));
	}

	//读取主轴倍率信号G30信号
	IODBPMC bufpmc = { 0 };
	ret = pmc_rdpmcrng(handler, 0, 0, 30, 30, 9, &bufpmc);
	if (EW_OK == ret)
	{
		*override = (long)(bufpmc.u.cdata[0]);
	}

	return ret;
}

/// <summary>
/// 获取当前刀具信息
/// </summary>
/// <param name="handler"></param>
/// <param name="tool_num"></param>
/// <returns></returns>
short get_tool_info(unsigned short handler, short* tool_num)
{
	if (handler <= 0)
		return -1;

	// 刀号为宏变量4120
	ODBM odbm;
	short ret = cnc_rdmacro(handler, 4120, sizeof(odbm), &odbm);
	if (EW_OK == ret)
	{
		*tool_num = (short)(odbm.mcr_val / pow(10.0, odbm.dec_val));
	}

#if false
	//读取F26~F29, 刀号默认F26
	IODBPMC f_signal = { 0 };
	short ret = pmc_rdpmcrng(handler, 1, 2, 26, 29, 12, &f_signal);
	if (EW_OK == ret)
		*tool_num = (int)f_signal.u.ldata[0];
#endif

	return ret;
}

/// <summary>
/// 获取私服轴负载
/// </summary>
/// <param name="handler"></param>
/// <param name="loads"></param>
/// <returns></returns>
short get_servo_axis_load(unsigned short handler, std::vector<double>& loads)
{
	if (handler <= 0)
		return -1;

	ODBSVLOAD odbsvload[MAX_AXIS];
	short num = MAX_AXIS;
	short ret = cnc_rdsvmeter(handler, &num, odbsvload);
	if (EW_OK == ret)
	{
		for (int i = 0; i < num; i++)
		{
			double temp = odbsvload[i].svload.data / pow(10.0, odbsvload[i].svload.dec);
			loads.push_back(temp);
		}
	}

	return ret;
}

/// <summary>
/// 获取主轴负载
/// </summary>
/// <param name="handler"></param>
/// <param name="loads"></param>
/// <returns></returns>
short get_spindle_axis_load(unsigned short handler, std::vector<double>& loads)
{
	if (handler <= 0)
		return -1;

	ODBSPLOAD odbspload[MAX_AXIS];
	short num = MAX_AXIS;
	short ret = cnc_rdspmeter(handler, 0, &num, odbspload);
	if (EW_OK == ret)
	{
		for (int i = 0; i < num; i++)
		{
			double temp = odbspload[i].spload.data / pow(10.0, odbspload[i].spload.dec);
			loads.push_back(temp);
		}
	}
	return 0;
}

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
short get_time_info(unsigned short handler, long* cycle_time, long* total_cutting_time, long* op_time, long* total_power_on_time)
{
	if (handler <= 0)
		return -1;

	// 参数6750 上电总时间
	IODBPSD param = { 0 };
	short ret = cnc_rdparam(handler, 6750, 0, 8, &param);
	if (EW_OK == ret)
		*total_power_on_time = param.u.ldata;

	// 操作时间 6751 毫秒  6752 分钟
	ret = cnc_rdparam(handler, 6751, 0, 8, &param);
	if (EW_OK == ret)
	{
		long secondes = param.u.ldata / 1000.0; // 转换成秒
		cnc_rdparam(handler, 6752, 0, 8, &param);
		*op_time = secondes + (param.u.ldata * 60);
	}

	// 切削总时间 6753 毫秒  6754 分钟
	ret = cnc_rdparam(handler, 6753, 0, 8, &param);
	if (EW_OK == ret)
	{
		long secondes = param.u.ldata / 1000.0; // 转换成秒
		cnc_rdparam(handler, 6754, 0, 8, &param);
		*total_cutting_time = secondes + (param.u.ldata * 60);
	}

	// 加工节拍 6757 毫秒 6758 分钟
	ret = cnc_rdparam(handler, 6757, 0, 8, &param);
	if (EW_OK == ret)
	{
		long secondes = param.u.ldata / 1000.0; // 转换成秒
		cnc_rdparam(handler, 6758, 0, 8, &param);
		*cycle_time = secondes + (param.u.ldata * 60);
	}

	return 0;
}

/// <summary>
/// 获取报警信息
/// </summary>
/// <param name="handler"></param>
/// <param name="alarms"></param>
/// <returns></returns>
short get_alarm_msg(unsigned short handler, std::vector<alarm_message>& alarms)
{
	if (handler <= 0)
		return -1;

	const int MAX_ALARMS = 20;
	ODBALMMSG alammsg[MAX_ALARMS];
	short num = MAX_ALARMS;
	short ret = cnc_rdalmmsg(handler, -1, &num, alammsg);
	if (EW_OK == ret)
	{
		for (int i = 0; i < num; i++)
		{
			alarm_message temp;
			temp.alarm_no = alammsg->alm_no;
			temp.type = alammsg->type;
			temp.axis = alammsg->axis;
			temp.message = alammsg->alm_msg;
			alarms.push_back(temp);
		}
	}
	return 0;
}

short get_servo_temperature(unsigned short handler, std::vector<double>& temperature)
{
	if (handler <= 0)
		return -1;

	// 获取诊断数据 308
	ODBAXISNAME axisname[MAX_AXIS];
	short axis_count = MAX_AXIS;
	short ret = cnc_rdaxisname(handler, &axis_count, axisname);
	if (ret == EW_OK)
	{
		for (int i = 1; i <= axis_count; i++)
		{
			ODBDGN dgn;
			memset((void*)&dgn, 0, sizeof(dgn));
			ret = cnc_diagnoss(handler, 308, i, 8, &dgn);
			if (EW_OK == ret)
			{
				REALDGN temp = dgn.u.rdata;
				double value = temp.dec_val != 0 ? temp.dgn_val / pow(10.0, temp.dec_val) : temp.dgn_val;
				temperature.push_back(value);
			}
		}
	}

	return ret;
}

short get_spindle_temperature(unsigned short handler, std::vector<double>& temperature)
{
	if (handler <= 0)
		return -1;

	// 获取诊断数据 403
	ODBSPDLNAME spname[MAX_AXIS];
	short axis_count = MAX_AXIS;
	short ret = cnc_rdspdlname(handler, &axis_count, spname);
	if (ret == EW_OK)
	{
		for (int i = 1; i <= axis_count; i++)
		{
			ODBDGN dgn = { 0 };
			ret = cnc_diagnoss(handler, 403, i, 8, &dgn);
			if (EW_OK == ret)
			{
				REALDGN temp = dgn.u.rdata;
				double value = temp.dec_val != 0 ? temp.dgn_val / pow(10.0, temp.dec_val) : temp.dgn_val;
				temperature.push_back(value);
			}
		}
	}
	return 0;
}

short read_pmc_data(unsigned short handler, std::string address)
{
	if (handler <= 0)
		return -1;

	// short WINAPI pmc_rdpmcrng(unsigned short FlibHndl, short adr_type, short data_type, unsigned short s_number, unsigned short e_number, unsigned short length, IODBPMC *buf);
	return 0;
}

short write_pmc_data(unsigned short handler, std::string address)
{
	if (handler <= 0)
		return -1;
	// short WINAPI pmc_wrpmcrng(unsigned short FlibHndl, short length,IODBPMC *buf);
	return 0;
}

int GetKindOfAddress(char c)
{
	const char address_table[] = "GFYXARTKCDMNEZ";
	c = toupper(c);
	for (int i = 0; i < (sizeof(address_table) / sizeof(char)); ++i)
	{
		if (c == address_table[i])
			return i;
	}
	return 0;
}

BOOL WINAPI GetDevicePMCValue(
	unsigned short handler, 
	std::string address,
	char cdata_type,
	unsigned int count, 
	byte* buffer)
{
	if (address.length() <= 1)
		return FALSE;

	if (count<= 0)
		return FALSE;

	int addr_type = GetKindOfAddress(address[0]);
	int data_type = 0;
	if (cdata_type == 'B')
		data_type = 0;
	else if (cdata_type == 'W')
		data_type = 1;
	else if (cdata_type == 'D')
		data_type = 2;

	std::string index_address = address.substr(1, address.length()-1);

	int startAddr = atoi(index_address.c_str());
	int endAddr = startAddr;
	if (data_type == 0)
		endAddr = startAddr + count - 1;
	else if (data_type == 1)
		endAddr = startAddr + count * 2 - 1;
	else if (data_type == 2)
		endAddr = startAddr + count * 4 - 1;

	int cbBuffer = 0;
	if (data_type == 0)
		cbBuffer = 8 + count;
	else if (data_type == 1)
		cbBuffer = 8 + count * 2;
	else if (data_type == 2)
		cbBuffer = 8 + count * 4;

	IODBPMC* outPMC = (IODBPMC*)(new BYTE[cbBuffer]);
	ZeroMemory(outPMC, cbBuffer);
	int result = pmc_rdpmcrng(handler, addr_type, data_type, startAddr, endAddr, cbBuffer, outPMC);
	if (EW_OK == result)
		memcpy(buffer, outPMC->u.cdata, cbBuffer - 8);
	delete[] outPMC;
	return (EW_OK == result);
}

BOOL SetDevicePMCValue(
	unsigned short handler, 
	std::string address, 
	char cdata_type, 
	unsigned int count, 
	const byte* buffer)
{

	if (address.length() <= 1)
		return FALSE;

	if (count <= 0)
		return FALSE;

	int addr_type = GetKindOfAddress(address[0]);
	int data_type = 0;
	if (cdata_type == 'B')
		data_type = 0;
	else if (cdata_type == 'W')
		data_type = 1;
	else if (cdata_type == 'D')
		data_type = 2;

	std::string index_address = address.substr(1, address.length() - 1);

	int startAddr = atoi(index_address.c_str());
	int endAddr = startAddr + count - 1;

	int cbBuffer = 0;
	if (data_type == 0)
		cbBuffer = 8 + count;
	else if (data_type == 1)
		cbBuffer = 8 + count * 2;
	else if (data_type == 2)
		cbBuffer = 8 + count * 4;

	IODBPMC* inPMC = (IODBPMC*)(new BYTE[cbBuffer]);
	inPMC->datano_s = startAddr;
	inPMC->datano_e = endAddr;
	inPMC->type_a = addr_type;
	inPMC->type_d = data_type;
	memcpy(inPMC->u.cdata, buffer, cbBuffer - 8);

	int result = pmc_wrpmcrng(handler, cbBuffer, inPMC);
	delete[] inPMC;
	return (EW_OK == result);
}

short read_macro_data(unsigned short handler, int number)
{
	if (handler <= 0)
		return -1;

	// cnc_rdmacro
	// short WINAPI cnc_rdmacro(unsigned short FlibHndl, short number, short length, ODBM *macro);
	return 0;
}

short write_macro_data(unsigned short handler, int number)
{
	if (handler <= 0)
		return -1;

	// short WINAPI cnc_wrmacro(unsigned short FlibHndl, short number, short length, long mcr_val, short dec_val);
	return 0;
}

short read_paramter_data(unsigned short handler, int number)
{
	if (handler <= 0)
		return -1;

	// 参数 
	IODBPSD param = { 0 };
	//  short WINAPI cnc_rdparam(unsigned short FlibHndl, short number, short axis, short length, IODBPSD *param);
	short ret = cnc_rdparam(handler, number, 0, 8, &param);
	if (EW_OK == ret)
	{

	}
	return 0;
}

short write_paramter_data(unsigned short handler, int number)
{
	if (handler <= 0)
		return -1;

	//short WINAPI cnc_wrparam(unsigned short FlibHndl, short length, IODBPSD * param);

	return 0;
}

short read_diagloas_data(unsigned short handler, int number, int axis_no)
{
	if (handler <= 0)
		return -1;

	// 获取诊断数据 ,详细使用参加api
	ODBDGN dgn = { 0 };
	short	ret = cnc_diagnoss(handler, number, axis_no, 8, &dgn);
	if (EW_OK == ret)
	{
		REALDGN temp = dgn.u.rdata;
		double value = temp.dec_val != 0 ? temp.dgn_val / pow(10.0, temp.dec_val) : temp.dgn_val;
	}
	return 0;
}