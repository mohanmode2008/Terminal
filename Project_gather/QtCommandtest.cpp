#include "QtCommandtest.h"
#include <Qt>
#include <QtDebug>
#include "Project_gather.h"
#include <QListWidgetItem> 
#include <QtNetwork>
#include "QtWorkparameter.h"
#include <QtEndian> 

#pragma execution_character_set("utf-8")

#pragma pack (1)

QtCommandtest::QtCommandtest(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    qint8 i;

    row_in_cmd = 0;
    tcp_to_ground_ctl_handle = Q_NULLPTR;
  //  this->resize(400, 300);

    status = new QStatusBar(this);

    menu[0] = new QMenu("数管分系统");
    menu[0]->addAction("XWOBC");
    menu[0]->addAction("ZKOBC");
    menu[0]->addAction("AMPOBC");

    menu[1] = new QMenu("控制运算分系统");
    menu[1]->addAction("控制运算分系统");
   
    menu[2] = new QMenu("电源分系统");
    menu[2]->addAction("PCDU遥控指令");

    menu[3] = new QMenu("测控数传分系统");
    menu[3]->addAction("UXB测控数传一体机");
    menu[3]->addAction("VU通信机");
    menu[3]->addAction("X相控阵天线");

    menu[4] = new QMenu("姿控分系统");
    menu[4]->addAction("GNSS接收机A");
    menu[4]->addAction("GNSS接收机B遥控指令");
    menu[4]->addAction("数字太阳敏感器");
    menu[4]->addAction("星敏感器A");
    menu[4]->addAction("星敏感器B");
    menu[4]->addAction("星敏感器C");
    menu[4]->addAction("磁强计遥控指令");
    menu[4]->addAction("飞轮遥控指令");
    menu[4]->addAction("光纤陀螺遥控指令");
    menu[4]->addAction("电推力遥控指令");

    menu[5] = new QMenu("成像分系统");
    menu[5]->addAction("摄像机");
    menu[5]->addAction("红外摄像机");
    menu[5]->addAction("对焦机构A遥控指令");
    menu[5]->addAction("对焦机构B遥控指令");
    menu[5]->addAction("数据压缩处理单元");

    menu[6] = new QMenu("任务遥控指令");
    menu[6]->addAction("交互操作分系统");

    menu[7] = new QMenu("数据上注指令");
    menu[7]->addAction("数据上注指令");

    menu[8] = new QMenu("载荷设备");
    menu[8]->addAction("GNSS掩星观测单元遥控指令");
    menu[8]->addAction("DCS采集单元遥控指令");

    menu[9] = new QMenu("直接指令");
    menu[9]->addAction("直接指令");

    menuBar = new QMenuBar(this);
    menuBar->addMenu(menu[0]);
    menuBar->addMenu(menu[1]);
    menuBar->addMenu(menu[2]);
    menuBar->addMenu(menu[3]);
    menuBar->addMenu(menu[4]);
    menuBar->addMenu(menu[5]);
    menuBar->addMenu(menu[6]);
    menuBar->addMenu(menu[7]);
    menuBar->addMenu(menu[8]);
    menuBar->addMenu(menu[9]);
    menuBar->setGeometry(0, 0,830, 30);

    connect(menuBar, SIGNAL(triggered(QAction*)), this, SLOT(trigerMenu(QAction*)));
    connect(ui.btn_to_selectlist, SIGNAL(clicked()), this, SLOT(btn_list_to_selectlist()));
    connect(ui.btn_to_del_cmd, SIGNAL(clicked()), this, SLOT(btn_to_del_select()));
    connect(ui.btn_buildup, SIGNAL(clicked()), this, SLOT(btn_to_buildup_validdata()));
    connect(ui.listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(select_in_list_list()));
    connect(ui.listWidget_selectcmd, SIGNAL(itemSelectionChanged()), this, SLOT(select_in_list_select()));
    connect(ui.listWidget_build_frame, SIGNAL(itemSelectionChanged()), this, SLOT(select_in_list_frame()));
    connect(ui.btn_cmd_send, SIGNAL(clicked()), this, SLOT(btn_to_send_cmd()));

  //  connect(ui.btn_to_Identification, SIGNAL(clicked()), this, SLOT(tcp_send_identity_data_to_ground_station()));
  //  connect(ui.btn_to_station, SIGNAL(clicked()), this, SLOT(tcp_send_connect_data_to_ground_station()));
    connect(ui.btn_clr_cmd, SIGNAL(clicked()), this, SLOT(btn_clr_cmd()));
    connect(ui.btn_to_server, SIGNAL(clicked()), this, SLOT(tcp_connect_server_as_client()));
    connect(ui.btn_to_send_Identification, SIGNAL(clicked()), this, SLOT(tcp_send_identity_data_to_ctl_center()));
    connect(ui.btn_to_bind_station, SIGNAL(clicked()), this, SLOT(tcp_send_connect_data_to_ctl_center()));
    /*---------------*/

     telecontrol_frame_up.satellite_syn_byte[0] = 0x1D;
     telecontrol_frame_up.satellite_syn_byte[1] = 0x6F;
     telecontrol_frame_up.satellite_syn_byte[2] = 0xE9;
     telecontrol_frame_up.satellite_syn_byte[3] = 0x3A;
    
    for (i = 0; i < 4; i++)
    {
        telecontrol_frame_up.authorize_byte[i] = 0xff;
    }    
    telecontrol_frame_down.frame_end_byte = 0xD709;      
}

QtCommandtest::~QtCommandtest()
{

}

void QtCommandtest::connected_success()
{
    ui.plainTextEdit_reminder->appendPlainText("连接服务器成功");
}

void QtCommandtest::trigerMenu(QAction* act)
{
    QStringList strList_XWOBC;
    QStringList strList_ZKOBC;
    QStringList strList_AMPOBC;
    QStringList strList_control_operation;
    QStringList strList_power;
    QStringList strList_UXB;
    QStringList strList_UV;
    QStringList strList_X_antenna;
    QStringList strList_GNSSA;
    QStringList strList_GNSSB;
    QStringList strList_sun_sensor;
    QStringList strList_star_sensor_A;
    QStringList strList_star_sensor_B;
    QStringList strList_star_sensor_C;
    QStringList strList_magnetometer;
    QStringList strList_flywheel_cmd;
    QStringList strList_fiber_optic_gyro;
    QStringList strList_electric_thruster;
    QStringList strList_camera;
    QStringList strList_infrared_camera;
    QStringList strList_focusing_a;
    QStringList strList_focusing_b;
    QStringList strList_data_compression;
    QStringList strList_GNSS_occultation_obs;
    QStringList strList_DCS_acquisition;
    QStringList strList_interactive_operation;
    QStringList strList_data_annotation;
    QStringList strList_direct_cmd;

    QListWidgetItem* aItem;


    strList_XWOBC << "进入测试模式" << "清空延时指令" << "清空延时任务" << "星务计算机复位" << "程序注入开始" << "程序注入结束"
        <<"星务计算机遥测查询"<<"星务计算机日志下载"<<"星务计算机访问记录下载"<<"星务计算机短信息下载"<<"任务请求记录文件下载"<<"任务执行记录文件下载"<<"单机设备透传遥测查询"
        <<"PCDU遥测查询"<<"测控数传一体机遥测查询"<<"姿控分系统遥测查询"<<"GNSS接收机AB遥测查询"<<"工作模式切换"<<"ACK应答开关设置"<<"卫星上行同步码设置"<<"数传通道禁用与使能设置"
        <<"展开释放机构的禁止和使能"<<"自动热控的禁止和使能"<<"遥测通道的选择"<<"自动轨控的禁止和使能"<<"定时断电时间间隔设置"<<"实时广播遥测的开启和关闭"<<"实时广播遥测的间隔时间"
        <<"授权码上注开始" << "授权码上注结束" << "星上授权码清除" << "星务软件上注开始" << "星务软件上注结束" << "短消息上注" << "时间上注" << "轨道上注" << "设置最高充电电压阈值"
        << "设置最低放电电压阈值" << "设置单个测控通道地球站" << "设置单个数传通道地球站" << "设置多个测控通道地球站" 
        << "设置多个数传通道地球站" << "删除延时任务" << "删除延时指令" << "星务计算机软件版本切换" << "单机设备指令透传" << "音频数据广播开启"
        << "音频数据广播停止";

    strList_ZKOBC << "进入自主姿态控制" << "进入对日捕获与对日定向" << "进入对日定向三轴稳定" << "进入对地定向三轴稳定"
        << "星敏感器A屏蔽使用" << "星敏感器A不屏蔽使用" << "星敏感器B屏蔽使用" << "星敏感器B不屏蔽使用" << "数字太阳敏感器屏蔽使用"
        << "数字太阳敏感器不屏蔽使用" << "磁强计A屏蔽使用" << "磁强计A不屏蔽使用" << "磁强计B屏蔽使用" << "磁强计B不屏蔽使用"
        << "陀螺A屏蔽使用" << "陀螺A不屏蔽使用" << "陀螺B屏蔽使用" << "陀螺B不屏蔽使用" << "陀螺C屏蔽使用" << "陀螺C不屏蔽使用"
        << "设置角速度漂移估计标志：估计" << "设置角速度漂移估计标志：停止估计" << "设置角速度漂移估计复位标志：复位置零"
        << "设置角速度漂移估计复位标志：不复位" << "设置飞轮摩擦估计标志：估计" << "设置飞轮摩擦估计标志：计算"
        << "设置飞轮角动量估计标志：估计" << "设置飞轮角动量估计标志：计算" << "设置使用上注轨道确定标志：使用"
        << "设置使用上注轨道确定标志：不使用" << "常规模式陀螺积分定姿高优先级" << "常规模式双矢量定姿高优先级"
        << "星敏补偿（使用参考角速度）" << "星敏补偿（使用本体角速度）" << "星敏补偿（不补偿）" << "使用计算磁场"
        << "不使用计算磁场，使用磁强计" << "星敏时间补偿" << "星敏时间不补偿" << "卫星姿态滚动、俯仰和偏航方向偏置量"
        << "卫星姿态滚动、俯仰和偏航目标姿态" << "卫星姿态滚动、俯仰和偏航目标姿态角速度"<< "进入凝视控制" 
        << "设置凝视目标位置" << "设置凝视目标位置偏置量";
    
    strList_control_operation << "磁力矩器阻尼系数" << "飞轮阻尼系数" << "飞轮控制比例系数" << "飞轮控制微分系数" << "飞轮控制积分系数"
        << "对地机动角速度参数" << "对日坐标系转角" << "姿态测量误差上界" << "陀螺测量误差上界" << "环境干扰力矩上界" << "陀螺漂移上界"
        << "磁矩最大值" << "飞轮角动量测量误差上界" << "飞轮摩擦上界" << "系统角动量卸载界" << "卸载系数" << "磁控不控值" << "寻星敏角速度"
        << "对日定向机动角速度参数" << "光纤陀螺A输出上界" << "光纤陀螺A输出变化界" << "光纤陀螺B输出上界" << "光纤陀螺B输出变化界"
        << "数字太阳输出变化界和上界" << "星敏感器输出变化界" << "磁强计输出上界" << "飞轮输出角动量上界" << "飞轮静摩擦力矩"
        << "飞轮动摩擦力矩系数" << "飞轮差分力矩与输出信号之差上界" << "遥控确定器件使用状态" << "器件强制状态" << "飞轮最大力矩"
        << "陀螺A预设漂移" << "陀螺B预设漂移" << "卫星转动惯量" << "姿态估计器滤波系数" << "星敏感器A到  星体旋转四元数" << "星敏感器B到星体旋转四元数"
        << "磁力矩器安装矩阵" << "磁强计A安装四元数" << "磁强计B安装四元数" << "数字太阳敏安装四元数" << "差分求角速度限幅值" << "飞轮安装矩阵"
        << "光纤陀螺A安装四元数" << "光纤陀螺B安装四元数" << "阻尼切换角速度" << "GNSS输出位置变化界" << "GNSS 输出速度变化界" << "GNSS 输出位置上界"
        << "GNSS 输出速度上界" << "上注轨道对应的参数" << "安全模式角速度" << "设置电推力器工作时间";

    strList_power << "蓄电池组放电开关通" << "蓄电池组放电开关断" << "硬件过放继电器使能" << "硬件过放继电器禁能" << "星箭分离继电器使能"
        << "星箭分离继电器禁能" << "蓄电池单体采样继电器使能" << "蓄电池单体采样继电器禁能" << "UXB - A断B通" << "UXB - A通B断" << "UXB - A通B通"
        << "太阳电池阵1分流控制开" << "太阳电池阵1分流控制关" << "太阳电池阵2分流控制开" << "太阳电池阵2分流控制关" << "太阳电池阵3分流控制开"
        << "太阳电池阵3分流控制关" << "太阳电池阵4分流控制开" << "太阳电池阵4分流控制关" << "蓄电池单体1均衡控制开" << "蓄电池单体1均衡控制关"
        << "蓄电池单体2均衡控制开" << "蓄电池单体2均衡控制关" << "蓄电池单体3均衡控制开" << "蓄电池单体3均衡控制关" << "蓄电池单体4均衡控制开"
        << "蓄电池单体4均衡控制关" << "蓄电池加热片A供电开（加热）" << "蓄电池加热片A供电关" << "蓄电池加热片B供电开（加热）" << "蓄电池加热片B供电关"
        << "备份 + 5V电源开" << "备份 + 5V电源关" << "备份 + 12V电源开" << "备份 + 12V电源关" << "AMP / 姿控供电开" << "AMP / 姿控供电关"
        << "整星加热片A供电开（加热）" << "整星加热片A供电关" << "整星加热片B供电开（加热）" << "整星加热片B供电关" << "整星加热片C供电开（加热）"
        << "整星加热片C供电关" << "整星加热片D供电开（加热）" << "整星加热片D供电关" << "太阳电池阵A展开热刀开" << "太阳电池阵A展开热刀关"
        << "太阳电池阵B展开热刀开" << "太阳电池阵B展开热刀关" << "飞轮X供电开" << "飞轮X供电关" << "飞轮Y供电开" << "飞轮Y供电关" << "飞轮Z供电开"
        << "飞轮Z供电关" << "飞轮S供电开" << "飞轮S供电关" << "磁力矩器总供电开" << "磁力矩器总供电关" << "电推力器供电开" << "电推力器供电关"
        << "光纤陀螺A供电开" << "光纤陀螺A供电关" << "光纤陀螺B供电开" << "光纤陀螺B供电关" << "光纤陀螺C供电开" << "光纤陀螺C供电关" << "星敏感器A供电开"
        << "星敏感器A供电关" << "星敏感器B供电开" << "星敏感器B供电关" << "星敏感器C供电开" << "星敏感器C供电关" << "数字太阳敏感器供电开"
        << "数字太阳敏感器供电关" << "磁强计A供电开" << "磁强计A供电关" << "磁强计B供电开" << "磁强计B供电关" << "UV通信机供电开" << "UV通信机供电关"
        << "X相控阵单元供电开" << "X相控阵单元供电关" << "DCS载荷供电开" << "DCS载荷供电关" << "DCS天线展开热刀开" << "DCS天线展开热刀关" << "U天线展开热刀开"
        << "U天线展开热刀关" << "V天线展开热刀开" << "V天线展开热刀关" << "GNSS掩星观测单元供电开" << "GNSS掩星观测单元供电关" << "数据压缩处理单元A供电开"
        << "数据压缩处理单元A供电关" << "数据压缩处理单元B供电开" << "数据压缩处理单元B供电关" << "高分摄像机供电开" << "高分摄像机供电关" << "低分摄像机供电开"
        << "低分摄像机供电关" << "广角摄像机供电开" << "广角摄像机供电关" << "红外摄像机供电开" << "红外摄像机供电关" << "对焦机构A供电开" << "对焦机构A供电关"
        << "对焦机构B供电开" << "对焦机构B供电关" << "GNSS接收机A主供电开" << "GNSS接收机A主供电关" << "GNSS接收机A备供电开" << "GNSS接收机A备供电关"
        << "GNSS接收机B供电开" << "GNSS接收机B供电关" << "UXB收发机供电A开B关" << "UXB收发机供电B开A关" << "UXB收发机供电A开B开" << "模拟开关地址选择信号A1开"
        << "模拟开关地址选择信号A1关" << "模拟开关地址选择信号A2开" << "模拟开关地址选择信号A2关" << "模拟开关地址选择信号A3开" << "模拟开关地址选择信号A3关"
        << "模拟开关地址选择信号A4开" << "模拟开关地址选择信号A4关" << "磁力矩器X控制 + " << "磁力矩器X控制 - " << "磁力矩器Y控制 + " << "磁力矩器Y控制 - "
        << "磁力矩器Z控制 + " << "磁力矩器Z控制 - " << "设备组合控制开指令" << "设备组合控制关指令";

    strList_UXB << "UXB - A OC1输出" << "UXB - A OC2输出" << "UXB - A OC3输出" << "UXB - A OC4输出" << "UXB - A发射机开" << "UXB - A发射机关"
        << "UXB - A调制开" << "UXB - A调制关" << "UXB - A定时复位开" << "UXB - A定时复位关" << "UXB - A FPGA复位" << "UXB - A切换到测控模式"
        << "UXB - A切换到数传模式" << "UXB - A数传速率为2Mbps模式" << "UXB - A数传速率为20Mbps模式" << "UXB - A数传测控天线通道打开，数传相控阵天线通道关闭"
        << "UXB - A数传相控阵天线通道打开，数传测控天线通道关闭" << "UXB - A LVDS直传" << "UXB - A LVDS缓存后再传" << "UXB - A数传开" << "UXB - A数传关"
        << "UXB - A固存擦除" << "UXB - A固存存储开始"  << "UXB - A固存存储停止"<< "UXB - A固存顺序回放（发送该指令后必须发送固存回放停止指令）" 
        << "UXB - A控制指定地址回放" << "UXB - A固存回放停止"<< "UXB - A写指针备份" << "UXB - A读指针备份" << "UXB - B OC1输出" << "UXB - B OC2输出"
        << "UXB - B OC3输出" << "UXB - B OC4输出"<< "UXB - B发射机开" << "UXB - B发射机关" << "UXB - B调制开" << "UXB - B调制关" << "UXB - B定时复位开" 
        << "UXB - B定时复位关" << "UXB - B FPGA复位"<< "UXB - B切换到测控模式" << "UXB - B切换到数传模式" << "UXB - B数传速率为2Mbps模式" 
        << "UXB - B数传速率为20Mbps模式"<< "UXB - B数传测控天线通道打开，数传相控阵天线通道关闭" << "UXB - B数传相控阵天线通道打开，数传测控天线通道关闭" 
        << "UXB - B LVDS直传" << "UXB - B LVDS缓存后再传"<< "UXB - B数传开" << "UXB - B数传关" << "UXB - B固存擦除" << "UXB - B固存存储开始" 
        << "UXB - B固存存储停止" << "UXB - B固存顺序回放" << "UXB - B控制指定地址回放" << "UXB - B固存回放停止" << "UXB - B写指针备份" << "UXB - B读指针备份";

    strList_UV << "发射机开" << "发射机关" << "调制开" << "调制关" << "定时复位开" << "定时复位关" << "FPGA复位" << "信标模式关" << "信标模式开";

    strList_X_antenna << "全阵待机" << "正常工作";

    strList_GNSSA << "定轨禁止" << " 定轨允许" << "查询原始观测数据";

    strList_sun_sensor << "设置正常工作模式" << "设置参数调整模式" << "工程参数设置";
        
    strList_star_sensor_A << "切换工作模式" << "内部参数调整" << "自检指令" << "控制参数获取" << "星点像数据获取" << "单星点像图像获取" << "星点像图像获取";

    strList_star_sensor_B << "复位" << "自检" << "进入待机模式" << "进入姿态测量模式" << "参数配置指令";

    strList_magnetometer << "磁强计A默认参数设置" << "磁强计B默认参数设置";

    strList_flywheel_cmd << "飞轮X转速模式指令" << "飞轮X电流模式指令" << "飞轮X力矩模式指令" << "飞轮Y转速模式指令" << "飞轮Y电流模式指令" << "飞轮Y力矩模式指令"
                         << "飞轮Z转速模式指令" << "飞轮Z电流模式指令" << "飞轮Z力矩模式指令" << "飞轮S转速模式指令" << "飞轮S电流模式指令" << "飞轮S力矩模式指令";

    strList_fiber_optic_gyro <<"光纤陀螺A复位"<<"光纤陀螺B复位"<<"光纤陀螺B默认参数设置";
    strList_electric_thruster << "点火指令" << "开启电源" << "关闭电源" << "清洗火花塞" << "连续点火";
    strList_camera << "高分通信波特率" << "高分图像高" << "高分图像宽" << "高分OffsetY" << "高分OffsetX" << "高分图像格式" << "高分帧读取周期" << "高分帧开始触发源"
                    << "高分曝光模式" << "高分曝光时间" << "高分白平衡" << "高分增益" << "高分自动增益" << "高分抽头" << "高分伽马选择" << "高分CameraLink时钟频率" << "高分参数配置装载" << "高分参数配置保存"
        << "低分通信波特率" << "低分图像高" << "低分图像宽" << "低分OffsetY" << "低分OffsetX" << "低分图像格式" << "低分帧读取周期" << "低分帧开始触发源"
        << "低分曝光模式" << "低分曝光时间" << "低分白平衡" << "低分增益" << "低分自动增益" << "低分抽头" << "低分伽马选择" << "低分CameraLink时钟频率" << "低分参数配置装载" << "低分参数配置保存"
        << "广角通信波特率" << "广角图像高" << "广角图像宽" << "广角OffsetY" << "广角OffsetX" << "广角图像格式" << "广角帧读取周期" << "广角帧开始触发源"
        << "广角曝光模式" << "广角曝光时间" << "广角白平衡" << "广角增益" << "广角自动增益" << "广角抽头" << "广角伽马选择" << "广角CameraLink时钟频率" << "广角参数配置装载" << "广角参数配置保存";

    strList_infrared_camera << "图像定格" << "测试画面切换" << "保存设置" << "恢复出厂值" << "温升补偿开关" << "快门闭合" << "增益控制" << "外同步开关" << 
                                "数字口并行类型" << "CMOS内容选择" << "CMOS接口形式" << "场景补偿" << "快门补偿" << "时域滤波" << "滤波强度" << "去竖纹" << "调光模式" 
                            << "上抛点比例" << "下抛点比例" << "亮度" << "对比度" << "混合调光映射" << "Y8纠偏" << "IDE增强" << "IDE滤波等级 " << "IDE细节增益 " 
                            << "Y8纠偏模式" << "分块直方图" << "去噪开关" << "去噪等级" << "手动调焦速度" << "自动调焦统计帧数" << "自动调焦速度 MAX" << "自动调焦速度 MIN"
                            << "十字光标开关" << "光标位置X" << "光标位置Y" << "光标R分量" << "光标G分量" << "光标B分量" << "伪彩条开关" << "伪彩视觉增强" << "伪彩视觉增强阈值上限"
                            << "伪彩视觉增强阈值下限" << "等温线开关" << "等温线显示模式" << "等温线阈值上限" << "等温线阈值下限" << "等温线伪彩色带选择";

    strList_focusing_a << "右转" << "左转" << "停止" << "移动到目标位置";

    strList_focusing_b <<"调焦 -"<<"调焦＋"<<"停止调焦"; 
        
    strList_data_compression << "单元A系统授时" << "单元A系统重启" << "单元A文件序号记录" << "单元A文件序号追加记录" << "单元A记录直传" << "单元A文件序号回放启用断点" << "单元A文件序号回放不启用断点" 
                            << "单元A顺序回放启用断点" << "单元A顺序回放不启用断点" << "单元A按时间回放" << "单元A序号擦除文件" << "单元A全部擦除" << "单元A停止记录" << "单元A停止回放" << "单元A全部停止" << "单元A预关机" 
                            << "单元A功能自检" << "单元A低级格式化" << "单元A压缩模式设置" << "单元A压缩比设置" << "单元A压缩回写" << "单元A停止压缩" << "单元A压缩直传" << "单元A相机单元配置透传" << "单元A获取当前文件系统信息"
                            << "单元A获取文件属性信息" << "单元A获取相机配置参数透传" << "单元A系统授时" << "单元A系统重启" << "单元A文件序号记录" << "单元A文件序号追加记录" << "单元A记录直传" << "单元A文件序号回放启用断点"
                            << "单元A文件序号回放不启用断点" << "单元A顺序回放启用断点" << "单元A顺序回放不启用断点" << "单元A按时间回放" << "单元A序号擦除文件" << "单元A全部擦除" << "停止记录" << "单元A停止回放" 
                            << "单元A全部停止" << "单元A预关机" << "单元A功能自检" << "单元A低级格式化" << "单元A压缩模式设置" << "单元A压缩比设置" << "单元A压缩回写" << "单元A停止压缩" << "单元A压缩直传" << "单元A相机单元配置透传"
                            << "单元A获取当前文件系统信息" << "单元A获取文件属性信息" << "单元A获取相机配置参数透传"
                            << "单元B系统授时" << "单元B系统重启" << "单元B文件序号记录" << "单元B文件序号追加记录" << "单元B记录直传" << "单元B文件序号回放启用断点" << "单元B文件序号回放不启用断点"
                            << "单元B顺序回放启用断点" << "单元B顺序回放不启用断点" << "单元B按时间回放" << "单元B序号擦除文件" << "单元B全部擦除" << "单元B停止记录" << "单元B停止回放" << "单元B全部停止" << "单元B预关机"
                            << "单元B功能自检" << "单元B低级格式化" << "单元B压缩模式设置" << "单元B压缩比设置" << "单元B压缩回写" << "单元B停止压缩" << "单元B压缩直传" << "单元B相机单元配置透传" << "单元B获取当前文件系统信息"
                            << "单元B获取文件属性信息" << "单元B获取相机配置参数透传" << "单元B系统授时" << "单元B系统重启" << "单元B文件序号记录" << "单元B文件序号追加记录" << "单元B记录直传" << "单元B文件序号回放启用断点"
                            << "单元B文件序号回放不启用断点" << "单元B顺序回放启用断点" << "单元B顺序回放不启用断点" << "单元B按时间回放" << "单元B序号擦除文件" << "单元B全部擦除" << "单元B停止记录" << "单元B停止回放"
                            << "单元B全部停止" << "单元B预关机" << "单元B功能自检" << "单元B低级格式化" << "单元B压缩模式设置" << "单元B压缩比设置" << "单元B压缩回写" << "单元B停止压缩" << "单元B压缩直传" << "单元B相机单元配置透传"
                            << "单元B获取当前文件系统信息" << "单元B获取文件属性信息" << "单元B获取相机配置参数透传";

    strList_GNSS_occultation_obs << "接收机复位" << "定轨开启" << "定轨关闭" << "原始观测量存储开始" << "原始观测量存储关闭" << "GPS大气开环通道开启" << "GPS大气开环通道关闭" 
                            << "BD大气开环通道开启" << "BD大气开环通道关闭" << "掩星接收机切正常工作模式" << "掩星接收机切数据传输模式" << "Flash数据擦除" << "Flash坏块更新" 
                            << "掩星接收机强制切正常工作模式" << "掩星接收机强制切数据传输模式" << "数据传输终止" << "程序上注允许" << "程序上注结束" << "程序上注启动" 
                            << "SPI数据发送开始" << "SPI数据发送停止" << "模式1测试开启" << "模式1测试关闭" << "模式2测试开启" 
                             << "模式2测试关闭" << "掩星观测单元遥测查询";

    strList_DCS_acquisition << "DCS载荷软复位" << "DCS固存开始记录" << "DCS固存停止记录" << "DCS数据顺序回放" << "DCS数据停止回放" << "DCS 12h自复位使能" << "DCS 12h自复位禁止" 
                            << "DCS固存读写地址备份" << "DCS载荷功放允许开启" << "DCS载荷功放禁止开启" << "DCS数传下行开启" << "DCS数传下行关闭" << "调整DCS记录地址" << "调整DCS回放地址";

    strList_interactive_operation << "成像任务设置" << "任务清除" << "数传站设置" << "系统预重启" << "存储回放" << "星务成像任务强制结束";

    strList_data_annotation << "星务软件预上注" << "星务软件上注" << "授权码上注" <<"音频数据上注";

    strList_direct_cmd << "展开释放机构A备起爆" << "展开释放机构B备起爆" << "U天线展开起爆" << "V天线展开起爆" << "DCS载荷备起爆"
        << "AMP供电通断" << "蓄电池放电开关通" << "蓄电池放电开关断" << "姿控/AMP计算机A开B关" << "姿控/AMP计算机B开A关" << "姿控/AMP计算机模式控制使用1"
        << "姿控/AMP计算机模式控制使用2" << "姿控/AMP计算机切为主节点" << "星务计算机A开B关" << "星务计算机B开A关" << "星务计算机备份主控切为从节点";
              
    if (act->text() == "XWOBC")
    {
        qDebug() << "XWOBC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
         for (int i = 0; i < strList_XWOBC.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_XWOBC[i]);
            ui.listWidget->addItem(aItem); 
        }      
    }
    
    if (act->text() == "ZKOBC")
    {
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_ZKOBC.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_ZKOBC[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "AMPOBC")
    {
         ui.listWidget->clear();
         ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_AMPOBC.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_AMPOBC[i]);
            ui.listWidget->addItem(aItem);
        }
    }
   // ui.listWidget->selectedItem（）;
    if (act->text() == "控制运算分系统") 
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_control_operation.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_control_operation[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "PCDU遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_power.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_power[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "UXB测控数传一体机")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_UXB.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_UXB[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "VU通信机")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_UV.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_UV[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "X相控阵天线")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_X_antenna.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_X_antenna[i]); 
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "GNSS接收机A")
    {
        qDebug() << "GNSS接收机A";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_GNSSA.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_GNSSA[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "GNSS接收机B遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_GNSSB.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_GNSSB[i]);
            ui.listWidget->addItem(aItem);
        }
    }
    if (act->text() == "数字太阳敏感器")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_sun_sensor.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_sun_sensor[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "星敏感器A")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_star_sensor_A.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_star_sensor_A[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "星敏感器B")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_star_sensor_B.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_star_sensor_B[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "星敏感器C")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_star_sensor_C.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_star_sensor_C[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "磁强计遥控指令")
    {
        qDebug() << "磁强计遥控指令";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_magnetometer.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_magnetometer[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "飞轮遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_flywheel_cmd.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_flywheel_cmd[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "光纤陀螺遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_fiber_optic_gyro.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_fiber_optic_gyro[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "电推力遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_electric_thruster.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_electric_thruster[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "摄像机")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_camera.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_camera[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "红外摄像机")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_infrared_camera.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_infrared_camera[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "对焦机构A遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_focusing_a.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_focusing_a[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "对焦机构B遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_focusing_b.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_focusing_b[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "数据压缩处理单元")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_data_compression.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_data_compression[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "交互操作分系统")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_interactive_operation.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_interactive_operation[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "数据上注指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_data_annotation.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_data_annotation[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "GNSS掩星观测单元遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_GNSS_occultation_obs.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_GNSS_occultation_obs[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "DCS采集单元遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_DCS_acquisition.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_DCS_acquisition[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "直接指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
        ui.listWidget_selectcmd->clear();
        for (int i = 0; i < strList_direct_cmd.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_direct_cmd[i]);
            ui.listWidget->addItem(aItem);
        }
    }
}

void QtCommandtest::btn_to_gather()
{
    Project_gather* Project_login = new Project_gather; 
    Project_login->show();
    this->hide();
}

void QtCommandtest::select_in_list_list()
{
    QListWidgetItem * aa =  ui.listWidget->currentItem();
    select_in_list = aa->text();
}

void QtCommandtest::select_in_list_frame()
{
    QListWidgetItem* aa = ui.listWidget_build_frame->currentItem();
    select_in_frame_list = aa->text();
}

void QtCommandtest::btn_list_to_selectlist()
{
    QList<QListWidgetItem*>  aItem1 = ui.listWidget_selectcmd->findItems(select_in_list, Qt::MatchExactly);
    if (aItem1.isEmpty())
    {
        QListWidgetItem* aItem;
        aItem = new QListWidgetItem();
        aItem->setText(select_in_list);
        ui.listWidget_selectcmd->addItem(aItem);
    }
    else
    {
        ui.plainTextEdit_reminder->appendPlainText("已选指令项有重复 ！");
    }
}

qint8 QtCommandtest::select_in_list_select()
{
    QListWidgetItem* aa = ui.listWidget_selectcmd->currentItem();
    QString  aa_string = aa->text();
     
    row_in_cmd = ui.listWidget_selectcmd->currentRow();//当前行
    select_in_cmd = aa_string;
 
    ui.textEdit_validdata->clear();
    ui.plainTextEdit_reminder->clear(); 

    /*---------------------XWOBC-------------------------*/
    telecontrol_frame_up.cmd_style = 0x3a;
    telecontrol_frame_up.single_number = 0x5A;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 0;
    single_multiframe = 0;

    if (aa_string == "进入测试模式")
    {   
       telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("进入测试模式", 0x1);
        map_name_to_validdatalength.insert("进入测试模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据1");
        return 1;
    }
    
    if (aa_string == "清空延时指令")
    {
        telecontrol_frame_up.cmd_code = 0x02;
         ui.textEdit_validdata->insertPlainText("02");
         map_name_to_code.insert("清空延时指令", 2);
         map_name_to_validdatalength.insert("清空延时指令", 1);
         ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
         return 1;
    }
    if (aa_string == "清空延时任务") 
    {
        telecontrol_frame_up.cmd_code = 0x03;
        ui.textEdit_validdata->insertPlainText("03");
        map_name_to_code.insert("清空延时任务", 3);
        map_name_to_validdatalength.insert("清空延时任务", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
        return 1;
    }
      if (aa_string == "星务计算机复位")
    {
         telecontrol_frame_up.cmd_code = 0x04;
        ui.textEdit_validdata->insertPlainText("04");
        map_name_to_code.insert("星务计算机复位", 4);
        map_name_to_validdatalength.insert("星务计算机复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
        return 1;
    }
    if (aa_string == "程序注入开始")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        ui.textEdit_validdata->insertPlainText("05");
        map_name_to_code.insert("程序注入开始", 5);
        map_name_to_validdatalength.insert("程序注入开始", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
        return 1;
    }
    if (aa_string == "程序注入结束")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        ui.textEdit_validdata->insertPlainText("06");
        map_name_to_code.insert("程序注入结束", 6);
        map_name_to_validdatalength.insert("程序注入结束", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
        return 1;
    }      
    if (aa_string == "星务计算机遥测查询")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x0a;
        map_name_to_code.insert("星务计算机遥测查询", 10);
        map_name_to_validdatalength.insert("星务计算机遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道：UXB测控数传一体机A：55H，");
        return 1;
    }
    if (aa_string == "星务计算机日志下载")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x0b;
        map_name_to_code.insert("星务计算机日志下载", 11);
        map_name_to_validdatalength.insert("星务计算机日志下载", 9);
               ui.plainTextEdit_reminder->appendPlainText("有效数据9字节，比如:dd0000123400002345,dd代表通道，\
                                                   00001234日志记录起始时间，00002345日志记录结束时间");
        return 1;
    }
    if (aa_string == "星务计算机访问记录下载")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x0c;
        map_name_to_code.insert("星务计算机访问记录下载", 12);
        map_name_to_validdatalength.insert("星务计算机访问记录下载", 9);
        ui.plainTextEdit_reminder->appendPlainText("有效数据9字节，比如:dd0000123400002345,dd代表通道，\
                                                   00001234访问记录起始时间，00002345访问记录结束时间");
        return 1;
    }
    if (aa_string == "星务计算机短信息下载")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x0d;
        map_name_to_code.insert("星务计算机短消息下载", 13);
        map_name_to_validdatalength.insert("星务计算机短消息下载", 4);
        ui.plainTextEdit_reminder->appendPlainText("有效数据4字节，短消息接收用户。比如:XXXXXXXX");
        return 1;
    }
    if (aa_string == "任务请求记录文件下载")
    {
        telecontrol_frame_up.cmd_code = 0x0e;
        map_name_to_code.insert("星务计算机短消息下载", 0x0e);
        map_name_to_validdatalength.insert("星务计算机短消息下载", 9);
        ui.plainTextEdit_reminder->appendPlainText("有效数据9字节，比如:dd0000123400002345,dd代表通道，\
                                                   00001234请求记录起始时间，00002345请求记录结束时间");
        return 1;
    }

    if (aa_string == "任务执行记录文件下载")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x0f;
        map_name_to_code.insert("星务计算机短消息下载", 0x0f);
        map_name_to_validdatalength.insert("星务计算机短消息下载", 9);
        ui.plainTextEdit_reminder->appendPlainText("有效数据9字节，比如:dd0000123400002345,dd代表通道，\
                                                   00001234执行记录起始时间，00002345执行记录结束时间");
        return 1;
    }

    if (aa_string == "单机设备透传遥测查询")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x10;
        map_name_to_code.insert("星务计算机短消息下载", 0x10);
        map_name_to_validdatalength.insert("星务计算机短消息下载", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }

    if (aa_string == "PCDU遥测查询")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x14;
        map_name_to_code.insert("PCDU遥测查询", 0x14);
        map_name_to_validdatalength.insert("PCDU遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }

    if (aa_string == "测控数传一体机遥测查询")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x15;
        map_name_to_code.insert("PCDU遥测查询", 0x15);
        map_name_to_validdatalength.insert("PCDU遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }

    if (aa_string == "姿控分系统遥测查询")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x16;
        map_name_to_code.insert("姿控分系统遥测查询", 0x16);
        map_name_to_validdatalength.insert("姿控分系统遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }
    
    if (aa_string == "GNSS接收机AB遥测查询")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x17;
        map_name_to_code.insert("GNSS接收机AB遥测查询", 0x17);
        map_name_to_validdatalength.insert("GNSS接收机AB遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }

    if (aa_string == "工作模式切换")
    {
        telecontrol_frame_up.cmd_code = 0x18;
        map_name_to_code.insert("工作模式切换", 0x18);
        map_name_to_validdatalength.insert("工作模式切换", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，data0：待工作模式；data1：工作模式。");
        return 1;

    }

    if (aa_string == "ACK应答开关设置")
    {
        telecontrol_frame_up.cmd_code = 0x19;
        map_name_to_code.insert("ACK应答开关设置", 0x19);
        map_name_to_validdatalength.insert("ACK应答开关设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，7F：开；EC：关");
        return 1;
    }
    
    if (aa_string == "卫星上行同步码设置")
    {
        telecontrol_frame_up.cmd_code = 0x1a;
        map_name_to_code.insert("卫星上行同步码设置", 0x1a);
        map_name_to_validdatalength.insert("卫星上行同步码设置", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，上行同步码");
        return 1;
    }

    if (aa_string == "数传通道禁用与使能设置")
    {
        telecontrol_frame_up.cmd_code = 0x1b;
        map_name_to_code.insert("数传通道禁用与使能设置", 0x1b);
        map_name_to_validdatalength.insert("数传通道禁用与使能设置", 8);
        ui.plainTextEdit_reminder->appendPlainText("有效数据8字节，4字节禁用起始时间，4字节禁用结束时间");  
        return 1;
    }

    if (aa_string == "展开释放机构的禁止和使能")
    {
        telecontrol_frame_up.cmd_code = 0x1c;
        map_name_to_code.insert("展开释放机构的禁止和使能", 0x1c);
        map_name_to_validdatalength.insert("展开释放机构的禁止和使能", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：7FH：使能；ECH：禁止;DATA1：7FH：使能；ECH：禁止");
        return 1;
    }

    if (aa_string == "自动热控的禁止和使能")
    {
        telecontrol_frame_up.cmd_code = 0x1d;
        map_name_to_code.insert("自动热控的禁止和使能", 0x1d);
        map_name_to_validdatalength.insert("自动热控的禁止和使能", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：7FH：使能；ECH：禁止;DATA1：7FH：使能；ECH：禁止");
        return 1;
    }
   
    if (aa_string == "遥测通道的选择")
    {
        telecontrol_frame_up.cmd_code = 0x1e;
        map_name_to_code.insert("遥测通道的选择", 0x1e);
        map_name_to_validdatalength.insert("遥测通道的选择", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：测控通道;DATA1：测控通道");
        return 1;
    }

    if (aa_string == "自动轨控的禁止和使能")
    {
        telecontrol_frame_up.cmd_code = 0x1f;
        map_name_to_code.insert("自动轨控的禁止和使能", 0x1f);
        map_name_to_validdatalength.insert("自动轨控的禁止和使能", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：7FH：使能；ECH：禁止;DATA1：7FH：使能；ECH：禁止");
        return 1;
    }

    if (aa_string == "定时断电时间间隔设置")
    {
        telecontrol_frame_up.cmd_code = 0x20;
        map_name_to_code.insert("定时断电时间间隔设置", 0x20);
        map_name_to_validdatalength.insert("定时断电时间间隔设置", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：未收到地面指令间隔时间，有效值1~30，单位分钟。DATA1：无任务需要执行间隔时间，有效值1~30，单位分钟。");
        return 1;
    }
    
    if (aa_string == "实时广播遥测的开启和关闭")
    {
        telecontrol_frame_up.cmd_code = 0x21;
        map_name_to_code.insert("实时广播遥测的开启和关闭", 0x21);
        map_name_to_validdatalength.insert("实时广播遥测的开启和关闭", 2);
        ui.plainTextEdit_reminder->appendPlainText("DATA0：7FH：使能；ECH：禁止DATA1：7FH：使能；ECH：禁止");
        return 1;
    }
   
    if (aa_string == "实时广播遥测的间隔时间")
    {
        telecontrol_frame_up.cmd_code = 0x22;
        map_name_to_code.insert("实时广播遥测的间隔时间", 0x22);
        map_name_to_validdatalength.insert("实时广播遥测的间隔时间", 2);
        ui.plainTextEdit_reminder->appendPlainText("DATA0：有效值为1~120，表示0.5~60s（设置值*0.5s），其他值无效DATA1：有效值为1~120，表示0.5~60s（设置值 * 0.5s），其他值无效");
        return 1;
    }

    if (aa_string == "授权码上注开始")
    {
        telecontrol_frame_up.cmd_code = 0x23;
        map_name_to_code.insert("授权码上注开始", 0x23);
        map_name_to_validdatalength.insert("授权码上注开始", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节.AAH：追加，55H：覆盖");
        return 1;
    }

    if (aa_string == "授权码上注结束")
    {
        telecontrol_frame_up.cmd_code = 0x24;
        ui.textEdit_validdata->insertPlainText("23");
        map_name_to_code.insert("授权码上注结束", 0x24);
        map_name_to_validdatalength.insert("授权码上注结束", 1);
        ui.plainTextEdit_reminder->appendPlainText("有效数据已有！");
        return 1;
    }

    if (aa_string == "星上授权码清除")
    {
        telecontrol_frame_up.cmd_code = 0x25;
        ui.textEdit_validdata->insertPlainText("25");
        map_name_to_code.insert("星上授权码清除", 0x25);
        map_name_to_validdatalength.insert("星上授权码清除", 1);
        ui.plainTextEdit_reminder->appendPlainText("有效数据已有！");
        return 1;
    }

    if (aa_string == "星上授权码下载")
    {
        ACK_reply = 1;
        telecontrol_frame_up.cmd_code = 0x26;
        map_name_to_code.insert("星上授权码下载", 0x26);
        map_name_to_validdatalength.insert("星上授权码下载", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节有效数据 ：AAH：未使用，55H：已使用，26H：星上所有授权码");
        return 1;
    }

    if (aa_string == "星务软件上注开始")
    {
        telecontrol_frame_up.cmd_code = 0x27;
        map_name_to_code.insert("星上授权码清除", 0x27);
        map_name_to_validdatalength.insert("星上授权码清除", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节。AAH=上注版本1；BBH = 上注版本2；CCH = 上注版本3；DDH = 上注版本4");
        return 1;
    }
    
    if (aa_string == "星务软件上注结束")
    {
        telecontrol_frame_up.cmd_code = 0x28;
        map_name_to_code.insert("星务软件上注结束", 0x28);
        map_name_to_validdatalength.insert("星务软件上注结束", 1);
        ui.plainTextEdit_reminder->appendPlainText("有效数据已有！");
        return 1;
    }

    if (aa_string == "短消息上注")
    {
        telecontrol_frame_up.cmd_code = 0x29;
        map_name_to_code.insert("短消息上注", 0x29);
        map_name_to_validdatalength.insert("短消息上注", 152);
        ui.plainTextEdit_reminder->appendPlainText("152字节，DATA0~DATA3：短消息接收用户ID；DATA4~DATA7：短消息发送用户ID； DATA8~DATA11：短消息发送终端ID\
                                                     DATA12~DATA151：短消息内容");
        return 1;
    }

    if (aa_string == "时间上注")
    {
        telecontrol_frame_up.cmd_code = 0x2a;
        map_name_to_code.insert("时间上注", 0x2a);
        map_name_to_validdatalength.insert("时间上注", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，DATA0~DATA3：授时时间，UTC时间秒\
                                                     DATA4~DATA5：授时时间毫秒");
        return 1;
    }

    if (aa_string == "轨道上注")
    {
        telecontrol_frame_up.cmd_code = 0x2b;
        map_name_to_code.insert("轨道上注", 0x2b);
        map_name_to_validdatalength.insert("轨道上注", 30);
        ui.plainTextEdit_reminder->appendPlainText("30字节，DATA0~DATA3：轨道对应的时间，UTC时间秒\
                                                    DATA4~DATA5：轨道对应的时间毫秒\
                                                    DATA6~DATA17：位置（X、 Y、 Z），单位：米 均为单精度浮点数\
                                                    DATA18~DATA29：速度（Vx、 Vy、 Vz），单位：米 / 秒 均为单精度浮点数");
     return 1;
    }

    if (aa_string == "设置最高充电电压阈值")
    {
        telecontrol_frame_up.cmd_code = 0x2c;
        map_name_to_code.insert("设置最高充电电压阈值", 0x2c);
        map_name_to_validdatalength.insert("设置最高充电电压阈值", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：单精度浮点数");
        return 1;
    }

    if (aa_string == "设置最低放电电压阈值")
    {
        telecontrol_frame_up.cmd_code = 0x2d;
        map_name_to_code.insert("设置最低放电电压阈值", 0x2d);
        map_name_to_validdatalength.insert("设置最低放电电压阈值", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：单精度浮点数");
        return 1;
    }

    if (aa_string == "设置单个测控通道地球站")
    {
        telecontrol_frame_up.cmd_code = 0x2e;
        map_name_to_code.insert("设置单个测控通道地球站", 0x2e);
        map_name_to_validdatalength.insert("设置单个测控通道地球站", 5);
        ui.plainTextEdit_reminder->appendPlainText("5字节，DATA0：测控地球站编号，1~10;\
            DATA1~DATA4：测控地球站经度，单精度浮点数;DATA5~DATA8：测控地球站纬度，单精度浮点数");
        return 1;
    }

    if (aa_string == "设置单个数传通道地球站")
    {
        telecontrol_frame_up.cmd_code = 0x2f;
        map_name_to_code.insert("设置单个数传通道地球站", 0x2f);
        map_name_to_validdatalength.insert("设置单个数传通道地球站", 5);
        ui.plainTextEdit_reminder->appendPlainText("5字节，DATA0：数传地球站编号，1~10;\
            DATA1~DATA4：数传地球站经度，单精度浮点数;DATA5~DATA8：数传地球站纬度，单精度浮点数");
        return 1;
    }

    if (aa_string == "设置多个测控通道地球站")
    {
        telecontrol_frame_up.cmd_code = 0x30;
        map_name_to_code.insert("设置多个测控通道地球站", 0x30);
        map_name_to_validdatalength.insert("设置多个测控通道地球站", 80);
        ui.plainTextEdit_reminder->appendPlainText("80字节，DATA0~DATA3：测控地球站1经度，单精度浮点数\
            数据为0表示无此地球站，10个测控站");
        return 1;
    }

    if (aa_string == "设置多个数传通道地球站")
    {
        telecontrol_frame_up.cmd_code = 0x31;
        map_name_to_code.insert("设置多个数传通道地球站", 0x31);
        map_name_to_validdatalength.insert("设置多个数传通道地球站", 80);
        ui.plainTextEdit_reminder->appendPlainText("80字节，DATA0~DATA3：数传地球站1经度，单精度浮点数\
            数据为0表示无此地球站，10个数传站");
        return 1;
    }
    
    if (aa_string == "删除延时任务")
    {
        telecontrol_frame_up.cmd_code = 0x98;
        map_name_to_code.insert("删除延时任务", 0x98);
        map_name_to_validdatalength.insert("删除延时任务", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：任务序号\
                                                     DATA2~DATA5：任务执行时间，UTC时间");
        return 1;
    }

    if (aa_string == "删除延时指令")
    {
        telecontrol_frame_up.cmd_code = 0x99;
        map_name_to_code.insert("删除延时指令", 0x99);
        map_name_to_validdatalength.insert("删除延时指令", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：单机及分系统编号\
            DATA1：指令代码");
        return 1;
    }

    if (aa_string == "星务计算机软件版本切换")
    {
        telecontrol_frame_up.cmd_code = 0x9a;
        map_name_to_code.insert("星务计算机软件版本切换", 0x9a);
        map_name_to_validdatalength.insert("星务计算机软件版本切换", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，AAH=版本1;BBH = 版本2;CCH = 版本3;DDH = 版本4");
        return 1;
    }

    if (aa_string == "单机设备指令透传")
    {
        telecontrol_frame_up.cmd_code = 0x9b;
        map_name_to_code.insert("单机设备指令透传", 0x9b);
        map_name_to_validdatalength.insert("单机设备指令透传", 0);
        ui.plainTextEdit_reminder->appendPlainText("n字节，依据实际指令长度");
        return 1;
    }

    if (aa_string == "音频数据广播开启")
    {
        telecontrol_frame_up.cmd_code = 0x9c;
        map_name_to_code.insert("音频数据广播开启", 0x9c);
        map_name_to_validdatalength.insert("音频数据广播开启", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，查看文档");
        return 1;
    }
    
    if (aa_string == "音频数据广播停止")
    {
        telecontrol_frame_up.cmd_code = 0x9d;
        map_name_to_code.insert("音频数据广播停止", 0x9d);
        map_name_to_validdatalength.insert("音频数据广播停止", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0: 01H：立即停止，02H：设定区域播放完成停止，\
            其他值：立即停止。DATA1：同DATA0");
        return 1;
    }
    /*------------------------- AMPOBC--------------------*/

    telecontrol_frame_up.cmd_style = 0x3a;
    telecontrol_frame_up.single_number = 0x5B;   
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "进入自主姿态控制")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("进入自主姿态控制", 0x01);
        map_name_to_validdatalength.insert("进入自主姿态控制", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "进入对日捕获与对日定向")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("02");
        map_name_to_code.insert("进入对日捕获与对日定向", 0x02);
        map_name_to_validdatalength.insert("进入对日捕获与对日定向", 2);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！度");
        return 1;
    }

    if (aa_string == "进入对日定向三轴稳定")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        ui.textEdit_validdata->insertPlainText("03");
        map_name_to_code.insert("进入对日定向三轴稳定", 0x03);
        map_name_to_validdatalength.insert("进入对日定向三轴稳定", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！度");
        return 1;
    }

    if (aa_string == "进入对地定向三轴稳定")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        ui.textEdit_validdata->insertPlainText("04");
        map_name_to_code.insert("进入对地定向三轴稳定", 0x04);
        map_name_to_validdatalength.insert("进入对地定向三轴稳定", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "卫星姿态俯仰方向Y + 调整")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("卫星姿态俯仰方向Y + 调整", 0x05);
        map_name_to_validdatalength.insert("卫星姿态俯仰方向Y + 调整", 2);
        ui.plainTextEdit_reminder->appendPlainText("有效数据2字节，调整角度。");
        return 1;
    }

    if (aa_string == "卫星姿态俯仰方向Y - 调整")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("卫星姿态俯仰方向Y - 调整", 0x06);
        map_name_to_validdatalength.insert("卫星姿态俯仰方向Y - 调整", 2);
        ui.plainTextEdit_reminder->appendPlainText("有效数据2字节，调整角度。");
        return 1;
    }

    if (aa_string == "卫星姿态滚动方向X + 调整")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("卫星姿态滚动方向X + 调整", 0x07);
        map_name_to_validdatalength.insert("卫星姿态滚动方向X + 调整", 2);
        ui.plainTextEdit_reminder->appendPlainText("有效数据2字节，调整角度。");
        return 1;
    }

    if (aa_string == "卫星姿态滚动方向X - 调整")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("卫星姿态滚动方向X - 调整", 0x08);
        map_name_to_validdatalength.insert("卫星姿态滚动方向X - 调整", 2);
        ui.plainTextEdit_reminder->appendPlainText("有效数据2字节，调整角度。");
        return 1;
    }

    if (aa_string == "星敏感器A屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        ui.textEdit_validdata->insertPlainText("09");
        map_name_to_code.insert("星敏感器A屏蔽使用", 0x09);
        map_name_to_validdatalength.insert("星敏感器A屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏感器A不屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x0a;
        ui.textEdit_validdata->insertPlainText("0A");
        map_name_to_code.insert("单机设备指令透传", 0x0a);
        map_name_to_validdatalength.insert("单机设备指令透传", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏感器B屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x0b;
        ui.textEdit_validdata->insertPlainText("0B");
        map_name_to_code.insert("星敏感器B屏蔽使用", 0x0b);
        map_name_to_validdatalength.insert("星敏感器B屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏感器B不屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x0c;
        ui.textEdit_validdata->insertPlainText("0C");
        map_name_to_code.insert("星敏感器B不屏蔽使用", 0x0c);
        map_name_to_validdatalength.insert("星敏感器B不屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "数字太阳敏感器屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x0D;
        ui.textEdit_validdata->insertPlainText("0D");
        map_name_to_code.insert("数字太阳敏感器屏蔽使用", 0x0D);
        map_name_to_validdatalength.insert("数字太阳敏感器屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "数字太阳敏感器不屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x0E;
        ui.textEdit_validdata->insertPlainText("0E");
        map_name_to_code.insert("数字太阳敏感器不屏蔽使用", 0x0E);
        map_name_to_validdatalength.insert("数字太阳敏感器不屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "磁强计A屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x0F;
        ui.textEdit_validdata->insertPlainText("0F");
        map_name_to_code.insert("磁强计A屏蔽使用", 0x0F);
        map_name_to_validdatalength.insert("磁强计A屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "磁强计A不屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        ui.textEdit_validdata->insertPlainText("10");
        map_name_to_code.insert("磁强计A不屏蔽使用", 0x10);
        map_name_to_validdatalength.insert("磁强计A不屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "磁强计B屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        ui.textEdit_validdata->insertPlainText("11");
        map_name_to_code.insert("磁强计B屏蔽使用", 0x11);
        map_name_to_validdatalength.insert("磁强计B屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "磁强计B不屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        ui.textEdit_validdata->insertPlainText("12");
        map_name_to_code.insert("磁强计B不屏蔽使用", 0x12);
        map_name_to_validdatalength.insert("磁强计B不屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "陀螺A屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x13;
        ui.textEdit_validdata->insertPlainText("13");
        map_name_to_code.insert("陀螺A屏蔽使用", 0x13);
        map_name_to_validdatalength.insert("陀螺A屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "陀螺A不屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x14;
        ui.textEdit_validdata->insertPlainText("14");
        map_name_to_code.insert("陀螺A不屏蔽使用", 0x14);
        map_name_to_validdatalength.insert("陀螺A不屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "陀螺B屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x15;
        ui.textEdit_validdata->insertPlainText("15");
        map_name_to_code.insert("陀螺B屏蔽使用", 0x15);
        map_name_to_validdatalength.insert("陀螺B屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "陀螺B不屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x16;
        ui.textEdit_validdata->insertPlainText("16");
        map_name_to_code.insert("陀螺B不屏蔽使用", 0x16);
        map_name_to_validdatalength.insert("陀螺B不屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "陀螺C屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x17;
        ui.textEdit_validdata->insertPlainText("17");
        map_name_to_code.insert("陀螺C屏蔽使用", 0x17);
        map_name_to_validdatalength.insert("陀螺C屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "陀螺C不屏蔽使用")
    {
        telecontrol_frame_up.cmd_code = 0x18;
        ui.textEdit_validdata->insertPlainText("18");
        map_name_to_code.insert("陀螺C不屏蔽使用", 0x18);
        map_name_to_validdatalength.insert("陀螺C不屏蔽使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置角速度漂移估计标志：估计")
    {
        telecontrol_frame_up.cmd_code = 0x19;
        ui.textEdit_validdata->insertPlainText("19");
        map_name_to_code.insert("设置角速度漂移估计标志：估计", 0x19);
        map_name_to_validdatalength.insert("设置角速度漂移估计标志：估计", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置角速度漂移估计标志：停止估计")
    {
        telecontrol_frame_up.cmd_code = 0x1a;
        ui.textEdit_validdata->insertPlainText("1a");
        map_name_to_code.insert("设置角速度漂移估计标志：停止估计", 0x1a);
        map_name_to_validdatalength.insert("设置角速度漂移估计标志：停止估计", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置角速度漂移估计复位标志：复位置零")
    {
        telecontrol_frame_up.cmd_code = 0x1b;
        ui.textEdit_validdata->insertPlainText("1b");
        map_name_to_code.insert("设置角速度漂移估计复位标志：复位置零", 0x1b);
        map_name_to_validdatalength.insert("设置角速度漂移估计复位标志：复位置零", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置角速度漂移估计复位标志：不复位")
    {
        telecontrol_frame_up.cmd_code = 0x1c;
        ui.textEdit_validdata->insertPlainText("1c");
        map_name_to_code.insert("设置角速度漂移估计复位标志：不复位", 0x1c);
        map_name_to_validdatalength.insert("设置角速度漂移估计复位标志：不复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置飞轮摩擦估计标志：估计")
    {
        telecontrol_frame_up.cmd_code = 0x1d;
        ui.textEdit_validdata->insertPlainText("1d");
        map_name_to_code.insert("设置飞轮摩擦估计标志：估计", 0x1d);
        map_name_to_validdatalength.insert("设置飞轮摩擦估计标志：估计", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置飞轮摩擦估计标志：计算")
    {
        telecontrol_frame_up.cmd_code = 0x1e;
        ui.textEdit_validdata->insertPlainText("1e");
        map_name_to_code.insert("设置飞轮摩擦估计标志：计算", 0x1e);
        map_name_to_validdatalength.insert("设置飞轮摩擦估计标志：计算", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置飞轮角动量估计标志：估计")
    {
        telecontrol_frame_up.cmd_code = 0x1f;
        ui.textEdit_validdata->insertPlainText("1f");
        map_name_to_code.insert("设置飞轮角动量估计标志：估计", 0x1f);
        map_name_to_validdatalength.insert("设置飞轮角动量估计标志：估计", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置飞轮角动量估计标志：计算")
    {
        telecontrol_frame_up.cmd_code = 0x20;
        ui.textEdit_validdata->insertPlainText("20");
        map_name_to_code.insert("设置飞轮角动量估计标志：计算", 0x20);
        map_name_to_validdatalength.insert("设置飞轮角动量估计标志：计算", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置使用上注轨道确定标志：使用")
    {
        telecontrol_frame_up.cmd_code = 0x21;
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        ui.textEdit_validdata->insertPlainText("21");
        map_name_to_code.insert("设置使用上注轨道确定标志：使用", 0x21);
        map_name_to_validdatalength.insert("设置使用上注轨道确定标志：使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置使用上注轨道确定标志：不使用")
    {
        telecontrol_frame_up.cmd_code = 0x22;
        ui.textEdit_validdata->insertPlainText("22");
        map_name_to_code.insert("设置使用上注轨道确定标志：不使用", 0x22);
        map_name_to_validdatalength.insert("设置使用上注轨道确定标志：不使用", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "常规模式陀螺积分定姿高优先级")
    {
        telecontrol_frame_up.cmd_code = 0x23;
        ui.textEdit_validdata->insertPlainText("23");
        map_name_to_code.insert("常规模式陀螺积分定姿高优先级", 0x23);
        map_name_to_validdatalength.insert("常规模式陀螺积分定姿高优先级", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "常规模式双矢量定姿高优先级")
    {
        telecontrol_frame_up.cmd_code = 0x24;
        ui.textEdit_validdata->insertPlainText("24");
        map_name_to_code.insert("常规模式双矢量定姿高优先级", 0x24);
        map_name_to_validdatalength.insert("常规模式双矢量定姿高优先级", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏补偿（使用参考角速度）")
    {
        telecontrol_frame_up.cmd_code = 0x25;
        ui.textEdit_validdata->insertPlainText("25");
        map_name_to_code.insert("星敏补偿（使用参考角速度）", 0x25);
        map_name_to_validdatalength.insert("星敏补偿（使用参考角速度）", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏补偿（使用本体角速度）")
    {
        telecontrol_frame_up.cmd_code = 0x26;
        ui.textEdit_validdata->insertPlainText("26");
        map_name_to_code.insert("星敏补偿（使用本体角速度）", 0x26);
        map_name_to_validdatalength.insert("星敏补偿（使用本体角速度）", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏补偿（不补偿）")
    {
        telecontrol_frame_up.cmd_code = 0x27;
        ui.textEdit_validdata->insertPlainText("27");
        map_name_to_code.insert("星敏补偿（不补偿）", 0x27);
        map_name_to_validdatalength.insert("星敏补偿（不补偿）", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "使用计算磁场")
    {
        telecontrol_frame_up.cmd_code = 0x28;
        ui.textEdit_validdata->insertPlainText("28");
        map_name_to_code.insert("使用计算磁场", 0x28);
        map_name_to_validdatalength.insert("使用计算磁场", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "不使用计算磁场，使用磁强计")
    {
        telecontrol_frame_up.cmd_code = 0x29;
        ui.textEdit_validdata->insertPlainText("29");
        map_name_to_code.insert("不使用计算磁场，使用磁强计", 0x29);
        map_name_to_validdatalength.insert("不使用计算磁场，使用磁强计", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏时间补偿")
    {
        telecontrol_frame_up.cmd_code = 0x2a;
        ui.textEdit_validdata->insertPlainText("2a");
        map_name_to_code.insert("星敏时间补偿", 0x2a);
        map_name_to_validdatalength.insert("星敏时间补偿", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏时间不补偿")
    {
        telecontrol_frame_up.cmd_code = 0x2b;
        ui.textEdit_validdata->insertPlainText("2b");
        map_name_to_code.insert("星敏时间不补偿", 0x2b);
        map_name_to_validdatalength.insert("星敏时间不补偿", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "卫星姿态滚动、俯仰和偏航方向偏置量")
    {
        telecontrol_frame_up.cmd_code = 0x28;
      //  ui.textEdit_validdata->insertPlainText("28");
        map_name_to_code.insert("卫星姿态滚动、俯仰和偏航方向偏置量", 0x28);
        map_name_to_validdatalength.insert("卫星姿态滚动、俯仰和偏航方向偏置量", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节有效数据，DATA0~DATA1：滚动调整角度，有符号短整型*0.01°;\
            DATA2~DATA3：俯仰调整角度，有符号短整型*0.01°;DATA4~DATA5：偏航调整角度，有符号短整型*0.01°");
        return 1;
    }

    if (aa_string == "卫星姿态滚动、俯仰和偏航目标姿态")
    {
        telecontrol_frame_up.cmd_code = 0x29;
     //   ui.textEdit_validdata->insertPlainText("29");
        map_name_to_code.insert("卫星姿态滚动、俯仰和偏航目标姿态", 0x29);
        map_name_to_validdatalength.insert("卫星姿态滚动、俯仰和偏航目标姿态", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节有效数据，DATA0~DATA1：滚动调整角度，有符号短整型*0.01°;\
            DATA2~DATA3：俯仰调整角度，有符号短整型*0.01°;DATA4~DATA5：偏航调整角度，有符号短整型*0.01°");
        return 1;
    }

    if (aa_string == "卫星姿态滚动、俯仰和偏航目标姿态角速度")
    {
        telecontrol_frame_up.cmd_code = 0x2a;
      //  ui.textEdit_validdata->insertPlainText("2a");
        map_name_to_code.insert("卫星姿态滚动、俯仰和偏航目标姿态", 0x2a);
        map_name_to_validdatalength.insert("卫星姿态滚动、俯仰和偏航目标姿态", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节有效数据，DATA0~DATA1：滚动调整角度，有符号短整型*0.01°;\
            DATA2~DATA3：俯仰调整角度，有符号短整型*0.01°;DATA4~DATA5：偏航调整角度，有符号短整型*0.01°");
        return 1;
    }

    if (aa_string == "进入凝视控制")
    {
        telecontrol_frame_up.cmd_code = 0x2b;
        ui.textEdit_validdata->insertPlainText("2b");
        map_name_to_code.insert("进入凝视控制", 0x2b);
        map_name_to_validdatalength.insert("进入凝视控制", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据！");
        return 1;
    }

    if (aa_string == "设置凝视目标位置")
    {
        telecontrol_frame_up.cmd_code = 0x2c;
      //  ui.textEdit_validdata->insertPlainText("2c");
        map_name_to_code.insert("设置凝视目标位置", 0x2c);
        map_name_to_validdatalength.insert("设置凝视目标位置", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA0~DATA3凝视目标经度：单精度浮点数，\
            DATA4~DATA7凝视目标纬度：单精度浮点数！");
        return 1;
    }

    if (aa_string == "设置凝视目标位置偏置量")
    {
        telecontrol_frame_up.cmd_code = 0x2d;
      //  ui.textEdit_validdata->insertPlainText("2b");
        map_name_to_code.insert("设置凝视目标位置偏置量", 0x2d);
        map_name_to_validdatalength.insert("设置凝视目标位置偏置量", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA0~DATA3凝视目标经度偏置量：单精度浮点数，\
            DATA4~DATA7凝视目标纬度偏置量：单精度浮点数！");
        return 1;
    }
    
    /*-----------------------------控制运算分系统----------------------------------*/
    telecontrol_frame_up.cmd_style = 0x3a;
    telecontrol_frame_up.single_number = 0x5C;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0; 
    if (aa_string == "磁力矩器阻尼系数")
    {
       
        telecontrol_frame_up.cmd_code = 0xA2;
        map_name_to_code.insert("磁力矩器阻尼系数", 0xA2);
        map_name_to_validdatalength.insert("磁力矩器阻尼系数",6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，3个补码形式！");
        return 1;
    }

    if (aa_string == "飞轮阻尼系数")
    {
        telecontrol_frame_up.cmd_code = 0xA3;
        map_name_to_code.insert("飞轮阻尼系数", 0xA3);
        map_name_to_validdatalength.insert("飞轮阻尼系数", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，3个补码形式！");
        return 1;
    }

    if (aa_string == "飞轮控制比例系数")
    {
        telecontrol_frame_up.cmd_code = 0xA4;
        map_name_to_code.insert("飞轮控制比例系数", 0xA4);
        map_name_to_validdatalength.insert("飞轮控制比例系数", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，3个补码形式！");
        return 1;
    }

    if (aa_string == "飞轮控制微分系数")
    {
        telecontrol_frame_up.cmd_code = 0xA5;
        map_name_to_code.insert("飞轮控制微分系数", 0xA5);
        map_name_to_validdatalength.insert("飞轮控制微分系数", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，3个补码形式！");
        return 1;
    }

    if (aa_string == "飞轮控制积分系数")
    {
        telecontrol_frame_up.cmd_code = 0xA6;
        map_name_to_code.insert("飞轮控制积分系数", 0xA6);
        map_name_to_validdatalength.insert("飞轮控制积分系数", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，3个补码形式！");
        return 1;
    }

    if (aa_string == "对地机动角速度参数")
    {
        telecontrol_frame_up.cmd_code = 0xA7;
        map_name_to_code.insert("对地机动角速度参数", 0xA7);
        map_name_to_validdatalength.insert("对地机动角速度参数", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^10，无符号！");
        return 1;
    }

    if (aa_string == "对日坐标系转角")
    {
        telecontrol_frame_up.cmd_code = 0xA8;
        map_name_to_code.insert("对日坐标系转角", 0xA8);
        map_name_to_validdatalength.insert("对日坐标系转角", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，Z轴90°，补码，量纲：2^13；  Y轴待定角度，补码，量纲：2 ^ 13");
        return 1;
    }

    if (aa_string == "姿态测量误差上界")
    {
        telecontrol_frame_up.cmd_code = 0xA9;
        map_name_to_code.insert("姿态测量误差上界", 0xA9);
        map_name_to_validdatalength.insert("姿态测量误差上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，处理方法：1-(DATA/2^19)，无符号");
        return 1;
    }

    if (aa_string == "陀螺测量误差上界")
    {
        telecontrol_frame_up.cmd_code = 0xAA;
        map_name_to_code.insert("姿态测量误差上界", 0xAA);
        map_name_to_validdatalength.insert("姿态测量误差上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^13，无符号");
        return 1;
    }

    if (aa_string == "环境干扰力矩上界")
    {
        telecontrol_frame_up.cmd_code = 0xAB;
        map_name_to_code.insert("环境干扰力矩上界", 0xAB);
        map_name_to_validdatalength.insert("环境干扰力矩上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^16，无符号");
        return 1;
    }

    if (aa_string == "陀螺漂移上界")
    {
        telecontrol_frame_up.cmd_code = 0xAC;
        map_name_to_code.insert("陀螺漂移上界", 0xAC);
        map_name_to_validdatalength.insert("陀螺漂移上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^16，无符号");
        return 1;
    }

    if (aa_string == "磁矩最大值")
    {
        telecontrol_frame_up.cmd_code = 0xAD;
        map_name_to_code.insert("磁矩最大值", 0xAD);
        map_name_to_validdatalength.insert("磁矩最大值", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^9，无符号");
        return 1;
    }

    if (aa_string == "飞轮角动量测量误差上界")
    {
        telecontrol_frame_up.cmd_code = 0xAE;
        map_name_to_code.insert("飞轮角动量测量误差上界", 0xAE);
        map_name_to_validdatalength.insert("飞轮角动量测量误差上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^12，无符号");
        return 1;
    }

    if (aa_string == "飞轮摩擦上界")
    {
        telecontrol_frame_up.cmd_code = 0xAF;
        map_name_to_code.insert("飞轮摩擦上界", 0xAF);
        map_name_to_validdatalength.insert("飞轮摩擦上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^16，无符号");
        return 1;
    }

    if (aa_string == "系统角动量卸载界")
    {
        telecontrol_frame_up.cmd_code = 0xB0;
        map_name_to_code.insert("系统角动量卸载界", 0xB0);
        map_name_to_validdatalength.insert("系统角动量卸载界", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，量纲：2^16，无符号");
        return 1;
    }

    if (aa_string == "卸载系数")
    {
        telecontrol_frame_up.cmd_code = 0xB1;
        map_name_to_code.insert("卸载系数", 0xB1);
        map_name_to_validdatalength.insert("卸载系数", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，量纲：2^14，无符号");
        return 1;
    }

    if (aa_string == "磁控不控值")
    {
        telecontrol_frame_up.cmd_code = 0xB2;
        map_name_to_code.insert("卸载系数", 0xB2);
        map_name_to_validdatalength.insert("卸载系数", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }

    if (aa_string == "寻星敏角速度")
    {
        telecontrol_frame_up.cmd_code = 0xB3;
        map_name_to_code.insert("寻星敏角速度", 0xB3);
        map_name_to_validdatalength.insert("寻星敏角速度", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }

    if (aa_string == "对日定向机动角速度参数")
    {
        telecontrol_frame_up.cmd_code = 0xB4;
        map_name_to_code.insert("对日定向机动角速度参数", 0xB4);
        map_name_to_validdatalength.insert("对日定向机动角速度参数", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }

    if (aa_string == "光纤陀螺A输出上界")
    {
        telecontrol_frame_up.cmd_code = 0xB5;
        map_name_to_code.insert("光纤陀螺A输出上界", 0xB5);
        map_name_to_validdatalength.insert("光纤陀螺A输出上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }

    if (aa_string == "光纤陀螺A输出变化界")
    {
        telecontrol_frame_up.cmd_code = 0xB6;
        map_name_to_code.insert("光纤陀螺A输出变化界", 0xB6);
        map_name_to_validdatalength.insert("光纤陀螺A输出变化界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }
    if (aa_string == "光纤陀螺B输出上界")
    {
        telecontrol_frame_up.cmd_code = 0xB7;
        map_name_to_code.insert("光纤陀螺B输出上界", 0xB7);
        map_name_to_validdatalength.insert("光纤陀螺B输出上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }

    if (aa_string == "光纤陀螺B输出变化界")
    {
        telecontrol_frame_up.cmd_code = 0xB8;
        map_name_to_code.insert("光纤陀螺B输出变化界", 0xB8);
        map_name_to_validdatalength.insert("光纤陀螺B输出变化界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }
    if (aa_string == "数字太阳输出变化界和上界")
    {
        telecontrol_frame_up.cmd_code = 0xB9;
        map_name_to_code.insert("数字太阳输出变化界和上界", 0xB9);
        map_name_to_validdatalength.insert("数字太阳输出变化界和上界", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA1~DATA2	量纲：2^14，无符号;DATA3~DATA4	量纲：2^13，无符号");
        return 1;
    }
    if (aa_string == "星敏感器输出变化界")
    {
        telecontrol_frame_up.cmd_code = 0xBA;
        map_name_to_code.insert("星敏感器输出变化界", 0xBA);
        map_name_to_validdatalength.insert("星敏感器输出变化界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }
    if (aa_string == "磁强计输出上界")
    {
        telecontrol_frame_up.cmd_code = 0xBB;
        map_name_to_code.insert("磁强计输出上界", 0xBB);
        map_name_to_validdatalength.insert("磁强计输出上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，量纲：2^15，无符号");
        return 1;
    }
    if (aa_string == "飞轮输出角动量上界")
    {
        telecontrol_frame_up.cmd_code = 0xBC;
        map_name_to_code.insert("飞轮输出角动量上界", 0xBC);
        map_name_to_validdatalength.insert("飞轮输出角动量上界", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	量纲：2^13，无符号;……");
        return 1;
    }
    if (aa_string == "飞轮静摩擦力矩")
    {
        telecontrol_frame_up.cmd_code = 0xBD;
        map_name_to_code.insert("飞轮静摩擦力矩", 0xBD);
        map_name_to_validdatalength.insert("飞轮静摩擦力矩", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	量纲：2^13，无符号;……");
        return 1;
    }
    if (aa_string == "飞轮动摩擦力矩系数")
    {
        telecontrol_frame_up.cmd_code = 0xBE;
        map_name_to_code.insert("飞轮动摩擦力矩系数", 0xBE);
        map_name_to_validdatalength.insert("飞轮动摩擦力矩系数", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	量纲：2^13，无符号;……");
        return 1;
    }
    if (aa_string == "飞轮差分力矩与输出信号之差上界")
    {
        telecontrol_frame_up.cmd_code = 0xBF;
        map_name_to_code.insert("飞轮差分力矩与输出信号之差上界", 0xBF);
        map_name_to_validdatalength.insert("飞轮差分力矩与输出信号之差上界", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	量纲：2^13，无符号;……");
        return 1;
    }

    if (aa_string == "遥控确定器件使用状态")
    {
        telecontrol_frame_up.cmd_code = 0xC0;
        map_name_to_code.insert("遥控确定器件使用状态", 0xC0);
        map_name_to_validdatalength.insert("遥控确定器件使用状态", 3);
        ui.plainTextEdit_reminder->appendPlainText("3字节，DATA1 十进制源码;DATA2 十进制源码;DATA3 十进制源码;");
        return 1;
    }
    if (aa_string == "器件强制状态")
    {
        telecontrol_frame_up.cmd_code = 0xC1;
        map_name_to_code.insert("器件强制状态", 0xC1);
        map_name_to_validdatalength.insert("器件强制状态", 3);
        ui.plainTextEdit_reminder->appendPlainText("3字节，DATA1 十进制源码;DATA2 十进制源码;DATA3 十进制源码;");
        return 1;
    }
    if (aa_string == "飞轮最大力矩")
    {
        telecontrol_frame_up.cmd_code = 0xC2;
        map_name_to_code.insert("飞轮最大力矩", 0xC2);
        map_name_to_validdatalength.insert("飞轮最大力矩", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	量纲：2^13，无符号;……");
        return 1;
    }

    if (aa_string == "陀螺A预设漂移")
    {
        telecontrol_frame_up.cmd_code = 0xC3;
        map_name_to_code.insert("陀螺A预设漂移", 0xC3);
        map_name_to_validdatalength.insert("陀螺A预设漂移", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，DATA1~DATA2	X轴，补码，量纲：2^25;……");
        return 1;
    }

    if (aa_string == "陀螺B预设漂移")
    {
        telecontrol_frame_up.cmd_code = 0xC4;
        map_name_to_code.insert("陀螺B预设漂移", 0xC4);
        map_name_to_validdatalength.insert("陀螺B预设漂移", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，DATA1~DATA2	X轴，补码，量纲：2^25;……");
        return 1;
    }

    if (aa_string == "卫星转动惯量")
    {
        telecontrol_frame_up.cmd_code = 0xC5;
        map_name_to_code.insert("卫星转动惯量", 0xC5);
        map_name_to_validdatalength.insert("卫星转动惯量", 18);
        ui.plainTextEdit_reminder->appendPlainText("18字节，DATA1~DATA2	补码，量纲：2^10;……");
        return 1;
    }

    if (aa_string == "姿态估计器滤波系数")
    {
        telecontrol_frame_up.cmd_code = 0xC6;
        map_name_to_code.insert("姿态估计器滤波系数", 0xC6);
        map_name_to_validdatalength.insert("姿态估计器滤波系数", 12);
        ui.plainTextEdit_reminder->appendPlainText("12字节，DATA1~DATA2	补码，量纲：2^10;……");
        return 1;
    }

    if (aa_string == "星敏感器A到星体旋转四元数")
    {
        telecontrol_frame_up.cmd_code = 0xC7;
        map_name_to_code.insert("星敏感器A到星体旋转四元数", 0xC7);
        map_name_to_validdatalength.insert("星敏感器A到星体旋转四元数", 16);
        ui.plainTextEdit_reminder->appendPlainText("16字节，DATA1~DATA4	补码，量纲：2^10;……");
        return 1;
    }

    if (aa_string == "星敏感器B到星体旋转四元数")
    {
        telecontrol_frame_up.cmd_code = 0xC8;
        map_name_to_code.insert("星敏感器B到星体旋转四元数", 0xC8);
        map_name_to_validdatalength.insert("星敏感器B到星体旋转四元数", 16);
        ui.plainTextEdit_reminder->appendPlainText("16字节，DATA1~DATA4	补码，量纲：2^10;……");
        return 1;
    }

    if (aa_string == "磁力矩器安装矩阵")
    {
        telecontrol_frame_up.cmd_code = 0xC9;
        map_name_to_code.insert("磁力矩器安装矩阵", 0xC9);
        map_name_to_validdatalength.insert("磁力矩器安装矩阵", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，DATA1~DATA2	补码，量纲：2^10;……");
        return 1;
    }

    if (aa_string == "磁强计A安装四元数")
    {
        telecontrol_frame_up.cmd_code = 0xCA;
        map_name_to_code.insert("磁强计A安装四元数", 0xCA);
        map_name_to_validdatalength.insert("磁强计A安装四元数", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	补码，量纲：2^10;……");
        return 1;
    }

    if (aa_string == "磁强计B安装四元数")
    {
        telecontrol_frame_up.cmd_code = 0xCB;
        map_name_to_code.insert("磁强计B安装四元数", 0xCB);
        map_name_to_validdatalength.insert("磁强计B安装四元数", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	补码，量纲：2^10;……");
        return 1;
    }
    if (aa_string == "数字太阳敏安装四元数")
    {
        telecontrol_frame_up.cmd_code = 0xCC;
        map_name_to_code.insert("数字太阳敏安装四元数", 0xCC);
        map_name_to_validdatalength.insert("数字太阳敏安装四元数", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	补码，量纲：2^10;……");
        return 1;
    }

    if (aa_string == "差分求角速度限幅值")
    {
        telecontrol_frame_up.cmd_code = 0xCD;
        map_name_to_code.insert("差分求角速度限幅值", 0xCD);
        map_name_to_validdatalength.insert("差分求角速度限幅值", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	补码，量纲：2^8;");
        return 1;
    }

    if (aa_string == "差分求角速度限幅值")
    {
        telecontrol_frame_up.cmd_code = 0xCD;
        map_name_to_code.insert("差分求角速度限幅值", 0xCD);
        map_name_to_validdatalength.insert("差分求角速度限幅值", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	补码，量纲：2^8;");
        return 1;
    }

    if (aa_string == "飞轮安装矩阵")
    {
        telecontrol_frame_up.cmd_code = 0xCE;
        map_name_to_code.insert("飞轮安装矩阵", 0xCE);
        map_name_to_validdatalength.insert("飞轮安装矩阵", 24);
        ui.plainTextEdit_reminder->appendPlainText("24字节，DATA1~DATA2	补码，量纲：2^8;……");
        return 1;
    }

    if (aa_string == "光纤陀螺A安装四元数")
    {
        telecontrol_frame_up.cmd_code = 0xCF;
        map_name_to_code.insert("光纤陀螺A安装四元数", 0xCF);
        map_name_to_validdatalength.insert("光纤陀螺A安装四元数", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	补码，量纲：2^13;……");
        return 1;
    }

    if (aa_string == "光纤陀螺B安装四元数")
    {
        telecontrol_frame_up.cmd_code = 0xD0;
        map_name_to_code.insert("光纤陀螺B安装四元数", 0xD0);
        map_name_to_validdatalength.insert("光纤陀螺B安装四元数", 8);
        ui.plainTextEdit_reminder->appendPlainText("8字节，DATA1~DATA2	补码，量纲：2^13;……");
        return 1;
    }

    if (aa_string == "阻尼切换角速度")
    {
        telecontrol_frame_up.cmd_code = 0xD1;
        map_name_to_code.insert("阻尼切换角速度", 0xD1);
        map_name_to_validdatalength.insert("阻尼切换角速度", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	补码，量纲：2^13;单位 °/s");
        return 1;
    }

    if (aa_string == "GNSS输出位置变化界")
    {
        telecontrol_frame_up.cmd_code = 0xD2;
        map_name_to_code.insert("GNSS输出位置变化界", 0xD2);
        map_name_to_validdatalength.insert("GNSS输出位置变化界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	十进制原码表示，无符号，单位：米");
        return 1;
    }

    if (aa_string == "GNSS输出速度变化界")
    {
        telecontrol_frame_up.cmd_code = 0xD3;
        map_name_to_code.insert("GNSS输出速度变化界", 0xD3);
        map_name_to_validdatalength.insert("GNSS输出速度变化界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	十进制原码表示，无符号，单位：米/秒");
        return 1;
    }

    if (aa_string == "GNSS输出位置上界")
    {
        telecontrol_frame_up.cmd_code = 0xD4;
        map_name_to_code.insert("GNSS输出位置上界", 0xD4);
        map_name_to_validdatalength.insert("GNSS输出位置上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	量纲：2^12，无符号，单位：米");
        return 1;
    }

    if (aa_string == "GNSS输出速度上界")
    {
        telecontrol_frame_up.cmd_code = 0xD5;
        map_name_to_code.insert("GNSS输出速度上界", 0xD5);
        map_name_to_validdatalength.insert("GNSS输出速度上界", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	十进制原码表示，无符号，单位：米/秒");
        return 1;
    }

    if (aa_string == "安全模式角速度")
    {
        telecontrol_frame_up.cmd_code = 0xD7;
        map_name_to_code.insert("安全模式角速度", 0xD7);
        map_name_to_validdatalength.insert("安全模式角速度", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	量纲：2^13，无符号，单位：°/s");
        return 1;
    }
    if (aa_string == "设置电推力器工作时间")
    {
        telecontrol_frame_up.cmd_code = 0xD8;
        map_name_to_code.insert("设置电推力器工作时间", 0xD8);
        map_name_to_validdatalength.insert("设置电推力器工作时间", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA1~DATA2	量纲：2^13，无符号，单位：°/s");
        return 1;
    }



    /*-----------------------------电源分系统----------------------------------*/
    telecontrol_frame_up.cmd_style = 0x3a;
    telecontrol_frame_up.single_number = 0x60;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "蓄电池组放电开关通")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("55AA115533ABE1");
        map_name_to_code.insert("蓄电池组放电开关通", 0x01);
        map_name_to_validdatalength.insert("蓄电池组放电开关通", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池组放电开关断")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("55AA115533A694");
        map_name_to_code.insert("蓄电池组放电开关断", 0x02);
        map_name_to_validdatalength.insert("蓄电池组放电开关断", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "硬件过放继电器使能")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        ui.textEdit_validdata->insertPlainText("55AA115533A257");
        map_name_to_code.insert("硬件过放继电器使能", 0x03);
        map_name_to_validdatalength.insert("硬件过放继电器使能", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "硬件过放继电器禁能")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        ui.textEdit_validdata->insertPlainText("55AA115533AF1A");
        map_name_to_code.insert("硬件过放继电器禁能", 0x04);
        map_name_to_validdatalength.insert("硬件过放继电器禁能", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "星箭分离继电器使能")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        ui.textEdit_validdata->insertPlainText("55AA115533A8CD");
        map_name_to_code.insert("星箭分离继电器使能", 0x05);
        map_name_to_validdatalength.insert("星箭分离继电器使能", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "星箭分离继电器禁能")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        ui.textEdit_validdata->insertPlainText("55AA115533A9B7");
        map_name_to_code.insert("星箭分离继电器禁能", 0x06);
        map_name_to_validdatalength.insert("星箭分离继电器禁能", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池单体采样继电器使能")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        ui.textEdit_validdata->insertPlainText("55AA115533A56A");
        map_name_to_code.insert("蓄电池单体采样继电器使能", 0x07);
        map_name_to_validdatalength.insert("蓄电池单体采样继电器使能", 7);
        ui.plainTextEdit_reminder->appendPlainText("6字节，3个补码形式！");
        return 1;
    }
    if (aa_string == "蓄电池单体采样继电器禁能")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        ui.textEdit_validdata->insertPlainText("55AA115533A12D");
        map_name_to_code.insert("蓄电池单体采样继电器禁能", 0x08);
        map_name_to_validdatalength.insert("蓄电池单体采样继电器禁能", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "UXB- A断B通")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        ui.textEdit_validdata->insertPlainText("55AA115533ACF1");
        map_name_to_code.insert("UXB- A断B通", 0x09);
        map_name_to_validdatalength.insert("UXB- A断B通", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");

        return 1;
    }
    if (aa_string == "UXB- A通B断")
    {
        telecontrol_frame_up.cmd_code = 0x0a;
        ui.textEdit_validdata->insertPlainText("55AA115533AE84");
        map_name_to_code.insert("UXB- A通B断", 0x0a);
        map_name_to_validdatalength.insert("UXB- A通B断", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，3个补码形式！");
        return 1;
    }
    if (aa_string == "UXB- A通B通")
    {
        telecontrol_frame_up.cmd_code = 0x0b;
        ui.textEdit_validdata->insertPlainText("55AA115533AD5B");
        map_name_to_code.insert("UXB- A通B通", 0x0b);
        map_name_to_validdatalength.insert("UXB- A通B通", 7);
        ui.plainTextEdit_reminder->appendPlainText("6字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "太阳电池阵1分流控制开")
    {
        telecontrol_frame_up.cmd_code = 0x0c;
        ui.textEdit_validdata->insertPlainText("5A5573AA275511");
        map_name_to_code.insert("太阳电池阵1分流控制开", 0x0c);
        map_name_to_validdatalength.insert("太阳电池阵1分流控制开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "太阳电池阵1分流控制关")
    {
        telecontrol_frame_up.cmd_code = 0x0d;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA11");
        map_name_to_code.insert("太阳电池阵1分流控制关", 0x0d);
        map_name_to_validdatalength.insert("太阳电池阵1分流控制关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "太阳电池阵2分流控制开")
    {
        telecontrol_frame_up.cmd_code = 0x0e;
        ui.textEdit_validdata->insertPlainText("5A5573AA275511");
        map_name_to_code.insert("太阳电池阵2分流控制开", 0x0e);
        map_name_to_validdatalength.insert("太阳电池阵2分流控制开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }  
    if (aa_string == "太阳电池阵2分流控制关")
    {
        telecontrol_frame_up.cmd_code = 0x0f;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA22");
        map_name_to_code.insert("太阳电池阵2分流控制关", 0x0f);
        map_name_to_validdatalength.insert("太阳电池阵2分流控制关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "太阳电池阵3分流控制开")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        ui.textEdit_validdata->insertPlainText("5A5573AA275522");
        map_name_to_code.insert("太阳电池阵3分流控制开", 0x10);
        map_name_to_validdatalength.insert("太阳电池阵3分流控制开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "太阳电池阵3分流控制关")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA33");
        map_name_to_code.insert("太阳电池阵3分流控制关", 0x11);
        map_name_to_validdatalength.insert("太阳电池阵3分流控制关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "太阳电池阵4分流控制开")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        ui.textEdit_validdata->insertPlainText("5A5573AA275533");
        map_name_to_code.insert("太阳电池阵4分流控制开", 0x12);
        map_name_to_validdatalength.insert("太阳电池阵4分流控制开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "太阳电池阵4分流控制关")
    {
        telecontrol_frame_up.cmd_code = 0x13;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA44");
        map_name_to_code.insert("太阳电池阵4分流控制关", 0x13);
        map_name_to_validdatalength.insert("太阳电池阵4分流控制关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "蓄电池单体1均衡控制开")
    {
        telecontrol_frame_up.cmd_code = 0x14;
        ui.textEdit_validdata->insertPlainText("5A5573AA275544");
        map_name_to_code.insert("蓄电池单体1均衡控制开", 0x14);
        map_name_to_validdatalength.insert("蓄电池单体1均衡控制开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池单体1均衡控制关")
    {
        telecontrol_frame_up.cmd_code = 0x15;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA55");
        map_name_to_code.insert("蓄电池单体1均衡控制关", 0x15);
        map_name_to_validdatalength.insert("蓄电池单体1均衡控制关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "蓄电池单体2均衡控制开")
    {
        telecontrol_frame_up.cmd_code = 0x16;
        ui.textEdit_validdata->insertPlainText("5A5573AA275511");
        map_name_to_code.insert("蓄电池单体2均衡控制开", 0x16);
        map_name_to_validdatalength.insert("蓄电池单体2均衡控制开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池单体2均衡控制关")
    {
        telecontrol_frame_up.cmd_code = 0x17;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA55");
        map_name_to_code.insert("蓄电池单体2均衡控制关", 0x17);
        map_name_to_validdatalength.insert("蓄电池单体2均衡控制关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池单体3均衡控制开")
    {
        telecontrol_frame_up.cmd_code = 0x18;
        ui.textEdit_validdata->insertPlainText("5A5573AA275566");
        map_name_to_code.insert("蓄电池单体3均衡控制开", 0x18);
        map_name_to_validdatalength.insert("蓄电池单体3均衡控制开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池单体3均衡控制关")
    {
        telecontrol_frame_up.cmd_code = 0x19;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA66");
        map_name_to_code.insert("蓄电池单体3均衡控制关", 0x19);
        map_name_to_validdatalength.insert("蓄电池单体3均衡控制关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池单体4均衡控制开")
    {
        telecontrol_frame_up.cmd_code = 0x1a;
        ui.textEdit_validdata->insertPlainText("5A5573AA275577");
        map_name_to_code.insert("蓄电池单体4均衡控制开", 0x1a);
        map_name_to_validdatalength.insert("蓄电池单体4均衡控制开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池单体4均衡控制关")
    {
        telecontrol_frame_up.cmd_code = 0x1b;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA77");
        map_name_to_code.insert("蓄电池单体4均衡控制关", 0x1b);
        map_name_to_validdatalength.insert("蓄电池单体4均衡控制关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "蓄电池加热片A供电开")
    {
        telecontrol_frame_up.cmd_code = 0x1c;
        ui.textEdit_validdata->insertPlainText("5A5573AA275588");
        map_name_to_code.insert("蓄电池加热片A供电开", 0x1c);
        map_name_to_validdatalength.insert("蓄电池加热片A供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池加热片A供电关")
    {
        telecontrol_frame_up.cmd_code = 0x1d;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA88");
        map_name_to_code.insert("蓄电池加热片A供电关", 0x1d);
        map_name_to_validdatalength.insert("蓄电池加热片A供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }


    if (aa_string == "蓄电池加热片B供电开")
    {
        telecontrol_frame_up.cmd_code = 0x1e;
        ui.textEdit_validdata->insertPlainText("5A5573AA275599");
        map_name_to_code.insert("蓄电池加热片B供电开", 0x1e);
        map_name_to_validdatalength.insert("蓄电池加热片B供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "蓄电池加热片B供电关")
    {
        telecontrol_frame_up.cmd_code = 0x1f;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AA99");
        map_name_to_code.insert("蓄电池加热片B供电关", 0x1f);
        map_name_to_validdatalength.insert("蓄电池加热片B供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "备份+5V电源开")
    {
        telecontrol_frame_up.cmd_code = 0x20;
        ui.textEdit_validdata->insertPlainText("5A5573AA2755AA");
        map_name_to_code.insert("备份+5V电源开", 0x20);
        map_name_to_validdatalength.insert("备份+5V电源开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "备份+5V电源关")
    {
        telecontrol_frame_up.cmd_code = 0x21;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AAAA");
        map_name_to_code.insert("备份+5V电源关", 0x21);
        map_name_to_validdatalength.insert("备份+5V电源关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "备份+12V电源开")
    {
        telecontrol_frame_up.cmd_code = 0x22;
        ui.textEdit_validdata->insertPlainText("5A5573AA2755BB");
        map_name_to_code.insert("备份+12V电源开", 0x22);
        map_name_to_validdatalength.insert("备份+12V电源开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "备份+5V电源关")
    {
        telecontrol_frame_up.cmd_code = 0x23;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AABB");
        map_name_to_code.insert("备份+5V电源关", 0x23);
        map_name_to_validdatalength.insert("备份+5V电源关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "AMP/姿控供电开")
    {
        telecontrol_frame_up.cmd_code = 0x24;
        ui.textEdit_validdata->insertPlainText("5A5573AA2755CC");
        map_name_to_code.insert("AMP/姿控供电开", 0x24);
        map_name_to_validdatalength.insert("AMP/姿控供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "AMP/姿控供电关")
    {
        telecontrol_frame_up.cmd_code = 0x25;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AACC");
        map_name_to_code.insert("AMP/姿控供电关", 0x25);
        map_name_to_validdatalength.insert("AMP/姿控供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "整星加热片A供电开")
    {
        telecontrol_frame_up.cmd_code = 0x26;
        ui.textEdit_validdata->insertPlainText("5A5573AA2755DD");
        map_name_to_code.insert("整星加热片A供电开", 0x26);
        map_name_to_validdatalength.insert("整星加热片A供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "整星加热片A供电关")
    {
        telecontrol_frame_up.cmd_code = 0x27;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AADD");
        map_name_to_code.insert("整星加热片A供电关", 0x27);
        map_name_to_validdatalength.insert("整星加热片A供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "整星加热片B供电开")
    {
        telecontrol_frame_up.cmd_code = 0x28;
        ui.textEdit_validdata->insertPlainText("5A5573AA2755EE");
        map_name_to_code.insert("整星加热片B供电开", 0x28);
        map_name_to_validdatalength.insert("整星加热片B供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "整星加热片B供电关")
    {
        telecontrol_frame_up.cmd_code = 0x29;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AAEE");
        map_name_to_code.insert("整星加热片B供电关", 0x29);
        map_name_to_validdatalength.insert("整星加热片B供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "整星加热片C供电开")
    {
        telecontrol_frame_up.cmd_code = 0x2A;
        ui.textEdit_validdata->insertPlainText("5A5573AA2755FF");
        map_name_to_code.insert("整星加热片C供电开", 0x2A);
        map_name_to_validdatalength.insert("整星加热片C供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "整星加热片C供电关")
    {
        telecontrol_frame_up.cmd_code = 0x2B;
        ui.textEdit_validdata->insertPlainText("5A5573AA27AAFF");
        map_name_to_code.insert("整星加热片C供电关", 0x2B);
        map_name_to_validdatalength.insert("整星加热片C供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "整星加热片D供电开")
    {
        telecontrol_frame_up.cmd_code = 0x2C;
        ui.textEdit_validdata->insertPlainText("5A5573AA274411");
        map_name_to_code.insert("整星加热片D供电开", 0x2C);
        map_name_to_validdatalength.insert("整星加热片D供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "整星加热片D供电关")
    {
        telecontrol_frame_up.cmd_code = 0x2D;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB11");
        map_name_to_code.insert("整星加热片D供电关", 0x2D);
        map_name_to_validdatalength.insert("整星加热片D供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "太阳电池阵A展开热刀开")
    {
        telecontrol_frame_up.cmd_code = 0x2E;
        ui.textEdit_validdata->insertPlainText("5A5573AA274422");
        map_name_to_code.insert("太阳电池阵A展开热刀开", 0x2E);
        map_name_to_validdatalength.insert("太阳电池阵A展开热刀开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "太阳电池阵A展开热刀关")
    {
        telecontrol_frame_up.cmd_code = 0x2F;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB22");
        map_name_to_code.insert("太阳电池阵A展开热刀关", 0x2F);
        map_name_to_validdatalength.insert("太阳电池阵A展开热刀关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "太阳电池阵B展开热刀开")
    {
        telecontrol_frame_up.cmd_code = 0x30;
        ui.textEdit_validdata->insertPlainText("5A5573AA274433");
        map_name_to_code.insert("太阳电池阵B展开热刀开", 0x30);
        map_name_to_validdatalength.insert("太阳电池阵B展开热刀开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "太阳电池阵B展开热刀关")
    {
        telecontrol_frame_up.cmd_code = 0x31;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB33");
        map_name_to_code.insert("太阳电池阵B展开热刀关", 0x31);
        map_name_to_validdatalength.insert("太阳电池阵B展开热刀关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "飞轮X供电开")
    {
        telecontrol_frame_up.cmd_code = 0x32;
        ui.textEdit_validdata->insertPlainText("5A5573AA274444");
        map_name_to_code.insert("飞轮X供电开", 0x32);
        map_name_to_validdatalength.insert("飞轮X供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "飞轮X供电关")
    {
        telecontrol_frame_up.cmd_code = 0x33;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB44");
        map_name_to_code.insert("飞轮X供电关", 0x33);
        map_name_to_validdatalength.insert("飞轮X供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "飞轮Y供电开")
    {
        telecontrol_frame_up.cmd_code = 0x34;
        ui.textEdit_validdata->insertPlainText("5A5573AA274455");
        map_name_to_code.insert("飞轮Y供电开", 0x34);
        map_name_to_validdatalength.insert("飞轮Y供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "飞轮Y供电关")
    {
        telecontrol_frame_up.cmd_code = 0x35;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB55");
        map_name_to_code.insert("飞轮Y供电关", 0x35);
        map_name_to_validdatalength.insert("飞轮Y供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "飞轮Z供电开")
    {
        telecontrol_frame_up.cmd_code = 0x36;
        ui.textEdit_validdata->insertPlainText("5A5573AA274466");
        map_name_to_code.insert("飞轮Z供电开", 0x36);
        map_name_to_validdatalength.insert("飞轮Z供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "飞轮Z供电关")
    {
        telecontrol_frame_up.cmd_code = 0x37;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB66");
        map_name_to_code.insert("飞轮Z供电关", 0x37);
        map_name_to_validdatalength.insert("飞轮Z供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "飞轮S供电开")
    {
        telecontrol_frame_up.cmd_code = 0x38;
        ui.textEdit_validdata->insertPlainText("5A5573AA274477");
        map_name_to_code.insert("飞轮S供电开", 0x38);
        map_name_to_validdatalength.insert("飞轮S供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "飞轮S供电关")
    {
        telecontrol_frame_up.cmd_code = 0x39;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB77");
        map_name_to_code.insert("飞轮S供电关", 0x39);
        map_name_to_validdatalength.insert("飞轮S供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "磁力矩器总供电开")
    {
        telecontrol_frame_up.cmd_code = 0x3a;
        ui.textEdit_validdata->insertPlainText("5A5573AA274488");
        map_name_to_code.insert("磁力矩器总供电开", 0x3a);
        map_name_to_validdatalength.insert("磁力矩器总供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "磁力矩器总供电关")
    {
        telecontrol_frame_up.cmd_code = 0x3b;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB88");
        map_name_to_code.insert("磁力矩器总供电关", 0x3b);
        map_name_to_validdatalength.insert("磁力矩器总供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "电推力器供电开")
    {
        telecontrol_frame_up.cmd_code = 0x3c;
        ui.textEdit_validdata->insertPlainText("5A5573AA274499");
        map_name_to_code.insert("电推力器供电开", 0x3c);
        map_name_to_validdatalength.insert("电推力器供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "电推力器供电关")
    {
        telecontrol_frame_up.cmd_code = 0x3d;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB99");
        map_name_to_code.insert("电推力器供电关", 0x3d);
        map_name_to_validdatalength.insert("电推力器供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "光纤陀螺A供电开")
    {
        telecontrol_frame_up.cmd_code = 0x3e;
        ui.textEdit_validdata->insertPlainText("5A5573AA2744AA");
        map_name_to_code.insert("光纤陀螺A供电开", 0x3e);
        map_name_to_validdatalength.insert("光纤陀螺A供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "光纤陀螺A供电关")
    {
        telecontrol_frame_up.cmd_code = 0x3f;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BBAA");
        map_name_to_code.insert("光纤陀螺A供电关", 0x3f);
        map_name_to_validdatalength.insert("光纤陀螺A供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "光纤陀螺B供电开")
    {
        telecontrol_frame_up.cmd_code = 0x40;
        ui.textEdit_validdata->insertPlainText("5A5573AA2744BB");
        map_name_to_code.insert("光纤陀螺B供电开", 0x40);
        map_name_to_validdatalength.insert("光纤陀螺B供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "光纤陀螺B供电关")
    {
        telecontrol_frame_up.cmd_code = 0x41;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BBBB");
        map_name_to_code.insert("光纤陀螺B供电关", 0x41);
        map_name_to_validdatalength.insert("光纤陀螺B供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "光纤陀螺C供电开")
    {
        telecontrol_frame_up.cmd_code = 0x42;
        ui.textEdit_validdata->insertPlainText("5A5573AA2744CC");
        map_name_to_code.insert("光纤陀螺C供电开", 0x42);
        map_name_to_validdatalength.insert("光纤陀螺C供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "光纤陀螺C供电关")
    {
        telecontrol_frame_up.cmd_code = 0x43;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BBCC");
        map_name_to_code.insert("光纤陀螺C供电关", 0x43);
        map_name_to_validdatalength.insert("光纤陀螺C供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏感器A供电开")
    {
        telecontrol_frame_up.cmd_code = 0x44;
        ui.textEdit_validdata->insertPlainText("5A5573AA2744DD");
        map_name_to_code.insert("星敏感器A供电开", 0x44);
        map_name_to_validdatalength.insert("星敏感器A供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "星敏感器A供电关")
    {
        telecontrol_frame_up.cmd_code = 0x45;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BBDD");
        map_name_to_code.insert("星敏感器A供电关", 0x45);
        map_name_to_validdatalength.insert("星敏感器A供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏感器B供电开")
    {
        telecontrol_frame_up.cmd_code = 0x46;
        ui.textEdit_validdata->insertPlainText("5A5573AA2744EE");
        map_name_to_code.insert("星敏感器B供电开", 0x46);
        map_name_to_validdatalength.insert("星敏感器B供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "星敏感器B供电关")
    {
        telecontrol_frame_up.cmd_code = 0x47;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BBEE");
        map_name_to_code.insert("星敏感器B供电关", 0x47);
        map_name_to_validdatalength.insert("星敏感器B供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "星敏感器C供电开")
    {
        telecontrol_frame_up.cmd_code = 0x48;
        ui.textEdit_validdata->insertPlainText("5A5573AA2744FF");
        map_name_to_code.insert("星敏感器C供电开", 0x48);
        map_name_to_validdatalength.insert("星敏感器C供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "星敏感器C供电关")
    {
        telecontrol_frame_up.cmd_code = 0x49;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BBFF");
        map_name_to_code.insert("星敏感器C供电关", 0x49);
        map_name_to_validdatalength.insert("星敏感器C供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "数字太阳敏感器供电开")
    {
        telecontrol_frame_up.cmd_code = 0x4a;
        ui.textEdit_validdata->insertPlainText("5A5573AA273311");
        map_name_to_code.insert("数字太阳敏感器供电开", 0x4a);
        map_name_to_validdatalength.insert("数字太阳敏感器供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "数字太阳敏感器供电关")
    {
        telecontrol_frame_up.cmd_code = 0x4b;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC11");
        map_name_to_code.insert("数字太阳敏感器供电关", 0x4b);
        map_name_to_validdatalength.insert("数字太阳敏感器供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "磁强计A供电开")
    {
        telecontrol_frame_up.cmd_code = 0x4c;
        ui.textEdit_validdata->insertPlainText("5A5573AA273322");
        map_name_to_code.insert("磁强计A供电开", 0x4c);
        map_name_to_validdatalength.insert("磁强计A供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "磁强计A供电关")
    {
        telecontrol_frame_up.cmd_code = 0x4d;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC22");
        map_name_to_code.insert("磁强计A供电关", 0x4d);
        map_name_to_validdatalength.insert("磁强计A供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "磁强计B供电开")
    {
        telecontrol_frame_up.cmd_code = 0x4e;
        ui.textEdit_validdata->insertPlainText("5A5573AA273333");
        map_name_to_code.insert("磁强计B供电开", 0x4e);
        map_name_to_validdatalength.insert("磁强计B供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "磁强计B供电关")
    {
        telecontrol_frame_up.cmd_code = 0x4f;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC33");
        map_name_to_code.insert("磁强计B供电关", 0x4f);
        map_name_to_validdatalength.insert("磁强计B供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "UV通信机供电开")
    {
        telecontrol_frame_up.cmd_code = 0x50;
        ui.textEdit_validdata->insertPlainText("5A5573AA273344");
        map_name_to_code.insert("UV通信机供电开", 0x50);
        map_name_to_validdatalength.insert("UV通信机供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "UV通信机供电关")
    {
        telecontrol_frame_up.cmd_code = 0x51;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC44");
        map_name_to_code.insert("UV通信机供电关", 0x51);
        map_name_to_validdatalength.insert("UV通信机供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "X相控阵单元供电开")
    {
        telecontrol_frame_up.cmd_code = 0x52;
        ui.textEdit_validdata->insertPlainText("5A5573AA273355");
        map_name_to_code.insert("X相控阵单元供电开", 0x52);
        map_name_to_validdatalength.insert("X相控阵单元供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "X相控阵单元供电关")
    {
        telecontrol_frame_up.cmd_code = 0x53;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC55");
        map_name_to_code.insert("X相控阵单元供电关", 0x53);
        map_name_to_validdatalength.insert("X相控阵单元供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "DCS载荷供电开")
    {
        telecontrol_frame_up.cmd_code = 0x54;
        ui.textEdit_validdata->insertPlainText("5A5573AA273366");
        map_name_to_code.insert("DCS载荷供电开", 0x54);
        map_name_to_validdatalength.insert("DCS载荷供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DCS载荷供电关")
    {
        telecontrol_frame_up.cmd_code = 0x55;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC66");
        map_name_to_code.insert("DCS载荷供电关", 0x55);
        map_name_to_validdatalength.insert("DCS载荷供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "DCS天线展开热刀开")
    {
        telecontrol_frame_up.cmd_code = 0x56;
        ui.textEdit_validdata->insertPlainText("5A5573AA273377");
        map_name_to_code.insert("DCS天线展开热刀开", 0x56);
        map_name_to_validdatalength.insert("DCS天线展开热刀开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DCS天线展开热刀关")
    {
        telecontrol_frame_up.cmd_code = 0x57;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC77");
        map_name_to_code.insert("DCS天线展开热刀关", 0x57);
        map_name_to_validdatalength.insert("DCS天线展开热刀关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "U天线展开热刀开")
    {
        telecontrol_frame_up.cmd_code = 0x58;
        ui.textEdit_validdata->insertPlainText("5A5573AA273388");
        map_name_to_code.insert("U天线展开热刀开", 0x58);
        map_name_to_validdatalength.insert("U天线展开热刀开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "U天线展开热刀关")
    {
        telecontrol_frame_up.cmd_code = 0x59;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC88");
        map_name_to_code.insert("U天线展开热刀关", 0x59);
        map_name_to_validdatalength.insert("U天线展开热刀关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "V天线展开热刀开")
    {
        telecontrol_frame_up.cmd_code = 0x5A;
        ui.textEdit_validdata->insertPlainText("5A5573AA273399");
        map_name_to_code.insert("V天线展开热刀开", 0x5A);
        map_name_to_validdatalength.insert("V天线展开热刀开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "V天线展开热刀关")
    {
        telecontrol_frame_up.cmd_code = 0x5B;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CC99");
        map_name_to_code.insert("V天线展开热刀关", 0x5B);
        map_name_to_validdatalength.insert("V天线展开热刀关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "GNSS掩星观测单元供电开")
    {
        telecontrol_frame_up.cmd_code = 0x5C;
        ui.textEdit_validdata->insertPlainText("5A5573AA2733AA");
        map_name_to_code.insert("GNSS掩星观测单元供电开", 0x5C);
        map_name_to_validdatalength.insert("GNSS掩星观测单元供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "GNSS掩星观测单元供电关")
    {
        telecontrol_frame_up.cmd_code = 0x5D;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CCAA");
        map_name_to_code.insert("GNSS掩星观测单元供电关", 0x5D);
        map_name_to_validdatalength.insert("GNSS掩星观测单元供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }


    if (aa_string == "数据压缩处理单元A供电开")
    {
        telecontrol_frame_up.cmd_code = 0x5E;
        ui.textEdit_validdata->insertPlainText("5A5573AA2733BB");
        map_name_to_code.insert("数据压缩处理单元A供电开", 0x5E);
        map_name_to_validdatalength.insert("数据压缩处理单元A供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "数据压缩处理单元A供电关")
    {
        telecontrol_frame_up.cmd_code = 0x5F;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CCBB");
        map_name_to_code.insert("数据压缩处理单元A供电关", 0x5F);
        map_name_to_validdatalength.insert("数据压缩处理单元A供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "数据压缩处理单元B供电开")
    {
        telecontrol_frame_up.cmd_code = 0x60;
        ui.textEdit_validdata->insertPlainText("5A5573AA2733CC");
        map_name_to_code.insert("数据压缩处理单元B供电开", 0x60);
        map_name_to_validdatalength.insert("数据压缩处理单元B供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "数据压缩处理单元B供电关")
    {
        telecontrol_frame_up.cmd_code = 0x61;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CCCC");
        map_name_to_code.insert("数据压缩处理单元B供电关", 0x61);
        map_name_to_validdatalength.insert("数据压缩处理单元B供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "高分摄像机供电开")
    {
        telecontrol_frame_up.cmd_code = 0x62;
        ui.textEdit_validdata->insertPlainText("5A5573AA2733DD");
        map_name_to_code.insert("高分摄像机供电开", 0x62);
        map_name_to_validdatalength.insert("高分摄像机供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "高分摄像机供电关")
    {
        telecontrol_frame_up.cmd_code = 0x63;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CCDD");
        map_name_to_code.insert("高分摄像机供电关", 0x63);
        map_name_to_validdatalength.insert("高分摄像机供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "低分摄像机供电开")
    {
        telecontrol_frame_up.cmd_code = 0x64;
        ui.textEdit_validdata->insertPlainText("5A5573AA2733EE");
        map_name_to_code.insert("低分摄像机供电开", 0x64);
        map_name_to_validdatalength.insert("低分摄像机供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "低分摄像机供电关")
    {
        telecontrol_frame_up.cmd_code = 0x65;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CCEE");
        map_name_to_code.insert("低分摄像机供电关", 0x65);
        map_name_to_validdatalength.insert("低分摄像机供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "广角摄像机供电开")
    {
        telecontrol_frame_up.cmd_code = 0x66;
        ui.textEdit_validdata->insertPlainText("5A5573AA2733FF");
        map_name_to_code.insert("广角摄像机供电开", 0x66);
        map_name_to_validdatalength.insert("广角摄像机供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "广角摄像机供电关")
    {
        telecontrol_frame_up.cmd_code = 0x67;
        ui.textEdit_validdata->insertPlainText("5A5573AA27CCFF");
        map_name_to_code.insert("广角摄像机供电关", 0x67);
        map_name_to_validdatalength.insert("广角摄像机供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "红外摄像机供电开")
    {
        telecontrol_frame_up.cmd_code = 0x68;
        ui.textEdit_validdata->insertPlainText("5A5573AA272211");
        map_name_to_code.insert("红外摄像机供电开", 0x68);
        map_name_to_validdatalength.insert("红外摄像机供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "红外摄像机供电关")
    {
        telecontrol_frame_up.cmd_code = 0x69;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD11");
        map_name_to_code.insert("红外摄像机供电关", 0x69);
        map_name_to_validdatalength.insert("红外摄像机供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "对焦机构A供电开")
    {
        telecontrol_frame_up.cmd_code = 0x6a;
        ui.textEdit_validdata->insertPlainText("5A5573AA272222");
        map_name_to_code.insert("对焦机构A供电开", 0x6a);
        map_name_to_validdatalength.insert("对焦机构A供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "对焦机构A供电关")
    {
        telecontrol_frame_up.cmd_code = 0x6b;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD22");
        map_name_to_code.insert("对焦机构A供电关", 0x6b);
        map_name_to_validdatalength.insert("对焦机构A供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "对焦机构B供电开")
    {
        telecontrol_frame_up.cmd_code = 0x6c;
        ui.textEdit_validdata->insertPlainText("5A5573AA272233");
        map_name_to_code.insert("对焦机构B供电开", 0x6c);
        map_name_to_validdatalength.insert("对焦机构B供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "对焦机构B供电关")
    {
        telecontrol_frame_up.cmd_code = 0x6d;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD33");
        map_name_to_code.insert("对焦机构B供电关", 0x6d);
        map_name_to_validdatalength.insert("对焦机构B供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "GNSS接收机A主供电开")
    {
        telecontrol_frame_up.cmd_code = 0x6e;
        ui.textEdit_validdata->insertPlainText("5A5573AA272244");
        map_name_to_code.insert("GNSS接收机A主供电开", 0x6e);
        map_name_to_validdatalength.insert("GNSS接收机A主供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "GNSS接收机A主供电关")
    {
        telecontrol_frame_up.cmd_code = 0x6f;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD44");
        map_name_to_code.insert("GNSS接收机A主供电关", 0x6f);
        map_name_to_validdatalength.insert("GNSS接收机A主供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }


    if (aa_string == "GNSS接收机A备供电开")
    {
        telecontrol_frame_up.cmd_code = 0x70;
        ui.textEdit_validdata->insertPlainText("5A5573AA272255");
        map_name_to_code.insert("GNSS接收机A备供电开", 0x70);
        map_name_to_validdatalength.insert("GNSS接收机A备供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "GNSS接收机A备供电关")
    {
        telecontrol_frame_up.cmd_code = 0x71;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD55");
        map_name_to_code.insert("GNSS接收机A备供电关", 0x71);
        map_name_to_validdatalength.insert("GNSS接收机A备供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "GNSS接收机B供电开")
    {
        telecontrol_frame_up.cmd_code = 0x72;
        ui.textEdit_validdata->insertPlainText("5A5573AA272266");
        map_name_to_code.insert("GNSS接收机B供电开", 0x72);
        map_name_to_validdatalength.insert("GNSS接收机B供电开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "GNSS接收机B供电关")
    {
        telecontrol_frame_up.cmd_code = 0x73;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD66");
        map_name_to_code.insert("GNSS接收机B供电关", 0x74);
        map_name_to_validdatalength.insert("GNSS接收机B供电关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "UXB收发机供电A开B关")
    {
        telecontrol_frame_up.cmd_code = 0x74;
        ui.textEdit_validdata->insertPlainText("5A5573AA27BB33");
        map_name_to_code.insert("UXB收发机供电A开B关", 0x74);
        map_name_to_validdatalength.insert("UXB收发机供电A开B关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "UXB收发机供电B开A关")
    {
        telecontrol_frame_up.cmd_code = 0x75;
        ui.textEdit_validdata->insertPlainText("5A5573AA2733BB");
        map_name_to_code.insert("UXB收发机供电B开A关", 0x75);
        map_name_to_validdatalength.insert("UXB收发机供电B开A关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "UXB收发机供电A开B开")
    {
        telecontrol_frame_up.cmd_code = 0x76;
        ui.textEdit_validdata->insertPlainText("5A5573AA2755AA");
        map_name_to_code.insert("UXB收发机供电B开A关", 0x76);
        map_name_to_validdatalength.insert("UXB收发机供电B开A关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "模拟开关地址选择信号A1开")
    {
        telecontrol_frame_up.cmd_code = 0x77;
        ui.textEdit_validdata->insertPlainText("5A5573AA272277");
        map_name_to_code.insert("模拟开关地址选择信号A1开", 0x77);
        map_name_to_validdatalength.insert("模拟开关地址选择信号A1开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "模拟开关地址选择信号A1关")
    {
        telecontrol_frame_up.cmd_code = 0x78;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD77");
        map_name_to_code.insert("模拟开关地址选择信号A1关", 0x78);
        map_name_to_validdatalength.insert("模拟开关地址选择信号A1关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "模拟开关地址选择信号A2开")
    {
        telecontrol_frame_up.cmd_code = 0x79;
        ui.textEdit_validdata->insertPlainText("5A5573AA272288");
        map_name_to_code.insert("模拟开关地址选择信号A2开", 0x79);
        map_name_to_validdatalength.insert("模拟开关地址选择信号A2开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "模拟开关地址选择信号A2关")
    {
        telecontrol_frame_up.cmd_code = 0x7a;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD88");
        map_name_to_code.insert("模拟开关地址选择信号A2关", 0x7a);
        map_name_to_validdatalength.insert("模拟开关地址选择信号A2关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "模拟开关地址选择信号A3开")
    {
        telecontrol_frame_up.cmd_code = 0x7b;
        ui.textEdit_validdata->insertPlainText("5A5573AA272299");
        map_name_to_code.insert("模拟开关地址选择信号A3开", 0x7b);
        map_name_to_validdatalength.insert("模拟开关地址选择信号A3开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "模拟开关地址选择信号A3关")
    {
        telecontrol_frame_up.cmd_code = 0x7c;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DD99");
        map_name_to_code.insert("模拟开关地址选择信号A3关", 0x7c);
        map_name_to_validdatalength.insert("模拟开关地址选择信号A3关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "模拟开关地址选择信号A4开")
    {
        telecontrol_frame_up.cmd_code = 0x7d;
        ui.textEdit_validdata->insertPlainText("5A5573AA2722AA");
        map_name_to_code.insert("模拟开关地址选择信号A4开", 0x7d);
        map_name_to_validdatalength.insert("模拟开关地址选择信号A4开", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "模拟开关地址选择信号A4关")
    {
        telecontrol_frame_up.cmd_code = 0x7e;
        ui.textEdit_validdata->insertPlainText("5A5573AA27DDAA");
        map_name_to_code.insert("模拟开关地址选择信号A4关", 0x7e);
        map_name_to_validdatalength.insert("模拟开关地址选择信号A4关", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "磁力矩器X控制+")
    {
        telecontrol_frame_up.cmd_code = 0x7f;
        ui.textEdit_validdata->insertPlainText("5A5573AA27");
        map_name_to_code.insert("磁力矩器X控制+", 0x7f);
        map_name_to_validdatalength.insert("磁力矩器X控制+", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节;加 无符号2字节整数，单位0.1ms");
        return 1;
    }
    if (aa_string == "磁力矩器X控制-")
    {
        telecontrol_frame_up.cmd_code = 0x80;
        ui.textEdit_validdata->insertPlainText("5A5573AA27");
        map_name_to_code.insert("磁力矩器X控制-", 0x80);
        map_name_to_validdatalength.insert("磁力矩器X控制-", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节;加 无符号2字节整数，单位0.1ms");
        return 1;
    }

    if (aa_string == "磁力矩器Y控制+")
    {
        telecontrol_frame_up.cmd_code = 0x81;
        ui.textEdit_validdata->insertPlainText("5A5573AA27");
        map_name_to_code.insert("磁力矩器Y控制+", 0x81);
        map_name_to_validdatalength.insert("磁力矩器Y控制+", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节;加 无符号2字节整数，单位0.1ms");
        return 1;
    }
    if (aa_string == "磁力矩器Y控制-")
    {
        telecontrol_frame_up.cmd_code = 0x82;
        ui.textEdit_validdata->insertPlainText("5A5573AA27");
        map_name_to_code.insert("磁力矩器Y控制-", 0x82);
        map_name_to_validdatalength.insert("磁力矩器Y控制-", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节;加 无符号2字节整数，单位0.1ms");
        return 1;
    }

    if (aa_string == "磁力矩器C控制+")
    {
        telecontrol_frame_up.cmd_code = 0x83;
        ui.textEdit_validdata->insertPlainText("5A5573AA27");
        map_name_to_code.insert("磁力矩器C控制+", 0x83);
        map_name_to_validdatalength.insert("磁力矩器C控制+", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节;加 无符号2字节整数，单位0.1ms");
        return 1;
    }
    if (aa_string == "磁力矩器C控制-")
    {
        telecontrol_frame_up.cmd_code = 0x84;
        ui.textEdit_validdata->insertPlainText("5A5573AA27");
        map_name_to_code.insert("磁力矩器C控制-", 0x84);
        map_name_to_validdatalength.insert("磁力矩器C控制-", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节;加 无符号2字节整数，单位0.1ms");
        return 1;
    }

    if (aa_string == "设备组合控制开指令")
    {
        telecontrol_frame_up.cmd_code = 0x85;
        ui.textEdit_validdata->insertPlainText("AA");
        map_name_to_code.insert("设备组合控制开指令", 0x85);
        map_name_to_validdatalength.insert("设备组合控制开指令", 12);
        ui.plainTextEdit_reminder->appendPlainText("12字节;加 组合设备的配电开关");
        return 1;
    }
    if (aa_string == "设备组合控制关指令")
    {
        telecontrol_frame_up.cmd_code = 0x86;
        ui.textEdit_validdata->insertPlainText("AA");
        map_name_to_code.insert("设备组合控制关指令", 0x86);
        map_name_to_validdatalength.insert("设备组合控制关指令", 12);
        ui.plainTextEdit_reminder->appendPlainText("12字节;加 组合设备的配电开关");
        return 1;
    }

    /*-----------------------------UXB测控数传一体机---------------------------------*/
    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x62;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "UXB-A OC14输出")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("UXB-A OC14输出", 0x01);
        map_name_to_validdatalength.insert("UXB-A OC14输出", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A OC15输出")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("UXB-A OC15输出", 0x02);
        map_name_to_validdatalength.insert("UXB-A OC15输出", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-A OC10输出")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("UXB-A OC10输出", 0x03);
        map_name_to_validdatalength.insert("UXB-A OC10输出", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A OC9输出")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("UXB-A OC9输出", 0x04);
        map_name_to_validdatalength.insert("UXB-A OC9输出", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A发射机开")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("UXB-A发射机开", 0x05);
        map_name_to_validdatalength.insert("UXB-A发射机开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A发射机关")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("UXB-A发射机关", 0x06);
        map_name_to_validdatalength.insert("UXB-A发射机关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-A调制开")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("UXB-A调制开", 0x07);
        map_name_to_validdatalength.insert("UXB-A调制开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A调制关")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("UXB-A调制关", 0x08);
        map_name_to_validdatalength.insert("UXB-A调制关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A定时复位开")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("UXB-A定时复位开", 0x09);
        map_name_to_validdatalength.insert("UXB-A定时复位开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A定时复位关")
    {
        telecontrol_frame_up.cmd_code = 0xA;
        map_name_to_code.insert("UXB-A定时复位关", 0xA);
        map_name_to_validdatalength.insert("UXB-A定时复位关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A FPGA复位")
    {
        telecontrol_frame_up.cmd_code = 0xB;
        map_name_to_code.insert("UXB-A FPGA复位", 0xB);
        map_name_to_validdatalength.insert("UUXB-A FPGA复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A切换到测控模式")
    {
        telecontrol_frame_up.cmd_code = 0xC;
        map_name_to_code.insert("UXB-A切换到测控模式", 0xC);
        map_name_to_validdatalength.insert("UXB-A切换到测控模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A切换到数传模式")
    {
        telecontrol_frame_up.cmd_code = 0xd;
        map_name_to_code.insert("UXB-A切换到数传模式", 0xd);
        map_name_to_validdatalength.insert("UXB-A切换到数传模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A数传速率为2Mbps模式")
    {
        telecontrol_frame_up.cmd_code = 0xe;
        map_name_to_code.insert("UXB-A数传速率为2Mbps模式", 0xe);
        map_name_to_validdatalength.insert("UXB-A数传速率为2Mbps模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A数传速率为40Mbps模式")
    {
        telecontrol_frame_up.cmd_code = 0xf;
        map_name_to_code.insert("UXB-A数传速率为40Mbps模式", 0xf);
        map_name_to_validdatalength.insert("UXB-A数传速率为40Mbps模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A数传测控天线通道打开，数传相控阵天线通道关闭")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        map_name_to_code.insert("UXB-A数传测控天线通道打开，数传相控阵天线通道关闭", 0x10);
        map_name_to_validdatalength.insert("UXB-A数传测控天线通道打开，数传相控阵天线通道关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A数传相控阵天线通道打开，数传测控天线通道关闭")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        map_name_to_code.insert("UXB-A数传相控阵天线通道打开，数传测控天线通道关闭", 0x11);
        map_name_to_validdatalength.insert("UXB-A数传相控阵天线通道打开，数传测控天线通道关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A LVDS直传")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        map_name_to_code.insert("UXB-A LVDS直传", 0x12);
        map_name_to_validdatalength.insert("UXB-A LVDS直传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A LVDS缓存后再传")
    {
        telecontrol_frame_up.cmd_code = 0x13;
        map_name_to_code.insert("UXB-A LVDS缓存后再传", 0x13);
        map_name_to_validdatalength.insert("UXB-A LVDS缓存后再传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A数传开")
    {
        telecontrol_frame_up.cmd_code = 0x14;
        map_name_to_code.insert("UXB-A数传开", 0x14);
        map_name_to_validdatalength.insert("UXB-A数传开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A数传关")
    {
        telecontrol_frame_up.cmd_code = 0x15;
        map_name_to_code.insert("UXB-A数传关", 0x15);
        map_name_to_validdatalength.insert("UXB-A数传关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A固存擦除")
    {
        telecontrol_frame_up.cmd_code = 0x16;
        map_name_to_code.insert("UXB-A固存擦除", 0x16);
        map_name_to_validdatalength.insert("UXB-A固存擦除", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A固存存储开始")
    {
        telecontrol_frame_up.cmd_code = 0x17;
        map_name_to_code.insert("UXB-A固存存储开始", 0x17);
        map_name_to_validdatalength.insert("UXB-A固存存储开始", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！发送该指令后必须发送固存存储停止指令");
        return 1;
    }

    if (aa_string == "UXB-A固存存储停止")
    {
        telecontrol_frame_up.cmd_code = 0x18;
        map_name_to_code.insert("UXB-A固存存储停止", 0x18);
        map_name_to_validdatalength.insert("UXB-A固存存储停止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A固存顺序回放")
    {
        telecontrol_frame_up.cmd_code = 0x19;
        map_name_to_code.insert("UXB-A固存顺序回放", 0x19);
        map_name_to_validdatalength.insert("UXB-A固存顺序回放", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！发送该指令后必须发送固存回放停止指令");
        return 1;
    }

    if (aa_string == "UXB-A控制指定地址回放")
    {
        telecontrol_frame_up.cmd_code = 0x1a;
        map_name_to_code.insert("UXB-A控制指定地址回放", 0x1a);
        map_name_to_validdatalength.insert("UXB-A控制指定地址回放", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A固存回放停止")
    {
        telecontrol_frame_up.cmd_code = 0x1b;
        map_name_to_code.insert("UXB-A固存回放停止", 0x1b);
        map_name_to_validdatalength.insert("UXB-A固存回放停止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A写指针备份")
    {
        telecontrol_frame_up.cmd_code = 0x1c;
        map_name_to_code.insert("UXB-A写指针备份", 0x1c);
        map_name_to_validdatalength.insert("UXB-A写指针备份", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-A读指针备份")
    {
        telecontrol_frame_up.cmd_code = 0x1d;
        map_name_to_code.insert("UXB-A读指针备份", 0x1d);
        map_name_to_validdatalength.insert("UXB-A读指针备份", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B14输出")
    {
        telecontrol_frame_up.cmd_code = 0x1e;
        map_name_to_code.insert("UXB-B14输出", 0x1e);
        map_name_to_validdatalength.insert("UXB-B14输出", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B15输出")
    {
        telecontrol_frame_up.cmd_code = 0x1f;
        map_name_to_code.insert("UXB-B15输出", 0x1f);
        map_name_to_validdatalength.insert("UXB-B15输出", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B10输出")
    {
        telecontrol_frame_up.cmd_code = 0x20;
        map_name_to_code.insert("UXB-B10输出", 0x20);
        map_name_to_validdatalength.insert("UXB-B10输出", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B9输出")
    {
        telecontrol_frame_up.cmd_code = 0x21;
        map_name_to_code.insert("UXB-B9输出", 0x21);
        map_name_to_validdatalength.insert("UXB-B9输出", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B发射机开")
    {
        telecontrol_frame_up.cmd_code = 0x22;
        map_name_to_code.insert("UXB-B发射机开", 0x22);
        map_name_to_validdatalength.insert("UXB-B发射机开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B发射机关")
    {
        telecontrol_frame_up.cmd_code = 0x23;
        map_name_to_code.insert("UXB-B发射机关", 0x23);
        map_name_to_validdatalength.insert("UXB-B发射机关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B调制开")
    {
        telecontrol_frame_up.cmd_code = 0x24;
        map_name_to_code.insert("UXB-B调制开", 0x24);
        map_name_to_validdatalength.insert("UXB-B调制开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B调制关")
    {
        telecontrol_frame_up.cmd_code = 0x25;
        map_name_to_code.insert("UXB-B调制关", 0x25);
        map_name_to_validdatalength.insert("UXB-B调制关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B定时复位开")
    {
        telecontrol_frame_up.cmd_code = 0x26;
        map_name_to_code.insert("UXB-B定时复位开", 0x26);
        map_name_to_validdatalength.insert("UXB-B定时复位开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B定时复位关")
    {
        telecontrol_frame_up.cmd_code = 0x27;
        map_name_to_code.insert("UXB-B定时复位关", 0x27);
        map_name_to_validdatalength.insert("UXB-B定时复位关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B FPGA复位")
    {
        telecontrol_frame_up.cmd_code = 0x28;
        map_name_to_code.insert("UXB-B FPGA复位", 0x28);
        map_name_to_validdatalength.insert("UXB-B FPGA复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B切换到测控模式")
    {
        telecontrol_frame_up.cmd_code = 0x29;
        map_name_to_code.insert("UXB-B切换到测控模式", 0x29);
        map_name_to_validdatalength.insert("UXB-B切换到测控模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-B切换到数传模式")
    {
        telecontrol_frame_up.cmd_code = 0x2a;
        map_name_to_code.insert("UXB-B切换到数传模式", 0x2a);
        map_name_to_validdatalength.insert("UXB-B切换到数传模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B数传速率为2Mbps模式")
    {
        telecontrol_frame_up.cmd_code = 0x2b;
        map_name_to_code.insert("UXB-B数传速率为2Mbps模式", 0x2b);
        map_name_to_validdatalength.insert("UXB-B数传速率为2Mbps模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-B数传速率为40Mbps模式")
    {
        telecontrol_frame_up.cmd_code = 0x2c;
        map_name_to_code.insert("UXB-B数传速率为40Mbps模式", 0x2c);
        map_name_to_validdatalength.insert("UXB-B数传速率为40Mbps模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B数传测控天线通道打开，数传相控阵天线通道关闭")
    {
        telecontrol_frame_up.cmd_code = 0x2d;
        map_name_to_code.insert("UXB-B数传测控天线通道打开，数传相控阵天线通道关闭", 0x2d);
        map_name_to_validdatalength.insert("UXB-B数传测控天线通道打开，数传相控阵天线通道关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-B数传相控阵天线通道打开，数传测控天线通道关闭")
    {
        telecontrol_frame_up.cmd_code = 0x2e;
        map_name_to_code.insert("UXB-B数传相控阵天线通道打开，数传测控天线通道关闭", 0x2e);
        map_name_to_validdatalength.insert("UXB-B数传相控阵天线通道打开，数传测控天线通道关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B LVDS直传")
    {
        telecontrol_frame_up.cmd_code = 0x2f;
        map_name_to_code.insert("UXB-B LVDS直传", 0x2f);
        map_name_to_validdatalength.insert("UXB-B LVDS直传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-B LVDS缓存后再传")
    {
        telecontrol_frame_up.cmd_code = 0x30;
        map_name_to_code.insert("UXB-B LVDS缓存后再传", 0x30);
        map_name_to_validdatalength.insert("UXB-B LVDS缓存后再传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B数传开")
    {
        telecontrol_frame_up.cmd_code = 0x31;
        map_name_to_code.insert("UXB-B数传开", 0x31);
        map_name_to_validdatalength.insert("UXB-B数传开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-B数传关")
    {
        telecontrol_frame_up.cmd_code = 0x32;
        map_name_to_code.insert("UXB-B数传关", 0x32);
        map_name_to_validdatalength.insert("UXB-B数传关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B固存擦除")
    {
        telecontrol_frame_up.cmd_code = 0x33;
        map_name_to_code.insert("UXB-B固存擦除", 0x33);
        map_name_to_validdatalength.insert("UXB-B固存擦除", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-B固存存储开始")
    {
        telecontrol_frame_up.cmd_code = 0x34;
        map_name_to_code.insert("UXB-B固存存储开始", 0x34);
        map_name_to_validdatalength.insert("UXB-B固存存储开始", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！发送该指令后必须发送固存存储停止指令");
        return 1;
    }

    if (aa_string == "UXB-B固存存储停止")
    {
        telecontrol_frame_up.cmd_code = 0x35;
        map_name_to_code.insert("UXB-B固存存储停止", 0x35);
        map_name_to_validdatalength.insert("UXB-B固存存储停止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-B固存顺序回放")
    {
        telecontrol_frame_up.cmd_code = 0x36;
        map_name_to_code.insert("UXB-B固存顺序回放", 0x36);
        map_name_to_validdatalength.insert("UXB-B固存顺序回放", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！发送该指令后必须发送固存回放停止指令");
        return 1;
    }

    if (aa_string == "UXB-B控制指定地址回放")
    {
        telecontrol_frame_up.cmd_code = 0x37;
        map_name_to_code.insert("UXB-B控制指定地址回放", 0x37);
        map_name_to_validdatalength.insert("UXB-B控制指定地址回放", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "UXB-B固存回放停止")
    {
        telecontrol_frame_up.cmd_code = 0x38;
        map_name_to_code.insert("UXB-B固存回放停止", 0x38);
        map_name_to_validdatalength.insert("UXB-B固存回放停止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "UXB-B写指针备份")
    {
        telecontrol_frame_up.cmd_code = 0x39;
        map_name_to_code.insert("UXB-B写指针备份", 0x39);
        map_name_to_validdatalength.insert("UXB-B写指针备份", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "XB-B读指针备份")
    {
        telecontrol_frame_up.cmd_code = 0x40;
        map_name_to_code.insert("XB-B读指针备份", 0x40);
        map_name_to_validdatalength.insert("XB-B读指针备份", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    /*----------------------VU通信机-------------------------------------*/
    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x61;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;

    if (aa_string == "发射机开")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("发射机开", 0x01);
        map_name_to_validdatalength.insert("发射机开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "发射机关")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("发射机关", 0x02);
        map_name_to_validdatalength.insert("发射机关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "调制开")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("调制开", 0x03);
        map_name_to_validdatalength.insert("调制开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "调制关")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("调制关", 0x04);
        map_name_to_validdatalength.insert("调制关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "定时复位开")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("定时复位开", 0x05);
        map_name_to_validdatalength.insert("定时复位开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "定时复位关")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("定时复位关", 0x06);
        map_name_to_validdatalength.insert("调制关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "FPGA复位")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("FPGA复位", 0x07);
        map_name_to_validdatalength.insert("FPGA复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    if (aa_string == "信标模式关")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("信标模式关", 0x08);
        map_name_to_validdatalength.insert("信标模式关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }
    if (aa_string == "信标模式开")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("信标模式开", 0x09);
        map_name_to_validdatalength.insert("信标模式开", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2，其他值：默认！");
        return 1;
    }

    /*----------------------X相控阵天线-------------------------------------*/
    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x63;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "全阵待机")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("全阵待机", 0x01);
        map_name_to_validdatalength.insert("全阵待机", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "正常工作")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("正常工作", 0x02);
        map_name_to_validdatalength.insert("正常工作", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA1	离轴角\
            16位无符号定点数，量化单位为0.1°，表示范围0°~360°;DATA2~DATA3	旋转角\
            16位无符号定点数，量化单位为0.1°，表示范围0°~360°！");
        return 1;
    }

    /*----------------------GNSS接收机A-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x64;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "定轨禁止")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("定轨禁止", 0x01);
        map_name_to_validdatalength.insert("定轨禁止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "定轨允许")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("02");
        map_name_to_code.insert("定轨允许", 0x02);
        map_name_to_validdatalength.insert("定轨允许", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "查询原始观测数据")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("查询原始观测数据", 0x03);
        map_name_to_validdatalength.insert("查询原始观测数据", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：UXB测控数传一体机A；\
            AAH：UXB测控数传一体机B；\
            BBH：VU通信机；\
            CCH：DCS测控；\
            DDH：地测CAN通道；\
            其他值：同当前遥控指令发送通道。！");
        return 1;
    }

    /*----------------------数字太阳敏感器-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x70;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "设置正常工作模式")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("设置正常工作模式", 0x01);
        map_name_to_validdatalength.insert("设置正常工作模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "设置参数调整模式")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("02");
        map_name_to_code.insert("设置参数调整模式", 0x02);
        map_name_to_validdatalength.insert("设置参数调整模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "工程参数设置")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("工程参数设置", 0x03);
        map_name_to_validdatalength.insert("工程参数设置", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：太敏参数！设置太敏工程参数，且只在参数调整模式下有效");
        return 1;
    }

    /*----------------------星敏感器A-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x71;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "切换工作模式")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("切换工作模式", 0x01);
        map_name_to_validdatalength.insert("切换工作模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x00：为切换应用模式参数；\
            0xFF：为切换优化模式参数。！");
        return 1;
    }

    if (aa_string == "内部参数调整")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("内部参数调整", 0x02);
        map_name_to_validdatalength.insert("内部参数调整", 5);
        ui.plainTextEdit_reminder->appendPlainText("5字节，DATA0：命令参数\
            DATA1~DATA4：内部参数！");
        return 1;
    }

    if (aa_string == "自检指令")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("自检指令", 0x03);
        map_name_to_validdatalength.insert("自检指令",1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：UXB测控数传一体机A；\
            AAH：UXB测控数传一体机B；\
            BBH：VU通信机；\
            CCH：DCS测控；\
            DDH：地测CAN通道。！");
        return 1;
    }

    if (aa_string == "控制参数获取")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("控制参数获取", 0x04);
        map_name_to_validdatalength.insert("控制参数获取", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：UXB测控数传一体机A；\
            AAH：UXB测控数传一体机B；\
            BBH：VU通信机；\
            CCH：DCS测控；\
            DDH：地测CAN通道。！");
        return 1;
    }

    if (aa_string == "星点像数据获取")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("星点像数据获取", 0x05);
        map_name_to_validdatalength.insert("星点像数据获取", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：UXB测控数传一体机A；\
            AAH：UXB测控数传一体机B；\
            BBH：VU通信机；\
            CCH：DCS测控；\
            DDH：地测CAN通道。！");
        return 1;
    }

    if (aa_string == "单星点像图像获取")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("单星点像图像获取", 0x06);
        map_name_to_validdatalength.insert("单星点像图像获取", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：UXB测控数传一体机A；\
            AAH：UXB测控数传一体机B；\
            BBH：VU通信机；\
            CCH：DCS测控；\
            DDH：地测CAN通道。！");
        return 1;
    }

    if (aa_string == "星点像图像获取")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("星点像图像获取", 0x07);
        map_name_to_validdatalength.insert("星点像图像获取", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：UXB测控数传一体机A；\
            AAH：UXB测控数传一体机B；\
            BBH：VU通信机；\
            CCH：DCS测控；\
            DDH：地测CAN通道。！");
        return 1;
    }

    /*----------------------星敏感器B-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x72;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "复位")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("复位", 0x01);
        map_name_to_validdatalength.insert("复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2");
        return 1;
    }
    if (aa_string == "自检")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("自检", 0x02);
        map_name_to_validdatalength.insert("自检", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2");
        return 1;
    }
    if (aa_string == "进入待机模式")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("进入待机模式", 0x03);
        map_name_to_validdatalength.insert("进入待机模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2");
        return 1;
    }
    if (aa_string == "进入姿态测量模式")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("进入姿态测量模式", 0x04);
        map_name_to_validdatalength.insert("进入姿态测量模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，55H：CAN1，AAH：CAN2");
        return 1;
    }
    if (aa_string == "参数配置指令")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("参数配置指令", 0x05);
        map_name_to_validdatalength.insert("参数配置指令", 5);
        ui.plainTextEdit_reminder->appendPlainText("5字节，DATA0：55H：CAN1，AAH：CAN2\
            DATA1：参数代号\
            DATA2~DATA4：参数值");
        return 1;
    }

    /*----------------------磁强计遥控指令-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x74;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "磁强计A默认参数设置")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("磁强计A默认参数设置", 0x01);
        map_name_to_validdatalength.insert("磁强计A默认参数设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }
    if (aa_string == "磁强计B默认参数设置")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("02");
        map_name_to_code.insert("磁强计B默认参数设置", 0x02);
        map_name_to_validdatalength.insert("磁强计B默认参数设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }

    /*----------------------飞轮遥控指令-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x75;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "飞轮X转速模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("飞轮X转速模式指令", 0x01);
        map_name_to_validdatalength.insert("飞轮X转速模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，32位IEEE浮点数，\
            转速值（rpm） - 6500rpm~+ 6500rpm");
        return 1;
    }
    if (aa_string == "飞轮X电流模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("飞轮X电流模式指令", 0x02);
        map_name_to_validdatalength.insert("飞轮X电流模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("1字节，32位IEEE浮点数，\
            电流值（mA） - 1200mA~+ 1200mA");
        return 1;
    }
    if (aa_string == "飞轮X力矩模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("飞轮X力矩模式指令", 0x03);
        map_name_to_validdatalength.insert("飞轮X力矩模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("1字节，32位IEEE浮点数，\
            力矩值（mNm） - 20mNm~+ 20mNm");
        return 1;
    }

    if (aa_string == "飞轮Y转速模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("飞轮Y转速模式指令", 0x04);
        map_name_to_validdatalength.insert("飞轮Y转速模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，32位IEEE浮点数，\
            转速值（rpm） - 6500rpm~+ 6500rpm");
        return 1;
    }
    if (aa_string == "飞轮Y电流模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("飞轮Y电流模式指令", 0x05);
        map_name_to_validdatalength.insert("飞轮Y电流模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("1字节，32位IEEE浮点数，\
            电流值（mA） - 1200mA~+ 1200mA");
        return 1;
    }
    if (aa_string == "飞轮Y力矩模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("飞轮Y力矩模式指令", 0x06);
        map_name_to_validdatalength.insert("飞轮Y力矩模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("1字节，32位IEEE浮点数，\
            力矩值（mNm） - 20mNm~+ 20mNm");
        return 1;
    }

    if (aa_string == "飞轮Z转速模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("飞轮Z转速模式指令", 0x07);
        map_name_to_validdatalength.insert("飞轮Z转速模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，32位IEEE浮点数，\
            转速值（rpm） - 6500rpm~+ 6500rpm");
        return 1;
    }
    if (aa_string == "飞轮Z电流模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("飞轮Z电流模式指令", 0x08);
        map_name_to_validdatalength.insert("飞轮Z电流模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("1字节，32位IEEE浮点数，\
            电流值（mA） - 1200mA~+ 1200mA");
        return 1;
    }
    if (aa_string == "飞轮Z力矩模式指令")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("飞轮Z力矩模式指令", 0x09);
        map_name_to_validdatalength.insert("飞轮Z力矩模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("1字节，32位IEEE浮点数，\
            力矩值（mNm） - 20mNm~+ 20mNm");
        return 1;
    }

    if (aa_string == "飞轮S转速模式指令")
    {
        telecontrol_frame_up.cmd_code = 0xA;
        map_name_to_code.insert("飞轮S转速模式指令", 0xA);
        map_name_to_validdatalength.insert("飞轮S转速模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，32位IEEE浮点数，\
            转速值（rpm） - 6500rpm~+ 6500rpm");
        return 1;
    }
    if (aa_string == "飞轮S电流模式指令")
    {
        telecontrol_frame_up.cmd_code = 0xB;
        map_name_to_code.insert("飞轮S电流模式指令", 0xB);
        map_name_to_validdatalength.insert("飞轮S电流模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("1字节，32位IEEE浮点数，\
            电流值（mA） - 1200mA~+ 1200mA");
        return 1;
    }
    if (aa_string == "飞轮S力矩模式指令")
    {
        telecontrol_frame_up.cmd_code = 0xC;
        map_name_to_code.insert("飞轮S力矩模式指令", 0xC);
        map_name_to_validdatalength.insert("飞轮S力矩模式指令", 4);
        ui.plainTextEdit_reminder->appendPlainText("1字节，32位IEEE浮点数，\
            力矩值（mNm） - 20mNm~+ 20mNm");
        return 1;
    }


    /*----------------------光纤陀螺A遥控指令-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x76;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "光纤陀螺A复位")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("光纤陀螺A复位", 0x01);
        map_name_to_validdatalength.insert("光纤陀螺A复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }


    /*----------------------光纤陀螺B遥控指令-------------------------------------*/
    telecontrol_frame_up.single_number = 0x77;
    if (aa_string == "光纤陀螺B复位")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("光纤陀螺B复位", 0x02);
        map_name_to_validdatalength.insert("光纤陀螺B复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }
    if (aa_string == "光纤陀螺B默认参数设置")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("光纤陀螺B复位", 0x02);
        map_name_to_code.insert("光纤陀螺B默认参数设置", 0x03);
        map_name_to_validdatalength.insert("光纤陀螺B默认参数设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }


    /*----------------------电推力器遥控指令-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x79;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "点火指令")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("点火指令", 0x01);
        map_name_to_validdatalength.insert("点火指令", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }
    if (aa_string == "开启电源")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("02");
        map_name_to_code.insert("开启电源", 0x02);
        map_name_to_validdatalength.insert("开启电源", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }
    if (aa_string == "关闭电源")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        ui.textEdit_validdata->insertPlainText("03");
        map_name_to_code.insert("关闭电源", 0x03);
        map_name_to_validdatalength.insert("关闭电源", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }
    if (aa_string == "点清洗火花塞")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        ui.textEdit_validdata->insertPlainText("04");
        map_name_to_code.insert("清洗火花塞", 0x04);
        map_name_to_validdatalength.insert("清洗火花塞", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据");
        return 1;
    }
    if (aa_string == "连续点火")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("连续点火", 0x05);
        map_name_to_validdatalength.insert("连续点火", 7);
        ui.plainTextEdit_reminder->appendPlainText("7字节，DATA0~DATA3：无符号整型，需要点火的次数\
                                                    DATA4~DATA6：无符号整型，连续点火的时间间隔，单位：ms");
        return 1;
    }

    /*----------------------高分摄像机-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x68;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "高分通信波特率")
    {
        telecontrol_frame_up.cmd_code = 0x01;
      //  ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("通信波特率", 0x01);
        map_name_to_validdatalength.insert("通信波特率", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，bit0: 9600bps\
            bit1: 19200bps\
            bit2 : 38400bps\
            bit3 : 57600bps\
            bit4 : 115200bps");
        return 1;
    }
    if (aa_string == "高分图像高")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        //  ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("图像高", 0x02);
        map_name_to_validdatalength.insert("图像高", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 2~2056");
        return 1;
    }
    if (aa_string == "高分图像宽")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        //  ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("图像宽", 0x03);
        map_name_to_validdatalength.insert("图像宽", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 96~2464");
        return 1;
    }

    if (aa_string == "高分Offset Y")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("Offset Y", 0x04);
        map_name_to_validdatalength.insert("Offset Y", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 0~2054");
        return 1;
    }
    if (aa_string == "高分Offset X")
    {
        map_name_to_code.insert("Offset X", 0x05);
        map_name_to_validdatalength.insert("Offset X", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 0~2448");
        return 1;
    }

    if (aa_string == "高分图像格式")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("图像格式", 0x06);
        map_name_to_validdatalength.insert("图像格式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: ColorRG8 1: ColorRG10  2 : ColorRG12");
        return 1;
    }
    if (aa_string == "高分帧读取周期")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("帧读取周期", 0x07);
        map_name_to_validdatalength.insert("帧读取周期", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：无符号整型，单位us。10~8000000");
        return 1;
    }

    if (aa_string == "高分帧开始触发源")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("帧开始触发源", 0x08);
        map_name_to_validdatalength.insert("帧开始触发源", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Low\
            1: High\
            2 : SoftTrigger\
            8 : PulseGenerator0\
            10 : UserOutput0\
            11 : UserOutput1\
            12 : TTL_In1(Std Only)\
            13 : CL_CC1_In\
            14 : Nand0\
            15 : Nand1");
        return 1;
    }
    if (aa_string == "高分曝光模式")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("曝光模式", 0x09);
        map_name_to_validdatalength.insert("曝光模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off，1: Timed，2: TriggerWidth");
        return 1;
    }
    if (aa_string == "高分曝光时间")
    {
        telecontrol_frame_up.cmd_code = 0xa;
        map_name_to_code.insert("曝光时间", 0xa);
        map_name_to_validdatalength.insert("曝光时间",4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，无符号整型，单位：us");
        return 1;
    }
    if (aa_string == "高分白平衡")
    {
        telecontrol_frame_up.cmd_code = 0x0b;
        map_name_to_code.insert("白平衡", 0x0b);
        map_name_to_validdatalength.insert("白平衡", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off\
            1: Once\
            2 : Continuous\
            3 : 4600K\
            4 : 5600K\
            5 : 6500K\
            Else : Off");
        return 1;
    }
    if (aa_string == "高分增益")
    {
        telecontrol_frame_up.cmd_code = 0xc;
        map_name_to_code.insert("增益", 0xc);
        map_name_to_validdatalength.insert("增益", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号短整型");
        return 1;
    }
    if (aa_string == "高分自动增益")
    {
        telecontrol_frame_up.cmd_code = 0xd;
        map_name_to_code.insert("自动增益", 0xd);
        map_name_to_validdatalength.insert("自动增益", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off，1: Continuous");
        return 1;
    }
    if (aa_string == "高分抽头")
    {
        telecontrol_frame_up.cmd_code = 0xe;
        map_name_to_code.insert("抽头", 0xe);
        map_name_to_validdatalength.insert("抽头", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1: Geometry_1X2_1Y\
            3: Geometry_1X4_1Y\
            7 : Geometry_1X3_1Y");
        return 1;
    }
    if (aa_string == "高分伽马选择")
    {
        telecontrol_frame_up.cmd_code = 0xf;
        map_name_to_code.insert("伽马选择", 0xf);
        map_name_to_validdatalength.insert("伽马选择", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0：γ=0.45，1：γ=0.60，2：γ=1.0");
        return 1;
    }

    if (aa_string == "高分CameraLink时钟频率")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        map_name_to_code.insert("增CameraLink时钟频率益", 0x10);
        map_name_to_validdatalength.insert("CameraLink时钟频率", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0= 84.85MHz,1 = 74.25MHz,2 = 37.12MHz");
        return 1;
    }
    if (aa_string == "高分参数配置装载")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        map_name_to_code.insert("参数配置装载", 0x11);
        map_name_to_validdatalength.insert("参数配置装载", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Default 1: UserSet1  2 : UserSet2 3 : UserSet3");
        return 1;
    }
    if (aa_string == "高分参数配置保存")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        map_name_to_code.insert("参数配置保存", 0x12);
        map_name_to_validdatalength.insert("参数配置保存", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1: UserSet1  2: UserSet2  3 : UserSet3");
        return 1;
    }


    /*----------------------低分摄像机-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x69;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "低分通信波特率")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        //  ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("通信波特率", 0x01);
        map_name_to_validdatalength.insert("通信波特率", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，bit0: 9600bps\
            bit1: 19200bps\
            bit2 : 38400bps\
            bit3 : 57600bps\
            bit4 : 115200bps");
        return 1;
    }
    if (aa_string == "低分图像高")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        //  ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("图像高", 0x02);
        map_name_to_validdatalength.insert("图像高", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 2~2056");
        return 1;
    }
    if (aa_string == "低分图像宽")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        //  ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("图像宽", 0x03);
        map_name_to_validdatalength.insert("图像宽", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 96~2464");
        return 1;
    }

    if (aa_string == "低分Offset Y")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("Offset Y", 0x04);
        map_name_to_validdatalength.insert("Offset Y", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 0~2054");
        return 1;
    }
    if (aa_string == "低分Offset X")
    {
        map_name_to_code.insert("Offset X", 0x05);
        map_name_to_validdatalength.insert("Offset X", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 0~2448");
        return 1;
    }

    if (aa_string == "低分图像格式")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("图像格式", 0x06);
        map_name_to_validdatalength.insert("图像格式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: ColorRG8 1: ColorRG10  2 : ColorRG12");
        return 1;
    }
    if (aa_string == "低分帧读取周期")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("帧读取周期", 0x07);
        map_name_to_validdatalength.insert("帧读取周期", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：无符号整型，单位us。10~8000000");
        return 1;
    }

    if (aa_string == "低分帧开始触发源")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("帧开始触发源", 0x08);
        map_name_to_validdatalength.insert("帧开始触发源", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Low\
            1: High\
            2 : SoftTrigger\
            8 : PulseGenerator0\
            10 : UserOutput0\
            11 : UserOutput1\
            12 : TTL_In1(Std Only)\
            13 : CL_CC1_In\
            14 : Nand0\
            15 : Nand1");
        return 1;
    }
    if (aa_string == "低分曝光模式")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("曝光模式", 0x09);
        map_name_to_validdatalength.insert("曝光模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off，1: Timed，2: TriggerWidth");
        return 1;
    }
    if (aa_string == "低分曝光时间")
    {
        telecontrol_frame_up.cmd_code = 0xa;
        map_name_to_code.insert("曝光时间", 0xa);
        map_name_to_validdatalength.insert("曝光时间", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，无符号整型，单位：us");
        return 1;
    }
    if (aa_string == "低分白平衡")
    {
        telecontrol_frame_up.cmd_code = 0x0b;
        map_name_to_code.insert("白平衡", 0x0b);
        map_name_to_validdatalength.insert("白平衡", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off\
            1: Once\
            2 : Continuous\
            3 : 4600K\
            4 : 5600K\
            5 : 6500K\
            Else : Off");
        return 1;
    }
    if (aa_string == "低分增益")
    {
        telecontrol_frame_up.cmd_code = 0xc;
        map_name_to_code.insert("增益", 0xc);
        map_name_to_validdatalength.insert("增益", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号短整型");
        return 1;
    }
    if (aa_string == "低分自动增益")
    {
        telecontrol_frame_up.cmd_code = 0xd;
        map_name_to_code.insert("自动增益", 0xd);
        map_name_to_validdatalength.insert("自动增益", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off，1: Continuous");
        return 1;
    }
    if (aa_string == "低分抽头")
    {
        telecontrol_frame_up.cmd_code = 0xe;
        map_name_to_code.insert("抽头", 0xe);
        map_name_to_validdatalength.insert("抽头", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1: Geometry_1X2_1Y\
            3: Geometry_1X4_1Y\
            7 : Geometry_1X3_1Y");
        return 1;
    }
    if (aa_string == "低分伽马选择")
    {
        telecontrol_frame_up.cmd_code = 0xf;
        map_name_to_code.insert("伽马选择", 0xf);
        map_name_to_validdatalength.insert("伽马选择", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0：γ=0.45，1：γ=0.60，2：γ=1.0");
        return 1;
    }

    if (aa_string == "低分CameraLink时钟频率")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        map_name_to_code.insert("增CameraLink时钟频率益", 0x10);
        map_name_to_validdatalength.insert("CameraLink时钟频率", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0= 84.85MHz,1 = 74.25MHz,2 = 37.12MHz");
        return 1;
    }
    if (aa_string == "低分参数配置装载")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        map_name_to_code.insert("参数配置装载", 0x11);
        map_name_to_validdatalength.insert("参数配置装载", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Default 1: UserSet1  2 : UserSet2 3 : UserSet3");
        return 1;
    }
    if (aa_string == "低分参数配置保存")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        map_name_to_code.insert("参数配置保存", 0x12);
        map_name_to_validdatalength.insert("参数配置保存", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1: UserSet1  2: UserSet2  3 : UserSet3");
        return 1;
    }

    /*----------------------广角摄像机-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x6A;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "广角通信波特率")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("通信波特率", 0x01);
        map_name_to_validdatalength.insert("通信波特率", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，bit0: 9600bps\
            bit1: 19200bps\
            bit2 : 38400bps\
            bit3 : 57600bps\
            bit4 : 115200bps");
        return 1;
    }
    if (aa_string == "广角图像高")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        //  ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("图像高", 0x02);
        map_name_to_validdatalength.insert("图像高", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 2~2056");
        return 1;
    }
    if (aa_string == "广角图像宽")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        //  ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("图像宽", 0x03);
        map_name_to_validdatalength.insert("图像宽", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 96~2464");
        return 1;
    }

    if (aa_string == "广角Offset Y")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("Offset Y", 0x04);
        map_name_to_validdatalength.insert("Offset Y", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 0~2054");
        return 1;
    }
    if (aa_string == "广角Offset X")
    {
        map_name_to_code.insert("Offset X", 0x05);
        map_name_to_validdatalength.insert("Offset X", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：无符号短整型 0~2448");
        return 1;
    }

    if (aa_string == "广角图像格式")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("图像格式", 0x06);
        map_name_to_validdatalength.insert("图像格式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: ColorRG8 1: ColorRG10  2 : ColorRG12");
        return 1;
    }
    if (aa_string == "广角帧读取周期")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("帧读取周期", 0x07);
        map_name_to_validdatalength.insert("帧读取周期", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：无符号整型，单位us。10~8000000");
        return 1;
    }

    if (aa_string == "广角帧开始触发源")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("帧开始触发源", 0x08);
        map_name_to_validdatalength.insert("帧开始触发源", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Low\
            1: High\
            2 : SoftTrigger\
            8 : PulseGenerator0\
            10 : UserOutput0\
            11 : UserOutput1\
            12 : TTL_In1(Std Only)\
            13 : CL_CC1_In\
            14 : Nand0\
            15 : Nand1");
        return 1;
    }
    if (aa_string == "广角曝光模式")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("曝光模式", 0x09);
        map_name_to_validdatalength.insert("曝光模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off，1: Timed，2: TriggerWidth");
        return 1;
    }
    if (aa_string == "广角曝光时间")
    {
        telecontrol_frame_up.cmd_code = 0xa;
        map_name_to_code.insert("曝光时间", 0xa);
        map_name_to_validdatalength.insert("曝光时间", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，无符号整型，单位：us");
        return 1;
    }
    if (aa_string == "广角白平衡")
    {
        telecontrol_frame_up.cmd_code = 0x0b;
        map_name_to_code.insert("白平衡", 0x0b);
        map_name_to_validdatalength.insert("白平衡", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off\
            1: Once\
            2 : Continuous\
            3 : 4600K\
            4 : 5600K\
            5 : 6500K\
            Else : Off");
        return 1;
    }
    if (aa_string == "广角增益")
    {
        telecontrol_frame_up.cmd_code = 0xc;
        map_name_to_code.insert("增益", 0xc);
        map_name_to_validdatalength.insert("增益", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号短整型");
        return 1;
    }
    if (aa_string == "广角自动增益")
    {
        telecontrol_frame_up.cmd_code = 0xd;
        map_name_to_code.insert("自动增益", 0xd);
        map_name_to_validdatalength.insert("自动增益", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Off，1: Continuous");
        return 1;
    }
    if (aa_string == "广角抽头")
    {
        telecontrol_frame_up.cmd_code = 0xe;
        map_name_to_code.insert("抽头", 0xe);
        map_name_to_validdatalength.insert("抽头", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1: Geometry_1X2_1Y\
            3: Geometry_1X4_1Y\
            7 : Geometry_1X3_1Y");
        return 1;
    }
    if (aa_string == "广角伽马选择")
    {
        telecontrol_frame_up.cmd_code = 0xf;
        map_name_to_code.insert("伽马选择", 0xf);
        map_name_to_validdatalength.insert("伽马选择", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0：γ=0.45，1：γ=0.60，2：γ=1.0");
        return 1;
    }

    if (aa_string == "广角CameraLink时钟频率")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        map_name_to_code.insert("增CameraLink时钟频率益", 0x10);
        map_name_to_validdatalength.insert("CameraLink时钟频率", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0= 84.85MHz,1 = 74.25MHz,2 = 37.12MHz");
        return 1;
    }
    if (aa_string == "广角参数配置装载")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        map_name_to_code.insert("参数配置装载", 0x11);
        map_name_to_validdatalength.insert("参数配置装载", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0: Default 1: UserSet1  2 : UserSet2 3 : UserSet3");
        return 1;
    }
    if (aa_string == "广角参数配置保存")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        map_name_to_code.insert("参数配置保存", 0x12);
        map_name_to_validdatalength.insert("参数配置保存", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1: UserSet1  2: UserSet2  3 : UserSet3");
        return 1;
    }

    /*----------------------红外摄像机-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x6B;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "图像定格")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("图像定格", 0x01);
        map_name_to_validdatalength.insert("图像定格", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：不定格，01H：定格");
        return 1;
    }
    if (aa_string == "测试画面切换")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("测试画面切换", 0x02);
        map_name_to_validdatalength.insert("测试画面切换", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：实时，01H：棋盘格，02H：行渐变，03H：列渐变");
        return 1;
    }

    if (aa_string == "保存设置")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("保存设置", 0x03);
        map_name_to_validdatalength.insert("保存设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，");
        return 1;
    }
    if (aa_string == "恢复出厂值")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("恢复出厂值", 0x04);
        map_name_to_validdatalength.insert("恢复出厂值", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，");
        return 1;
    }
    if (aa_string == "温升补偿开关")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("温升补偿开关", 0x05);
        map_name_to_validdatalength.insert("温升补偿开关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：关，01H：开");
        return 1;
    }
    if (aa_string == "快门闭合")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("快门闭合", 0x06);
        map_name_to_validdatalength.insert("快门闭合", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：快门闭合，01H：快门弹开");
        return 1;
    }

    if (aa_string == "增益控制")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("增益控制", 0x07);
        map_name_to_validdatalength.insert("增益控制", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：标准，01H：低噪声");
        return 1;
    }
    if (aa_string == "外同步开关")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("外同步开关", 0x08);
        map_name_to_validdatalength.insert("外同步开关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：从模式-关，01H：从模式-开，02H：主模式");
        return 1;
    }
    if (aa_string == "数字口并行类型")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("数字口并行类型", 0x09);
        map_name_to_validdatalength.insert("数字口并行类型", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：关，01H：BT.656，02H：CMOS");
        return 1;
    }
    if (aa_string == "CMOS内容选择")
    {
        telecontrol_frame_up.cmd_code = 0xA;
        map_name_to_code.insert("CMOS内容选择", 0xa);
        map_name_to_validdatalength.insert("CMOS内容选择", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：YUV422，\
            01H：YUV422_参数行，\
            02H：Y16，\
            03H：Y16_参数行，\
            04H：Y16_YUV422，\
            05H：Y16_参数行_YUV422");
        return 1;
    }

    if (aa_string == "CMOS接口形式")
    {
        telecontrol_frame_up.cmd_code = 0x0b;
        map_name_to_code.insert("CMOS接口形式", 0x0b);
        map_name_to_validdatalength.insert("CMOS接口形式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：CMOS16\
            01H：CMOS8(MSB first)\
            02H：CMOS8(LSB first)");
        return 1;
    }
    if (aa_string == "场景补偿")
    {
        telecontrol_frame_up.cmd_code = 0x0c;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("场景补偿", 0x0c);
        map_name_to_validdatalength.insert("场景补偿", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "快门补偿")
    {
        telecontrol_frame_up.cmd_code = 0x0d;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("快门补偿", 0x0d);
        map_name_to_validdatalength.insert("快门补偿", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "时域滤波")
    {
        telecontrol_frame_up.cmd_code = 0xe;
        map_name_to_code.insert("时域滤波", 0xe);
        map_name_to_validdatalength.insert("时域滤波", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x00关 0x01开");
        return 1;
    }

    if (aa_string == "滤波强度")
    {
        telecontrol_frame_up.cmd_code = 0x0f;
        map_name_to_code.insert("滤波强度", 0x0f);
        map_name_to_validdatalength.insert("滤波强度", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x00~0x09：等级0~等级9");
        return 1;
    }
    if (aa_string == "去竖纹")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        map_name_to_code.insert("去竖纹", 0x10);
        map_name_to_validdatalength.insert("去竖纹", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x00关  0x01开！");
        return 1;
    }

    if (aa_string == "调光模式")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        map_name_to_code.insert("调光模式", 0x11);
        map_name_to_validdatalength.insert("调光模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：线性，01H：平台，02H：混合！");
        return 1;
    }
    if (aa_string == "上抛点比例")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        map_name_to_code.insert("上抛点比例", 0x12);
        map_name_to_validdatalength.insert("上抛点比例", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0~20");
        return 1;
    }

    if (aa_string == "下抛点比例")
    {
        telecontrol_frame_up.cmd_code = 0x13;
        map_name_to_code.insert("下抛点比例", 0x13);
        map_name_to_validdatalength.insert("下抛点比例", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0~20");
        return 1;
    }
    if (aa_string == "亮度")
    {
        telecontrol_frame_up.cmd_code = 0x14;
        map_name_to_code.insert("亮度", 0x14);
        map_name_to_validdatalength.insert("亮度", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0~100！");
        return 1;
    }

    if (aa_string == "对比度")
    {
        telecontrol_frame_up.cmd_code = 0x15;
        map_name_to_code.insert("对比度", 0x15);
        map_name_to_validdatalength.insert("对比度", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0~100！");
        return 1;
    }
    if (aa_string == "混合调光映射")
    {
        telecontrol_frame_up.cmd_code = 0x16;
        map_name_to_code.insert("混合调光映射", 0x16);
        map_name_to_validdatalength.insert("混合调光映射", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0~255");
        return 1;
    }

    if (aa_string == "Y8纠偏")
    {
        telecontrol_frame_up.cmd_code = 0x17;
        map_name_to_code.insert("Y8纠偏", 0x17);
        map_name_to_validdatalength.insert("Y8纠偏", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：关，01H：开");
        return 1;
    }
    if (aa_string == "IDE增强")
    {
        telecontrol_frame_up.cmd_code = 0x18;
        map_name_to_code.insert("IDE增强", 0x18);
        map_name_to_validdatalength.insert("IDE增强", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：关，01H：开！");
        return 1;
    }

    if (aa_string == "IDE滤波等级")
    {
        telecontrol_frame_up.cmd_code = 0x19;
        map_name_to_code.insert("IDE滤波等级", 0x19);
        map_name_to_validdatalength.insert("IDE滤波等级", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0~4！");
        return 1;
    }
    if (aa_string == "IDE细节增益")
    {
        telecontrol_frame_up.cmd_code = 0x1a;
        map_name_to_code.insert("IDE细节增益", 0x1a);
        map_name_to_validdatalength.insert("IDE细节增益", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00~64");
        return 1;
    }

    if (aa_string == "Y8纠偏模式")
    {
        telecontrol_frame_up.cmd_code = 0x1b;
        map_name_to_code.insert("Y8纠偏模式", 0x1b);
        map_name_to_validdatalength.insert("Y8纠偏模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：Y8纠偏自动模式 01H：Y8纠偏手动模式");
        return 1;
    }
    if (aa_string == "分块直方图")
    {
        telecontrol_frame_up.cmd_code = 0x1c;
        map_name_to_code.insert("分块直方图", 0x1c);
        map_name_to_validdatalength.insert("分块直方图", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：关，01H：开！");
        return 1;
    }

    if (aa_string == "去噪开关")
    {
        telecontrol_frame_up.cmd_code = 0x1d;
        map_name_to_code.insert("去噪开关", 0x1d);
        map_name_to_validdatalength.insert("去噪开关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：去噪关，01H：去噪开！");
        return 1;
    }
    if (aa_string == "去噪等级")
    {
        telecontrol_frame_up.cmd_code = 0x1e;
        map_name_to_code.insert("去噪等级", 0x1e);
        map_name_to_validdatalength.insert("去噪等级", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0~9");
        return 1;
    }

    if (aa_string == "手动调焦速度")
    {
        telecontrol_frame_up.cmd_code = 0x1f;
        map_name_to_code.insert("手动调焦速度", 0x1f);
        map_name_to_validdatalength.insert("手动调焦速度", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1~10");
        return 1;
    }
    if (aa_string == "自动调焦统计帧数")
    {
        telecontrol_frame_up.cmd_code = 0x20;
        map_name_to_code.insert("自动调焦统计帧数", 0x20);
        map_name_to_validdatalength.insert("自动调焦统计帧数", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1~15！");
        return 1;
    }

    if (aa_string == "自动调焦速度MAX")
    {
        telecontrol_frame_up.cmd_code = 0x21;
        map_name_to_code.insert("自动调焦速度MAX", 0x21);
        map_name_to_validdatalength.insert("自动调焦速度MAX", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1~10！");
        return 1;
    }
    if (aa_string == "自动调焦速度MIN")
    {
        telecontrol_frame_up.cmd_code = 0x22;
        map_name_to_code.insert("自动调焦速度MIN", 0x22);
        map_name_to_validdatalength.insert("自动调焦速度MIN", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，1~10");
        return 1;
    }

    if (aa_string == "十字光标开关")
    {
        telecontrol_frame_up.cmd_code = 0x23;
        map_name_to_code.insert("十字光标开关", 0x23);
        map_name_to_validdatalength.insert("十字光标开关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：关，01H：开");
        return 1;
    }
    if (aa_string == "光标位置X")
    {
        telecontrol_frame_up.cmd_code = 0x24;
        map_name_to_code.insert("光标位置X", 0x24);
        map_name_to_validdatalength.insert("光标位置X", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号短整型，0~width-1！");
        return 1;
    }

    if (aa_string == "光标位置Y")
    {
        telecontrol_frame_up.cmd_code = 0x25;
        map_name_to_code.insert("光标位置Y", 0x25);
        map_name_to_validdatalength.insert("光标位置Y",2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号短整型，0~height-1！");
        return 1;
    }
    if (aa_string == "光标R分量")
    {
        telecontrol_frame_up.cmd_code = 0x26;
        map_name_to_code.insert("光标R分量", 0x26);
        map_name_to_validdatalength.insert("光标R分量", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，无符号整型");
        return 1;
    }

    if (aa_string == "光标G分量")
    {
        telecontrol_frame_up.cmd_code = 0x27;
        map_name_to_code.insert("光标G分量", 0x27);
        map_name_to_validdatalength.insert("光标G分量", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，无符号整型");
        return 1;
    }
    if (aa_string == "光标B分量")
    {
        telecontrol_frame_up.cmd_code = 0x28;
        map_name_to_code.insert("光标B分量", 0x28);
        map_name_to_validdatalength.insert("光标B分量", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，无符号整型！");
        return 1;
    }

    if (aa_string == "伪彩条开关")
    {
        telecontrol_frame_up.cmd_code = 0x29;
        map_name_to_code.insert("伪彩条开关", 0x29);
        map_name_to_validdatalength.insert("伪彩条开关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：关，01H：开！");
        return 1;
    }
    if (aa_string == "伪彩视觉增强")
    {
        telecontrol_frame_up.cmd_code = 0x2a;
        map_name_to_code.insert("伪彩视觉增强", 0x2a);
        map_name_to_validdatalength.insert("伪彩视觉增强", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：伪彩视觉增强手动模式\
            01H：伪彩视觉增强半自动模式\
            02H：伪彩视觉增强自动模式");
        return 1;
    }

    if (aa_string == "伪彩视觉增强阈值上限")
    {
        telecontrol_frame_up.cmd_code = 0x2B;
        map_name_to_code.insert("伪彩视觉增强阈值上限", 0x2B);
        map_name_to_validdatalength.insert("伪彩视觉增强阈值上限", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号短整型");
        return 1;
    }
    if (aa_string == "伪彩视觉增强阈值下限")
    {
        telecontrol_frame_up.cmd_code = 0x2c;
        map_name_to_code.insert("伪彩视觉增强阈值下限", 0x2c);
        map_name_to_validdatalength.insert("伪彩视觉增强阈值下限", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号整型！");
        return 1;
    }

    if (aa_string == "等温线开关")
    {
        telecontrol_frame_up.cmd_code = 0x2d;
        map_name_to_code.insert("等温线开关", 0x2d);
        map_name_to_validdatalength.insert("等温线开关", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：关，01H：开！");
        return 1;
    }
    if (aa_string == "等温线显示模式")
    {
        telecontrol_frame_up.cmd_code = 0x2e;
        map_name_to_code.insert("等温线显示模式", 0x2e);
        map_name_to_validdatalength.insert("等温线显示模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，00H：上下等温线显示模式 01H：中等温线显示模式");
        return 1;
    }

    if (aa_string == "等温线阈值上限")
    {
        telecontrol_frame_up.cmd_code = 0x2f;
        map_name_to_code.insert("等温线阈值上限", 0x2f);
        map_name_to_validdatalength.insert("等温线阈值上限", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号短整型");
        return 1;
    }
    if (aa_string == "等温线阈值下限")
    {
        telecontrol_frame_up.cmd_code = 0x30;
        map_name_to_code.insert("等温线阈值下限", 0x30);
        map_name_to_validdatalength.insert("等温线阈值下限", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，无符号整型！");
        return 1;
    }

    if (aa_string == "等温线伪彩色带选择")
    {
        telecontrol_frame_up.cmd_code = 0x31;
        map_name_to_code.insert("等温线伪彩色带选择", 0x31);
        map_name_to_validdatalength.insert("等温线伪彩色带选择", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x00 白热\
            0x01 熔岩\
            0x02 铁红\
            0x03 热铁\
            0x04 医疗\
            0x05 北极\
            0x06 彩虹 1\
            0x07 彩虹 2\
            0x08 描红\
            0x09 黑热！");
        return 1;
    }

    /*----------------------对焦机构A-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x6C;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "右转")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("右转", 0x01);
        map_name_to_validdatalength.insert("右转", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：移动的值，0~2047");
        return 1;
    }
    if (aa_string == "左转")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("左转", 0x02);
        map_name_to_validdatalength.insert("左转", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：移动的值，0~2047");
        return 1;
    }
    if (aa_string == "停止")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        ui.textEdit_validdata->insertPlainText("03");
        map_name_to_code.insert("停止", 0x03);
        map_name_to_validdatalength.insert("停止",1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "移动到目标位置")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("移动到目标位置", 0x04);
        map_name_to_validdatalength.insert("移动到目标位置", 5);
        ui.plainTextEdit_reminder->appendPlainText("5字节，DATA0：类型00H：绝对位置01H：相对位置02H：坐标位置\
            DATA1~DATA4：目标位置");
        return 1;
    }

    /*----------------------对焦机构B-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x6D;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "调焦-")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("调焦-", 0x01);
        map_name_to_validdatalength.insert("调焦-", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，调焦速度参数，值为0~9");
        return 1;
    }
    if (aa_string == "调焦＋")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        map_name_to_code.insert("调焦＋", 0x02);
        map_name_to_validdatalength.insert("调焦＋", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，调焦速度参数，值为0~9");
        return 1;
    }
    if (aa_string == "停止调焦")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("停止调焦", 0x03);
        map_name_to_validdatalength.insert("停止调焦", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }


    /*----------------------数据压缩处理单元A------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x6E;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "单元A系统授时")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("系统授时", 0x01);
        map_name_to_validdatalength.insert("系统授时", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：自UTC格林威治2000年1月1日12时零秒的整秒累加值");
        return 1;
    }
    if (aa_string == "单元A系统重启")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("02");
        map_name_to_code.insert("系统重启", 0x02);
        map_name_to_validdatalength.insert("系统重启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元A文件序号记录")
    {
        telecontrol_frame_up.cmd_code = 0x03;        
        map_name_to_code.insert("文件序号记录", 0x03);
        map_name_to_validdatalength.insert("文件序号记录", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H-80H！");
        return 1;
    }

    if (aa_string == "单元A文件序号追加记录")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("文件序号追加记录", 0x04);
        map_name_to_validdatalength.insert("文件序号追加记录", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H-80H");
        return 1;
    }
    if (aa_string == "单元A记录直传")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        ui.textEdit_validdata->insertPlainText("05");
        map_name_to_code.insert("记录直传", 0x05);
        map_name_to_validdatalength.insert("记录直传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元A文件序号回放启用断点")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("文件序号回放启用断点", 0x06);
        map_name_to_validdatalength.insert("文件序号回放启用断点", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H~FFH！");
        return 1;
    }

    if (aa_string == "单元A文件序号回放不启用断点")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("文件序号回放不启用断点", 0x07);
        map_name_to_validdatalength.insert("文件序号回放不启用断点", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H~FFH");
        return 1;
    }
    if (aa_string == "单元A顺序回放启用断点")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("顺序回放启用断点", 0x08);
        map_name_to_validdatalength.insert("顺序回放启用断点", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x01: 记录文件  0x02: 压缩文件！");
        return 1;
    }
    if (aa_string == "单元A顺序回放不启用断点")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("顺序回放不启用断点", 0x09);
        map_name_to_validdatalength.insert("顺序回放不启用断点", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x01: 记录文件  0x02: 压缩文件！");
        return 1;
    }

    if (aa_string == "单元A按时间回放")
    {
        telecontrol_frame_up.cmd_code = 0x0A;
        map_name_to_code.insert("按时间回放", 0x0A);
        map_name_to_validdatalength.insert("按时间回放", 9);
        ui.plainTextEdit_reminder->appendPlainText("9字节，DATA0：0x01: 记录文件，0x02: 压缩文件\
            DATA1~DATA4：起始回放时间，UTC时间\
            DATA5~DATA8：结束回放时间，UTC时间");
        return 1;
    }
    if (aa_string == "单元A序号擦除文件")
    {
        telecontrol_frame_up.cmd_code = 0x0B;
        map_name_to_code.insert("序号擦除文件", 0x0B);
        map_name_to_validdatalength.insert("序号擦除文件", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H~FFH！");
        return 1;
    }
    if (aa_string == "单元A全部擦除")
    {
        telecontrol_frame_up.cmd_code = 0x0C;
        ui.textEdit_validdata->insertPlainText("0C");
        map_name_to_code.insert("全部擦除", 0x0C);
        map_name_to_validdatalength.insert("全部擦除", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "单元A停止记录")
    {
        telecontrol_frame_up.cmd_code = 0x0D;
        ui.textEdit_validdata->insertPlainText("0D");
        map_name_to_code.insert("停止记录", 0x0D);
        map_name_to_validdatalength.insert("停止记录", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元A停止回放")
    {
        telecontrol_frame_up.cmd_code = 0x0E;
        ui.textEdit_validdata->insertPlainText("0E");
        map_name_to_code.insert("停止回放", 0x0E);
        map_name_to_validdatalength.insert("停止回放", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元A全部停止")
    {
        telecontrol_frame_up.cmd_code = 0x0F;
        ui.textEdit_validdata->insertPlainText("0F");
        map_name_to_code.insert("全部停止", 0x0F);
        map_name_to_validdatalength.insert("全部停止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "单元A预关机")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        ui.textEdit_validdata->insertPlainText("10");
        map_name_to_code.insert("预关机", 0x10);
        map_name_to_validdatalength.insert("预关机", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元A功能自检")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        ui.textEdit_validdata->insertPlainText("11");
        map_name_to_code.insert("功能自检", 0x11);
        map_name_to_validdatalength.insert("功能自检", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元A低级格式化")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        ui.textEdit_validdata->insertPlainText("12");
        map_name_to_code.insert("低级格式化", 0x12);
        map_name_to_validdatalength.insert("低级格式化", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "单元A压缩模式设置")
    {
        telecontrol_frame_up.cmd_code = 0x13;
        map_name_to_code.insert("压缩模式设置", 0x13);
        map_name_to_validdatalength.insert("压缩模式设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x00：无压缩 0x01: 有损压缩 0x02 : 无损压缩！");
        return 1;
    }
    if (aa_string == "单元A压缩比设置")
    {
        telecontrol_frame_up.cmd_code = 0x14;
          map_name_to_code.insert("压缩比设置", 0x14);
        map_name_to_validdatalength.insert("压缩比设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，2，50！");
        return 1;
    }
    if (aa_string == "单元A压缩回写")
    {
        telecontrol_frame_up.cmd_code = 0x15;
        map_name_to_code.insert("压缩回写", 0x15);
        map_name_to_validdatalength.insert("压缩回写", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：源文件号，0x01-0x80 DATA1：压缩文件号，0x81 - 0xFF！");
        return 1;
    }

    if (aa_string == "单元A停止压缩")
    {
        telecontrol_frame_up.cmd_code = 0x16;
        ui.textEdit_validdata->insertPlainText("16");
        map_name_to_code.insert("停止压缩", 0x16);
        map_name_to_validdatalength.insert("停止压缩", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元A压缩直传")
    {
        telecontrol_frame_up.cmd_code = 0x17;
        map_name_to_code.insert("压缩直传", 0x17);
        map_name_to_validdatalength.insert("压缩直传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x01-0x80！");
        return 1;
    }
    if (aa_string == "单元A相机单元配置透传")
    {
        telecontrol_frame_up.cmd_code = 0x18;
        map_name_to_code.insert("相机单元配置透传", 0x18);
        map_name_to_validdatalength.insert("相机单元配置透传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，DATA0~DATA1：数据长度  DATA2：相机编号，1~4\
            数据内容！");
        return 1;
    }

    if (aa_string == "单元A成像任务控制")
    {
        telecontrol_frame_up.cmd_code = 0x19;
        map_name_to_code.insert("成像任务控制", 0x19);
        map_name_to_validdatalength.insert("成像任务控制", 58);
        ui.plainTextEdit_reminder->appendPlainText("58字节，0DATA0:相机选择 01H：高分，02H：广角，03H：低分，04H：红外\
            DATA1:成像类型 01H：单张，02H：多张，03H：视频\
            DATA2~DATA3 : 成像张数 / 视频成像时长, 当成像类型为“单张”和“多张”时为成像张数；当成像类型为“视频”时为视频成像时长，单位：s\
            DATA4 : 图像分辨率	01H：可见光2K（2048 * 2048），02H：红外640 * 512\
            DATA5 : 图像帧频	1, 15\
            DATA6 : 工作模式	01H：记录，02H：记录压缩直传\
            DATA7 : 压缩模式	00H：无压缩, 01H：有损压缩, 02H：无损压缩\
            DATA8 : 压缩比 2, 50\
            DATA9 : 文件记录号 0x01 - 0x80\
            DATA10 : 压缩文件号 0x81 - 0xFF\
            DATA11~DATA14 : 成像目标经度\
            DATA15~DATA18 : 成像目标纬度\
            DATA19~DATA22 : 用户ID\
            DATA23~DATA26 : 终端ID\
            DATA27~DATA30 : 授权码\
            DATA31~DATA34 : 终端经度\
            DATA35~DATA38 : 终端纬度\
            DATA39 : 任务模式	01H：实时成像和实时数传，02H：实时成像和延时数传，03H：延时成像和延时数传，04H：实时交互操作（实时成像和实时数传）\
            DATA40~DATA43 : 数传站经度\
            DATA44~DATA47 : 数传站纬度\
            DATA48~DATA51 : （成像）指令开始时间, UTC时间\
            DATA52~DATA55 : 任务最晚有效时间, UTC时间\
            DATA56 : 数传速率	01H：5Mbps，02H：40Mbps\
            DATA57 : 指令属性转换\
            55H：允许，其他值：不允许！");
        return 1;
    }
    if (aa_string == "单元A获取当前文件系统信息")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x1A;
        ui.textEdit_validdata->insertPlainText("1A");
        map_name_to_code.insert("获取当前文件系统信息", 0x1A);
        map_name_to_validdatalength.insert("获取当前文件系统信息", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元A获取文件属性信息")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x1B;
        map_name_to_code.insert("获取文件属性信息", 0x1B);
        map_name_to_validdatalength.insert("获取文件属性信息", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x01-0xFF！");
        return 1;
    }

    if (aa_string == "单元A获取相机配置参数透传")
    {
        telecontrol_frame_up.cmd_code = 0x1C;
        ui.textEdit_validdata->insertPlainText("1C");
        map_name_to_code.insert("获取相机配置参数透传", 0x1C);
        map_name_to_validdatalength.insert("获取相机配置参数透传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，DATA0~DATA1：数据长度\
            DATA2：相机编号，1~4\
            数据内容！");
        return 1;
    }

    /*----------------------数据压缩处理单元B------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x6F;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "单元B系统授时")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("系统授时", 0x01);
        map_name_to_validdatalength.insert("系统授时", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：自UTC格林威治2000年1月1日12时零秒的整秒累加值");
        return 1;
    }
    if (aa_string == "单元B系统重启")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("02");
        map_name_to_code.insert("系统重启", 0x02);
        map_name_to_validdatalength.insert("系统重启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元B文件序号记录")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        map_name_to_code.insert("文件序号记录", 0x03);
        map_name_to_validdatalength.insert("文件序号记录", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H-80H！");
        return 1;
    }

    if (aa_string == "单元B文件序号追加记录")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        map_name_to_code.insert("文件序号追加记录", 0x04);
        map_name_to_validdatalength.insert("文件序号追加记录", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H-80H");
        return 1;
    }
    if (aa_string == "单元B记录直传")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        ui.textEdit_validdata->insertPlainText("05");
        map_name_to_code.insert("记录直传", 0x05);
        map_name_to_validdatalength.insert("记录直传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元B文件序号回放启用断点")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        map_name_to_code.insert("文件序号回放启用断点", 0x06);
        map_name_to_validdatalength.insert("文件序号回放启用断点", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H~FFH！");
        return 1;
    }

    if (aa_string == "单元B文件序号回放不启用断点")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        map_name_to_code.insert("文件序号回放不启用断点", 0x07);
        map_name_to_validdatalength.insert("文件序号回放不启用断点", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H~FFH");
        return 1;
    }
    if (aa_string == "单元B顺序回放启用断点")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        map_name_to_code.insert("顺序回放启用断点", 0x08);
        map_name_to_validdatalength.insert("顺序回放启用断点", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x01: 记录文件  0x02: 压缩文件！");
        return 1;
    }
    if (aa_string == "单元B顺序回放不启用断点")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        map_name_to_code.insert("顺序回放不启用断点", 0x09);
        map_name_to_validdatalength.insert("顺序回放不启用断点", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x01: 记录文件  0x02: 压缩文件！");
        return 1;
    }

    if (aa_string == "单元B按时间回放")
    {
        telecontrol_frame_up.cmd_code = 0x0A;
        map_name_to_code.insert("按时间回放", 0x0A);
        map_name_to_validdatalength.insert("按时间回放", 9);
        ui.plainTextEdit_reminder->appendPlainText("9字节，DATA0：0x01: 记录文件，0x02: 压缩文件\
            DATA1~DATA4：起始回放时间，UTC时间\
            DATA5~DATA8：结束回放时间，UTC时间");
        return 1;
    }
    if (aa_string == "单元B序号擦除文件")
    {
        telecontrol_frame_up.cmd_code = 0x0B;
        map_name_to_code.insert("序号擦除文件", 0x0B);
        map_name_to_validdatalength.insert("序号擦除文件", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，01H~FFH！");
        return 1;
    }
    if (aa_string == "单元B全部擦除")
    {
        telecontrol_frame_up.cmd_code = 0x0C;
        ui.textEdit_validdata->insertPlainText("0C");
        map_name_to_code.insert("全部擦除", 0x0C);
        map_name_to_validdatalength.insert("全部擦除", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "单元B停止记录")
    {
        telecontrol_frame_up.cmd_code = 0x0D;
        ui.textEdit_validdata->insertPlainText("0D");
        map_name_to_code.insert("停止记录", 0x0D);
        map_name_to_validdatalength.insert("停止记录", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元B停止回放")
    {
        telecontrol_frame_up.cmd_code = 0x0E;
        ui.textEdit_validdata->insertPlainText("0E");
        map_name_to_code.insert("停止回放", 0x0E);
        map_name_to_validdatalength.insert("停止回放", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元B全部停止")
    {
        telecontrol_frame_up.cmd_code = 0x0F;
        ui.textEdit_validdata->insertPlainText("0F");
        map_name_to_code.insert("全部停止", 0x0F);
        map_name_to_validdatalength.insert("全部停止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "单元B预关机")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        ui.textEdit_validdata->insertPlainText("10");
        map_name_to_code.insert("预关机", 0x10);
        map_name_to_validdatalength.insert("预关机", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元B功能自检")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        ui.textEdit_validdata->insertPlainText("11");
        map_name_to_code.insert("功能自检", 0x11);
        map_name_to_validdatalength.insert("功能自检", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元B低级格式化")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        ui.textEdit_validdata->insertPlainText("12");
        map_name_to_code.insert("低级格式化", 0x12);
        map_name_to_validdatalength.insert("低级格式化", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "单元B压缩模式设置")
    {
        telecontrol_frame_up.cmd_code = 0x13;
        map_name_to_code.insert("压缩模式设置", 0x13);
        map_name_to_validdatalength.insert("压缩模式设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x00：无压缩 0x01: 有损压缩 0x02 : 无损压缩！");
        return 1;
    }
    if (aa_string == "单元B压缩比设置")
    {
        telecontrol_frame_up.cmd_code = 0x14;
        map_name_to_code.insert("压缩比设置", 0x14);
        map_name_to_validdatalength.insert("压缩比设置", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，2，50！");
        return 1;
    }
    if (aa_string == "单元B压缩回写")
    {
        telecontrol_frame_up.cmd_code = 0x15;
        map_name_to_code.insert("压缩回写", 0x15);
        map_name_to_validdatalength.insert("压缩回写", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：源文件号，0x01-0x80 DATA1：压缩文件号，0x81 - 0xFF！");
        return 1;
    }

    if (aa_string == "单元B停止压缩")
    {
        telecontrol_frame_up.cmd_code = 0x16;
        ui.textEdit_validdata->insertPlainText("16");
        map_name_to_code.insert("停止压缩", 0x16);
        map_name_to_validdatalength.insert("停止压缩", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元B压缩直传")
    {
        telecontrol_frame_up.cmd_code = 0x17;
        map_name_to_code.insert("压缩直传", 0x17);
        map_name_to_validdatalength.insert("压缩直传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x01-0x80！");
        return 1;
    }
    if (aa_string == "单元B相机单元配置透传")
    {
        telecontrol_frame_up.cmd_code = 0x18;
        map_name_to_code.insert("相机单元配置透传", 0x18);
        map_name_to_validdatalength.insert("相机单元配置透传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，DATA0~DATA1：数据长度  DATA2：相机编号，1~4\
            数据内容！");
        return 1;
    }

    if (aa_string == "单元B成像任务控制")
    {
        telecontrol_frame_up.cmd_code = 0x19;
        map_name_to_code.insert("成像任务控制", 0x19);
        map_name_to_validdatalength.insert("成像任务控制", 58);
        ui.plainTextEdit_reminder->appendPlainText("58字节，0DATA0:相机选择 01H：高分，02H：广角，03H：低分，04H：红外\
            DATA1:成像类型 01H：单张，02H：多张，03H：视频\
            DATA2~DATA3 : 成像张数 / 视频成像时长, 当成像类型为“单张”和“多张”时为成像张数；当成像类型为“视频”时为视频成像时长，单位：s\
            DATA4 : 图像分辨率	01H：可见光2K（2048 * 2048），02H：红外640 * 512\
            DATA5 : 图像帧频	1, 15\
            DATA6 : 工作模式	01H：记录，02H：记录压缩直传\
            DATA7 : 压缩模式	00H：无压缩, 01H：有损压缩, 02H：无损压缩\
            DATA8 : 压缩比 2, 50\
            DATA9 : 文件记录号 0x01 - 0x80\
            DATA10 : 压缩文件号 0x81 - 0xFF\
            DATA11~DATA14 : 成像目标经度\
            DATA15~DATA18 : 成像目标纬度\
            DATA19~DATA22 : 用户ID\
            DATA23~DATA26 : 终端ID\
            DATA27~DATA30 : 授权码\
            DATA31~DATA34 : 终端经度\
            DATA35~DATA38 : 终端纬度\
            DATA39 : 任务模式	01H：实时成像和实时数传，02H：实时成像和延时数传，03H：延时成像和延时数传，04H：实时交互操作（实时成像和实时数传）\
            DATA40~DATA43 : 数传站经度\
            DATA44~DATA47 : 数传站纬度\
            DATA48~DATA51 : （成像）指令开始时间, UTC时间\
            DATA52~DATA55 : 任务最晚有效时间, UTC时间\
            DATA56 : 数传速率	01H：5Mbps，02H：40Mbps\
            DATA57 : 指令属性转换\
            55H：允许，其他值：不允许！");
        return 1;
    }
    if (aa_string == "单元B获取当前文件系统信息")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x1A;
        ui.textEdit_validdata->insertPlainText("1A");
        map_name_to_code.insert("获取当前文件系统信息", 0x1A);
        map_name_to_validdatalength.insert("获取当前文件系统信息", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "单元B获取文件属性信息")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x1B;
        map_name_to_code.insert("获取文件属性信息", 0x1B);
        map_name_to_validdatalength.insert("获取文件属性信息", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，0x01-0xFF！");
        return 1;
    }

    if (aa_string == "单元B获取相机配置参数透传")
    {
        telecontrol_frame_up.cmd_code = 0x1C;
        ui.textEdit_validdata->insertPlainText("1C");
        map_name_to_code.insert("获取相机配置参数透传", 0x1C);
        map_name_to_validdatalength.insert("获取相机配置参数透传", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，DATA0~DATA1：数据长度\
            DATA2：相机编号，1~4\
            数据内容！");
        return 1;
    }


    /*----------------------GNSS掩星观测单元------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x66;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "接收机复位")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        ui.textEdit_validdata->insertPlainText("03");
        map_name_to_code.insert("接收机复位", 0x03);
        map_name_to_validdatalength.insert("接收机复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "定轨开启")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        ui.textEdit_validdata->insertPlainText("04");
        map_name_to_code.insert("定轨开启", 0x04);
        map_name_to_validdatalength.insert("定轨开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "定轨关闭")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        map_name_to_code.insert("定轨关闭", 0x05);
        map_name_to_validdatalength.insert("定轨关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "原始观测量存储开始")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        ui.textEdit_validdata->insertPlainText("06");
        map_name_to_code.insert("原始观测量存储开始", 0x06);
        map_name_to_validdatalength.insert("原始观测量存储开始", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "原始观测量存储关闭")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        ui.textEdit_validdata->insertPlainText("07");
        map_name_to_code.insert("原始观测量存储关闭", 0x07);
        map_name_to_validdatalength.insert("原始观测量存储关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "GPS大气开环通道开启")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        ui.textEdit_validdata->insertPlainText("08");
        map_name_to_code.insert("GPS大气开环通道开启", 0x08);
        map_name_to_validdatalength.insert("GPS大气开环通道开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "GPS大气开环通道关闭")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        ui.textEdit_validdata->insertPlainText("09");
        map_name_to_code.insert("GPS大气开环通道关闭", 0x09);
        map_name_to_validdatalength.insert("GPS大气开环通道关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "BD大气开环通道开启")
    {
        telecontrol_frame_up.cmd_code = 0x0A;
        ui.textEdit_validdata->insertPlainText("0A");
        map_name_to_code.insert("BD大气开环通道开启", 0x0A);
        map_name_to_validdatalength.insert("BD大气开环通道开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "BD大气开环通道关闭")
    {
        telecontrol_frame_up.cmd_code = 0x0B;
        ui.textEdit_validdata->insertPlainText("0B");
        map_name_to_code.insert("BD大气开环通道关闭", 0x0B);
        map_name_to_validdatalength.insert("BD大气开环通道关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "掩星接收机切正常工作模式")
    {
        telecontrol_frame_up.cmd_code = 0x0C;
        ui.textEdit_validdata->insertPlainText("0C");
        map_name_to_code.insert("掩星接收机切正常工作模式", 0x0C);
        map_name_to_validdatalength.insert("掩星接收机切正常工作模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "掩星接收机切数据传输模式")
    {
        telecontrol_frame_up.cmd_code = 0x0D;
        ui.textEdit_validdata->insertPlainText("0D");
        map_name_to_code.insert("掩星接收机切数据传输模式", 0x0D);
        map_name_to_validdatalength.insert("掩星接收机切数据传输模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "Flash数据擦除")
    {
        telecontrol_frame_up.cmd_code = 0x0E;
        ui.textEdit_validdata->insertPlainText("0E");
        map_name_to_code.insert("Flash数据擦除", 0x0E);
        map_name_to_validdatalength.insert("Flash数据擦除", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "Flash坏块更新")
    {
        telecontrol_frame_up.cmd_code = 0x0F;
        ui.textEdit_validdata->insertPlainText("0F");
        map_name_to_code.insert("Flash坏块更新", 0x0F);
        map_name_to_validdatalength.insert("Flash坏块更新", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DDR监控开启")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        ui.textEdit_validdata->insertPlainText("10");
        map_name_to_code.insert("DDR监控开启", 0x10);
        map_name_to_validdatalength.insert("DDR监控开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DDR监控关闭")
    {
        telecontrol_frame_up.cmd_code = 0x11;
        ui.textEdit_validdata->insertPlainText("11");
        map_name_to_code.insert("DDR监控关闭", 0x11);
        map_name_to_validdatalength.insert("DDR监控关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "程序上注允许")
    {
        telecontrol_frame_up.cmd_code = 0x12;
        ui.textEdit_validdata->insertPlainText("12");
        map_name_to_code.insert("程序上注允许", 0x12);
        map_name_to_validdatalength.insert("程序上注允许", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "程序上注结束")
    {
        telecontrol_frame_up.cmd_code = 0x13;
        ui.textEdit_validdata->insertPlainText("13");
        map_name_to_code.insert("程序上注结束", 0x13);
        map_name_to_validdatalength.insert("程序上注结束", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "程序上注启动")
    {
        telecontrol_frame_up.cmd_code = 0x14;
        ui.textEdit_validdata->insertPlainText("14");
        map_name_to_code.insert("程序上注启动", 0x14);
        map_name_to_validdatalength.insert("程序上注启动", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "SPI数据发送开始")
    {
        telecontrol_frame_up.cmd_code = 0x15;
        ui.textEdit_validdata->insertPlainText("15");
        map_name_to_code.insert("SPI数据发送开始", 0x15);
        map_name_to_validdatalength.insert("SPI数据发送开始", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "SPI数据发送停止")
    {
        telecontrol_frame_up.cmd_code = 0x16;
        ui.textEdit_validdata->insertPlainText("16");
        map_name_to_code.insert("SPI数据发送停止", 0x16);
        map_name_to_validdatalength.insert("SPI数据发送停止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "模式1测试开启")
    {
        telecontrol_frame_up.cmd_code = 0x17;
        ui.textEdit_validdata->insertPlainText("17");
        map_name_to_code.insert("模式1测试开启", 0x17);
        map_name_to_validdatalength.insert("模式1测试开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "模式1测试关闭")
    {
        telecontrol_frame_up.cmd_code = 0x18;
        ui.textEdit_validdata->insertPlainText("18");
        map_name_to_code.insert("模式1测试关闭", 0x18);
        map_name_to_validdatalength.insert("模式1测试关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "模式2测试开启")
    {
        telecontrol_frame_up.cmd_code = 0x19;
        ui.textEdit_validdata->insertPlainText("19");
        map_name_to_code.insert("模式2测试开启", 0x19);
        map_name_to_validdatalength.insert("模式2测试开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "模式2测试关闭")
    {
        telecontrol_frame_up.cmd_code = 0x1A;
        ui.textEdit_validdata->insertPlainText("1A");
        map_name_to_code.insert("模式2测试关闭", 0x1A);
        map_name_to_validdatalength.insert("模式2测试关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "掩星观测单元遥测查询")
    {
        ACK_reply = 0;
        telecontrol_frame_up.cmd_code = 0x1C;
        ui.textEdit_validdata->insertPlainText("1C");
        map_name_to_code.insert("掩星观测单元遥测查询", 0x1C);
        map_name_to_validdatalength.insert("掩星观测单元遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    /*----------------------DCS采集单元------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x67;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "DCS载荷软复位")
    {
        telecontrol_frame_up.cmd_code = 0x01;
        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("DCS载荷软复位", 0x01);
        map_name_to_validdatalength.insert("DCS载荷软复位", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "实时DCS数据帧传输使能")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("02");
        map_name_to_code.insert("实时DCS数据帧传输使能", 0x02);
        map_name_to_validdatalength.insert("实时DCS数据帧传输使能", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "实时DCS数据帧传输禁止")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        ui.textEdit_validdata->insertPlainText("03");
        map_name_to_code.insert("实时DCS数据帧传输禁止", 0x03);
        map_name_to_validdatalength.insert("实时DCS数据帧传输禁止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "固存数据顺序422回放")
    {
        telecontrol_frame_up.cmd_code = 0x04;
        ui.textEdit_validdata->insertPlainText("04");
        map_name_to_code.insert("固存数据顺序422回放", 0x04);
        map_name_to_validdatalength.insert("固存数据顺序422回放", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "固存数据停止422回放")
    {
        telecontrol_frame_up.cmd_code = 0x05;
        ui.textEdit_validdata->insertPlainText("05");
        map_name_to_code.insert("固存数据停止422回放", 0x05);
        map_name_to_validdatalength.insert("固存数据停止422回放", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DCS 12h自复位使能")
    {
        telecontrol_frame_up.cmd_code = 0x06;
        ui.textEdit_validdata->insertPlainText("06");
        map_name_to_code.insert("DCS 12h自复位使能", 0x06);
        map_name_to_validdatalength.insert("DCS 12h自复位使能", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "DCS 12h自复位禁止")
    {
        telecontrol_frame_up.cmd_code = 0x07;
        ui.textEdit_validdata->insertPlainText("07");
        map_name_to_code.insert("DCS 12h自复位禁止", 0x07);
        map_name_to_validdatalength.insert("DCS 12h自复位禁止", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DCS固存读写地址备份")
    {
        telecontrol_frame_up.cmd_code = 0x08;
        ui.textEdit_validdata->insertPlainText("08");
        map_name_to_code.insert("DCS固存读写地址备份", 0x08);
        map_name_to_validdatalength.insert("DCS固存读写地址备份", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DCS载荷功放允许开启")
    {
        telecontrol_frame_up.cmd_code = 0x09;
        ui.textEdit_validdata->insertPlainText("09");
        map_name_to_code.insert("DCS载荷功放允许开启", 0x09);
        map_name_to_validdatalength.insert("DCS载荷功放允许开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "DCS载荷功放禁止开启")
    {
        telecontrol_frame_up.cmd_code = 0x0A;
        ui.textEdit_validdata->insertPlainText("0A");
        map_name_to_code.insert("DCS载荷功放禁止开启", 0x0A);
        map_name_to_validdatalength.insert("DCS载荷功放禁止开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DCS数传下行开启")
    {
        telecontrol_frame_up.cmd_code = 0x0B;
        ui.textEdit_validdata->insertPlainText("0B");
        map_name_to_code.insert("DCS数传下行开启", 0x0B);
        map_name_to_validdatalength.insert("DCS数传下行开启", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "DCS数传下行关闭")
    {
        telecontrol_frame_up.cmd_code = 0x0C;
        ui.textEdit_validdata->insertPlainText("0C");
        map_name_to_code.insert("DCS数传下行关闭", 0x0C);
        map_name_to_validdatalength.insert("DCS数传下行关闭", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，已有有效数据！");
        return 1;
    }

    if (aa_string == "调整DCS记录地址")
    {
        telecontrol_frame_up.cmd_code = 0x0D;
        map_name_to_code.insert("调整DCS记录地址", 0x0D);
        map_name_to_validdatalength.insert("调整DCS记录地址", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：W1W2（单位32KB）！");
        return 1;
    }
    if (aa_string == "调整DCS回放地址")
    {
        telecontrol_frame_up.cmd_code = 0x0E;
        map_name_to_code.insert("调整DCS回放地址", 0x0E);
        map_name_to_validdatalength.insert("调整DCS回放地址", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0~DATA1：W1W2（单位32KB）！");
        return 1;
    }


    /*----------------------交互操作指令-------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x5e;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;

    QtWorkparameter* QtWorkparameter1;
    QtWorkparameter1 = new QtWorkparameter;

    uchar* aa2 = QtWorkparameter1->valid_data_to_send_data();
    QString str0;
    QString str;
    if (aa_string == "成像任务设置")
    {
        qDebug() << "aa2";
        
        for (int i = 0; i < 100; i++)
        {
            str0 = QString("%1").arg(aa2[i] & 0xFF, 2, 16, QLatin1Char('0'));
            str += str0;
        }

        ui.textEdit_validdata->insertPlainText(str);
        telecontrol_frame_up.cmd_code = 0x01;
        map_name_to_code.insert("成像任务设置", 0x01);
        map_name_to_validdatalength.insert("成像任务设置", 100);
        ui.plainTextEdit_reminder->appendPlainText("100字节，任务中设置！");
        return 1;
    }

    if (aa_string == "任务清除")
    {
        telecontrol_frame_up.cmd_code = 0x02;
        ui.textEdit_validdata->insertPlainText("0202");
        map_name_to_code.insert("任务清除", 0x02);
        map_name_to_validdatalength.insert("任务清除", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "数传站设置")
    {
        telecontrol_frame_up.cmd_code = 0x03;
        ui.textEdit_validdata->insertPlainText("0202");
        map_name_to_code.insert("数传站设置", 0x03);
        map_name_to_validdatalength.insert("数传站设置", 80);
        ui.plainTextEdit_reminder->appendPlainText("80字节，！");
        return 1;
    }
    if (aa_string == "系统预重启")
    {
        telecontrol_frame_up.cmd_code = 0xD1;
        ui.textEdit_validdata->insertPlainText("D1D1");
        map_name_to_code.insert("系统预重启", 0xD1);
        map_name_to_validdatalength.insert("系统预重启", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，已有有效数据！");
        return 1;
    }
    if (aa_string == "存储回放")
    {
        telecontrol_frame_up.cmd_code = 0xF2;
        map_name_to_code.insert("存储回放", 0xF2);
        map_name_to_validdatalength.insert("存储回放", 11);
        ui.plainTextEdit_reminder->appendPlainText("11字节，DATA0~DATA3：数传站经度\
            DATA4~DATA7：数传站纬度\
            DATA8：固存单机编号\
            6EH：数据压缩处理单元A\
            6FH：数据压缩处理单元B\
            DATA9：固存文件号\
            DATA10：是否是续传！");
        return 1;
    }
    if (aa_string == "星务成像任务强制结束")
    {
        telecontrol_frame_up.cmd_code = 0xF4;
        map_name_to_code.insert("星务成像任务强制结束", 0xF4);
        map_name_to_validdatalength.insert("星务成像任务强制结束", 2);
        ui.plainTextEdit_reminder->appendPlainText("2字节，DATA0：55H，强制置星务成像状态为空闲，结束超时成像任务；\
            AAH，强制结束回放任务。DATA1：与DATA0一致则指令有效！");
        return 1;
    }

    /*----------------------数据上注指令------------------------------------*/

    telecontrol_frame_up.cmd_style = 0x4a;
    telecontrol_frame_up.single_number = 0x5A;
    telecontrol_frame_up.multiframe_count = 0;
    ACK_reply = 1;
    single_multiframe = 0;
    if (aa_string == "星务软件预上注")
    {
        telecontrol_frame_up.cmd_code = 0xC9;
        map_name_to_code.insert("星务软件预上注", 0xC9);
        map_name_to_validdatalength.insert("星务软件预上注", 9);
        ui.plainTextEdit_reminder->appendPlainText("9字节，DATA0~DATA1：软件版本\
            DATA2~DATA5：软件大小，KB\
            DATA6：软件类型\
            DATA7：上注版本\
            AAH = 上注版本1，BBH = 上注版本2，\
            CCH = 上注版本3，DDH = 上注版本4，\
            55H = 上注版本5，66H = 上注版本6\
            DATA8：指定存储区域\
            bit1：备份区1，bit2：备份区2，\
            bit3：备份区3，bit4：备份区4，\
            bit5：备份区5，bit6：备份区6");
        return 1;
    }
    if (aa_string == "星务软件上注")
    {
        telecontrol_frame_up.cmd_code = 0xCA;
        map_name_to_code.insert("星务软件上注", 0xCA);
        map_name_to_validdatalength.insert("星务软件上注", 242);
        ui.plainTextEdit_reminder->appendPlainText("242字节，DATA0~DATA3：数据帧总包数\
            DATA4~DATA7：当前数据包序号（0~n）\
            DATA8：当前帧数据长度\
            DATA9~DATA241：有效数据");
        return 1;
    }
    if (aa_string == "授权码上注")
    {
        telecontrol_frame_up.cmd_code = 0xCB;
        map_name_to_code.insert("授权码上注", 0xCB);
        map_name_to_validdatalength.insert("授权码上注", 242);
        ui.plainTextEdit_reminder->appendPlainText("242字节，DATA0~DATA3：数据帧总包数\
            DATA4~DATA7：当前数据包序号（0~n）\
            DATA8：当前帧数据长度\
            DATA9~DATA241：有效数据");
        return 1;
    }
    if (aa_string == "音频数据上注")
    {
        telecontrol_frame_up.cmd_code = 0xCC;
        map_name_to_code.insert("音频数据上注", 0xCC);
        map_name_to_validdatalength.insert("音频数据上注", 242);
        ui.plainTextEdit_reminder->appendPlainText("242字节，DATA0：上注区域 01H~05H共5个区域\
            DATA1~DATA4：数据帧总包数\
            DATA5~DATA8：当前数据包序号（0~n）\
            DATA9：当前帧数据长度\
            DATA10~DATA241：有效数据");
        return 1;
    }

    /*----------------------直接遥控指令------------------------------------*/

    telecontrol_frame_up.cmd_style = 0xCC;

    if (aa_string == "展开释放机构A备起爆（热刀）开")
    {
        ui.textEdit_validdata->insertPlainText("BE1BE1BE1BE1BE1BE1");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "展开释放机构A备起爆（热刀）关")
    {
        ui.textEdit_validdata->insertPlainText("1EB1EB1EB1EB1EB1EB");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "展开释放机构B备起爆（热刀）开")
    {
        ui.textEdit_validdata->insertPlainText("694694694694694694");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "展开释放机构B备起爆（热刀）关")
    {
        ui.textEdit_validdata->insertPlainText("496496496496496496");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "U天线展开起爆（热刀）开")
    {
        ui.textEdit_validdata->insertPlainText("257257257257257257");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "U天线展开起爆（热刀）关")
    {
        ui.textEdit_validdata->insertPlainText("752752752752752752");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "V天线展开起爆（热刀）开")
    {
        ui.textEdit_validdata->insertPlainText("F1AF1AF1AF1AF1AF1A");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "V天线展开起爆（热刀）关")
    {
        ui.textEdit_validdata->insertPlainText("A1FA1FA1FA1FA1FA1F");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }

    if (aa_string == "DCS载荷备起爆（热刀）开")
    {
        ui.textEdit_validdata->insertPlainText("8CD8CD8CD8CD8CD8CD");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "DCS载荷备起爆（热刀）关")
    {
        ui.textEdit_validdata->insertPlainText("DC8DC8DC8DC8DC8DC8");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "蓄电池放电开关通")
    {
        ui.textEdit_validdata->insertPlainText("9B79B79B79B79B79B7");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "蓄电池放电开关断")
    {
        ui.textEdit_validdata->insertPlainText("56A56A56A56A56A56A");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "AMP供电通")
    {
        ui.textEdit_validdata->insertPlainText("12D12D12D12D12D12D");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "AMP供电断")
    {
        ui.textEdit_validdata->insertPlainText("D21D21D21D21D21D21");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }

    if (aa_string == "姿控/AMP计算机A开B关")
    {
        ui.textEdit_validdata->insertPlainText("CF1CF1CF1CF1CF1CF1");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "姿控/AMP计算机B开A关")
    {
        ui.textEdit_validdata->insertPlainText("E84E84E84E84E84E84");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "姿控/AMP计算机模式控制使用1")
    {
        ui.textEdit_validdata->insertPlainText("D5BD5BD5BD5BD5BD5B");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "姿控/AMP计算机模式控制使用2")
    {
        ui.textEdit_validdata->insertPlainText("416416416416416416");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "姿控/AMP计算机切位主节点")
    {
        ui.textEdit_validdata->insertPlainText("7E27E27E27E27E27E2");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "星务计算机A开B关")
    {
        ui.textEdit_validdata->insertPlainText("ACFACFACFACFACFACF");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }

    if (aa_string == "星务计算机B开A关")
    {
        ui.textEdit_validdata->insertPlainText("DE8DE8DE8DE8DE8DE8");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }
    if (aa_string == "星务计算机备份主控切位从节点")
    {
        ui.textEdit_validdata->insertPlainText("7CB7CB7CB7CB7CB7CB");
        ui.plainTextEdit_reminder->appendPlainText("已有重复6次的有效数据");
        return 1;
    }

}

void QtCommandtest::btn_to_del_select()
{
    qDebug() << "btn_to_del_select";
    if (row_in_cmd > 0)
    {
        QListWidgetItem* aItem = ui.listWidget_selectcmd->takeItem(row_in_cmd); //移除指定行的项，但不delete
        delete aItem; //需要手工删除对象
    }
    else
    {
        qDebug() << "no_del_select";
    }   
}

void QtCommandtest::btn_to_buildup_validdata()
{

    QList<QListWidgetItem*>  aItem1 = ui.listWidget_build_frame->findItems(select_in_list, Qt::MatchExactly);
    if (aItem1.isEmpty())
    {
        QListWidgetItem* aItem;
        aItem = new QListWidgetItem();
        aItem->setText(select_in_list);
        ui.listWidget_build_frame->addItem(aItem);
    }
    else
    {
        ui.plainTextEdit_reminder->appendPlainText("已组帧项中有重复 ！");
        return;
    }

    QString run_attr = ui.comboBox_exeattr->currentText();
    QString delay_time = ui.lineEdit->text();
    quint32 delay_time_int = delay_time.toInt();
    qint8   cmd_attribute;
    qint8   validdata_length = map_name_to_validdatalength[select_in_cmd];
    quint16 total_validdata_length;
    QString valid_data = ui.textEdit_validdata->toPlainText();
    quint8* data_to_send;
    quint16 CRC_byte_change_order;
    int j = 0;
    quint8 validdata_length_in_text = valid_data.length();
    quint8* valid_data_int;
    valid_data_int = new quint8[validdata_length];
    QString valid_data_byte;

    quint32 total_data_len;

    if (validdata_length_in_text % 2)
    {
        valid_data = valid_data.prepend("0");
    }
    for (int i = 0; i < validdata_length_in_text; i = i + 2, j++)
    {
        valid_data_byte = valid_data.left(2);
        valid_data = valid_data.mid(2);
        valid_data_int[j] = valid_data_byte.toUInt(NULL, 16);
    }
    if (run_attr == "立即执行")
    {
        cmd_attribute = 0x1;
    }
    else
    {
        cmd_attribute = 0x2;
    }
    delay_time_int = qToBigEndian(delay_time_int);
    telecontrol_frame_up.cmd_run_time = delay_time_int;
    telecontrol_frame_up.ACK_cmdattribute_multiframe = ACK_reply << 4 | cmd_attribute << 2 | single_multiframe;

    total_validdata_length = 0;
    /*----------------------*/

    if ((validdata_length + 14) <= 16)
    {
        telecontrol_frame_up.pattern_byte = 0x5a;
        total_validdata_length = 16;
        data_to_send = new quint8[9 + total_validdata_length];
    }
    else if ((validdata_length + 14) <= 64)
    {
        telecontrol_frame_up.pattern_byte = 0x69;
        total_validdata_length = 64;
        data_to_send = new quint8[9 + total_validdata_length];
    }
    else if ((validdata_length + 14) <= 128)
    {
        telecontrol_frame_up.pattern_byte = 0x59;
        total_validdata_length = 128;
        data_to_send = new quint8[9 + total_validdata_length];
    }
    else if ((validdata_length + 14) <= 256)
    {
        telecontrol_frame_up.pattern_byte = 0xa5;
        total_validdata_length = 256;
        data_to_send = new quint8[9 + total_validdata_length];
    }
    else
    {
        ui.plainTextEdit_reminder->appendPlainText("有效数据位数错误！！");       
    }

    memset(data_to_send, 0xAA, total_validdata_length + 9);
    memcpy(data_to_send, &telecontrol_frame_up, sizeof(telecontrol_frame_up_t));
    memcpy(data_to_send + sizeof(telecontrol_frame_up_t), valid_data_int, validdata_length);
    CRC_byte = crc16_ccitt(total_validdata_length, (quint8*)(data_to_send + 5));
    CRC_byte_change_order = qToBigEndian(CRC_byte);
    telecontrol_frame_down.CRC_byte = CRC_byte_change_order;
    memcpy(data_to_send + 5 + total_validdata_length, &telecontrol_frame_down, sizeof(telecontrol_frame_down_t));

    quint8* send_data_add_len = new quint8[4 + total_validdata_length + 9];

    quint32 msg_len = total_validdata_length + 9;

    msg_len = qToBigEndian(msg_len);
    memcpy(send_data_add_len, &msg_len, 4);
    memcpy(send_data_add_len + 4, data_to_send, total_validdata_length + 9);
   
   if (tcp_to_ground_ctl_handle != NULL)
    {
       qDebug() << "连接到了运控！";
       send_data_build_frame[select_in_cmd] = send_data_add_len;
       send_data_len_build_frame[select_in_cmd] = 4 + total_validdata_length + 9;
    }
    else
    {
        qDebug() << "没有连接运控";

        send_data_build_frame[select_in_cmd] = data_to_send;

        send_data_len_build_frame[select_in_cmd] = total_validdata_length + 9;
    }    
}

quint16 QtCommandtest::crc16_ccitt(int len, quint8* data)
{
    quint16 crc = 0xFFFF;
    quint8 i = 0;

    if ((data == NULL) || (len == 0))
    {
        return 0;
    }
    while (len--)
    {
        crc ^= *data++ << 8;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;    
}

qint8 QtCommandtest::btn_to_send_cmd()
{ 
    qDebug() << "btn_to_send_cmd";
    if (select_in_frame_list == Q_NULLPTR)
    {
        ui.plainTextEdit_reminder->appendPlainText("没有选择组帧命令！！");
        return -1;
    }
    int send_data_amount = send_data_len_build_frame[select_in_frame_list];
    quint8* send_data_build_frmae = send_data_build_frame[select_in_frame_list];
    qDebug() << send_data_amount;
  
    for (quint16 i = 0; i < send_data_amount; i++)
    {
   //     qDebug() << send_data_build_frmae[i];
    }
   tcp_to_ground_ctl_handle->write((char*)send_data_build_frmae, send_data_amount);
    return 1;
}

static QByteArray rev_telemetry;
qint8 QtCommandtest::update_ReadServer_data()
{
    qint8 frame_data_len;
    QByteArray temp;
    rev_valid_data_head_t rev_valid_data_head;
    rev_data_t rev_data;
    reply_valid_data_t reply_valid_data;

    int i;
    if (rev_telemetry.isNull())
    {
        temp = m_tcpClient->readAll();
    }
    else
    {
        qDebug() <<"enter rev_telemetry" << endl;
        temp = rev_telemetry;
    }
    
    QDataStream out(&temp, QIODevice::ReadWrite);    //将字节数组读入

    out >>rev_data.satellite_syn_byte;
    rev_data.satellite_syn_byte = qFromBigEndian(rev_data.satellite_syn_byte);

    out >> rev_data.authorize_byte;
    rev_data.authorize_byte = qFromBigEndian(rev_data.authorize_byte);

    for (i = 0; i < 7; i++)
    {
        out >> rev_data.frame_head[i];
    }
    out >> rev_data.usr_id;
    rev_data.usr_id = qFromBigEndian(rev_data.usr_id);

    out >> rev_data.terminal_id;
    rev_data.terminal_id = qFromBigEndian(rev_data.terminal_id);

    quint8 pkg_type = (rev_data.frame_head[0] & 0x18) >> 3;

    if (pkg_type == 3)
    {
        qDebug() << "广播遥测";

        for (i = 0; i < 231; i++)
        {
            out >> rev_data.valid_data[i];
        }
    }
    else if (pkg_type == 1)
    {
        qDebug() << "应答遥测";
        out >> reply_valid_data.valid_data_len;
        reply_valid_data.valid_data_len = qFromBigEndian(reply_valid_data.valid_data_len);

        out >> reply_valid_data.cmd_type;
        out >> reply_valid_data.single_num;
        out >> reply_valid_data.cmd_num;
        out >> reply_valid_data.reply_type;
        out >> reply_valid_data.reply_result;

        qDebug() << reply_valid_data.cmd_num;

        for (i = 0; i < 224; i++)
        {
            out >> reply_valid_data.reply_telemetry_data[i];
        } 
    }
    else
    {
        qDebug() << "pkg_type err!";
        qDebug() << pkg_type;
    }

    
    out >> rev_data.fault_tolerance;
    rev_data.fault_tolerance = qFromBigEndian(rev_data.fault_tolerance);

   
    quint16 CRC_byte_a = crc16_ccitt(250, (quint8*)&(rev_data.authorize_byte));

    QString str;

    QString display_analyze_result;

    frame_data_len = rev_data.frame_head[6];  
    if(pkg_type == 3)   //广播遥测
    {
        ui.plainTextEdit_result->appendPlainText("星上时间源：");
        str = QString::number(rev_data.valid_data[0], 16);
        if (str == "22")
        {
            ui.plainTextEdit_result->insertPlainText("星务计算机");
        }

        UTC_time = (quint32*)&(rev_data.valid_data[1]);

        str = QString::number(*UTC_time, 16);

        ui.plainTextEdit_result->appendPlainText("UTC时间：");
        ui.plainTextEdit_result->insertPlainText(str);

        sys_run_time = (quint32*)& (rev_data.valid_data[5]);
        str = QString::number(*sys_run_time, 16);
        ui.plainTextEdit_result->appendPlainText("系统运行时间：");
        ui.plainTextEdit_result->insertPlainText(str);

        sys_restart_time = (quint16*)&(rev_data.valid_data[11]);
        str = QString::number(*sys_restart_time, 16);
        ui.plainTextEdit_result->appendPlainText("系统重启次数：");
        ui.plainTextEdit_result->insertPlainText(str);

        sys_reset_time = (quint16*)&(rev_data.valid_data[11]);
        str = QString::number(*sys_reset_time, 16);
        ui.plainTextEdit_result->appendPlainText("系统复位次数：");
        ui.plainTextEdit_result->insertPlainText(str);

        cmd_err_cnt = (quint16*)&(rev_data.valid_data[13]);
        str = QString::number(*cmd_err_cnt, 16);
        ui.plainTextEdit_result->appendPlainText("遥控指令解析错误计数：");
        ui.plainTextEdit_result->insertPlainText(str);


        cmd_cnt = (quint16*)&(rev_data.valid_data[15]);
        str = QString::number(*cmd_cnt, 16);
        ui.plainTextEdit_result->appendPlainText("遥控指令计数：");
        ui.plainTextEdit_result->insertPlainText(str);


 //       ui.plainTextEdit_result->insertPlainText(str);
        qDebug() << "rev_data.frame_head[0]  != 0x18  aaa" << endl;
        return 1;
    }    
    else if (pkg_type == 1)  //应答遥测
    {
        if (reply_valid_data.cmd_type != 0x3a)  //间接指令
        {
            return -1;
        }
        if (reply_valid_data.single_num == 0x5a) // 星务计算机XW  单机编号
        {
            display_analyze_result += "星务计算机 ";

         //   if (reply_valid_data.cmd_num == 0x01)
            
          //      display_analyze_result += "测试模式 ";

                if (reply_valid_data.reply_type == 0x01)  //ACK应答遥测
                {
                    display_analyze_result += analyze_cmd_number(reply_valid_data);
                    display_analyze_result += "ACK应答遥测 ";
                    display_analyze_result += analyze_ack_telemetry_reply(reply_valid_data);
                }
                else if (reply_valid_data.reply_type == 0x02)   //数据应答遥测
                {
                    display_analyze_result += "数据应答遥测";

                    if (reply_valid_data.reply_result == 0)
                    {
                        display_analyze_result += "失败";
                    }
                    else if (reply_valid_data.reply_result == 1)
                    {
                        display_analyze_result += "成功";

                        switch (reply_valid_data.cmd_num) 
                        {
                        case 0xa:
                            display_analyze_result += "星务计算机遥测查询 ";
                            display_analyze_result += analyze_xw_telemetry_inquire(reply_valid_data);
                            break;
                        case 0xb:
                            display_analyze_result += "星务计算机日志下载 ";
                            display_analyze_result += analyze_xw_log_download(reply_valid_data);
                            break;
                        case 0xc:
                            display_analyze_result += "星务计算机访问记录下载 ";
                            display_analyze_result += analyze_xw_visit_log_download(reply_valid_data);
                            break;
                        case 0xd:
                            display_analyze_result += "星务计算机短信息下载 ";
                            display_analyze_result += analyze_xw_short_message_download(reply_valid_data);
                            break;
                        case 0xe:
                            display_analyze_result += "任务请求记录文件下载 ";
                            display_analyze_result += analyze_xw_task_apply_log_download(reply_valid_data);
                            break;
                        case 0xf:
                            display_analyze_result += "任务执行记录文件下载 ";
                            display_analyze_result += analyze_xw_task_execute_log_download(reply_valid_data);
                            break;
                        case 0x10:
                            display_analyze_result += "单机设备透传遥测查询 ";
                            display_analyze_result += analyze_xw_dev_penetrate_telemetry_inquire(reply_valid_data);
                            break;
                        case 0x14:
                            display_analyze_result += "PCDU遥测查询 ";
                            display_analyze_result += analyze_xw_PCDU_telemetry_inquire(reply_valid_data);
                            break;
                        case 0x15:
                            display_analyze_result += "测控数传一体机遥测查询 ";
                            display_analyze_result += analyze_xw_mx_data_tran_telemetry_inquire(reply_valid_data);
                            break;
                        case 0x16:
                            display_analyze_result += "姿控分系统遥测查询  ";
                            display_analyze_result += analyze_xw_posture_sys_telemetry_inquire(reply_valid_data);
                            break;
                        case 0x17:
                            display_analyze_result += "GNSS接收机AB遥测查询 ";
                            display_analyze_result += analyze_xw_GNSS_receiver_AB_telemetry_inquire(reply_valid_data);
                            break;
                        case 0x26:
                            display_analyze_result += "星上授权码下载 ";
                            display_analyze_result += analyze_xw_star_authorization_code_download(reply_valid_data);
                            break;
                        default:
                            display_analyze_result += "解析错误！ ";
                        }
                            
                    }
                    else
                    {
                        display_analyze_result += "应答数据错误";
                    }
                }
                else
                {
                    return -2;
                }
                        
            ui.plainTextEdit_reply_telemetry->appendPlainText(display_analyze_result);
        }
        else if (reply_valid_data.single_num == 0x5b) // AMP、ZK分系统 AZ
        {
            display_analyze_result += "姿控计算机 ";
            if (reply_valid_data.reply_type == 0x01)  //ACK应答遥测
            {
                display_analyze_result += "ACK应答遥测 ";
                display_analyze_result += analyze_ack_telemetry_reply(reply_valid_data);
            }
            else if (reply_valid_data.reply_type == 0x02)   //数据应答遥测
            {
                display_analyze_result += "数据应答遥测";
            }
            else
            {
                return -2;
            }
            ui.plainTextEdit_reply_telemetry->appendPlainText(display_analyze_result);
        }
        else if (rev_valid_data_head.single_number == 0x5c) //控制运算分系统
        {

        }
        else if (rev_valid_data_head.single_number == 0x5d)  //交互操作分系统
        {

        }
        else  if (rev_valid_data_head.single_number == 0x5e) //PCDU
        {
            if (rev_valid_data_head.ACK_type == 0x02)   //数据应答遥测
            {
                if (rev_valid_data_head.ACK_result == 0x01)   //应答结果成功
                {
                    if (rev_valid_data_head.cmd_code == 0x0a) //遥测查询
                    {
                        if (rev_data.valid_data[8] == 0x11)
                        {
                            ui.plainTextEdit_result->appendPlainText("星上事件源：GNSS");
                        }
                        else if (rev_data.valid_data[8] == 0x22)
                        {
                            ui.plainTextEdit_result->appendPlainText("星上事件源：星务计算机");
                        }
                        else
                        {
                            ui.plainTextEdit_result->appendPlainText("星上事件源错误");
                        }

                        UTC_time = (quint32*)&(rev_data.valid_data[1]);

                        str = QString::number(*UTC_time, 16);

                        ui.plainTextEdit_result->appendPlainText("UTC时间：");
                        ui.plainTextEdit_result->insertPlainText(str);

                        sys_run_time = (quint32*)&(rev_data.valid_data[5]);
                        str = QString::number(*sys_run_time, 16);
                        ui.plainTextEdit_result->appendPlainText("系统运行时间：");
                        ui.plainTextEdit_result->insertPlainText(str);

                        sys_restart_time = (quint16*)&(rev_data.valid_data[11]);
                        str = QString::number(*sys_restart_time, 16);
                        ui.plainTextEdit_result->appendPlainText("系统重启次数：");
                        ui.plainTextEdit_result->insertPlainText(str);

                        sys_reset_time = (quint16*)&(rev_data.valid_data[11]);
                        str = QString::number(*sys_reset_time, 16);
                        ui.plainTextEdit_result->appendPlainText("系统复位次数：");
                        ui.plainTextEdit_result->insertPlainText(str);

                        cmd_err_cnt = (quint16*)&(rev_data.valid_data[13]);
                        str = QString::number(*cmd_err_cnt, 16);
                        ui.plainTextEdit_result->appendPlainText("遥控指令解析错误计数：");
                        ui.plainTextEdit_result->insertPlainText(str);


                        cmd_cnt = (quint16*)&(rev_data.valid_data[15]);
                        str = QString::number(*cmd_cnt, 16);
                        ui.plainTextEdit_result->appendPlainText("遥控指令计数：");
                        ui.plainTextEdit_result->insertPlainText(str);


                    }
                    else if (rev_valid_data_head.cmd_code == 0x0b)  //日志下载
                    {
                        memcpy(&log_download, &rev_data.valid_data[8], sizeof(log_download_t));
                        str = QString::number(log_download.log_notes_1, 16);
                        ui.plainTextEdit_result->appendPlainText("日志记录号1：");
                        ui.plainTextEdit_result->insertPlainText(str);
                    }
                    else if (rev_valid_data_head.cmd_code == 0x0c)  //访问记录下载
                    {
                        memcpy(&visit_record_download, &rev_data.valid_data[8], sizeof(visit_record_download_t));
                        str = QString::number(visit_record_download.visit_record_time_1, 16);
                        ui.plainTextEdit_result->appendPlainText("访问记录号1：");
                        ui.plainTextEdit_result->insertPlainText(str);
                    }
                    else if (rev_valid_data_head.cmd_code == 0x0d) //短信息下载
                    {
                        memcpy(&short_message_download, &rev_data.valid_data[8], sizeof(short_message_download_t));
                        str = QString::number(short_message_download.message_rev_time, 16);
                        ui.plainTextEdit_result->appendPlainText("接收时间：");
                        ui.plainTextEdit_result->insertPlainText(str);
                    }
                }
            }
        }
        else if (rev_valid_data_head.single_number == 0x5f)  //VU通信机
        {

        }
        else if (rev_valid_data_head.single_number == 0x60)  // UXB测控数传一体机
        {

        }
        else if (rev_valid_data_head.single_number == 0x61)  // X相控阵天线
        {

        }
        else  if (rev_valid_data_head.single_number == 0x62) //GNSS 接收机A
        {

        }
    }
    else
    {
        display_analyze_result = "包类型错误！";
    }
}

QString QtCommandtest::analyze_cmd_number(reply_valid_data_t reply_valid_data)
{
    QString  display_analyze_result;
    switch (reply_valid_data.cmd_num) {
    case 1:
        display_analyze_result = "进入测试模式 ";
        break;
    case 2:
        display_analyze_result = "清空延时指令 ";
        break;
    case 3:
        display_analyze_result = "清空延时任务 ";
        break;
    case 4:
        display_analyze_result = "星务计算机复位 ";
        break;
    case 5:
        display_analyze_result = "程序注入开始 ";
        break;
    case 6:
        display_analyze_result = "程序注入结束 ";
        break;
    case 0x18:
        display_analyze_result = "工作模式切换 ";
        break;
    case 0x19:
        display_analyze_result = "ACK应答开关设置 ";
        break;
    case 0x1a:
        display_analyze_result = "卫星上行同步码设置 ";
        break;
    case 0x1b:
        display_analyze_result = "数传通道禁用与使能设置 ";
        break;
    case 0x1c:
        display_analyze_result = "展开释放机构的禁止和使能 ";
        break;
    case 0x1d:
        display_analyze_result = "自动热控的禁止和使能 ";
        break;
    case 0x1e:
        display_analyze_result = "遥测通道的选择 ";
        break;
    case 0x1f:
        display_analyze_result = "自动轨控的禁止和使能 ";
        break;
    case 0x20:
        display_analyze_result = "定时断电时间间隔设置 ";
        break;
    case 0x21:
        display_analyze_result = "实时广播遥测的开启和关闭 ";
        break;
    case 0x22:
        display_analyze_result = "实时广播遥测的间隔时间 ";
        break;
    case 0x23:
        display_analyze_result = "授权码上注开始 ";
        break;
    case 0x24:
        display_analyze_result = "授权码上注结束 ";
        break;
    case 0x25:
        display_analyze_result = "星上授权码清除 ";
        break;
    case 0x27:
        display_analyze_result = "星务软件上注开始 ";
        break;
    case 0x28:
        display_analyze_result = "星务软件上注结束 ";
        break;
    case 0x29:
        display_analyze_result = "短消息上注 ";
        break;
    case 0x2a:
        display_analyze_result = "时间上注 ";
        break;
    case 0x2b:
        display_analyze_result = "轨道上注 ";
        break;
    case 0x2c:
        display_analyze_result = "设置最高充电电压阈值 ";
        break;
    case 0x2d:
        display_analyze_result = "设置最低放电电压阈值 ";
        break;
    case 0x2e:
        display_analyze_result = "设置单个测控通道地球站 ";
        break;
    case 0x2f:
        display_analyze_result = "设置单个数传通道地球站 ";
        break;
    case 0x30:
        display_analyze_result = "设置多个测控通道地球站 ";
        break;
    case 0x31:
        display_analyze_result = "设置多个数传通道地球站 ";
        break;
    case 0x98:
        display_analyze_result = "删除延时任务 ";
        break;
    case 0x99:
        display_analyze_result = "删除延时指令 ";
        break;
    case 0x9a:
        display_analyze_result = "星务计算机软件版本切换 ";
        break;
    case 0x9b:
        display_analyze_result = "单机设备指令透传 ";
        break;
    case 0x9c:
        display_analyze_result = "音频数据广播开启 ";
        break;
    case 0x9d:
        display_analyze_result = "音频数据广播停止 ";
        break;
    default:
        display_analyze_result = "ACK应答解析错误 ";
    }
    return display_analyze_result;
}

QString QtCommandtest::analyze_ack_telemetry_reply(reply_valid_data_t reply_valid_data)
{
    QString  analyze_result;
    if (reply_valid_data.reply_result == 0x01)   //应答结果成功
    {
        if (reply_valid_data.reply_telemetry_data[0] == 0x00)  //应答结果的处理方法
        {
            analyze_result = "指令成功接收!";
        }
        else
        {
            analyze_result = "指令成功接收 数据错误!";
        }
    }
    else if (reply_valid_data.reply_result == 0x00)  //应答结果失败
    {
        if (reply_valid_data.reply_telemetry_data[0] == 0x01)
        {
            analyze_result = "CRC校验错误!";
        }
        else if (reply_valid_data.reply_telemetry_data[0] == 0x02)
        {
            analyze_result = "授权码错误!";
        }
        else if (reply_valid_data.reply_telemetry_data[0] == 0x03)
        {
            analyze_result = "非法用户!";
        }
        else if (reply_valid_data.reply_telemetry_data[0] == 0x04)
        {
            analyze_result = "设备故障!";
        }
        else
        {
            analyze_result = "指令失败 数据错误!";                
        }
    }
    else
    {
        analyze_result = "应答结果错误!";
    }
    return analyze_result;
}

QString QtCommandtest::analyze_xw_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    QString str;

    if (reply_valid_data.reply_telemetry_data[0] == 0x11)
    {
        ui.plainTextEdit_reply_telemetry->appendPlainText("星上事件源：GNSS");
    }
    else if (reply_valid_data.reply_telemetry_data[0] == 0x22)
    {
        ui.plainTextEdit_reply_telemetry->appendPlainText("星上事件源：星务计算机");
    }
    else
    {
        ui.plainTextEdit_reply_telemetry->appendPlainText("星上事件源错误");
    }

    UTC_time = (quint32*)&(reply_valid_data.reply_telemetry_data[1]);

    str = QString::number(*UTC_time, 16);

    ui.plainTextEdit_reply_telemetry->appendPlainText("UTC时间：");
    ui.plainTextEdit_reply_telemetry->insertPlainText(str);

    sys_run_time = (quint32*)&(reply_valid_data.reply_telemetry_data[5]);
    str = QString::number(*sys_run_time, 16);
    ui.plainTextEdit_reply_telemetry->appendPlainText("系统运行时间：");
    ui.plainTextEdit_reply_telemetry->insertPlainText(str);

    sys_restart_time = (quint16*)&(reply_valid_data.reply_telemetry_data[11]);
    str = QString::number(*sys_restart_time, 16);
    ui.plainTextEdit_reply_telemetry->appendPlainText("系统重启次数：");
    ui.plainTextEdit_reply_telemetry->insertPlainText(str);

    sys_reset_time = (quint16*)&(reply_valid_data.reply_telemetry_data[11]);
    str = QString::number(*sys_reset_time, 16);
    ui.plainTextEdit_reply_telemetry->appendPlainText("系统复位次数：");
    ui.plainTextEdit_reply_telemetry->insertPlainText(str);

    cmd_err_cnt = (quint16*)&(reply_valid_data.reply_telemetry_data[13]);
    str = QString::number(*cmd_err_cnt, 16);
    ui.plainTextEdit_reply_telemetry->appendPlainText("遥控指令解析错误计数：");
    ui.plainTextEdit_reply_telemetry->insertPlainText(str);

    cmd_cnt = (quint16*)&(reply_valid_data.reply_telemetry_data[15]);
    str = QString::number(*cmd_cnt, 16);
    ui.plainTextEdit_reply_telemetry->appendPlainText("遥控指令计数：");
    ui.plainTextEdit_reply_telemetry->insertPlainText(str);

    return "解析xw 查询成功";
}

QString QtCommandtest::analyze_xw_log_download(reply_valid_data_t reply_valid_data)
{
    QString str;

    quint16 log_total_number;
    quint16 log_current_down;
    quint32 log_note;
    quint8  log_type;
    quint32 log_recore_time_second;
    quint16 log_recore_time_millisecond;
    quint8 log_content[200];

    log_total_number = (quint16)&(reply_valid_data.reply_telemetry_data[0]);
    log_current_down = (quint16) & (reply_valid_data.reply_telemetry_data[2]);
    log_note = (quint32) & (reply_valid_data.reply_telemetry_data[4]);
    log_type = (quint8) & (reply_valid_data.reply_telemetry_data[8]);
    log_recore_time_second = (quint32) & (reply_valid_data.reply_telemetry_data[9]);
    log_recore_time_millisecond = (quint16) & (reply_valid_data.reply_telemetry_data[13]);

    memcpy(log_content, &reply_valid_data.reply_telemetry_data[15],200);

    return "解析xw log成功";
}

QString QtCommandtest::analyze_xw_visit_log_download(reply_valid_data_t reply_valid_data)
{
    quint16 visit_total_number;
    quint16 visit_current_down;
    quint32 visit_note;
    quint8  visit_type;
    quint32 visit_recore_time_second;
    quint16 visit_recore_time_millisecond;
    quint8 visit_content[200];

    visit_total_number = (quint16) & (reply_valid_data.reply_telemetry_data[0]);
    visit_current_down = (quint16) & (reply_valid_data.reply_telemetry_data[2]);
    visit_note = (quint32) & (reply_valid_data.reply_telemetry_data[4]);
    visit_type = (quint8) & (reply_valid_data.reply_telemetry_data[8]);
    visit_recore_time_second = (quint32) & (reply_valid_data.reply_telemetry_data[9]);
    visit_recore_time_millisecond = (quint16) & (reply_valid_data.reply_telemetry_data[13]);

    memcpy(visit_content, &reply_valid_data.reply_telemetry_data[15], 200);

    return "解析xw visit log成功";
}

QString QtCommandtest::analyze_xw_short_message_download(reply_valid_data_t reply_valid_data)
{
    quint8 short_message_total_number;
    quint16 short_message_current_down;
    quint32 short_message_rev_time;
    quint32  short_message_terminal_id;
    quint32 short_message_send_id;
    quint32 short_message_rev_id;
    quint8 short_message_content[140];

    short_message_total_number = (quint8) & (reply_valid_data.reply_telemetry_data[0]);
    short_message_current_down = (quint16) & (reply_valid_data.reply_telemetry_data[1]);
    short_message_rev_time = (quint32) & (reply_valid_data.reply_telemetry_data[3]);
    short_message_terminal_id = (quint32) & (reply_valid_data.reply_telemetry_data[7]);
    short_message_send_id = (quint32) & (reply_valid_data.reply_telemetry_data[11]);
    short_message_rev_id = (quint32) & (reply_valid_data.reply_telemetry_data[15]);

    memcpy(short_message_content, &reply_valid_data.reply_telemetry_data[19], 140);

    return "解析xw short message成功";

}

QString QtCommandtest::analyze_xw_task_apply_log_download(reply_valid_data_t reply_valid_data)
{
    quint8 task_total_number;
    quint16 task_current_down;
    quint32 task_rev_time;
    quint32  task_terminal_id;
    quint32 task_send_id;
    quint32 task_rev_id;
    quint8 task_content[214];

    task_total_number = (quint8) & (reply_valid_data.reply_telemetry_data[0]);
    task_current_down = (quint16) & (reply_valid_data.reply_telemetry_data[1]);
    task_rev_time = (quint32) & (reply_valid_data.reply_telemetry_data[3]);
    task_terminal_id = (quint32) & (reply_valid_data.reply_telemetry_data[7]);
    task_send_id = (quint32) & (reply_valid_data.reply_telemetry_data[11]);
    task_rev_id = (quint32) & (reply_valid_data.reply_telemetry_data[15]);

    memcpy(task_content, &reply_valid_data.reply_telemetry_data[19], 140);

    return "解析xw taskapply log成功";
}

QString QtCommandtest::analyze_xw_task_execute_log_download(reply_valid_data_t reply_valid_data)
{
    quint16 task_execute_total_number;
    quint16 task_execute_current_down;
    quint32 task_execute_rev_time;
    quint32  task_execute_terminal_id;
    quint32 task_execute_send_id;
    quint8 task_execute_result;
    quint8 task_execute_result_cause;
    quint32 task_execute_start_time;
    quint32 task_execute_end_time;
    quint8 storage_device;
    quint8 storage_number;

    task_execute_total_number = (quint16) & (reply_valid_data.reply_telemetry_data[0]);
    task_execute_current_down = (quint16) & (reply_valid_data.reply_telemetry_data[2]);
    task_execute_rev_time = (quint32) & (reply_valid_data.reply_telemetry_data[4]);
    task_execute_terminal_id = (quint32) & (reply_valid_data.reply_telemetry_data[8]);
    task_execute_send_id = (quint32) & (reply_valid_data.reply_telemetry_data[12]);
    task_execute_result = (quint8) & (reply_valid_data.reply_telemetry_data[16]);
    task_execute_result_cause = (quint8) & (reply_valid_data.reply_telemetry_data[17]);
    task_execute_start_time = (quint32) & (reply_valid_data.reply_telemetry_data[18]);
    task_execute_end_time = (quint32) & (reply_valid_data.reply_telemetry_data[22]);
    storage_device = (quint8) & (reply_valid_data.reply_telemetry_data[26]);
    storage_number = (quint8) & (reply_valid_data.reply_telemetry_data[27]);

    return "解析xw task execute log成功";
}

QString QtCommandtest::analyze_xw_dev_penetrate_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint16 penetrate_data_len;
    QByteArray array;
    quint8 penetrate_content[223];

    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据

    penetrate_data_len = (quint16) & (reply_valid_data.reply_telemetry_data[0]);

    QDataStream out(&array, QIODevice::ReadWrite);

    out >> penetrate_data_len; 

    quint16 i = 0;
    while (i<223)
    {
        out >> penetrate_content[i];
        i++;
    }
    return "解析xw dev_penetrate_telemetry成功";
}

QString QtCommandtest::analyze_xw_PCDU_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint16 bus_voltage;
    quint16 bus_current;
    quint16 load_current;
    quint16 solar_cell_currentA;
    quint16 solar_cell_currentB;
    quint16 solar_cell_currentC;
    quint16 solar_cell_currentD;
    quint16 solar_cell_voltageA;
    quint16 solar_cell_voltageB;
    quint16 solar_cell_voltageC;
    quint16 solar_cell_voltageD;
    quint16 PCU_temperature;
    quint16 battery_discharge_switch_status;
    quint16 battery_discharge_charge_current;
    quint16 hardware_overdischarge_status;
    quint16 star_rocket_dispart_switch_Relay_status;
    quint16 battery_sample_status;
    quint16 battery_pack_voltage;
    quint16 battery_single_voltage1;
    quint16 battery_single_voltage2;
    quint16 battery_single_voltage3;
    quint16 battery_single_voltage4;
    quint16 unfold_release_mechanism_voltageA;
    quint16 unfold_release_mechanism_voltageB;
    quint16 unfold_release_mechanism_currentA;
    quint16 unfold_release_mechanism_currentB;
    quint16 solar_cell_array_temperature_sample1;
    quint16 solar_cell_array_temperature_sample2;
    quint16 solar_cell_array_temperature_sample3;
    quint16 solar_cell_array_temperature_sample4;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据

    QDataStream out(&array, QIODevice::ReadWrite);

    out >> bus_voltage;
    out >> bus_current;

    return "解析PCDU应答遥测成功";
}

QString QtCommandtest::analyze_xw_mx_data_tran_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8 work_voltage;
    quint8 baseband_current;
    quint8 rev_AGC;
    quint8 amplifier_current;
    quint8 aisle_status;
    quint8 telecontrol_cmd_count;
    quint8 bet_cmd_count;
    quint8 bus_cmd_count;
    quint8 self_status1;
    quint8 self_status2;
    quint8 data_transmission_status;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> work_voltage;
    out >> baseband_current;
    out >> rev_AGC;
    out >> amplifier_current;

    return "解析 测控数传一体机遥测查询 成功！";

}

QString QtCommandtest::analyze_xw_posture_sys_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint16 temperature;
    quint16 sun_sensor_light_current1;
    quint16 sun_sensor_light_current2;
    quint16 sun_sensor_light_current3;
    quint16 sun_sensor_light_current4;
    quint32 sun_sensor_posture_angle_a;
    quint32 sun_sensor_posture_angle_b;
    quint8 sun_sensor_int_dec;
    quint16 sun_sensor_sun_sum;
    quint16 sun_sensor_sun_err;
    quint16 sun_sensor_k_temp;
    quint16 sun_sensor_b_temp;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> temperature;
    out >> sun_sensor_light_current1;
    out >> sun_sensor_light_current2;
    out >> sun_sensor_light_current3;
    return "解析 姿控分系统遥测查询 成功！";
    
}

QString QtCommandtest::analyze_xw_GNSS_receiver_AB_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8 telemetry_apply_sum;
    quint8 CAN_bus_right_frame;
    quint8 CAN_bus_correct_frame;
    quint8 recent_cmd_code;
    quint8 aisle_status;
    quint8 UTC_time_date;
    quint8 UTC_time_month;
    quint8 UTC_time_year;
    quint8 UTC_time_hour;
    quint8 UTC_time_min;
    quint8 UTC_time_second;
    quint8 UTC_time_millisecond;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> telemetry_apply_sum;
    out >> CAN_bus_right_frame;
    out >> CAN_bus_correct_frame;
    out >> recent_cmd_code;
    out >> aisle_status;
    out >> UTC_time_date;
    out >> UTC_time_month;
    out >> UTC_time_year;
    return "解析 GNSS接收机AB遥测查询 成功！";
}

QString QtCommandtest::analyze_xw_star_authorization_code_download(reply_valid_data_t reply_valid_data)
{
    quint16 authorization_code_total;
    quint16 i = 0;
    typedef struct authorization_code
    {
        quint32 authorization_code;
        quint8 authorization_code_attribute;
    }authorization_code_t;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> authorization_code_total;

    authorization_code_t* authorization_code = new authorization_code_t[authorization_code_total];

    for (i = 0; i < authorization_code_total; i++)
    {
        out >> authorization_code->authorization_code;
        out >> authorization_code->authorization_code_attribute;
        authorization_code++;
    }
    authorization_code -= i;

    return "解析 星上授权码下载遥测 成功！";
}

QString QtCommandtest::analyze_GNSS_receiver_A_raw_observation_data_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8 data_type;
    quint8 data_encode;
    quint8 CAN_bus_right_frame;
    quint8 CAN_bus_correct_frame;
    quint8 recent_cmd_code;
    quint8 aisle_status;
    quint8 UTC_time_date;
    quint8 UTC_time_month;
    quint8 UTC_time_year;
    quint8 UTC_time_hour;
    quint8 UTC_time_min;
    quint8 UTC_time_second;
    quint8 UTC_time_millisecond;
    quint8 work_state;
    quint8 temperature;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> data_type;
    out >> data_encode;
    out >> CAN_bus_right_frame;
    out >> CAN_bus_correct_frame;
    out >> recent_cmd_code;
    out >> aisle_status;
    out >> UTC_time_date;
    out >> UTC_time_month;
    out >> UTC_time_year;
    out >> UTC_time_hour;
    out >> UTC_time_min;
    out >> UTC_time_second;
    out >> UTC_time_millisecond;
    out >> work_state;
    out >> temperature;

    return "解析 GNSS接收机A原始观测数据应答遥测 成功！";
}

QString QtCommandtest::analyze_occultation_observation_unit_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8 locate_state_and_flags;
    quint8 GPS_available_star;
    quint16 enumerate_cycle;
    quint32 UTC_total_second;
    quint8 monitoring_status_1;
    quint8 monitoring_status_2;
    quint8 monitoring_status_3;
    quint8 monitoring_status_4;
    quint8 monitoring_status_5;
    quint8 monitoring_status_6;
    quint8 monitoring_status_7;
    quint8 monitoring_status_8;
    quint8 monitoring_status_9;
    quint8 monitoring_status_10;
    quint8 monitoring_status_11;
    quint8 reserve_8_bit;
    quint8 tic_count;
    qint16 clock_difference;
    qint16 clock_drift;
    quint32 monitoring_status_12;
    quint16 monitoring_status_13;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> locate_state_and_flags;
    out >> GPS_available_star;
    out >> enumerate_cycle;
    out >> UTC_total_second;
    out >> monitoring_status_1;
    out >> monitoring_status_2;
    out >> monitoring_status_3;
    out >> monitoring_status_4;
    out >> monitoring_status_5;
    out >> monitoring_status_6;
    out >> monitoring_status_7;
    out >> monitoring_status_8;
    out >> monitoring_status_9;
    out >> monitoring_status_10;
    out >> monitoring_status_11;
    out >> reserve_8_bit;
    out >> tic_count;
    out >> clock_difference;
    out >> clock_drift;
    out >> monitoring_status_12;
    out >> monitoring_status_13;

    return "解析 GNSS掩星观测单元应答遥测 成功！";

}

QString QtCommandtest::analyze_star_sensor_A_selfcheck_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8 posture_valid_register;
    quint32 posture_four_elements_q1;
    quint32 posture_four_elements_q2;
    quint32 posture_four_elements_q3;
    quint32 posture_four_elements_q4;
    quint8 parameter_a;
    quint8 parameter_b;
    quint8 total_number_star;
    quint8 total_number_match_star;
    quint32 time_tran;
    quint32 parameter_c;
    quint16 restart_times;
    quint16 parameter_d;
    quint16 PCB_Temp;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> posture_valid_register;
    out >> posture_four_elements_q1;
    out >> posture_four_elements_q2;
    out >> posture_four_elements_q3;
    out >> posture_four_elements_q4;
    out >> parameter_a;
    out >> parameter_b;
    out >> total_number_star;
    out >> total_number_match_star;
    out >> time_tran;
    out >> parameter_c;
    out >> restart_times;
    out >> parameter_d;
    out >> PCB_Temp;

    return "解析 星敏感器A自检响应遥测 成功！";
}

QString QtCommandtest::analyze_star_sensor_A_control_parameters_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint16 centroiding_method;
    quint16 roi;
    quint16 thresh_ifadapt;
    quint16 threshold_lit;
    quint16 interval_grow;
    quint16 Internal_1;
    quint16 Internal_2;
    quint16 Internal_3;
    quint16 Internal_4;
    quint16 Internal_5;
    quint16 Internal_6;
    quint16 Internal_7;
    quint16 Internal_8;
    quint16 Internal_9;
    quint16 exposure_time;
    quint16 exposure_gap;
    quint16 analog_offset;
    quint16 clamp_param;
    quint16 roi_param;
    quint16 Internal_10;
    quint16 Internal_11;
    quint16 Internal_12;
    quint16 Internal_13;
    quint16 Internal_14;
    quint16 Internal_15;
    quint16 Internal_16;
    quint16 Internal_17;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> centroiding_method;
    out >> roi;
    out >> thresh_ifadapt;
    out >> threshold_lit;
    out >> interval_grow;
    out >> Internal_1;
    out >> Internal_2;
    out >> Internal_3;
    out >> Internal_4;
    out >> Internal_5;
    out >> Internal_6;
    out >> Internal_7;
    out >> Internal_8;
    out >> Internal_9;
    out >> exposure_time;
    out >> exposure_gap;
    out >> analog_offset;
    out >> clamp_param;
    out >> roi_param;
    out >> Internal_10;
    out >> Internal_11;
    out >> Internal_12;
    out >> Internal_13;
    out >> Internal_14;
    out >> Internal_15;
    out >> Internal_16;
    out >> Internal_17;
    return "解析 星敏感器A控制参数获取应答遥测 成功！";
}
 
QString QtCommandtest::analyze_star_sensor_A_star_point_get_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint32 litpixel_num;
    quint16 star_num;
    quint16 mat_num;
    quint32 tri_num;
    quint32 cons_time;
    quint16 clamp_fb;
    quint8 star_register;

    typedef struct star_data
    {

        quint32 star_row;
        quint32 star_col;
        quint16 star_light;
        quint16 star_num;
        quint16 star_index;
    }star_data_t;

    star_data_t* star_data = new star_data_t[30];

    quint32 time_expo;
    quint32 time_tran;
    quint16 i = 0;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);

    out >> litpixel_num;
    out >> star_num;
    out >> mat_num;
    out >> tri_num;
    out >> cons_time;
    out >> clamp_fb;
    out >> star_register;
    for(i = 0; i < 30 ; i++)
    {
        out >> star_data->star_row;
        out >> star_data->star_col;
        out >> star_data->star_light;
        out >> star_data->star_num;
        out >> star_data->star_index;
        star_data++;
    }
    star_data -= i;
    out >> time_expo;
    out >> time_tran;
    return "解析 星敏感器A星点像数据获取应答遥测 成功！";
}

QString QtCommandtest::analyze_star_sensor_A_star_point_get_image_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8 star_id;
    quint16 star_row;
    quint16 star_col;
    quint16 star_light;
    quint16* star_image = new quint16[226];
    quint16 i = 0;
    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> star_id;
    out >> star_row;
    out >> star_col;
    out >> star_light;

    for (i = 0; i < 226; i++)
    {
        out >> star_image[i];
    }
    return "解析 星敏感器A单星点像图像数据获取应答遥测 成功！";
}

QString QtCommandtest::analyze_star_sensor_A_star_point_get_image_data_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8 star_id;
    quint16 star_row;
    quint16 star_col;
    quint16 star_light;

    quint16* star_image = new quint16[226];
    quint16 i = 0;
    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> star_id;
    out >> star_row;
    out >> star_col;
    out >> star_light;

    for (i = 0; i < 226; i++)
    {
        out >> star_image[i];
    }
    return "解析 星敏感器A星点像图像数据获取应答遥测 成功！";
}

QString QtCommandtest::analyze_image_subsystem_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8  baud_rate;
    quint16 image_height;
    quint16 image_width;
    quint16 offset_Y;
    quint16 offset_X;
    quint32 image_format;
    quint8 trigger_source_frmae;
    quint8 exposure_mode;
    quint32 exposure_time;

    quint16* star_image = new quint16[226];
    quint16 i = 0;
    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> baud_rate;
    out >> image_height;
    out >> image_width;
    out >> offset_Y;
    out >> offset_X;
    out >> image_format;
    out >> trigger_source_frmae;
    out >> exposure_mode;
    out >> exposure_time;
    return "解析 成像分系统应答遥测 成功！";
}

QString QtCommandtest::analyze_data_compose_unit_current_file_system_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8  record_file_amount;
    quint8 compress_file_amount;
    quint8* record_file_number;
    quint8* compress_file_number;
  
    quint16 i = 0;
    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> record_file_amount;
    out >> compress_file_amount;
    record_file_number = new quint8[record_file_amount];
    compress_file_number = new quint8[compress_file_amount];

    for (i = 0; i < record_file_amount; i++)
    {
        out >> record_file_number[i];
    }
    for (i = 0; i < record_file_amount; i++)
    {
        out >> compress_file_number[i];
    } 
    return "解析 数据压缩处理单元当前文件系统信息应答遥测 成功！";
}

QString QtCommandtest::analyze_data_compose_unit_file_attributes_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint8 file_type;
    quint32 file_size;
    quint32 file_create_time;
    quint32 file_end_time;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> file_type;
    out >> file_size;
    out >> file_create_time;
    out >> file_end_time;
    return "解析 数据压缩处理单元文件属性信息应答遥测 成功！";
}

QString QtCommandtest::analyze_data_compose_unit_camera_parameters_penetrate_telemetry_inquire(reply_valid_data_t reply_valid_data)
{
    quint16 data_len;
    quint8 camera_encode;
    quint8* data_content;

    QByteArray array;
    array.resize(sizeof(reply_valid_data.reply_telemetry_data));//重置数据大小
    memcpy(array.data(), reply_valid_data.reply_telemetry_data, sizeof(224));//copy数据
    QDataStream out(&array, QIODevice::ReadWrite);
    out >> data_len;
    out >> camera_encode;
    data_content = new quint8[data_len];
    for (quint16 i = 0; i < data_len; i++)
    {
        out >> data_content[i];
    }

    return "解析 数据压缩处理单元相机参数透传应答遥测 成功！";
}

void QtCommandtest::btn_clr_cmd()
{
    QString valid_data = ui.textEdit_validdata->toPlainText();
    quint8 validdata_length_in_text = valid_data.length();
    quint8* valid_data_int;
    valid_data_int = new quint8[8];

    QString  valid_data_byte;
    int j = 0;
    for (int i = 0; i < validdata_length_in_text; i = i + 2,j++)
    {
        valid_data_byte = valid_data.left(2);
        valid_data = valid_data.mid(2);
        valid_data_int[j] = valid_data_byte.toInt();
        qDebug() << valid_data_int[j];
    } 
}

void QtCommandtest::btn_to_connect_tcp()
{
     
    m_tcpClient = new QTcpSocket(this);

    QString line_ip;;
    QString line_port;
 
    QFile file("d:/test.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open the file!" << endl;
    }
    int i = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        if (i == 0)
        {
            line_ip = str;
            i++;
        }
        else
        {
            line_port = str;
        }
        qDebug() << str;
    }
    int num = line_port.toInt();
    m_tcpClient->connectToHost(line_ip, num);
    connect(m_tcpClient, SIGNAL(connected()), this, SLOT(connected_success()));


    connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(update_ReadServer_data()));


 //   m_tcpClient->connectToHost("192.168.0.107", 10086);

    file.close();
}

void QtCommandtest::by_udp_connect_to_sky()
{
    sender1 = new QUdpSocket(this);

    sender1->bind(10087, QUdpSocket::ShareAddress);

    connect(sender1, SIGNAL(readyRead()), this, SLOT(udp_rev_data_from_sky()));

    qDebug() << "have initial!";
}

void QtCommandtest::udp_rev_data_from_sky() //处理等待的数据报
{
    while (sender1->hasPendingDatagrams()) //拥有等待的数据报
    {
        QByteArray datagram; //拥于存放接收的数据报
        datagram.resize(sender1->pendingDatagramSize());
        sender1->readDatagram(datagram.data(), datagram.size());
        unsigned short size = datagram.size();
        unsigned char msg[10] = { 0 };
        memcpy(msg, datagram.data(), size);
        unsigned short arr = msg[0];
        arr = (arr << 8) + msg[1];
        qDebug() << msg[0] << msg[1] << msg[2] << msg[3];
    }
}

void QtCommandtest::udp_send_device_reset_data()
{
    unsigned char msg[10] = { 0,1,2,3 };

    QString  send_data = "7EA58C9101011111999988";
    char* ch;
    QByteArray ba = send_data.toLatin1();
    ch = ba.data();

    quint8* data_to_send;
    data_to_send = new uchar[11];

    quint32 smg_head = 0x7EA58C91;
    quint16 smg_type = 0x0101;
    quint16 smg_len = 0x2;
    //quint16 smg_content = 0x9999;
    quint8  smg_aisle = 0x01;
    quint8  smg_reset = 0xFF;
    quint8  smg_crc = 0x88;

    memcpy(data_to_send, &smg_head, 4);
    memcpy(data_to_send + 4, &smg_type, 2);
    memcpy(data_to_send + 6, &smg_len, 2);
    memcpy(data_to_send + 8, &smg_aisle, 1);
    memcpy(data_to_send + 8, &smg_aisle, 1);
    memcpy(data_to_send + 10, &smg_crc, 1);

    sender1->writeDatagram((char*)data_to_send, 11, QHostAddress("192.168.0.70"), 10086);
}

void QtCommandtest::udp_send_signal_pata_data()
{
    quint8* data_to_send;
    data_to_send = new uchar[21];
    memset(data_to_send, 0, 21);
    quint32 smg_head = 0x7EA58C91;
    quint16 smg_type = 0x0101;
    quint16 smg_len = 12;
    quint8  smg_aisle = 0x01;
    quint8  smg_launch_switch = 0x01;
    quint8  smg_modulation_switch = 0x01;

    quint8  smg_crc = 0x88;

    memcpy(data_to_send, &smg_head, 4);
    memcpy(data_to_send + 4, &smg_type, 2);
    memcpy(data_to_send + 6, &smg_len, 2);
    memcpy(data_to_send + 8, &smg_aisle, 1);
    memcpy(data_to_send + 9, &smg_launch_switch, 1);
    memcpy(data_to_send + 10, &smg_modulation_switch, 1);
    memcpy(data_to_send + 21, &smg_crc, 1);

    sender1->writeDatagram((char*)data_to_send, 21, QHostAddress("192.168.0.70"), 10086);
}

void QtCommandtest::udp_send_data(quint8* total_data_to_send, quint16 total_data_len)
{
    quint8* data_to_send;
    data_to_send = new uchar[10 + total_data_len];

    memset(data_to_send, 0, 21);
    quint32 smg_head = 0x7EA58C91;
    quint16 smg_type = 0x0104;
    quint16 smg_len = total_data_len + 1;
    quint8  smg_crc = 0x88;
    memcpy(data_to_send, &smg_head, 4);
    memcpy(data_to_send + 4, &smg_type, 2);
    memcpy(data_to_send + 6, &smg_len, 2);
    memset(data_to_send + 8, 0, 1);
    memcpy(data_to_send + 9, &total_data_to_send, total_data_len);

    memcpy(data_to_send + 10 + total_data_len - 1, &smg_crc, 1);

    sender1->writeDatagram((char*)data_to_send, total_data_len +10, QHostAddress("192.168.0.70"), 10086);
}

static qint8 send_data_to_station_switch = 0;
void QtCommandtest::tcp_rev_data_from_ctl_center()
{
    qDebug() << "tcp_rev_data_from_ctl_center";
    static QString left_string;

    QByteArray left_array = left_string.toLatin1();

    QByteArray temp = tcp_to_ground_ctl_handle->readAll();
    QString rev_data;

    left_array.append(temp);

    QDataStream out(&left_array, QIODevice::ReadWrite);    //将字节数组读入   

    qint32 data_len = 0;
    int i = 0;
    qint8 outChar = 0;

    rev_telemetry = Q_NULLPTR;

    while (!out.atEnd())
    {        
        out >> outChar;   //每字节填充一次，直到结束

        if (i < 4)
        {
            data_len |= outChar << (8 * (3-i));
            i++;
        }
        else
        {
            if (data_len + 4 > left_array.length())
            {
                left_string += outChar;
            }
            else
            {
                if (data_len > 0)
                {
                    rev_data += outChar;
                    data_len--;
                }
                else
                {
                    left_string += outChar;
                }
            }
        }   
    }
        if (!rev_data.compare("auth_ok"))
    {
        ui.plainTextEdit_reminder->appendPlainText("身份合法！");
    }
    else if (!rev_data.compare("auth_fail"))
    {
        ui.plainTextEdit_reminder->appendPlainText("数据错误！");
    }
    else if (!rev_data.compare("connect_ok"))
    {
        ui.plainTextEdit_reminder->appendPlainText("绑定地面站完成");
        send_data_to_station_switch = 1;
    }
    else if (!rev_data.compare("connect_fail"))
    {
        ui.plainTextEdit_reminder->appendPlainText("绑定地面站失败");
    }
    else if (rev_data.length() == 256)
    {
        ui.plainTextEdit_reminder->appendPlainText("遥测数据");       
        rev_telemetry = rev_data.toLatin1();
        update_ReadServer_data();
    }
    else
    {
        ui.plainTextEdit_reminder->appendPlainText("返回错误数据");
    }
}

void QtCommandtest::tcp_send_identity_data_to_ctl_center()
{
    qDebug() << "tcp_to_ground_ctl_0";
    quint8* data_to_send;
    data_to_send = new quint8(16);

    QString msg_type;
    msg_type = "hshs";
    quint32 dev_type = 0;
    quint32 usr_id = 0xffffffff;
    quint32 dev_id = 0xffffffff;
    struct data_to_send
    {
        quint32 msg_len;
        quint32 msg_type;
        quint32 dev_type;
        quint32 usr_id;
        quint32 dev_id;
    };

    struct data_to_send send_data;
    quint32 msg_len = qToBigEndian(0x00000010);// 0x10000000;
    memcpy((void*)&send_data.msg_len, &msg_len, 4);
    memcpy((void*)&send_data.msg_type, "hshs", 4);
    memcpy((void*)&send_data.dev_type, &dev_type, 4);
    memcpy((void*)&send_data.usr_id, &usr_id, 4);
    memcpy((void*)&send_data.dev_id, &dev_id, 4);
   
    qint64 tcp_return_result = tcp_to_ground_ctl_handle->write((char*)&send_data, 20);

    qDebug() << tcp_return_result;   
    qDebug() << "tcp_to_ground_ctl_1";
}

void QtCommandtest::tcp_send_connect_data_to_ctl_center()
{
    quint8* data_to_send;

  //  data_to_send = new quint8(8 + stationId.size());

    data_to_send = new quint8(8 + 4);

    struct data_to_send
    {
        quint32 msg_len;
        quint64 msg_type;
        quint32 station_id;
    };
    struct data_to_send send_data;

    quint32 station_id = 0x0000000a;  //网络序转化
    station_id = qToBigEndian(station_id);
    quint32 msg_len = 0x0000000c;   //网络序转化
    msg_len = qToBigEndian(msg_len);
    memcpy((void*)&send_data.msg_len,&msg_len, 4);
    memcpy((void*)&send_data.msg_type, "connect_", 8);
    memcpy((void*)&send_data.station_id, &station_id, 4);

    tcp_to_ground_ctl_handle->write((char*)&send_data, 8 + 8);
}

void QtCommandtest::tcp_connect_server_as_client()
{
  //  tcp_to_ground_ctl_handle = new QTcpSocket(this);
  //  tcp_to_ground_ctl_handle->connectToHost("192.168.2.88", 6666);
    Project_gather Project_gather1 = new Project_gather;
   tcp_to_ground_ctl_handle = Project_gather1.get_tcp_to_ground_handle();
    
    connect(tcp_to_ground_ctl_handle, SIGNAL(connected()), this, SLOT(tcp_connected_ctl_center_success()));
    connect(tcp_to_ground_ctl_handle, SIGNAL(readyRead()), this, SLOT(tcp_rev_data_from_ctl_center()));
  //  tcp_to_ground_handle = tcp_to_ground_ctl;
}

void QtCommandtest::tcp_connected_ctl_center_success()
{
    ui.plainTextEdit_reminder->appendPlainText("连接服务器成功");
}