#include "QTRegister.h"

QTRegister::QTRegister(QWidget *parent)
	: QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this);
	connect(ui.btn_to_getcode, SIGNAL(clicked()), this, SLOT(btn_post_getcode()));
	connect(ui.btn_to_register, SIGNAL(clicked()), this, SLOT(btn_post_register()));
    connect(ui.btn_to_login, SIGNAL(clicked()), this, SLOT(btn_login()));
    connect(ui.btn_to_edit, SIGNAL(clicked()), this, SLOT(btn_post_edit()));

    manager_post_register = new QNetworkAccessManager(this);

	connect(manager_post_register, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_register(QNetworkReply*)));

    manager_post_code = new QNetworkAccessManager(this);
	connect(manager_post_code, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_getcode(QNetworkReply*)));

    manager_post_edit = new QNetworkAccessManager(this);
	connect(manager_post_edit, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_edit(QNetworkReply*)));

    file = new QFile;
    file->setFileName("D:/VS_QT/project_gather/operation_record.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
}

QTRegister::~QTRegister()
{
    file->close();
}

void QTRegister::btn_login ()
{
    file->write("btn_to_login\n");
	QtLogin* Project_login = new QtLogin;
	Project_login->show();
	this->hide();
}

void QTRegister::btn_quit()
{
    file->write("btn_quit\n");
	this->close();   
}

void QTRegister::btn_post_register()
{
    file->write("btn_post_register\n");
    QString login = ui.lineEdit_name->text();
    QString password = ui.lineEdit_code->text();
    QString phone = ui.lineEdit_phonenum->text();
    QString smsCode = ui.lineEdit_smsCode->text();
    QString firstName = ui.lineEdit_firstName->text();
    QString lastName = ui.lineEdit_lastName->text();
    QString gender = ui.lineEdit_gender->text();
    QString birthday = ui.lineEdit_birthday->text();
    QString email = ui.lineEdit_emilbox->text();
    QString companyName = ui.lineEdit_companyname->text();
    QString companyPosition = ui.lineEdit_companyPosition->text();
    QString companyAddress = ui.lineEdit_companyaddress->text();

    QByteArray  http_data;
    QJsonDocument http_docum;
    QJsonObject http_object;

    http_object.insert("login", login);
    http_object.insert("password", password);
    http_object.insert("phone", phone);
    http_object.insert("smsCode", smsCode);
    http_object.insert("firstName", firstName);
    http_object.insert("lastName", lastName);
    if (gender.length())
    {
        http_object.insert("gender", gender);
    }
    else
    {
        http_object.insert("gender", "SECRET");
    }
    http_object.insert("birthday", birthday);
    http_object.insert("email", email);
    http_object.insert("companyName", companyName);
    http_object.insert("companyPosition", companyPosition);
    http_object.insert("companyAddress", companyAddress);

    http_docum.setObject(http_object);
    http_data = http_docum.toJson(QJsonDocument::Compact);

    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.254:8080/api/register"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
 
    manager_post_register->post(netReq, http_data);
}

void QTRegister::finished_post_register(QNetworkReply* reply)
{
    QByteArray bytes = reply->readAll();

    QString string;
    string.prepend(bytes);
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() == QNetworkReply::NoError)
    {
        ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
    }
    else
    {
        qDebug() << "statusCode is err!";
    }
    reply->deleteLater();
}

int QTRegister::btn_post_getcode()
{
    file->write("btn_post_getcode\n");
    QString Phonenum_string = ui.lineEdit_phonenum->text();
    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
    QJsonObject _exampleObject;

    if (Phonenum_string.count() != 11)
    {
        ui.plainTextEdit->appendPlainText("string in phone number not fit!");
        return 0;
    }
    _exampleObject.insert("phone", Phonenum_string);

    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);

    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.254:8080/api/sms/generateVerificationCode"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager_post_code->post(netReq, m_httpData);
    return 1;
}

void QTRegister::finished_post_getcode(QNetworkReply* reply) 
{
    QByteArray bytes = reply->readAll();
    QString string;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (statusCode == 201)
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
        qDebug() << "=========";
    }
    reply->deleteLater();
}

int QTRegister::btn_post_edit()
{
    QtLogin* login;
    login = new QtLogin;
    QString bearer_string = login->get_id_token();

    ui.plainTextEdit->appendPlainText(bearer_string);
    ui.plainTextEdit->appendPlainText("12");

    bearer_string.prepend("Bearer ");

    QString firstName = ui.lineEdit_firstName->text();
    QString lastName = ui.lineEdit_lastName->text();
    QString gender = ui.lineEdit_gender->text();
    QString birthday = ui.lineEdit_birthday->text();
    QString email = ui.lineEdit_emilbox->text();
    QString companyName = ui.lineEdit_companyname->text();
    QString companyPosition = ui.lineEdit_companyPosition->text();
    QString companyAddress = ui.lineEdit_companyaddress->text();

    QByteArray  http_data;
    QJsonDocument http_docum;
    QJsonObject http_object;

    http_object.insert("firstName", firstName);
    http_object.insert("lastName", lastName);
    if (gender.length())
    {
        http_object.insert("gender", gender);
    }
    else
    {
        http_object.insert("gender", "SECRET");
    }
    http_object.insert("gender", gender);
    http_object.insert("birthday", birthday);
    http_object.insert("email", email);
    http_object.insert("companyName", companyName);
    http_object.insert("companyPosition", companyPosition);
    http_object.insert("companyAddress", companyAddress);

    http_docum.setObject(http_object);
    http_data = http_docum.toJson(QJsonDocument::Compact);

    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.254:8080//api/account"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray  Bearer_bytes = bearer_string.toUtf8();
    netReq.setRawHeader("Authorization", Bearer_bytes);//服务器要求的数据头部

    // 将用户名和密码发送至web服务器进行验证
    manager_post_edit->post(netReq, http_data);
  
    return 1;
}

void QTRegister::finished_post_edit(QNetworkReply* reply)
{
    QByteArray bytes = reply->readAll();
    QString string;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "statusCode:" << statusCode;
    if (statusCode == 201)
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
        qDebug() << "=========";
    }
    reply->deleteLater();
}
