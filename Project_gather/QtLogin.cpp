#include "QtLogin.h"

#include "Project_gather.h"

#include <QMessageBox>

#include<Windows.h>

#include <QboxLayout.h>

//#include "QtImagetask.h"

QtLogin::QtLogin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	reply = Q_NULLPTR;

	networkAccessManager_post_login = new QNetworkAccessManager(this);
	connect(networkAccessManager_post_login, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_login(QNetworkReply*)));

    manager_get_regiter_dev = new QNetworkAccessManager(this);
    connect(manager_get_regiter_dev, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_get_dev(QNetworkReply*)));

	connect(ui.btn_to_login, SIGNAL(clicked()), this, SLOT(btn_post_login()));
	connect(ui.btn_to_register, SIGNAL(clicked()), this, SLOT(btn_to_register()));
    connect(ui.btn_to_logout, SIGNAL(clicked()), this, SLOT(btn_post_logout()));    
    connect(ui.btn_to_quit, SIGNAL(clicked()), this, SLOT(btn_to_quit()));
    connect(ui.btn_to_status, SIGNAL(clicked()), this, SLOT(btn_return_status()));
    connect(ui.btn_to_taskset, SIGNAL(clicked()), this, SLOT(btn_to_Workparameter()));
    connect(ui.btn_to_imagetask, SIGNAL(clicked()), this, SLOT(btn_to_Workparameter()));

    file = new QFile;
    file->setFileName("D:/VS_QT/project_gather/operation_record.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
}

QtLogin::~QtLogin()
{
    file->close();
}

void QtLogin::btn_return_status()
{
    file->write("login:btn_return_status\n");
	Project_gather* Project_login = new Project_gather;
	Project_login->show();
	this->hide();
}

void QtLogin::btn_to_register()
{
    file->write("login:btn_to_register\n");
    QTRegister *Register = new QTRegister;
	Register->show();
	this->hide();
}

void QtLogin::btn_to_quit()
{
    file->write("login:btn_to_quit\n");
    this->close();
}

void QtLogin::btn_to_Workparameter()
{
    file->write("login:btn_to_Workparameter\n");
	QtWorkparameter* Workparameter = new QtWorkparameter;
	Workparameter->show();
	this->hide();
}

void QtLogin::btn_post_login()
{
    file->write("login:btn_post_login\n");
    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
    QJsonObject _exampleObject;

    QString username = ui.Edit_name->text();
    QString password = ui.Edit_password->text();

    _exampleObject.insert("username", username);
    _exampleObject.insert("password", password);
    _exampleObject.insert("rememberMe", "False");

    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
    // 转成Json格式
    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.254:8080/api/authenticate"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 将用户名和密码发送至web服务器进行验证
    networkAccessManager_post_login->post(netReq, m_httpData);
}

static QString id_token1;
void QtLogin::finished_post_login(QNetworkReply* reply)
{
    QByteArray bytes = reply->readAll();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "statusCode:" << statusCode;

    if (statusCode == 200)
    {
        qDebug() << "Code apply success!";
        ui.plainTextEdit->appendPlainText("Code apply success!");
    }
    else
    {
        qDebug() << "Code apply fail!";
        ui.plainTextEdit->appendPlainText("Code apply fail!");
    }

    if (reply->error() == QNetworkReply::NoError)
    {
        ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
    }
    else
    {
        qDebug() << "receive fail!";
        ui.plainTextEdit->appendPlainText("login is fail!!!");
    }
    reply->deleteLater();
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    //判断是否对象
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        id_token1 = obj["id_token"].toString();
    }
    else
    {
        qDebug() << "login return doc is not Object";
    }
}

QString QtLogin::get_id_token()
{
    return id_token1;
}


void QtLogin::btn_post_logout()
{
    file->write("login:btn_post_logout\n");

    QNetworkRequest request;

    request.setUrl(QUrl("http://192.168.1.254:8080/management/info"));

    if (reply != Q_NULLPTR) {//更改reply指向位置钱一定要保证之前的定义了自动delete
        reply->deleteLater();
    }
    reply = manager_get_logout.get(request);
    qDebug() << "start get";
    connect(reply, &QNetworkReply::finished, this, &QtLogin::finished_post_logout);
}


void QtLogin::finished_post_logout() {
    QByteArray bytes = reply->readAll();
    //  ui.plainTextEdit->appendPlainText("btn_sure bbb");

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
}


void QtLogin::btn_get_usrid()
{
    file->write("login:btn_get_id\n");

    manager_get_id = new QNetworkAccessManager(this);

    QNetworkRequest request;

    request.setUrl(QUrl("http://192.168.1.254:8080//api/account"));

    if (reply != Q_NULLPTR) 
    {//更改reply指向位置钱一定要保证之前的定义了自动delete
        reply->deleteLater();
    }

    connect(manager_get_id, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_get_usrid(QNetworkReply*)));
    reply = manager_get_id->get(request);
}

static QString usr_id;

void QtLogin::finished_get_usrid(QNetworkReply* reply)
{
    
    QByteArray bytes = reply->readAll();
 //   QString usr_id;
    QString gender;
    QString firstName;
    QString lastName;
    QString email;
    QString address;
    QString company;
    QString phone;

   
    if (reply->error() == QNetworkReply::NoError)
    {
        ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
    }
    else
    {
        qDebug() << "statusCode is err!";
    }
    
    reply->deleteLater();
    QJsonParseError jsonParserError;
    QJsonDocument   jsonDocument = QJsonDocument::fromJson(bytes, &jsonParserError);
   
    if (!jsonDocument.isEmpty() && jsonParserError.error == QJsonParseError::NoError)
    {
        qDebug() << "文件解析成功\n";
        if (jsonDocument.isArray())
        {
            QJsonArray array = jsonDocument.array();  // 转数组 
            int nSize = array.size();
            for (int i = 0; i < nSize; ++i)
            {
                
                usr_id    = array.at(i).toObject().value("id").toString();
                gender    = array.at(i).toObject().value("gender").toString();
                firstName = array.at(i).toObject().value("firstName").toString();
                lastName  = array.at(i).toObject().value("lastName").toString();
                email     = array.at(i).toObject().value("email").toString();
                address   = array.at(i).toObject().value("address").toString();
                company   = array.at(i).toObject().value("company").toString();
                phone     = array.at(i).toObject().value("phone").toString();
                
            }
        }
    }
    
}


QString QtLogin::get_usr_id()
{
    return usr_id;
}

void QtLogin::btn_post_regiser_dev()
{
    file->write("login:btn_post_regiser_dev\n");

    QtLogin* login_success;
    login_success = new QtLogin;
    QString Bearer_string = login_success->get_id_token();

    QString token = Bearer_string;
    QString token = "123456";

    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
    QJsonObject _exampleObject;

    QString description = "terminal";
    QString host = get_local_ip();
    qDebug()<<host;
    QString name = "terminal_device";

    _exampleObject.insert("description", description);
    _exampleObject.insert("host", host);
    _exampleObject.insert("name", name);

    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
    // 转成Json格式
    QNetworkRequest netReq;

    if (!token.isEmpty()) {
        netReq.setRawHeader("authorization", token.toStdString().c_str());
    }

    netReq.setUrl(QUrl("http://192.168.1.254:8080/api/authenticate"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 将用户名和密码发送至web服务器进行验证
    networkAccessManager_post_login->post(netReq, m_httpData);
}


void QtLogin::finished_post_regiser_dev() 
{
    QByteArray bytes = reply->readAll();
    //  ui.plainTextEdit->appendPlainText("btn_sure bbb");

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
}

QString QtLogin::get_local_ip()
{    
    QString strIpAddress;  
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();  
    // 获取第一个本主机的IPv4地址 

    int nListSize = ipAddressesList.size(); 
    for (int i = 0; i < nListSize; ++i)  
    {  
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && 
            ipAddressesList.at(i).toIPv4Address())
        {
            strIpAddress = ipAddressesList.at(i).toString();    
            break; 
        }  
    }     // 如果没有找到，则以本地IP地址为IP 
    if (strIpAddress.isEmpty())    
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();  

    return strIpAddress;
 }


void QtLogin::btn_get_dev_interface()
{
    file->write("login:btn_get_dev_interface\n");

    QtLogin* login_success;
    login_success = new QtLogin;
    QString Bearer_string = login_success->get_id_token();
  //  QString token = Bearer_string;
    QString token = "123456";
    QUrl url("http://192.168.1.254:8080/api/devices");

    QNetworkRequest request(url);
    if (!token.isEmpty()) {
        request.setRawHeader("authorization", token.toStdString().c_str());
    }
    request.setRawHeader("Content-Type", "application/json");
    manager_get_regiter_dev->get(request);
}


void QtLogin::finished_get_dev(QNetworkReply* reply)
{
    QByteArray bytes = reply->readAll();
    //  ui.plainTextEdit->appendPlainText("btn_sure bbb");

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
}