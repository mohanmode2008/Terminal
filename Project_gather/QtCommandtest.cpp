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

    menu[0] = new QMenu("���ܷ�ϵͳ");
    menu[0]->addAction("XWOBC");
    menu[0]->addAction("AMPOC");

    menu[1] = new QMenu("���������ϵͳ");
    menu[1]->addAction("���������ϵͳ");
   
    menu[2] = new QMenu("��Դ��ϵͳ");
    menu[2]->addAction("PCDUң��ָ��");

    menu[3] = new QMenu("���������ϵͳ");
    menu[3]->addAction("UXB�������һ���");
    menu[3]->addAction("VUͨ�Ż�");
    menu[3]->addAction("X���������");

    menu[4] = new QMenu("�˿ط�ϵͳ");
    menu[4]->addAction("GNSS���ջ�A");
    menu[4]->addAction("GNSS���ջ�Bң��ָ��");
    menu[4]->addAction("����̫��������");
    menu[4]->addAction("��������A");
    menu[4]->addAction("��������B");
    menu[4]->addAction("��������C");
    menu[4]->addAction("��ǿ��ң��ָ��");
    menu[4]->addAction("����ң��ָ��");
    menu[4]->addAction("��������ң��ָ��");
    menu[4]->addAction("������ң��ָ��");

    menu[5] = new QMenu("�����ϵͳ");
    menu[5]->addAction("�����");
    menu[5]->addAction("���������");
    menu[5]->addAction("�Խ�����Aң��ָ��");
    menu[5]->addAction("�Խ�����Bң��ָ��");
    menu[5]->addAction("����ѹ������Ԫ");

    menu[6] = new QMenu("����ң��ָ��");
    menu[6]->addAction("����������ϵͳ");

    menu[7] = new QMenu("������עָ��");
    menu[7]->addAction("������עָ��");

    menu[8] = new QMenu("�غ��豸");
    menu[8]->addAction("GNSS���ǹ۲ⵥԪң��ָ��");
    menu[8]->addAction("DCS�ɼ���Ԫң��ָ��");

    menu[9] = new QMenu("ֱ��ָ��");
    menu[9]->addAction("ֱ��ָ��");

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


    strList << "�������ģʽ" << "�����ʱָ��" << "�����ʱ����" << "����������λ" << "����ע�뿪ʼ" << "����ע�����"
        <<"��������ң���ѯ"<<"����������־����"<<"�����������ʼ�¼����"<<"������������Ϣ����"<<"���������¼�ļ�����"<<"����ִ�м�¼�ļ�����"<<"�����豸͸��ң���ѯ"
        <<"PCDUң���ѯ"<<"�������һ���ң���ѯ"<<"�˿ط�ϵͳң���ѯ"<<"GNSS���ջ�ABң���ѯ"<<"����ģʽ�л�"<<"ACKӦ�𿪹�����"<<"��������ͬ��������"<<"����ͨ��������ʹ������"
        <<"չ���ͷŻ����Ľ�ֹ��ʹ��"<<"�Զ��ȿصĽ�ֹ��ʹ��"<<"ң��ͨ����ѡ��"<<"�Զ���صĽ�ֹ��ʹ��"<<"��ʱ�ϵ�ʱ��������"<<"ʵʱ�㲥ң��Ŀ����͹ر�"<<"ʵʱ�㲥ң��ļ��ʱ��";

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

   // ui.listWidget->selectedItem����;

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
     
    row_in_cmd = ui.listWidget_selectcmd->currentRow();//��ǰ��
    select_in_cmd = aa_string;
 
    ui.textEdit_validdata->clear();
    ui.plainTextEdit_reminder->clear();
    if (aa_string == "�������ģʽ")
    {   
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "01";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

        ui.textEdit_validdata->insertPlainText("01");
        map_name_to_code.insert("�������ģʽ", 1);
        ui.plainTextEdit_reminder->appendPlainText("������Ч����1");
    }
    if (aa_string == "�����ʱָ��")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "02";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

         ui.textEdit_validdata->insertPlainText("02");
         map_name_to_code.insert("�����ʱָ��", 2);
         ui.plainTextEdit_reminder->appendPlainText("������Ч����1");
    }
    if (aa_string == "�����ʱ����") 
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "03";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

        ui.textEdit_validdata->insertPlainText("03");
        map_name_to_code.insert("�����ʱ����", 3);
        ui.plainTextEdit_reminder->appendPlainText("������Ч����2");
    }
    if (aa_string == "����������λ")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "04";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";
        ui.textEdit_validdata->insertPlainText("04");
        map_name_to_code.insert("����������λ", 4);
        ui.plainTextEdit_reminder->appendPlainText("������Ч����2");
    }
    if (aa_string == "����ע�뿪ʼ")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "05";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

        ui.textEdit_validdata->insertPlainText("05");
        map_name_to_code.insert("����ע�뿪ʼ", 5);
        ui.plainTextEdit_reminder->appendPlainText("������Ч����2");
    }
    if (aa_string == "����ע�����")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "06";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";
        ui.textEdit_validdata->insertPlainText("06");

        map_name_to_code.insert("����ע�����", 6);
        ui.plainTextEdit_reminder->appendPlainText("������Ч����2");
    }
    if (aa_string == "��������ң���ѯ")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "0A";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";
     
     //   ui.textEdit_validdata->insertPlainText("7");
        map_name_to_code.insert("��������ң���ѯ", 0x0a);
        ui.plainTextEdit_reminder->appendPlainText("���ͨ����UXB�������һ���A��55H��");
    }

    if (aa_string == "����������־����")
    {
        pattern_byte = "BC";
        cmd_type = "3A";
        single_serial_number = "5A";
        cmd_code = "0B";

        ACK_reply = 1;
        single_multiframe = 0;
        multiframe_count = "0000";

        map_name_to_code.insert("����������־����", 0x0B);
        ui.plainTextEdit_reminder->appendPlainText("��Ч����9�ֽڣ�����:dd0000123400002345,dd����ͨ����\
                                                   00001234��־��¼��ʼʱ�䣬00002345��־��¼����ʱ��");
    }
//    delete aa;
}
void QtCommandtest::btn_to_del_select()
{
    qDebug() << "btn_to_del_select";
    QListWidgetItem* aItem = ui.listWidget_selectcmd->takeItem(row_in_cmd); //�Ƴ�ָ���е������delete
    delete aItem; //��Ҫ�ֹ�ɾ������
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
 
    if (run_attr == "����ִ��")
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
 //   thirteenth_byte_to_string  = thirteenth_byte_to_string.setNum(thirteenth_byte, 16); //��ʾΪʮ������

    qint64 delay_time_int = delay_time.toInt();

    cmd_run_time = QString("%1").arg(delay_time_int, 1, 16, QLatin1Char('0'));

    cmd_valid_data = QString("%1").arg(valid_data_size, 4, 16, QLatin1Char('0'));
    cmd_valid_data.append(valid_data);

    valid_data_int = valid_data.toInt();

    qint64  CRC_int = crc16_ccitt(valid_data_int,0); 
    CRC_byte = CRC_byte.setNum(CRC_int, 16); //��ʾΪʮ������
 
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