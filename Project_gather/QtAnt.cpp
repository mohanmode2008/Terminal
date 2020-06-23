#include "QtAnt.h"
#include "Project_gather.h"
#include "Qttimedimage.h"
#include "QtRealtimeimage.h"
#include <QtNetwork>

#pragma execution_character_set("utf-8")

QtAnt::QtAnt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    tcp_to_ground_detect_handle = nullptr;

    QMap<QString, QString> ant_axis;
    ant_axis.insert("天线方位轴", "1");
    ant_axis.insert("天线俯仰轴", "2");
    ui.comboBox_ant_axis->clear();
    foreach(const QString & str, ant_axis.keys())
        ui.comboBox_ant_axis->addItem(str, ant_axis.value(str));

    connect(ui.btn_to_reset, SIGNAL(clicked()), this, SLOT(btn_to_reset()));
    connect(ui.btn_to_set_angle, SIGNAL(clicked()), this, SLOT(btn_to_set_angle()));
    connect(ui.btn_to_set_speed, SIGNAL(clicked()), this, SLOT(btn_to_set_speed()));
    connect(ui.btn_to_stop_ant, SIGNAL(clicked()), this, SLOT(btn_to_stop_ant()));
    connect(ui.btn_to_decide_ant_direction, SIGNAL(clicked()), this, SLOT(btn_to_decide_ant_direction()));
    connect(ui.btn_to_decide_ant_pitch, SIGNAL(clicked()), this, SLOT(btn_to_decide_ant_pitch()));
    connect(ui.btn_inquire_ant, SIGNAL(clicked()), this, SLOT(btn_to_inquire_ant()));
    connect(ui.btn_inquire_gps, SIGNAL(clicked()), this, SLOT(btn_to_inquire_gps()));
    connect(ui.btn_connect_tcp, SIGNAL(clicked()), this, SLOT(btn_to_connect_ant()));
    connect(ui.btn_to_set_rev, SIGNAL(clicked()), this, SLOT(btn_set_rev_ant_data()));  

    connect(ui.btn_to_rev_data_tran, SIGNAL(clicked()), this, SLOT(btn_set_ground_detect_rev()));
    

    connect(ui.btn_to_init_timer, SIGNAL(clicked()), this, SLOT(init_timer())); 

    connect(ui.btn_to_realtieme, SIGNAL(clicked()), this, SLOT(btn_to_realtime_image()));
    connect(ui.btn_to_delaytime, SIGNAL(clicked()), this, SLOT(btn_to_timed_image()));
    

    Project_gather* project_gather = new Project_gather;

    tcp_to_ant_handle = project_gather->get_ant_handle();

    tcp_to_ground_detect_handle = project_gather->get_ground_detect_handle();

    QMap<QString, int> set_type;
    set_type.insert("射频设置", 0x01);
    set_type.insert("速率设置", 0x02);
    set_type.insert("解调参数", 0x03);
    set_type.insert("帧设置", 0x04);
    set_type.insert("解扰", 0x05);
    set_type.insert("接收", 0x06);
    set_type.insert("保存文件", 0x07);
    ui.comboBox->clear();
    foreach(const QString & str, set_type.keys())
        ui.comboBox->addItem(str, set_type.value(str));

}

QtAnt::~QtAnt()
{
}

void QtAnt::btn_to_gather()
{
    Project_gather* project_gather = new Project_gather;
    project_gather->show();
    this->hide();
}

void QtAnt::btn_to_realtime_image()
{
    //	m_tcpClient->write((char*)data_to_send, (total_validdata_length + 9));
    QtRealtimeimage* realtimeimage = new QtRealtimeimage;
    realtimeimage->show();
    this->hide();
}

void QtAnt::btn_to_timed_image()
{
    //	m_tcpClient->write((char*)data_to_send, (total_validdata_length + 9));

    Qttimedimage* timedimage = new Qttimedimage;
    timedimage->show();
    this->hide();
}

void QtAnt::btn_to_reset()
{
    QString reset = "RESET,ANT*ff";

    char* ch;
    QByteArray ba = reset.toLatin1();
    ch = ba.data();
    tcp_to_ant_handle->write((char*)ch, reset.size());
}

void QtAnt::btn_to_set_angle()
{
    QString set_angle ;
    QString direction = ui.line_direction->text();// .toFloat();
    QString pitch = ui.line_pitch->text();
    set_angle = "$$POS," + direction + "," + pitch + "*ff";

    char* ch;
    QByteArray ba = set_angle.toLatin1();
    ch = ba.data();
    tcp_to_ant_handle->write((char*)ch, set_angle.size());
    qDebug() << set_angle;
}

void QtAnt::btn_to_set_speed()
{
    QString set_speed;

    QString ant_axis = ui.comboBox_ant_axis->currentData().toString();// .toFloat();
    QString speed = ui.line_speed->text();
    set_speed = "$$SPEED," + ant_axis + "," + speed + "*ff";

    char* ch;
    QByteArray ba = set_speed.toLatin1();
    ch = ba.data();
    tcp_to_ant_handle->write((char*)ch, set_speed.size());
    qDebug() << set_speed;
}

void QtAnt::btn_to_stop_ant()
{
    QString stop_ant = "STOP,ANT*ff";

    char* ch;
    QByteArray ba = stop_ant.toLatin1();
    ch = ba.data();
    tcp_to_ant_handle->write((char*)ch, stop_ant.size());

    qDebug() << stop_ant;
}

void QtAnt::btn_to_decide_ant_direction()
{
    QString decide_ant_direction = "SET,AZ-ZERO*ff";

    char* ch;
    QByteArray ba = decide_ant_direction.toLatin1();
    ch = ba.data();
    tcp_to_ant_handle->write((char*)ch, decide_ant_direction.size());
    qDebug() << decide_ant_direction;
}

void QtAnt::btn_to_decide_ant_pitch()
{
    QString decide_ant_pitch = "SET,PI-ZERO*ff";

    char* ch;
    QByteArray ba = decide_ant_pitch.toLatin1();
    ch = ba.data();
    tcp_to_ant_handle->write((char*)ch, decide_ant_pitch.size());
    qDebug() << decide_ant_pitch;
}

void QtAnt::btn_to_inquire_ant()
{
    QString inquire_ant = "GET,ANT-STATE*ff";

    char* ch;
    QByteArray ba = inquire_ant.toLatin1();
    ch = ba.data();
    tcp_to_ant_handle->write((char*)ch, inquire_ant.size());
    qDebug() << inquire_ant;
}

void QtAnt::btn_to_inquire_gps()
{
    QString inquire_gps = "GET,GPS-STATE*ff";

    char* ch;
    QByteArray ba = inquire_gps.toLatin1();
    ch = ba.data();
    tcp_to_ant_handle->write((char*)ch, inquire_gps.size());
    qDebug() << inquire_gps;
}

void QtAnt::btn_set_rev_ant_data()
{
    connect(tcp_to_ant_handle, SIGNAL(readyRead()), this, SLOT(read_ant_data()));
}

void QtAnt::read_ant_data()
{
    QByteArray temp = tcp_to_ant_handle->readAll();
    QString answer = temp;
    int val;
    QString ant_state_ans;
    QString ant_initial_state;
    QString ant_run_state;
    QString direction_angle;
    QString pitch_angle;
    QString GPS_state;
    QString UTC_time;
    QString latitude;
    QString latitude_direction;
    QString longitude;
    QString longitude_direction;
    QString satellite_num;
    QString level_acc;
    QString height;

     val = QString::compare(answer, "$$SET,OK*ff", Qt::CaseInsensitive);
    if (val == 0)
    {
        ui.plainTextEdit->appendPlainText("设置成功！");
    }

     ant_state_ans = answer.section(",", 0, 0);
     val = QString::compare(ant_state_ans, "$$ANT-STATE", Qt::CaseInsensitive);
    if (val == 0)
    {
        ant_initial_state = answer.section(",", 1, 1);
        ant_run_state     = answer.section(",", 2, 2);
        direction_angle   = answer.section(",", 3, 3);
        pitch_angle       = answer.section(",", 4, 4);
        pitch_angle       = pitch_angle.left(pitch_angle.indexOf("*"));
        if(ant_initial_state.toInt() )

        switch (ant_initial_state.toInt()) {
        case 0:
            ui.plainTextEdit->appendPlainText("天线初始化状态：初始化！");
            break; 
        case 1:
            ui.plainTextEdit->appendPlainText("天线初始化状态：定位！");
            break; 
        case 2:
            ui.plainTextEdit->appendPlainText("天线初始化状态：定向！");
            break;
        case 3:
            ui.plainTextEdit->appendPlainText("天线初始化状态：初始化完成！");
            break;
        default: 
            ui.plainTextEdit->appendPlainText("数据错误！");
        }

        switch (ant_run_state.toInt()) {
        case 0:
            ui.plainTextEdit->appendPlainText("天线运行状态：初始化！");
            break;
        case 1:
            ui.plainTextEdit->appendPlainText("天线运行状态：正在跟星中……");
            break;
        case 2:
            ui.plainTextEdit->appendPlainText("天线运行状态：系统故障！");
            break;
        case 3:
            ui.plainTextEdit->appendPlainText("天线运行状态：触发方位轴限位！");
            break;
        case 4:
            ui.plainTextEdit->appendPlainText("天线运行状态：触发俯仰轴限位！");
            break;
        default:
            ui.plainTextEdit->appendPlainText("数据错误！");
        }

        ui.plainTextEdit->appendPlainText("方位当前角度值:" + direction_angle);
        ui.plainTextEdit->appendPlainText("俯仰当前角度值:" + pitch_angle);
    }

    GPS_state = answer.section(",", 0, 0);
    val = QString::compare(GPS_state, "$$GPS-STATE", Qt::CaseInsensitive);
    if (val == 0)
    {
        UTC_time            = answer.section(",", 1, 1);
        latitude            = answer.section(",", 2, 2);
        latitude_direction  = answer.section(",",3, 3);
        longitude           = answer.section(",", 4, 4);
        longitude_direction = answer.section(",", 5, 5);
        satellite_num       = answer.section(",", 6,6);
        level_acc           = answer.section(",", 7, 7);
        height              = answer.section(",",8, 8);
        height = height.left(height.indexOf("*"));
        ui.plainTextEdit->appendPlainText("UTC 时间:" + UTC_time);
        ui.plainTextEdit->appendPlainText("纬度:" + latitude);
        ui.plainTextEdit->appendPlainText("纬度方位:" + latitude_direction);
        ui.plainTextEdit->appendPlainText("经度:" + longitude);
        ui.plainTextEdit->appendPlainText("经度方位:" + longitude_direction);
        ui.plainTextEdit->appendPlainText("使用中卫星数目:" + satellite_num);
        ui.plainTextEdit->appendPlainText("水平精度因子:" + level_acc);
        ui.plainTextEdit->appendPlainText("高度:" + height);
    }

    qDebug() << "ant_state";

/*    QDataStream out(&temp, QIODevice::ReadWrite);    //将字节数组读入
    rev_valid_data_head_t rev_valid_data_head;

    rev_data_t rev_data;
    int i;
    for (i = 0; i < 4; i++)
    {
        out >> rev_data.satellite_syn_byte[i];
    }
    */
}

void QtAnt::init_timer()
{
    timer = new QTimer;

    timer->setSingleShot(false);

    timer->start(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(timer_time_out()));
}

void QtAnt::timer_time_out()
{
    qDebug() << "TimerTimeOut";
    //判断定时器是否运行
   // if (m_timer->isActive())
    //   m_timer->stop();   //停止定时器
    //执行定时器触发时需要处理的业务
    btn_to_reset();
}

void QtAnt::btn_set_param_format_to_data_server()
{
    char* send_data;
    int data_len;
    qint8 set_type;
    quint8 check_sum;
     if (ui.comboBox->currentData() == 0x04)
    {
         send_data = new char[15];
         data_len = 9;
         memset(send_data + 10, ui.frame_len->text().toInt(), 4);
    }
    else
    {
         send_data = new char[11];
         data_len = 5;
    }
     memset(send_data, 0XEB90,2);
     memset(send_data + 2, 0X02, 1);
     memset(send_data + 3, data_len, 2);
     set_type = (ui.comboBox->currentData().toString()).toInt();
     memset(send_data + 5, set_type, 1);
     memset(send_data + 6, ui.lineEdit->text().toInt(), 4);


     check_sum = check_sum1((quint8*)&send_data, data_len);

     if (ui.comboBox->currentData() == 0x04)
     {
         memset(send_data + 14, check_sum, 1);
     }
     else
     {
         memset(send_data + 10, check_sum, 1);
     }
    if (tcp_to_ground_detect_handle)
    {
        if(data_len == 9)
        tcp_to_ground_detect_handle->write((char*)send_data, 15);//发送到串口
        else
        tcp_to_ground_detect_handle->write((char*)send_data, 11);//发送到串口
        ui.plainTextEdit->setPlainText("发送设置参数命令成功");
    }
    else
    {
        ui.plainTextEdit->setPlainText("句柄读取失败");
    }   
}

void QtAnt::btn_set_ground_detect_rev()
{
    if (tcp_to_ground_detect_handle !=nullptr)
    {
        connect(tcp_to_ground_detect_handle, SIGNAL(readyRead()), this, SLOT(tcp_rev_data_from_ground_detect()));
        ui.plainTextEdit->setPlainText("设置读取ground_detect_rev成功！");
    }
    else
    {
        ui.plainTextEdit->setPlainText("地检句柄为NULL！");
    } 
}

void QtAnt::tcp_rev_data_from_ground_detect()
{
    QByteArray temp = tcp_to_ground_detect_handle->readAll();
    QDataStream out(&temp, QIODevice::ReadWrite);    //将字节数组读入
    qint16 frame_head;
    qint8 frame_type;
    qint16 frame_len;
    qint8 set_type;
    qlonglong time_param;
    qint8 run_status;
    qint8 check_sum;

        out >> frame_head;
        frame_head = qFromBigEndian(frame_head);
        if (frame_head != 0XEB90)
        {
            qDebug() << "帧头错误！";
        }
        out >> frame_type;
        if (frame_type == 0X02)
        {
            analyze_param_setting_req(temp.mid(3));
        }
        else if (frame_type == 0X01)
        {
            analyze_dev_status(temp.mid(3));
        }
        else if (frame_type == 0Xdd)
        {
            ui.plainTextEdit->setPlainText("数传数据，不在此解析！");
        }
        else
        {
            ui.plainTextEdit->setPlainText("帧类型错误！");
        }

    //    qChecksum(temp, 10);
}

void QtAnt::analyze_param_setting_req(QByteArray bytearray)
{
    QDataStream out(&bytearray, QIODevice::ReadWrite);    //将字节数组读入
    qint16 frame_len;
    qint8 set_type;
    qlonglong time_param;
    qint8 run_status;

    out >> frame_len;
    out >> set_type;
    out >> time_param;
    out >> run_status;
    if (run_status)
    {
        ui.plainTextEdit->setPlainText("执行失败！");
    }
    else
    {
        ui.plainTextEdit->setPlainText("执行成功！");
    }
}

void QtAnt::analyze_dev_status(QByteArray bytearray)
{
    QDataStream out(&bytearray, QIODevice::ReadWrite);    //将字节数组读入
    qint16 frame_len;  
    qint8 pack_serial;
    qlonglong time;
    qint8 signal_strength;
    qint8 AGC_gain;
    qint8 reserve;
    qint16 demodulation_param;
    qint8 synthesize_data;

    out >> frame_len;
    out >> pack_serial;
    out >> time;
    out >> signal_strength;
    out >> AGC_gain;
    out >> reserve;
    out >> demodulation_param;
    out >> synthesize_data;
  
}

quint8 QtAnt::check_sum1(quint8* a, int len)
{
    unsigned int sum = 0;

    while (len > 1) {
        sum += *a++;
        len -= 1;
    }

    if (len) {
        sum += *(unsigned char*)a;
    }

    while (sum >> 8) {
        sum = (sum >> 8) + (sum & 0xff);
    }

    return (quint8)(~sum);
}