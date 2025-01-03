#include <iostream>
#include <string>
#include "fwlib32.h"
#include "focas_capi.h"

using namespace std;

#pragma comment(lib, "Fwlib32.lib")

void HexDump(char* buf, int len, int addr);

int main(int argc, char** argv)
{
	unsigned short h = 0;
	//string ip = "180.169.31.34";
	//short port = 18193;
	string ip = "192.168.20.101";
	short port = 8193;

	if (argc > 1)
	{
		ip = argv[1];
	}

	if (EW_OK == get_alloc_handler(ip.c_str(), port, &h))
	{
		std::cout << "============= get_system_info =============" << std::endl;
		system_info system_info;
		memset((void*)&system_info, 0, sizeof(system_info));
		get_system_info(h, system_info);
		std::cout << "mt type:\t" << system_info.mt_type << std::endl;
		std::cout << "cnc type:\t" << system_info.cnc_type << std::endl;
		std::cout << "series:\t" << system_info.series << std::endl;
		std::cout << "version:\t" << system_info.version << std::endl;

		std::cout << "============= get_status_info =============" << std::endl;
		status_info st_info ;
		memset((void*)&st_info, 0, sizeof(st_info));
		get_status_info(h, st_info);
		std::cout << "status:\t" << st_info.status << std::endl;
		std::cout << "op_mode:\t" << st_info.op_mode << std::endl;
		std::cout << "run_status:\t" << st_info.run_status << std::endl;
		std::cout << "emg:\t" << st_info.emergency << std::endl;
		std::cout << "alarm:\t" << st_info.alarm << std::endl;

		std::cout << "============= get_program_info =============" << std::endl;
		program_info prog_info;
		memset((void*)&prog_info, 0, sizeof(prog_info));
		get_program_info(h, prog_info);
		std::cout << "main prog:\t" << prog_info.main_prog << std::endl;
		std::cout << "main prog msg:\t" << prog_info.main_prog_msg << std::endl;
		std::cout << "current prog :\t" << prog_info.current_prog << std::endl;
		std::cout << "currnet prog msg:\t" << prog_info.current_prog_msg << std::endl;

		std::cout << "============= get_spindle_names =============" << std::endl;
		std::vector<string> vc_axis_name;
		get_spindle_names(h, vc_axis_name);
		std::cout << "spinle aixs name: ";
		for (size_t i = 0; i < vc_axis_name.size(); i++)
			std::cout << vc_axis_name[i] << ", ";
		std::cout << std::endl;

		std::cout << "============= get_axis_names =============" << std::endl;
		vc_axis_name.clear();
		get_axis_names(h, vc_axis_name);
		std::cout << "axis name: ";
		for (size_t i = 0; i < vc_axis_name.size(); i++)
			std::cout << vc_axis_name[i] << ", ";
		std::cout << std::endl;

		std::cout << "============= get_position_info =============" << std::endl;
		std::vector<double> vc_pos;
		get_position_info(h, pos_mach, vc_pos);
		std::cout << "axis mach pos: ";
		for (size_t i = 0; i < vc_pos.size(); i++)
			std::cout << vc_pos[i] << ", ";
		std::cout << std::endl;

		std::cout << "============= get_part_counts =============" << std::endl;
		unsigned int cur_count, total_count, req_count;
		get_part_counts(h, &cur_count, &total_count, &req_count);
		std::cout << "current counter:" << cur_count << std::endl;
		std::cout << "total counter:" << total_count << std::endl;
		std::cout << "require counter:" << req_count << std::endl;

		std::cout << "============= get_feed_info =============" << std::endl;
		int act_speed, set_speed;
		long override;
		get_feed_info(h, &act_speed, &set_speed, &override);
		std::cout << "act speed:" << act_speed << std::endl;
		std::cout << "set speed:" << set_speed << std::endl;
		std::cout << "overide speed:" << override << std::endl;

		std::cout << "============= get_spindle_info =============" << std::endl;
		get_spindle_info(h, &act_speed, &set_speed, &override);
		std::cout << "act speed:" << act_speed << std::endl;
		std::cout << "set speed:" << set_speed << std::endl;
		std::cout << "overide speed:" << override << std::endl;

		std::cout << "============= get_tool_info =============" << std::endl;
		short tool_num;
		get_tool_info(h, &tool_num);
		std::cout << "tool num:" << tool_num << std::endl;

		std::cout << "============= get_servo_axis_load =============" << std::endl;
		std::vector<double> vc_loads;
		get_servo_axis_load(h, vc_loads);
		std::cout << "servo axis load:";
		for (size_t i = 0; i < vc_loads.size(); i++)
			std::cout << vc_loads[i] << ", ";
		std::cout << std::endl;

		std::cout << "============= get_spindle_axis_load =============" << std::endl;
		vc_loads.clear();
		get_spindle_axis_load(h, vc_loads);
		std::cout << "spindle axis load:";
		for (size_t i = 0; i < vc_loads.size(); i++)
			std::cout << vc_loads[i] << ", ";
		std::cout << std::endl;

		std::cout << "============= get_servo_temperature =============" << std::endl;
		std::vector<double> vc_temp;
		get_servo_temperature(h, vc_temp);
		std::cout << "servo axis temperature:";
		for (size_t i = 0; i < vc_temp.size(); i++)
			std::cout << vc_temp[i] << ", ";
		std::cout << std::endl;

		std::cout << "============= get_spindle_temperature =============" << std::endl;
		vc_temp.clear();
		get_spindle_temperature(h, vc_temp);
		std::cout << "spindle axis temperature:";
		for (size_t i = 0; i < vc_temp.size(); i++)
			std::cout << vc_temp[i] << ", ";
		std::cout << std::endl;

		std::cout << "============= get_time_info =============" << std::endl;
		long cycle, total_cutting, operate, total_power_on;
		get_time_info(h, &cycle, &total_cutting, &operate, &total_power_on);
		std::cout << "cycle :" << cycle << std::endl;
		std::cout << "total cutting:" << total_cutting << std::endl;
		std::cout << "op time:" << operate << std::endl;
		std::cout << "total power on:" << total_power_on << std::endl;

		std::cout << "============= get_alarm_msg =============" << std::endl;
		std::vector<alarm_message> vc_alarms;
		get_alarm_msg(h, vc_alarms);
		for (size_t i = 0; i < vc_alarms.size(); i++)
		{
			std::cout << "alarm" << i + 1 << std::endl;
			std::cout << "no: " << vc_alarms[i].alarm_no <<
				", type: " << vc_alarms[i].type <<
				", axis: " << vc_alarms[i].axis <<
				", msg: " << vc_alarms[i].message << std::endl;
		}

		free_alloc_handler(h);
	}

	getchar();

	return 0;
}