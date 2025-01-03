# FANUC CNC 支持项目

## 1. 支持型号

本项目支持发那科全系列设备（某些型号可能需要确认参数，实际遇到问题时再处理），包括但不限于以下型号：

- FANUC Series 0i-MODEL A
- FANUC Series 0i-MODEL B
- FANUC Series 0i-MODEL C Note1)
- FANUC Series 0i-MODEL D
- FANUC Series 0i Mate-MODEL D
- FANUC Series 0i-MODEL F
- FANUC Series 0i Mate-MODEL F
- FANUC Series 0i-PD
- FANUC Series 0i-PF
- FANUC Series 15/150-MODEL B
- FANUC Series 15i/150i-MODEL A
- FANUC Series 15i/150i-MODEL B
- FANUC Series 16/160-MODEL B
- FANUC Series 16/160-MODEL C
- FANUC Series 18/180-MODEL B
- FANUC Series 18/180-MODEL C
- FANUC Series 21/210-MODEL B
- FANUC Series 16/160i-MODEL A
- FANUC Series 18i/180i-MODEL A
- FANUC Series 21i/210i-MODEL A
- FANUC Series 16i/160i-MODEL B
- FANUC Series 18i/180i-MODEL B
- FANUC Series 21i/210i-MODEL B
- FANUC Series 16i/160i-P
- FANUC Series 18i/180i-P
- FANUC Series 16i/160i-L
- FANUC Series 16i/160i-W
- FANUC Series 18i/180i-W
- FANUC Series 30i-MODEL A
- FANUC Series 31i-MODEL A
- FANUC Series 32i-MODEL A
- FANUC Series 30i-MODEL B
- FANUC Series 31i-MODEL B
- FANUC Series 32i-MODEL B
- FANUC Series 35i-MODEL B
- FANUC Series 30i-P MODEL B
- FANUC Series 31i-P MODEL B
- FANUC Series 30i-L MODEL B
- FANUC Series 31i-L MODEL B
- FANUC Series 31i-W MODEL A
- FANUC Series 31i-W MODEL B
- FANUC Power Mate i-MODEL H
- FANUC Power Mate i-MODEL D
- FANUC Power Motion i-MODEL A

## 2. 使用协议

本项目使用发那科提供的 FOCAS 协议，理论上支持所有带网口的发那科设备。个别点位读取可能存在差异，详细信息请参阅 **docs** 目录下的相关文档，使用 **IE** 浏览器打开显示效果最佳。

目前协议支持以下平台：

- Win32
- Win64
- Linux Ubuntu
- ARM

详见 **libs** 目录。

## 3. 采集标准参数点

具体采集标准参数点请参阅 **docs** 目录下的 Excel 文件 _支持采集项详细说明.xlsx_。

## 4. 样例

```cpp
#include <iostream>
#include <string>
#include <vector>
#include "fwlib32.h"
#include "focas_capi.h"

using namespace std;

#pragma comment(lib, "Fwlib32.lib")

void HexDump(char* buf, int len, int addr);

void print_vector(const std::vector<string>& vec, const std::string& label) {
    std::cout << label;
    for (const auto& item : vec)
        std::cout << item << ", ";
    std::cout << std::endl;
}

void print_vector(const std::vector<double>& vec, const std::string& label) {
    std::cout << label;
    for (const auto& item : vec)
        std::cout << item << ", ";
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    unsigned short h = 0;
    string ip = "192.168.123.131";
    short port = 8193;

    if (argc > 1) {
        ip = argv[1];
    }

    if (EW_OK == get_alloc_handler(ip.c_str(), port, &h)) {
        std::cout << "============= get_system_info =============" << std::endl;
        system_info system_info;
        memset((void*)&system_info, 0, sizeof(system_info));
        get_system_info(h, system_info);
        std::cout << "mt type:\t" << system_info.mt_type << std::endl;
        std::cout << "cnc type:\t" << system_info.cnc_type << std::endl;
        std::cout << "series:\t" << system_info.series << std::endl;
        std::cout << "version:\t" << system_info.version << std::endl;

        std::cout << "============= get_status_info =============" << std::endl;
        status_info st_info;
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
        print_vector(vc_axis_name, "spinle aixs name: ");

        std::cout << "============= get_axis_names =============" << std::endl;
        vc_axis_name.clear();
        get_axis_names(h, vc_axis_name);
        print_vector(vc_axis_name, "axis name: ");

        std::cout << "============= get_position_info =============" << std::endl;
        std::vector<double> vc_pos;
        get_position_info(h, pos_mach, vc_pos);
        print_vector(vc_pos, "axis mach pos: ");

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
        print_vector(vc_loads, "servo axis load:");

        std::cout << "============= get_spindle_axis_load =============" << std::endl;
        vc_loads.clear();
        get_spindle_axis_load(h, vc_loads);
        print_vector(vc_loads, "spindle axis load:");

        std::cout << "============= get_servo_temperature =============" << std::endl;
        std::vector<double> vc_temp;
        get_servo_temperature(h, vc_temp);
        print_vector(vc_temp, "servo axis temperature:");

        std::cout << "============= get_spindle_temperature =============" << std::endl;
        vc_temp.clear();
        get_spindle_temperature(h, vc_temp);
        print_vector(vc_temp, "spindle axis temperature:");

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
        for (size_t i = 0; i < vc_alarms.size(); i++) {
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
```

## 5. 贡献

欢迎提交问题和贡献代码！请确保在提交 PR 之前阅读并遵循我们的贡献指南。

## 6. 许可证

本项目基于 MIT 许可证开源，详细信息请参阅 LICENSE 文件。
