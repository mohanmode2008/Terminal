#include "GenerateVerificationCode.h"

#include <QTime>
#include <QString>
#include <QDebug>

#include "Qtlogin.h"
#include "QtRegister.h"


GenerateVerificationCode::GenerateVerificationCode(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    connect(ui.btn_GenerateCode, SIGNAL(clicked()), this, SLOT(btn_GenerateCode()));

    networkAccessManager_storage = new QNetworkAccessManager(this);
    connect(ui.btn_putstorage, SIGNAL(clicked()), this, SLOT(btn_post_storage_clicked()));
    connect(networkAccessManager_storage, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_storage_post(QNetworkReply*)));

    networkAccessManager_toppouring = new QNetworkAccessManager(this);
    connect(ui.btn_toppouring, SIGNAL(clicked()), this, SLOT(btn_post_toppouring_clicked()));
    connect(networkAccessManager_toppouring, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_toppouring_post(QNetworkReply*)));

    networkAccessManager_del = new QNetworkAccessManager(this);
    connect(ui.btn_del, SIGNAL(clicked()), this, SLOT(btn_post_del_clicked()));
    connect(networkAccessManager_del, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_del_post(QNetworkReply*)));

    connect(ui.btn_register, SIGNAL(clicked()), this, SLOT(turn_register()));
    connect(ui.btn_login, SIGNAL(clicked()), this, SLOT(turn_login()));

    reply = Q_NULLPTR;   
}

void GenerateVerificationCode::btn_GenerateCode()
{

    int i, j;
    int num_to_generate;

 //   QString str3，str2;

    int Single;
    int Tens;


   
    QList<int> numbersList;

    num_to_generate = (ui.comboBox_number_code->currentText()).toInt();
    
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for (i = 0; i < num_to_generate; i++)
    {
        numbersList.append(qrand() % num_to_generate);
        bool flag = true;
        while (flag)
        {
            for (j = 0; j < i; j++)
            {
                if (numbersList[i] == numbersList[j])
                {
                    break;
                }
            }
            if (j < i)
            {
                numbersList[i] = rand() % num_to_generate;
            }
            if (j == i)
            {
                flag = !flag;
            }
        }
    }


    for (i = 0; i < num_to_generate; i++)
    {
        qDebug() << numbersList[i];


        Tens = numbersList[i] / 16;
        Single = numbersList[i] % 16;

        str2 = str2.setNum(Tens, 16);
            
        str3 = str3.setNum(Single,16);

        crccheck = crccheck.setNum((Tens + Single), 16);

        str = str2;
        str.append(str3);
        str.append("00");
        str.append(crccheck);

        aList.append(str);


    }
    for (i = 0; i < num_to_generate; i++)
    {
        ui.textEdit->append(aList[i]);
    }
     
    ui.textEdit->append(str3);

}


void GenerateVerificationCode::Get(QUrl u)
{
    QNetworkRequest request;
    url = u;
    request.setUrl(url);
    if (reply != Q_NULLPTR) {//更改reply指向位置钱一定要保证之前的定义了自动delete
        reply->deleteLater();
    }
    reply = manager.get(request);
    qDebug() << "start get";
    connect(reply, &QNetworkReply::finished, this, &GenerateVerificationCode::finished_get);
}


void GenerateVerificationCode::btn_get_clicked()
{

    html_text = "";
    Get(QUrl("http://192.168.1.222:8080/management/info"));
}


void GenerateVerificationCode::finished_get() 
{
    QByteArray bytes = reply->readAll();
   // ui.plainTextEdit->appendPlainText("btn_sure bbb");

    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    //判断是否对象
    if (doc.isObject())
    {
        //把json文档转换为json对象
        QJsonObject obj = doc.object();

        qDebug() << "jsonObject[name]==" << obj["display-ribbon-on-profiles"].toString();
        QJsonValue value = obj.value("git");

        if (value.isObject())
        {
            QJsonObject subobj = value.toObject();

            qDebug() << "jsonObject1[name]==" << subobj["dirty"].toString();

            QJsonValue value = subobj.value("build");

            if (value.isObject())
            {
                QJsonObject subobj = value.toObject();

                qDebug() << "jsonObject2[name]==" << subobj["time"].toString();

                QJsonValue value = subobj.value("id");

                if (value.isObject())
                {

                    QJsonObject subobj = value.toObject();

                    qDebug() << "jsonObject3[name]==" << subobj["abbrev"].toString();

                    //  QJsonValue value=obj.value("id");

                    //  QString name1=subobj.value("commit").toString();
 //
                 //     QString str3=ui->lineEdit->text();
 //
                  //    ui->plainTextEdit->appendPlainText(str3);
                }

            }

        }
        else
        {
            qDebug() << "value.isObject";
        }
    }
    else
    {
        qDebug() << "doc.isObject";
    }



    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    reply->deleteLater();
    reply = Q_NULLPTR;

    if (!redirectionTarget.isNull()) {//如果网址跳转重新请求
        const QUrl redirectedUrl = url.resolved(redirectionTarget.toUrl());
        qDebug() << "redirectedUrl:" << redirectedUrl.url();
        Get(redirectedUrl);
        return;
    }
    qDebug() << "finished";
    html_text = bytes;
    qDebug() << "get ready,read size:" << html_text.size();
    //   QFile f("result.html");//写出文件
    //   f.open(QFile::WriteOnly);
    //   f.write(bytes);
    //   f.close();

    ui.textEdit->setText(html_text);
 //   ui.plainTextEdit->appendPlainText("btn_sure aaa：");
}



void GenerateVerificationCode::btn_post_storage_clicked() 
{

    QtLogin* login1;
    login1 = new QtLogin;
    QString Bearer_string = login1->get_id_token();

    Bearer_string.prepend("Bearer ");

    QString satellite_id = ui.comboBox_satellite_id->currentText();
    QString Authorization_code = aList[0];

    QString str_to_send;

    QString str_of_codenumber = ui.comboBox_number_code->currentText();//读取n数量n

   // int num = str.toInt();

    int number_to_send = str_of_codenumber.toInt();
    int i;

 
    for (i = 0; i < number_to_send; i++)
    {


    }
    Authorization_code = aList[i];

    str_to_send = satellite_id;
    str_to_send.append(Authorization_code);

    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
    QJsonObject _exampleObject;

    _exampleObject.insert("satellite_id", satellite_id);
    _exampleObject.insert("code", Authorization_code);
         //post请求的数据是Json格式。首先创建QJsonObject对象，并插入数据。
    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
    // 转成Json格式

    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.222:8080/api/authenticate"));

    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray  Bearer_bytes = Bearer_string.toUtf8();
    netReq.setRawHeader("Authorization", Bearer_bytes);//服务器要求的数据头部
 
    networkAccessManager_storage->post(netReq, m_httpData);
}

void GenerateVerificationCode::finished_storage_post(QNetworkReply* reply) 
{
    QByteArray bytes = reply->readAll();

    //   QByteArray bytes("hello world");

    QString string;

    string.prepend(bytes);
  //  ui->plainTextEdit->appendPlainText(string);
 //   ui->plainTextEdit->appendPlainText("btn_sure bbb");

    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "statusCode:" << statusCode;

    if (reply->error() == QNetworkReply::NoError)
    {
        ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
    }
    else
    {
        qDebug() << "=========";
    }

    reply->deleteLater();
//    ui->plainTextEdit->appendPlainText("btn_sure aaa：");
}


void GenerateVerificationCode::btn_post_toppouring_clicked() 
{

    QString strUserInfo = QString("username=admin&password=admin&rememberMe=False");

    QByteArray  content = strUserInfo.toUtf8();
    int contentLength = content.length();

    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.222:8080/api/authenticate"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    netReq.setHeader(QNetworkRequest::ContentLengthHeader, contentLength);

    // 将用户名和密码发送至web服务器进行验证
 //   QNetworkAccessManager *m_manager = new QNetworkAccessManager(this);
    // 发送参数
    networkAccessManager_toppouring->post(netReq, content);
    //  ui->plainTextEdit->appendPlainText("btn_sure ccc");
       //    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post(QNetworkReply*)));
}

void GenerateVerificationCode::finished_toppouring_post(QNetworkReply* reply) 
{
    QByteArray bytes = reply->readAll();

    //   QByteArray bytes("hello world");

    QString string;

    string.prepend(bytes);
    //  ui->plainTextEdit->appendPlainText(string);
   //   ui->plainTextEdit->appendPlainText("btn_sure bbb");

    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "statusCode:" << statusCode;

    if (reply->error() == QNetworkReply::NoError)
    {
        ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
    }
    else
    {
        qDebug() << "=========";
    }

    reply->deleteLater();
    //    ui->plainTextEdit->appendPlainText("btn_sure aaa：");
}



void GenerateVerificationCode::btn_post_del_clicked()
{

    QString strUserInfo = QString("username=admin&password=admin&rememberMe=False");

    QByteArray  content = strUserInfo.toUtf8();
    int contentLength = content.length();

    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.222:8080/api/authenticate"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    netReq.setHeader(QNetworkRequest::ContentLengthHeader, contentLength);

    // 将用户名和密码发送至web服务器进行验证
 //   QNetworkAccessManager *m_manager = new QNetworkAccessManager(this);
    // 发送参数
    networkAccessManager_del->post(netReq, content);
    //  ui->plainTextEdit->appendPlainText("btn_sure ccc");
       //    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post(QNetworkReply*)));
}

void GenerateVerificationCode::finished_del_post(QNetworkReply* reply)
{
    QByteArray bytes = reply->readAll();

    //   QByteArray bytes("hello world");

    QString string;

    string.prepend(bytes);
    //  ui->plainTextEdit->appendPlainText(string);
   //   ui->plainTextEdit->appendPlainText("btn_sure bbb");

    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "statusCode:" << statusCode;

    if (reply->error() == QNetworkReply::NoError)
    {
        ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
    }
    else
    {
        qDebug() << "=========";
    }

    reply->deleteLater();
    //    ui->plainTextEdit->appendPlainText("btn_sure aaa：");
}

void GenerateVerificationCode::turn_register()
{ 
    QtRegister* register1;
    register1 = new QtRegister;
    register1->show();
    this->hide();
    delete register1;
}

void GenerateVerificationCode::turn_login()
{
    QtLogin* login;
    login = new QtLogin;
    login->show();
    this->hide();
    delete login;
}

