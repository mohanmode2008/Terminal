#include "QtCommandtest.h"

#include <Qt>
#include <QtDebug>
#include "Project_gather.h"

#include <QListWidgetItem> 


#include <QtNetwork>

#include "QtWorkparameter.h"

#pragma execution_character_set("utf-8")

#pragma pack (1)

QtCommandtest::QtCommandtest(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    qint8 i;

  //  this->resize(400, 300);

    status = new QStatusBar(this);

    menu[0] = new QMenu("数管分系统");
    menu[0]->addAction("XWOBC");
    menu[0]->addAction("AMPOC");

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

  //  qDebug() << this->width();
    connect(menuBar, SIGNAL(triggered(QAction*)), this, SLOT(trigerMenu(QAction*)));
    connect(ui.btn_to_selectlist, SIGNAL(clicked()), this, SLOT(btn_to_selectlist()));
    connect(ui.btn_to_del_cmd, SIGNAL(clicked()), this, SLOT(btn_to_del_select()));
    connect(ui.btn_buildup, SIGNAL(clicked()), this, SLOT(btn_to_buildup_validdata()));
    connect(ui.listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(lineeditEnable()));
    connect(ui.listWidget_selectcmd, SIGNAL(itemSelectionChanged()), this, SLOT(select_lineeditEnable()));

    connect(ui.btn_cmd_send, SIGNAL(clicked()), this, SLOT(btn_to_send_cmd()));
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

/*    m_tcpClient = new QTcpSocket(this);

    connect(m_tcpClient, SIGNAL(connected()), this, SLOT(connected_success()));


    connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(update_ReadServer_data()));


    m_tcpClient->connectToHost("192.168.0.107", 10086);
    */
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
    QStringList strList_AMPOC;
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
        << "设置最低放电电压阈值" << "删除延时任务" << "删除延时指令" << "星务计算机软件版本切换" << "单机设备指令透传";

    strList_AMPOC << "进入自主姿态控制" << "进入对日捕获与对日定向" << "进入对日定向三轴稳定" << "进入对地定向三轴稳定"
        << "卫星姿态俯仰方向Y + 调整" << "卫星姿态俯仰方向Y - 调整" << "卫星姿态滚动方向X + 调整" << "卫星姿态滚动方向X - 调整"
        << "星敏感器A屏蔽使用" << "星敏感器A不屏蔽使用" << "星敏感器B屏蔽使用" << "星敏感器B不屏蔽使用" << "数字太阳敏感器屏蔽使用"
        << "数字太阳敏感器不屏蔽使用" << "磁强计A屏蔽使用" << "磁强计A不屏蔽使用" << "磁强计B屏蔽使用" << "磁强计B不屏蔽使用"
        << "陀螺A屏蔽使用" << "陀螺A不屏蔽使用" << "陀螺B屏蔽使用" << "陀螺B不屏蔽使用" << "陀螺C屏蔽使用" << "陀螺C不屏蔽使用"
        << "设置角速度漂移估计标志：估计" << "设置角速度漂移估计标志：停止估计" << "设置角速度漂移估计复位标志：复位置零"
        << "设置角速度漂移估计复位标志：不复位" << "设置飞轮摩擦估计标志：估计" << "设置飞轮摩擦估计标志：计算"
        << "设置飞轮角动量估计标志：估计" << "设置飞轮角动量估计标志：计算" << "设置使用上注轨道确定标志：使用"
        << "设置使用上注轨道确定标志：不使用" << "常规模式陀螺积分定姿高优先级" << "常规模式双矢量定姿高优先级"
        << "星敏补偿（使用参考角速度）" << "星敏补偿（使用本体角速度）" << "星敏补偿（不补偿）" << "使用计算磁场"
        << "不使用计算磁场，使用磁强计" << "星敏时间补偿" << "星敏时间不补偿";
    
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

    strList_flywheel_cmd << "飞轮X转速模式指令" << "飞轮X电流模式指令" << "飞轮X力矩模式指令" << "飞轮Y转速模式指令" << "飞轮Y电流模式指令" << "飞轮Y力矩模式指令"
                         << "飞轮Z转速模式指令" << "飞轮Z电流模式指令" << "飞轮Z力矩模式指令" << "飞轮S转速模式指令" << "飞轮S电流模式指令" << "飞轮S力矩模式指令";

    strList_fiber_optic_gyro <<"光纤陀螺A复位"<<"光纤陀螺B复位";
    strList_electric_thruster << "点火指令" << "开启电源" << "关闭电源" << "清洗火花塞" << "连续点火";
    strList_camera << "通信波特率" << "图像高（Height）" << "图像宽（Width）" << "OffsetY" << "OffsetX" << "图像格式" << "帧读取周期（帧频）" << "帧开始触发源"
                    << "曝光模式" << "曝光时间" << "白平衡" << "增益" << "自动增益" << "抽头" << "伽马选择" << "CameraLink时钟频率";

    strList_infrared_camera << "图像定格" << "测试画面切换" << "保存设置" << "恢复出厂值" << "温升补偿开关" << "快门闭合" << "增益控制" << "外同步开关" << 
                                "数字口并行类型" << "CMOS内容选择" << "CMOS接口形式" << "场景补偿" << "快门补偿" << "时域滤波" << "滤波强度" << "去竖纹" << "调光模式" 
                            << "上抛点比例" << "下抛点比例" << "亮度" << "对比度" << "混合调光映射" << "Y8纠偏" << "IDE增强" << "IDE滤波等级 " << "IDE细节增益 " 
                            << "Y8纠偏模式" << "分块直方图" << "去噪开关" << "去噪等级" << "手动调焦速度" << "自动调焦统计帧数" << "自动调焦速度 MAX" << "自动调焦速度 MIN"
                            << "十字光标开关" << "光标位置X" << "光标位置Y" << "光标R分量" << "光标G分量" << "光标B分量" << "伪彩条开关" << "伪彩视觉增强" << "伪彩视觉增强阈值上限"
                            << "伪彩视觉增强阈值下限" << "等温线开关" << "等温线显示模式" << "等温线阈值上限" << "等温线阈值下限" << "等温线伪彩色带选择";

    strList_focusing_a << "右转" << "左转" << "停止" << "移动到目标位置";

    strList_focusing_b <<"调焦 -"<<"调焦＋"<<"停止调焦"; 
        
    strList_data_compression << "系统授时" << "系统重启" << "文件序号记录" << "文件序号追加记录" << "记录直传" << "文件序号回放启用断点" << "文件序号回放不启用断点" 
                            << "顺序回放启用断点" << "顺序回放不启用断点" << "按时间回放" << "序号擦除文件" << "全部擦除" << "停止记录" << "停止回放" << "全部停止" << "预关机" 
                            << "功能自检" << "低级格式化" << "压缩模式设置" << "压缩比设置" << "压缩回写" << "停止压缩" << "压缩直传" << "相机单元配置透传" << "获取当前文件系统信息"
                            << "获取文件属性信息" << "获取相机配置参数透传" << "系统授时" << "系统重启" << "文件序号记录" << "文件序号追加记录" << "记录直传" << "文件序号回放启用断点"
                            << "文件序号回放不启用断点" << "顺序回放启用断点" << "顺序回放不启用断点" << "按时间回放" << "序号擦除文件" << "全部擦除" << "停止记录" << "停止回放" 
                            << "全部停止" << "预关机" << "功能自检" << "低级格式化" << "压缩模式设置" << "压缩比设置" << "压缩回写" << "停止压缩" << "压缩直传" << "相机单元配置透传"
                            << "获取当前文件系统信息" << "获取文件属性信息" << "获取相机配置参数透传";

    strList_GNSS_occultation_obs << "接收机复位" << "定轨开启" << "定轨关闭" << "原始观测量存储开始" << "原始观测量存储关闭" << "GPS大气开环通道开启" << "GPS大气开环通道关闭" 
                            << "BD大气开环通道开启" << "BD大气开环通道关闭" << "掩星接收机切正常工作模式" << "掩星接收机切数据传输模式" << "Flash数据擦除" << "Flash坏块更新" 
                            << "掩星接收机强制切正常工作模式" << "掩星接收机强制切数据传输模式" << "数据传输终止" << "程序上注允许" << "程序上注结束" << "程序上注启动" 
                            << "422数据发送开始" << "422数据发送停止" << "掩星观测单元遥测查询";

    strList_DCS_acquisition << "DCS载荷软复位" << "DCS固存开始记录" << "DCS固存停止记录" << "DCS数据顺序回放" << "DCS数据停止回放" << "DCS 12h自复位使能" << "DCS 12h自复位禁止" 
                            << "DCS固存读写地址备份" << "DCS载荷功放允许开启" << "DCS载荷功放禁止开启" << "DCS数传下行开启" << "DCS数传下行关闭" << "调整DCS记录地址" << "调整DCS回放地址";

    strList_interactive_operation << "成像任务设置" << "实时成像开始成像" << "实时成像结束成像" << "开始数传" << "结束数传";

    strList_data_annotation <<"星务软件上注"<<"授权码上注";

    strList_direct_cmd << "展开释放机构A备起爆" << "展开释放机构B备起爆" << "U天线展开起爆" << "V天线展开起爆" << "DCS载荷备起爆"
        << "AMP供电通断" << "蓄电池放电开关通" << "蓄电池放电开关断" << "姿控/AMP计算机A开B关" << "姿控/AMP计算机B开A关" << "姿控/AMP计算机模式控制使用1"
        << "姿控/AMP计算机模式控制使用2" << "姿控/AMP计算机切为主节点" << "星务计算机A开B关" << "星务计算机B开A关" << "星务计算机备份主控切为从节点";
              
    if (act->text() == "XWOBC")
    {
        qDebug() << "XWOBC";
        ui.listWidget->clear();
         for (int i = 0; i < strList_XWOBC.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_XWOBC[i]);
            ui.listWidget->addItem(aItem); 
        }      
    }

    if (act->text() == "AMPOC")
    {
         ui.listWidget->clear();
        for (int i = 0; i < strList_AMPOC.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_AMPOC[i]);
            ui.listWidget->addItem(aItem);
        }
    }
   // ui.listWidget->selectedItem（）;
    if (act->text() == "控制运算分系统") 
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
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
        for (int i = 0; i < strList_X_antenna.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_X_antenna[i]); 
            ui.listWidget->addItem(aItem);
        }
    }


    if (act->text() == "GNSS接收机A遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
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
        for (int i = 0; i < strList_star_sensor_C.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList_star_sensor_C[i]);
            ui.listWidget->addItem(aItem);
        }
    }

    if (act->text() == "飞轮遥控指令")
    {
        qDebug() << "AMPOC";
        ui.listWidget->clear();
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


void QtCommandtest::lineeditEnable()
{
    QListWidgetItem * aa =  ui.listWidget->currentItem();
    QString  aa_string = aa->text();

    select_in_list = aa_string; 

  qDebug() << aa_string;
}

void QtCommandtest::btn_to_selectlist()
{
    QListWidgetItem* aItem;
    aItem = new QListWidgetItem();
    aItem->setText(select_in_list);
    ui.listWidget_selectcmd->addItem(aItem);
}

qint8 QtCommandtest::select_lineeditEnable()
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
    ACK_reply = 1;
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
        telecontrol_frame_up.cmd_code = 0x0a;
        map_name_to_code.insert("星务计算机遥测查询", 10);
        map_name_to_validdatalength.insert("星务计算机遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道：UXB测控数传一体机A：55H，");
        return 1;
    }
    if (aa_string == "星务计算机日志下载")
    {
        telecontrol_frame_up.cmd_code = 0x0b;
        map_name_to_code.insert("星务计算机日志下载", 11);
        map_name_to_validdatalength.insert("星务计算机日志下载", 9);
               ui.plainTextEdit_reminder->appendPlainText("有效数据9字节，比如:dd0000123400002345,dd代表通道，\
                                                   00001234日志记录起始时间，00002345日志记录结束时间");
        return 1;
    }
    if (aa_string == "星务计算机访问记录下载")
    {
        telecontrol_frame_up.cmd_code = 0x0c;
        map_name_to_code.insert("星务计算机访问记录下载", 12);
        map_name_to_validdatalength.insert("星务计算机访问记录下载", 9);
        ui.plainTextEdit_reminder->appendPlainText("有效数据9字节，比如:dd0000123400002345,dd代表通道，\
                                                   00001234访问记录起始时间，00002345访问记录结束时间");
        return 1;
    }
    if (aa_string == "星务计算机短消息下载")
    {
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
        telecontrol_frame_up.cmd_code = 0x0f;
        map_name_to_code.insert("星务计算机短消息下载", 0x0f);
        map_name_to_validdatalength.insert("星务计算机短消息下载", 9);
        ui.plainTextEdit_reminder->appendPlainText("有效数据9字节，比如:dd0000123400002345,dd代表通道，\
                                                   00001234执行记录起始时间，00002345执行记录结束时间");
        return 1;
    }

    if (aa_string == "单机设备透传遥测查询")
    {
        telecontrol_frame_up.cmd_code = 0x10;
        map_name_to_code.insert("星务计算机短消息下载", 0x10);
        map_name_to_validdatalength.insert("星务计算机短消息下载", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }

    if (aa_string == "PCDU遥测查询")
    {
        telecontrol_frame_up.cmd_code = 0x14;
        map_name_to_code.insert("PCDU遥测查询", 0x14);
        map_name_to_validdatalength.insert("PCDU遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }

    if (aa_string == "测控数传一体机遥测查询")
    {
        telecontrol_frame_up.cmd_code = 0x15;
        map_name_to_code.insert("PCDU遥测查询", 0x15);
        map_name_to_validdatalength.insert("PCDU遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }

    if (aa_string == "姿控分系统遥测查询")
    {
        telecontrol_frame_up.cmd_code = 0x16;
        map_name_to_code.insert("姿控分系统遥测查询", 0x16);
        map_name_to_validdatalength.insert("姿控分系统遥测查询", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节，测控通道。例如：UXB测控数传一体机A：55H；");
        return 1;
    }
    
    if (aa_string == "GNSS接收机AB遥测查询")
    {
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
        ui.textEdit_validdata->insertPlainText("24");
        map_name_to_code.insert("星上授权码清除", 0x25);
        map_name_to_validdatalength.insert("星上授权码清除", 1);
        ui.plainTextEdit_reminder->appendPlainText("有效数据已有！");
        return 1;
    }

    if (aa_string == "星务软件上注开始")
    {
        telecontrol_frame_up.cmd_code = 0x26;
        map_name_to_code.insert("星上授权码清除", 0x26);
        map_name_to_validdatalength.insert("星上授权码清除", 1);
        ui.plainTextEdit_reminder->appendPlainText("1字节。AAH=上注版本1；BBH = 上注版本2；CCH = 上注版本3；DDH = 上注版本4");
        return 1;
    }
    
    if (aa_string == "星务软件上注结束")
    {
        telecontrol_frame_up.cmd_code = 0x27;
        map_name_to_code.insert("星务软件上注结束", 0x27);
        map_name_to_validdatalength.insert("星务软件上注结束", 1);
        ui.plainTextEdit_reminder->appendPlainText("有效数据已有！");
        return 1;
    }

    if (aa_string == "短消息上注")
    {
        telecontrol_frame_up.cmd_code = 0x28;
        map_name_to_code.insert("短消息上注", 0x28);
        map_name_to_validdatalength.insert("短消息上注", 152);
        ui.plainTextEdit_reminder->appendPlainText("152字节，DATA0~DATA3：短消息接收用户ID；DATA4~DATA7：短消息发送用户ID； DATA8~DATA11：短消息发送终端ID\
                                                     DATA12~DATA151：短消息内容");
        return 1;
    }

    if (aa_string == "时间上注")
    {
        telecontrol_frame_up.cmd_code = 0x29;
        map_name_to_code.insert("时间上注", 0x29);
        map_name_to_validdatalength.insert("时间上注", 6);
        ui.plainTextEdit_reminder->appendPlainText("6字节，DATA0~DATA3：授时时间，UTC时间秒\
                                                     DATA4~DATA5：授时时间毫秒");
        return 1;
    }

    if (aa_string == "轨道上注")
    {
        telecontrol_frame_up.cmd_code = 0x2a;
        map_name_to_code.insert("轨道上注", 0x2a);
        map_name_to_validdatalength.insert("轨道上注", 30);
        ui.plainTextEdit_reminder->appendPlainText("30字节，DATA0~DATA3：轨道对应的时间，UTC时间秒\
                                                    DATA4~DATA5：轨道对应的时间毫秒\
                                                    DATA6~DATA17：位置（X、 Y、 Z），单位：米 均为单精度浮点数\
                                                    DATA18~DATA29：速度（Vx、 Vy、 Vz），单位：米 / 秒 均为单精度浮点数");
     return 1;
    }

    if (aa_string == "设置最高冲段电压阈值")
    {
        telecontrol_frame_up.cmd_code = 0x2b;
        map_name_to_code.insert("设置最高冲段电压阈值", 0x2b);
        map_name_to_validdatalength.insert("设置最高冲段电压阈值", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：单精度浮点数");
        return 1;
    }

    if (aa_string == "设置最低放电电压阈值")
    {
        telecontrol_frame_up.cmd_code = 0x2c;
        map_name_to_code.insert("设置最低放电电压阈值", 0x2c);
        map_name_to_validdatalength.insert("设置最低放电电压阈值", 4);
        ui.plainTextEdit_reminder->appendPlainText("4字节，DATA0~DATA3：单精度浮点数");
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

    /*-----------------------------电源分系统----------------------------------*/
    telecontrol_frame_up.cmd_style = 0x3a;
    telecontrol_frame_up.single_number = 0x62;
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

    /*----------------------交互操作分系统-------------------------------------*/

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
}


void QtCommandtest::btn_to_del_select()
{
    qDebug() << "btn_to_del_select";
    QListWidgetItem* aItem = ui.listWidget_selectcmd->takeItem(row_in_cmd); //移除指定行的项，但不delete
    delete aItem; //需要手工删除对象
}

void QtCommandtest::btn_to_buildup_validdata()
{
  /*
    qDebug() << "btn_to_buildup_validdata";
    QString run_attr = ui.comboBox_exeattr->currentText();
    QString delay_time = ui.lineEdit->text(); 
    qint8 cmd_attribute;
    qint8  validdata_length = map_name_to_validdatalength[select_in_cmd];
    qint8 * validdata = new qint8[validdata_length];
    QString cLeft;
    QString cMid;
    QString  cRight;
    QString valid_data = ui.textEdit_validdata->toPlainText();
    qint16 CRC_byte;
    *validdata = valid_data.toInt();    

  //  QString strValue = QString("%1").arg(num1, 2, 16, QLatin1Char('0'));
  //  cmd_code = strValue;
 
    if (run_attr == "立即执行")
    {
        cmd_attribute = 0x1;
    }
    else
    {
        cmd_attribute = 0x2;
    }

    telecontrol_frame_up.ACK_cmdattribute_multiframe = ACK_reply << 4 | cmd_attribute << 2 | single_multiframe;

    validdata_byte_1_t validdata_byte_1_testmode;

    validdata_byte_1_testmode.validdata = *validdata;

  //  CRC_byte = crc16_ccitt(validdata_length, (char*)validdata);

  //  telecontrol_frame_down.CRC_byte = CRC_byte >> 4;;
   // telecontrol_frame_down.CRC_byte = CRC_byte & 0xff;;

    telecontrol_frame_down.CRC_byte = 0xffff;

    memcpy(&validdata_byte_1_testmode.telecontrol_frame_up, &telecontrol_frame_up, sizeof(telecontrol_frame_up_t));
    memcpy(&validdata_byte_1_testmode.telecontrol_frame_down, &telecontrol_frame_down, sizeof(telecontrol_frame_down_t));

    m_tcpClient->write((char*)&validdata_byte_1_testmode, sizeof(validdata_byte_1_t));
  //  qint64  CRC_int = crc16_ccitt(valid_data_int,0); 
  */
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
       // qDebug() << *data;

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

quint8* total_data_to_send;
quint16 total_data_len;
qint8 QtCommandtest::btn_to_send_cmd()
{ 
    QString run_attr = ui.comboBox_exeattr->currentText();
    QString delay_time = ui.lineEdit->text();
    quint32 delay_time_int = delay_time.toInt();
    qint8 cmd_attribute;
    qint8  validdata_length = map_name_to_validdatalength[select_in_cmd];
    quint16  total_validdata_length;
    QString valid_data = ui.textEdit_validdata->toPlainText();
    quint8 *data_to_send;
    quint16 CRC_byte_change_order;
    //  QString strValue = QString("%1").arg(num1, 2, 16, QLatin1Char('0'));
    int j = 0;
    quint8 validdata_length_in_text  = valid_data.length();
    quint8* valid_data_int;
    valid_data_int = new quint8[validdata_length];    
    QString valid_data_byte;
    if (validdata_length_in_text%2)
    {
       valid_data = valid_data.prepend("0");
    }
    for (int i = 0; i < validdata_length_in_text; i = i + 2, j++)
    {
        valid_data_byte = valid_data.left(2);
        valid_data = valid_data.mid(2);
        valid_data_int[j] = valid_data_byte.toUInt(NULL, 16);
        
    }
  //  qDebug() << validdata_length;
  //  qDebug() << validdata_length_in_text;
  //  qDebug() << "validdata_length_in_text";
    if (run_attr == "立即执行")
    {
        cmd_attribute = 0x1;
    }
    else
    {
        cmd_attribute = 0x2;
    }
    delay_time_int = byteOrderChange32Bit(&delay_time_int);
    telecontrol_frame_up.cmd_run_time = delay_time_int;   
    telecontrol_frame_up.ACK_cmdattribute_multiframe = ACK_reply << 4 | cmd_attribute << 2 | single_multiframe;

    total_validdata_length = 0;
    /*----------------------*/

    if ((validdata_length +14) <= 16)
    {
        telecontrol_frame_up.pattern_byte = 0x5a;
        total_validdata_length = 16;
        data_to_send = new quint8[23+ total_validdata_length];
    }
    else if ((validdata_length + 14) <= 64)
    {
        telecontrol_frame_up.pattern_byte = 0x69;
        total_validdata_length = 64;
        data_to_send = new quint8[23 + total_validdata_length];
    }
    else if ((validdata_length + 14)<= 128)
    {
        telecontrol_frame_up.pattern_byte = 0x59;
        total_validdata_length = 128;
        data_to_send = new quint8[23 + total_validdata_length];
    }
    else if ((validdata_length + 14 )<= 256)
    {
        telecontrol_frame_up.pattern_byte = 0xa5;
        total_validdata_length = 256;
        data_to_send = new quint8[23 + total_validdata_length];
    } 
    else
    {
        ui.plainTextEdit_reminder->appendPlainText("有效数据位数错误！！");
        return -1;
    } 

    memset(data_to_send, 0xAA, total_validdata_length +9);   
    memcpy(data_to_send, &telecontrol_frame_up, sizeof(telecontrol_frame_up_t));
    memcpy(data_to_send + sizeof(telecontrol_frame_up_t), valid_data_int, validdata_length);
    CRC_byte = crc16_ccitt(total_validdata_length, (quint8*)(data_to_send + 5));
    CRC_byte_change_order = byteOrderChange16Bit(CRC_byte);
    telecontrol_frame_down.CRC_byte = CRC_byte_change_order;
    memcpy(data_to_send + 5 + total_validdata_length, &telecontrol_frame_down, sizeof(telecontrol_frame_down_t));

    total_data_len = total_validdata_length + 9;
    total_data_to_send = new quint8[total_data_len];
    memcpy(total_data_to_send, data_to_send, total_data_len);

    m_tcpClient->write((char*)data_to_send, (total_validdata_length +9));

    return 1;
 }


qint8 QtCommandtest::update_ReadServer_data()
{

    qint8 valid_data_len;
    QByteArray temp = m_tcpClient->readAll();
    QDataStream out(&temp, QIODevice::ReadWrite);    //将字节数组读入
    rev_valid_data_head_t rev_valid_data_head;

    rev_data_t rev_data;
    int i; 
    for (i = 0; i < 4; i++)
    {
        out >>rev_data.satellite_syn_byte[i];
    }
    for (i = 0; i < 4; i++)
    {
        out >> rev_data.authorize_byte[i];
    }
    for (i = 0; i < 7; i++)
    {
        out >> rev_data.frame_head[i];
    }
    for (i = 0; i < 4; i++)
    {
        out >> rev_data.usr_id[i];
    }
    for (i = 0; i < 4; i++)
    {
        out >> rev_data.terminal_id[i];
    }
    for (i = 0; i < 231; i++)
    {
        out >> rev_data.valid_data[i];
    }
    qDebug() << rev_data.authorize_byte[0] << endl;
    qDebug() << rev_data.valid_data[230] << endl;

    quint16 CRC_byte_a = crc16_ccitt(250, (quint8*)&(rev_data.authorize_byte[0]));
    qDebug() << "crc16_ccitt" << endl;
   qDebug() << CRC_byte_a << endl;
    qDebug() << "CRC_byte_a" << endl;

    QString str;
    for (i = 0; i < 2; i++)
    {
        out >> rev_data.fault_tolerance[i];
        qDebug() << (rev_data.fault_tolerance[i] & 0xFF) << endl;
        str = QString("%1").arg(rev_data.fault_tolerance[i] & 0xFF, 2, 16, QLatin1Char('0'));
        qDebug() << str << endl;
    }



    valid_data_len = rev_data.frame_head[7];  

    if (rev_data.fault_tolerance[0] != 0xea || rev_data.fault_tolerance[1] != 0xc8)
    {
        qDebug() << "rev_data.fault_tolerance[0] != 0xc8" << endl;
        return -1;
    }

    if (rev_data.frame_head[0] & 0x18 != 0x18)
    { 
        qDebug() <<"rev_data.frame_head[0]  != 0x18" << endl;
        for (i = 0; i < 4; i++)
        {
            if (rev_data.authorize_byte[i] != telecontrol_frame_up.authorize_byte[i])
            {
                return -2;
            }
        }
    }
    else
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

    rev_valid_data_head.total_data_len[0] = rev_data.valid_data[0];
    rev_valid_data_head.total_data_len[1] = rev_data.valid_data[1];
    rev_valid_data_head.cmd_type          = rev_data.valid_data[3];
    rev_valid_data_head.single_number     = rev_data.valid_data[4];
    rev_valid_data_head.cmd_code          = rev_data.valid_data[5];
    rev_valid_data_head.ACK_type          = rev_data.valid_data[6];
    rev_valid_data_head.ACK_result        = rev_data.valid_data[7];

    if (rev_valid_data_head.cmd_type != 0x3a)  //间接指令
    {
        return -1; 
    }

    if (rev_valid_data_head.single_number == 0x5a) // 星务计算机XW
    {        
        if (rev_valid_data_head.ACK_type == 0x01)  //ACK应答遥测
        {
            if (rev_valid_data_head.ACK_result == 0x01)   //应答结果成功
            {
                if (rev_data.valid_data[8] == 0x01)  //应答结果的处理方法
                {
                    ui.plainTextEdit_result->appendPlainText("指令成功接收！");
                }
               
            }
            else if (rev_valid_data_head.ACK_result == 0x00)  //应答结果失败
            {
                if (rev_data.valid_data[8] == 0x01)
                {
                    ui.plainTextEdit_result->appendPlainText("CRC校验错误：");
                }
                else if (rev_data.valid_data[8] == 0x02)
                {
                    ui.plainTextEdit_result->appendPlainText("授权码错误：");
                }
                else if (rev_data.valid_data[8] == 0x03)
                {
                    ui.plainTextEdit_result->appendPlainText("非法用户：");
                }
                else if (rev_data.valid_data[8] == 0x04)
                {
                    ui.plainTextEdit_result->appendPlainText("设备故障：");
                }
                else
                {
                    ui.plainTextEdit_result->appendPlainText("应答结果返回错误！");
                    return -1;
                }
            }
            else
            {
                return - 1;
            }
        }         
        else if (rev_valid_data_head.ACK_type == 0x02)   //数据应答遥测
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
                    memcpy(&log_download,&rev_data.valid_data[8],sizeof(log_download_t));
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
        else
        {
            return -1;
        }       
    }
    else if (rev_valid_data_head.single_number == 0x5b) // AMP、ZK分系统 AZ
    {

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
    else
        {
        return -1;
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

    /*
    while (!out.atEnd())
    {
        qint8 outChar = 0;
        out >> outChar;   //每字节填充一次，直到结束
        //十六进制的转换
        QString str = QString("%1").arg(outChar & 0xFF, 2, 16, QLatin1Char('0'));
        ui.plainTextEdit_result->insertPlainText(str.toUpper());//大写
        ui.plainTextEdit_result->insertPlainText(" ");//每发送两个字符后添加一个空格
     //   ui.plainTextEdit_result->moveCursor(QTextCursor::End);
    }

    */
}

void QtCommandtest::btn_clr_cmd()
{
 /*   quint8 data_to_send[16] = {0xFF,0xFF ,0xFF ,0xFF ,0x3A, 0x5A, 0x01, 0x18, 0x40, 0xE2, 0x01, 0x00, 0x00, 0x00, 0x01, 0xAA };

    quint16 CRC_byteaa = crc16_ccitt(16, data_to_send);

    qDebug() << CRC_byteaa;

    quint16 CRC_byteaa11 = byteOrderChange16Bit(CRC_byteaa);
    qDebug() << CRC_byteaa11;
*/

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

quint32 QtCommandtest::byteOrderChange32Bit(quint32 *data)
{
    quint32 tmp;
    tmp = (*data & 0x000f << 24) | (*data & 0x00f0 << 8) | (*data & 0x0f00 >> 8) | (*data & 0xf000 >> 24);
    return tmp;
}

quint16 QtCommandtest::byteOrderChange16Bit(quint16 data)
{
    quint16 tmp;
    tmp = data>> 8 | data << 8;
    return tmp;
   
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

void QtCommandtest::udp_send_data()
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