#include "QtLogin.h"
#include "Project_gather.h"
#include <QMessageBox>
#include<Windows.h>
#include <QboxLayout.h>

QtLogin::QtLogin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	reply = Q_NULLPTR;

	manager_post_login = new QNetworkAccessManager(this);
	connect(manager_post_login, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_login(QNetworkReply*)));

    manager_get_logout = new QNetworkAccessManager(this);
    connect(manager_get_logout, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_logout(QNetworkReply*)));

    manager_get_regiter_dev = new QNetworkAccessManager(this);
    connect(manager_get_regiter_dev, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_get_dev(QNetworkReply*)));

    regiser_dev_handle = new QNetworkAccessManager(this);
    connect(regiser_dev_handle, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_regiser_dev(QNetworkReply*)));

	connect(ui.btn_to_login, SIGNAL(clicked()), this, SLOT(btn_post_login()));
	connect(ui.btn_to_register, SIGNAL(clicked()), this, SLOT(btn_register()));
    connect(ui.btn_to_logout, SIGNAL(clicked()), this, SLOT(btn_post_logout()));    
    connect(ui.btn_to_quit, SIGNAL(clicked()), this, SLOT(btn_quit()));
    connect(ui.btn_to_status_show, SIGNAL(clicked()), this, SLOT(btn_return_status_show()));
    connect(ui.btn_to_imagetask, SIGNAL(clicked()), this, SLOT(btn_configurate_imageparameter()));

    file = new QFile;
    file->setFileName("D:/VS_QT/project_gather/operation_record.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
}

QtLogin::~QtLogin()
{
    file->close();
}

void QtLogin::btn_return_status_show()
{
    file->write("login:btn_return_status\n");
	Project_gather* Project_login = new Project_gather;
	Project_login->show();
	this->hide();
}

void QtLogin::btn_register()
{
    file->write("login:btn_to_register\n");
    QTRegister *Register = new QTRegister;
	Register->show();
	this->hide();
}

void QtLogin::btn_quit()
{
    file->write("login:btn_to_quit\n");
    this->close();
}

void QtLogin::btn_configurate_imageparameter()
{
    file->write("login:btn_to_Workparameter\n");
	QtWorkparameter* Workparameter = new QtWorkparameter;
	Workparameter->show();
	this->hide();
}

void QtLogin::btn_post_login()
{
    file->write("login:btn_post_login\n");
    QByteArray  http_data;
    QJsonDocument http_docum;
    QJsonObject  http_object;

    QString username = ui.Edit_name->text();
    QString password = ui.Edit_password->text();

    http_object.insert("username", username);
    http_object.insert("password", password);
    http_object.insert("rememberMe", "False");

    http_docum.setObject(http_object);
    http_data = http_docum.toJson(QJsonDocument::Compact);

    QNetworkRequest net_req;
    net_req.setUrl(QUrl("http://192.168.1.254:8080/api/authenticate"));
    net_req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager_post_login->post(net_req, http_data);
}

static QString id_token;
void QtLogin::finished_post_login(QNetworkReply* reply)
{
    QByteArray bytes = reply->readAll();
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    
    if (status_code == 200)
    {
        ui.plainTextEdit->appendPlainText("Code apply success!");
    }
    else
    {
        ui.plainTextEdit->appendPlainText("Code apply fail!");
    }

    if (reply->error() == QNetworkReply::NoError)
    {
        ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
    }
    else
    {
        ui.plainTextEdit->appendPlainText("login is fail!!!");
    }
    reply->deleteLater();
    QJsonDocument doc = QJsonDocument::fromJson(bytes);

    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        id_token = obj["id_token"].toString();
    }
    else
    {
        qDebug() << "login return doc is not Object";
    }
}

QString QtLogin::get_id_token()
{
    return id_token;
}

void QtLogin::btn_post_logout()
{
    file->write("login:btn_post_logout\n");
    QNetworkRequest request;
    request.setUrl(QUrl("http://192.168.1.254:8080/management/info"));
    if (reply != Q_NULLPTR) 
    {
        reply->deleteLater();
    }
    reply = manager_get_logout->get(request);
  //  connect(reply, &QNetworkReply::finished, this, &QtLogin::finished_post_logout);
}

void QtLogin::finished_post_logout() 
{
    QByteArray bytes = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(bytes);

    if (doc.isObject())
    {
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
    {
        reply->deleteLater();
    }
    connect(manager_get_id, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_get_usrid(QNetworkReply*)));
    reply = manager_get_id->get(request);
}

static QString usr_id;
void QtLogin::finished_get_usrid(QNetworkReply* reply)
{    
    QByteArray bytes = reply->readAll();
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
    QJsonParseError json_parse_error;
    QJsonDocument   json_document = QJsonDocument::fromJson(bytes, &json_parse_error);   
    if (!json_document.isEmpty() && json_parse_error.error == QJsonParseError::NoError)
    {
        qDebug() << "文件解析成功\n";
        if (json_document.isArray())
        {
            QJsonArray array = json_document.array();  // 转数组 
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
    QString bearer_string = login_success->get_id_token();
    QString token = "123456";
    QByteArray  http_data;
    QJsonDocument http_docum;
    QJsonObject http_object;

    QString description = "terminal";
    QString host = get_local_ip();
    QString mac_addr = get_local_mac();
    QString name = "terminal_device";

    QNetworkRequest net_req;

    http_object.insert("description", description);
    http_object.insert("host", host);
    http_object.insert("name", name);
    http_object.insert("mac_addr", mac_addr);

    http_docum.setObject(http_object);
    http_data = http_docum.toJson(QJsonDocument::Compact);  

    if (!token.isEmpty()) {
        net_req.setRawHeader("authorization", token.toStdString().c_str());
    }

    net_req.setUrl(QUrl("http://192.168.1.254:8080/api/devices"));
    net_req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    regiser_dev_handle->post(net_req, http_data);
}

void QtLogin::finished_post_regiser_dev(QNetworkReply* reply)
{
    QByteArray bytes = reply->readAll();
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

     if (status_code == 201)
    {
        ui.plainTextEdit->appendPlainText("regiser_dev success!");
    }
    else
    {
        ui.plainTextEdit->appendPlainText("regiser_dev fail!");
    }
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
    QJsonDocument doc = QJsonDocument::fromJson(bytes);

    if (doc.isObject())
    {
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

    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            strIpAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();

    return strIpAddress;
}

QString QtLogin::get_local_mac()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
    int nCnt = nets.count();
    QString strMacAddr = "";
    for (int i = 0; i < nCnt; i++)
    {
        if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && 
            nets[i].flags().testFlag(QNetworkInterface::IsRunning) &&
            !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    qDebug() << strMacAddr;
    return strMacAddr;
}