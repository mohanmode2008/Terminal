#include "QtLogin.h"

#include "GenerateVerificationCode.h"
#include "QtRegister.h"

QtLogin::QtLogin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    reply = Q_NULLPTR;

    networkAccessManager_post_login = new QNetworkAccessManager(this);
    connect(networkAccessManager_post_login, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_login(QNetworkReply*)));
    connect(ui.btn_login, SIGNAL(clicked()), this, SLOT(btn_post_login()));

    connect(ui.btn_register, SIGNAL(clicked()), this, SLOT(turn_register()));
    connect(ui.btn_verification, SIGNAL(clicked()), this, SLOT(turn_verification()));
}

QtLogin::~QtLogin()
{
}


void QtLogin::btn_post_login()
{         
   /*------------------------*/
    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
    QJsonObject _exampleObject;

    _exampleObject.insert("username", "admin");
    _exampleObject.insert("password", "admin");
    _exampleObject.insert("rememberMe", "False");

        //post请求的数据是Json格式。首先创建QJsonObject对象，并插入数据。
    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
    // 转成Json格式
    QNetworkRequest netReq;
    netReq.setUrl(QUrl("http://192.168.1.222:8080/api/authenticate"));
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 将用户名和密码发送至web服务器进行验证
    networkAccessManager_post_login->post(netReq, m_httpData);
 //   ui->plainTextEdit->appendPlainText("btn_sure verification");

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
    //    ui->plainTextEdit->appendPlainText("btn_sure aaa：");

    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    //判断是否对象
    if (doc.isObject())
    {
        //把json文档转换为json对象
        QJsonObject obj = doc.object();

        id_token1 = obj["id_token"].toString();
      
   //     qDebug() << "jsonObject[name]==" << obj["id_token"].toString();
    }
    else
    {
        qDebug() << "doc is not Object";
    }
}


void QtLogin::turn_verification()
{

    GenerateVerificationCode* verification;
    verification = new GenerateVerificationCode;
    verification->show();
    this->hide();
}

void QtLogin::turn_register()
{
    QtRegister* register1;
    register1 = new QtRegister;
    register1->show();
    this->hide();

}

QString QtLogin::get_id_token()
{
    return id_token1;
}

/*
void Widget::Get(QUrl u) {
    QNetworkRequest request;
    url = u;
    request.setUrl(url);
    if (reply != Q_NULLPTR) {//更改reply指向位置钱一定要保证之前的定义了自动delete
        reply->deleteLater();
    }
    reply = manager_get_logout.get(request);
    qDebug() << "start get";
    connect(reply, &QNetworkReply::finished, this, &Widget::finished);
}
*/

void QtLogin::pushButton_get_clicked_logout()
{
    QNetworkRequest request;

    request.setUrl(QUrl("http://192.168.1.222:8080/management/info"));

    if (reply != Q_NULLPTR) {//更改reply指向位置钱一定要保证之前的定义了自动delete
        reply->deleteLater();
    }
    reply = manager_get_logout.get(request);
    qDebug() << "start get";
    connect(reply, &QNetworkReply::finished, this, &QtLogin::get_logout_finished);
}


void QtLogin::get_logout_finished() {
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


/*
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

    ui->textEdit->setText(html_text);
    ui->plainTextEdit->appendPlainText("btn_sure aaa：");

    */
}
