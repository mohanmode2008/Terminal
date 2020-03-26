#include "QtCommandtest.h"

#include <Qt>
#include <QtDebug>
#include "Project_gather.h"

#include <QListWidgetItem> 

#pragma execution_character_set("utf-8")

QtCommandtest::QtCommandtest(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

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

    /*---------------*/
    conduct_code = "555555555555555";
    sequence_start_byte = "EB90";
    sequence_end_byte = "09D7";
    protect_sequence = "555555555555555";

    satellite_syn_byte = "XXXXXXXX";
    authorize_byte = "xxxxxxxx";
  //  pattern_byte = 

   // CRC_byte = 
    frame_end_byte = "09D7";

}

QtCommandtest::~QtCommandtest()
{

}

void QtCommandtest::trigerMenu(QAction* act)
{

 //   ui.comboBox_command->clear();
    QStringList strList;
    QListWidgetItem* aItem;


    strList << "进入测试模式" << "清空延时指令" << "清空延时任务" << "星务计算机复位" << "程序注入开始" << "程序注入结束"
        <<"星务计算机遥测查询"<<"星务计算机日志下载"<<"星务计算机访问记录下载"<<"星务计算机短信息下载"<<"任务请求记录文件下载"<<"任务执行记录文件下载"<<"单机设备透传遥测查询"
        <<"PCDU遥测查询"<<"测控数传一体机遥测查询"<<"姿控分系统遥测查询"<<"GNSS接收机AB遥测查询"<<"工作模式切换"<<"ACK应答开关设置"<<"卫星上行同步码设置"<<"数传通道禁用与使能设置"
        <<"展开释放机构的禁止和使能"<<"自动热控的禁止和使能"<<"遥测通道的选择"<<"自动轨控的禁止和使能"<<"定时断电时间间隔设置"<<"实时广播遥测的开启和关闭"<<"实时广播遥测的间隔时间";

    if (act->text() == "XWOBC")
    {
        qDebug() << "XWOBC";

         for (int i = 0; i < strList.size(); i++)
        {
            aItem = new QListWidgetItem();
            aItem->setText(strList[i]);
            ui.listWidget->addItem(aItem); 
        }      
    }

   // ui.listWidget->selectedItem（）;

    if (act->text() == "AMPOC")
    {
        qDebug() << "AMPOC";
    }

}

void QtCommandtest::btn_first_clicked()
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

void QtCommandtest::select_lineeditEnable()
{
    QListWidgetItem* aa = ui.listWidget_selectcmd->currentItem();
    QString  aa_string = aa->text();
     
    row_in_cmd = ui.listWidget_selectcmd->currentRow();//当前行
    select_in_cmd = aa_string;
 
    ui.textEdit_validdata->clear();
    ui.plainTextEdit_reminder->clear();
    if (aa_string == "进入测试模式")
    {   
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "01";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("进入测试模式", 1);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据1");
    }
    if (aa_string == "清空延时指令")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "02";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

         ui.textEdit_validdata->insertPlainText("02");
         map_name_to_code.insert("清空延时指令", 2);
         ui.plainTextEdit_reminder->appendPlainText("已有有效数据1");
    }
    if (aa_string == "清空延时任务") 
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "03";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

        ui.textEdit_validdata->insertPlainText("03");
        map_name_to_code.insert("清空延时任务", 3);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
    }
    if (aa_string == "星务计算机复位")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "04";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";
        ui.textEdit_validdata->insertPlainText("04");
        map_name_to_code.insert("星务计算机复位", 4);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
    }
    if (aa_string == "程序注入开始")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "05";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

        ui.textEdit_validdata->insertPlainText("05");
        map_name_to_code.insert("程序注入开始", 5);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
    }
    if (aa_string == "程序注入结束")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "06";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";
        ui.textEdit_validdata->insertPlainText("06");

        map_name_to_code.insert("程序注入结束", 6);
        ui.plainTextEdit_reminder->appendPlainText("已有有效数据2");
    }
    if (aa_string == "星务计算机遥测查询")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "0A";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";
     
     //   ui.textEdit_validdata->insertPlainText("7");
        map_name_to_code.insert("星务计算机遥测查询", 0x0a);
        ui.plainTextEdit_reminder->appendPlainText("测控通道：UXB测控数传一体机A：55H，");
    }

    if (aa_string == "星务计算机日志下载")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "0B";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

        map_name_to_code.insert("星务计算机日志下载", 0x0B);
        ui.plainTextEdit_reminder->appendPlainText("有效数据9字节，比如:dd0000123400002345,dd代表通道，\
                                                   00001234日志记录起始时间，00002345日志记录结束时间");
    }
//    delete aa;
}
void QtCommandtest::btn_to_del_select()
{
    qDebug() << "btn_to_del_select";
    QListWidgetItem* aItem = ui.listWidget_selectcmd->takeItem(row_in_cmd); //移除指定行的项，但不delete
    delete aItem; //需要手工删除对象
}

void QtCommandtest::btn_to_buildup_validdata()
{
    qDebug() << "btn_to_buildup_validdata";
    QString run_attr = ui.comboBox_exeattr->currentText();
    QString run_attr_code;
    QString delay_time = ui.lineEdit->text();
    QString valid_data = ui.textEdit_validdata->toPlainText(); 
    qint16 valid_data_size = valid_data.size();
    qint64 valid_data_int;
    qint8 thirteenth_byte;
   
    int num1 = map_name_to_code[select_in_cmd];


    QString strValue = QString("%1").arg(num1, 2, 16, QLatin1Char('0'));
    cmd_code = strValue;
 
    if (run_attr == "立即执行")
    {
        run_attr_code = "01";
        cmd_attribute = 1;
    }
    else
    {
        run_attr_code = "10";
        cmd_attribute = 2;
    }
    thirteenth_byte = ACK_reply << 4 | cmd_attribute << 2 | single_multiframe;

    thirteenth_byte_to_string = QString("%1").arg(thirteenth_byte, 1, 16, QLatin1Char('0'));
 //   thirteenth_byte_to_string  = thirteenth_byte_to_string.setNum(thirteenth_byte, 16); //显示为十六进制

    qint64 delay_time_int = delay_time.toInt();

    cmd_run_time = QString("%1").arg(delay_time_int, 1, 16, QLatin1Char('0'));

    cmd_valid_data = QString("%1").arg(valid_data_size, 4, 16, QLatin1Char('0'));
    cmd_valid_data.append(valid_data);

    valid_data_int = valid_data.toInt();

    qint64  CRC_int = crc16_ccitt(valid_data_int,0); 
    CRC_byte = CRC_byte.setNum(CRC_int, 16); //显示为十六进制
 
    up_data_frame = satellite_syn_byte;
    up_data_frame.append(authorize_byte);
    up_data_frame.append(pattern_byte);
    up_data_frame.append(cmd_type);
    up_data_frame.append(single_serial_number);
    up_data_frame.append(cmd_code);
    up_data_frame.append(thirteenth_byte_to_string);
    up_data_frame.append(cmd_run_time);
    up_data_frame.append(multiframe_count);
    up_data_frame.append(cmd_valid_data);
    up_data_frame.append(CRC_byte);
    up_data_frame.append(frame_end_byte);
    qDebug() << satellite_syn_byte;
    qDebug() << authorize_byte;
    qDebug() << pattern_byte;
    qDebug() << cmd_type;
    qDebug() << single_serial_number;
    qDebug() << cmd_code;
    qDebug() << thirteenth_byte_to_string;
    qDebug() << cmd_run_time;
    qDebug() << multiframe_count;
    qDebug() << cmd_valid_data;
    qDebug() << CRC_byte;
    qDebug() << frame_end_byte;
    qDebug() << up_data_frame;
    qDebug() << "btn_to_del_select www";
}


qint16 QtCommandtest::crc16_ccitt(qint8 data, qint16 crc)
{
    qint16 ccitt16 = 0x1021;

    qint8 i;

    crc ^= (data << 8);

    for (i = 0; i < 8; i++)
    {
        if (crc & 0x8000)
        {
            crc <<= 1;
            crc ^= ccitt16;
        }
        else
        {
            crc <<= 1;
        }
    }
    return crc;
}