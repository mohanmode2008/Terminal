//#include "QTLogin.h"
#include "QTRegister.h"

QTRegister::QTRegister(QWidget *parent)
	: QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this);
	connect(ui.btn_to_getcode, SIGNAL(clicked()), this, SLOT(btn_post_getcode()));
	connect(ui.btn_register, SIGNAL(clicked()), this, SLOT(btn_post_register()));
    connect(ui.btn_to_login, SIGNAL(clicked()), this, SLOT(btn_to_login()));
 //   connect(ui.btn_to_getcode, SIGNAL(clicked()), this, SLOT(turn_verification()));
    connect(ui.btn_to_edit, SIGNAL(clicked()), this, SLOT(btn_post_edit()));

	networkAccessManager_post_code = new QNetworkAccessManager(this);
	connect(networkAccessManager_post_code, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_getcode(QNetworkReply*)));

	networkAccessManager_post_register = new QNetworkAccessManager(this);
	connect(networkAccessManager_post_register, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_register(QNetworkReply*)));

	networkAccessManager_post_edit = new QNetworkAccessManager(this);
	connect(networkAccessManager_post_edit, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_edit(QNetworkReply*)));	

    file = new QFile;
    file->setFileName("D:/VS_QT/project_gather/operation_record.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
}

QTRegister::~QTRegister()
{
    file->close();
}

void QTRegister::btn_to_login ()
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

    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
    QJsonObject _exampleObject;

    _exampleObject.insert("login", login);
    _exampleObject.insert("password", password);
    _exampleObject.insert("phone", phone);
    _exampleObject.insert("smsCode", smsCode);
    _exampleObject.insert("firstName", firstName);
    _exampleObject.insert("lastName", lastName);
    if (gender.length())
    {
        _exampleObject.insert("gender", gender);
    }
    else
    {
        _exampleObject.insert("gender", "SECRET");
    }
    _exampleObject.insert("birthday", birthday);
    _exampleObject.insert("email", email);
    _exampleObject.insert("companyName", companyName);
    _exampleObject.insert("companyPosition", companyPosition);
    _exampleObject.insert("companyAddress", companyAddress);
    //post请求的数据是Json格式。首先创建QJsonObject对象，并插入数据。
    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
    // 转成Json格式
    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.254:8080/api/register"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 将用户名和密码发送至web服务器进行验证
    qDebug() << "btn_post_register";
    networkAccessManager_post_code->post(netReq, m_httpData);
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
    QString string_to_send;
    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
    QJsonObject _exampleObject;

    if (Phonenum_string.count() != 11)
    {
        ui.plainTextEdit->appendPlainText("string in phone number not fit!");
        return 0;
    }
    _exampleObject.insert("phone", Phonenum_string);
    //post请求的数据是Json格式。首先创建QJsonObject对象，并插入数据。
    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
    // 转成Json格式
    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.254:8080/api/sms/generateVerificationCode"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkAccessManager_post_code->post(netReq, m_httpData);
    return 1;
}

void QTRegister::finished_post_getcode(QNetworkReply* reply) {

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
        //  ui.plainTextEdit->appendPlainText("Code apply fail!");
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
 /*   QtLogin* login1;
    login1 = new QtLogin;
    QString Bearer_string = login1->get_id_token();

    qDebug() << Bearer_string;
    ui.plainTextEdit->appendPlainText(Bearer_string);
    ui.plainTextEdit->appendPlainText("12");

    Bearer_string.prepend("Bearer ");

    QString firstName = ui.lineEdit_firstName->text();
    QString lastName = ui.lineEdit_lastName->text();
    QString gender = ui.lineEdit_gender->text();
    QString birthday = ui.lineEdit_birthday->text();
    QString email = ui.lineEdit_emilbox->text();
    QString companyName = ui.lineEdit_companyname->text();
    QString companyPosition = ui.lineEdit_companyPosition->text();
    QString companyAddress = ui.lineEdit_companyaddress->text();

    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
    QJsonObject _exampleObject;

    _exampleObject.insert("firstName", firstName);
    _exampleObject.insert("lastName", lastName);
    _exampleObject.insert("gender", gender);
    _exampleObject.insert("birthday", birthday);
    _exampleObject.insert("email", email);
    _exampleObject.insert("companyName", companyName);
    _exampleObject.insert("companyPosition", companyPosition);
    _exampleObject.insert("companyAddress", companyAddress);
    //post请求的数据是Json格式。首先创建QJsonObject对象，并插入数据。
    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
    // 转成Json格式
    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.254:8080//api/account"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    qDebug() << Bearer_string;
    QByteArray  Bearer_bytes = Bearer_string.toUtf8();
    netReq.setRawHeader("Authorization", Bearer_bytes);//服务器要求的数据头部

    // 将用户名和密码发送至web服务器进行验证
  //      networkAccessManager_post_edit->post(netReq, m_httpData);
  */
    return 1;
}
void QTRegister::finished_post_edit(QNetworkReply* reply) {

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


void QTRegister::on_pushButton_2_clicked()
{
  //  QString e = ui.text_e->toPlainText();
  //  QFile file("D:/VS_QT/project_gather/operation_record.txt");
  //  file.open(QIODevice::WriteOnly | QIODevice::Text);
  //  file.write("yangxioajun\n");
 //   file.close();
}