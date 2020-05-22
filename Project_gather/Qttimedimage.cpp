#include "Qttimedimage.h"

#include "Project_gather.h"

#pragma execution_character_set("utf-8")

#pragma pack(1)

Qttimedimage::Qttimedimage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	Project_gather* pproject_gather;
	pproject_gather = new Project_gather;
	ground_detect_handle = pproject_gather->get_ground_detect_handle();

	connect(ui.btn_to_project_gather, SIGNAL(clicked()), this, SLOT(btn_to_project_gather()));

	connect(ui.btn_to_read_data, SIGNAL(clicked()), this, SLOT(tcp_send_data_trans_req_to_ground_detect()));

    connect(ui.btn_to_connect_data_tran, SIGNAL(clicked()), this, SLOT(btn_connect_ground_detect_revpoint()));
}

Qttimedimage::~Qttimedimage()
{
}

void Qttimedimage::btn_connect_ground_detect_revpoint()
{
	connect(ground_detect_handle, SIGNAL(readyRead()), this, SLOT(tcp_rev_data_from_ground_detect()));
    ui.textEdit->setPlainText("设置读取数据成功！");
}

void Qttimedimage::tcp_rev_data_from_ground_detect()
{
	QByteArray temp = ground_detect_handle->readAll();
    QDataStream out(&temp, QIODevice::ReadWrite);    //将字节数组读入
    data_tran_frame_t data_tran_frame;
    while (!out.atEnd())
    {
        out >> data_tran_frame.frame_head;
        data_tran_frame.frame_head = qFromBigEndian(data_tran_frame.frame_head);
        if (data_tran_frame.frame_head != 0XEB90)
        {
            qDebug() << "帧头错误！";
        }
        out >> data_tran_frame.frame_type;
        if (data_tran_frame.frame_type != 0Xdd)
        {
            qDebug() << "帧头错误！";
        }
        out >> data_tran_frame.frame_len;
        data_tran_frame.frame_len = qFromBigEndian(data_tran_frame.frame_len);
        out >> data_tran_frame.frame_aisle;
        if (data_tran_frame.frame_aisle != 0X0d)
        {
            qDebug() << "非数传通道！";
        }
        out >> data_tran_frame.frame_serial;
        out >> data_tran_frame.frame_time;
        if (data_tran_frame.frame_len - 10 > 0)
        {
            data_tran_frame.frame_data = new quint8[data_tran_frame.frame_len - 10];
        }
        else
        {
            qDebug() << "数据错误";
        }
        for (quint64 i = 0; i < data_tran_frame.frame_len - 10; i++)
        {
            out >> data_tran_frame.frame_data[i];
        }
        out >> data_tran_frame.frame_sum;
    }

    QFile filename("D:/aa.jpg");
    QByteArray b;
    quint64 num_byte = 0;
    char* buffer = new char[1];

    if (!filename.open(QIODevice::ReadWrite))
    {
        qDebug() << "Open File Error!";
        return;
    }
    else
    {
        char* str;
        str = new char[data_tran_frame.frame_len - 10];
        memcpy(str, data_tran_frame.frame_data, data_tran_frame.frame_len - 10);
        for (quint64 i = 0; i < data_tran_frame.frame_len - 10; i++)
        {
            memcpy(buffer, &data_tran_frame.frame_data[i], 1);
            b = buffer;
            filename.write(b);
            num_byte++;
        }
        qDebug() << num_byte;
        ui.textEdit->setPlainText("写完成");
    }
    qDebug() << "Save File Success";
    filename.close();
	ui.textEdit->setPlainText("地检数传接收成功");
}

void Qttimedimage::tcp_send_data_trans_req_to_ground_detect()
{
	data_trans_req_frame_t data_trans_req_frame;
	data_trans_req_frame.frame_head = 0XEB90;
	data_trans_req_frame.frame_type = 0Xdd;
	data_trans_req_frame.frame_len = 0x2;
	data_trans_req_frame.frame_data = 0x0100;
	data_trans_req_frame.frame_sum = 0xff;

    if (ground_detect_handle)
    {
        ground_detect_handle->write((char*)&data_trans_req_frame, 8);//发送到串口
    }
    else
    {
        ui.textEdit->setPlainText("句柄读取失败");
    }

	ui.textEdit->setPlainText("发送数传接收命令成功");
}

void Qttimedimage::btn_to_project_gather()
{
	Project_gather* Project_login = new Project_gather;
	Project_login->show();
	this->hide();
}