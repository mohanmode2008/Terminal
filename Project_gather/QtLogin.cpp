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

    _exampleObject.insert("username", "admin");
    _exampleObject.insert("password", "admin");
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
        qDebug() << "doc is not Object";
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
