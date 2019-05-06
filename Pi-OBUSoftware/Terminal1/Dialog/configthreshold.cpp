#include "configthreshold.h"
#include "ui_configthreshold.h"
#include <QDebug>
configThreshold::configThreshold(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configThreshold)
{
    ui->setupUi(this);

    QFile file("/home/lab411/Desktop/gateway/Pi-OBUSoftware/build/WSAN/thresol.txt");
        QLabel *testLabel= new QLabel;

        QString line;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&file);
                DATA::temp_t = stream.readLine().toDouble();
                testLabel->setText(line+"\n");
                qDebug()<<DATA::temp_t;
                DATA::hump_t=stream.readLine().toDouble();
                DATA::lux_t=stream.readLine().toDouble();
                DATA::mois_t=stream.readLine().toInt();
        }
        file.close();
        ui->dsb_Temp->setValue(DATA::temp_t);
        ui->dsb_Humi->setValue(DATA::hump_t);
        ui->dsb_Light->setValue(DATA::lux_t);
        ui->dsb_mois->setValue(DATA::mois_t);

}

configThreshold::~configThreshold()
{
    delete ui;
}

void configThreshold::on_btn_accept_clicked()
{

    DATA::temp_t=ui->dsb_Temp->value();
    //qDebug()<<DATA::temp_t;
    DATA::hump_t=ui->dsb_Humi->value();
    //qDebug()<<DATA::hump_t;
    DATA::lux_t=ui->dsb_Light->value();
    //qDebug()<<DATA::lux_t;
    DATA::mois_t=ui->dsb_mois->value();
    QString filename = "/home/lab411/Desktop/gateway/Pi-OBUSoftware/build/WSAN/thresol.txt";
    QFile file( filename );
    if (file.open(QIODevice::ReadWrite)){
        QTextStream stream(&file);
        stream << clearenv();
        stream << DATA::temp_t << endl;
        stream << DATA::hump_t << endl;
        stream << DATA::lux_t << endl;
        stream << DATA::mois_t << endl;
    }
}

void configThreshold::on_pushButton_2_clicked()
{
    this ->close();
}
