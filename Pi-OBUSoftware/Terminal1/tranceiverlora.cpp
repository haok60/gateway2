#include "tranceiverlora.h"

tranceiverlora::tranceiverlora(QObject *parent):QObject(parent)
{
    timer = new QTimer(this);
    timer->setInterval(1000);

    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    port = new QextSerialPort("ttyS0", settings, QextSerialPort::Polling);
    enumerator = new QextSerialEnumerator(this);
    enumerator->setUpNotifications();
    connect(timer, SIGNAL(timeout()), SLOT(readDataLR()));
    connect(port, SIGNAL(readyRead()), SLOT(readDataLR()));
}

void tranceiverlora::requestWorkLR()
{
    emit workRequestedLR();
}
void tranceiverlora::doWorkLR()
{
    if(!port->isOpen())
    {
        port->open(QIODevice::ReadWrite);
    }
    if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
        timer->start();
    else
        timer->stop();
}

void tranceiverlora::WriteAppend(QString FileName, QString Image)
{
    QString str, image;
    int hex;
    int len = Image.length()/2;
    for(int i=0; i < len; i++)
    {
        str = Image.mid(i*2, 2);
        bool ok;
        hex = str.toInt(&ok, 16);
        image += (char) hex;
    }
    QFile file(FileName);
    if(!file.open(QIODevice::Append|QIODevice::Truncate)) return;
    file.write(image.toLatin1());
    file.close();
}

int tranceiverlora::CheckCodeLora(QString code)
{
    if(!code.compare("#LO")) return 1;
    if(!code.compare("#L1")) return 2;
    if(!code.compare("#LJ")) return 3;
    if(!code.compare("#LR")) return 4;
    return 0;
}
void tranceiverlora::writeData(QString cmd)
{
    int N = cmd.length();
    int hex_len = N/2;
    int hex_val;
    QString tmp;
    //qDebug() << N << cmd << endl;
    QByteArray ba;
    for(int i = 0; i < hex_len; i++)
    {
        tmp = cmd.mid(i*2, 2);
        bool ok;
        hex_val = tmp.toInt(&ok, 16);
        ba[i] = (char)hex_val;
        qDebug() << ba[i] << endl;
    }
    //qDebug();
    port->write(cmd.toLocal8Bit());
    //port->write(ba);

}

void tranceiverlora::WriteTextAppend(QString FileName, QString Text)
{
    QFile file(FileName);
    if(file.open(QIODevice::Append))
    {
        QTextStream stream(&file);
        stream << Text << endl;
    }
}
void tranceiverlora::readDataLR()
{
    readfile x;
    int t = port->bytesAvailable();
    if (t) {
        QString Code, Start, End, Image, my_Imgage;
        QByteArray ba = port->readAll();
        QString test(ba);
        //WriteTextAppend(x.LOG_FILE,test + "------------------------------------------\n");
        char* tmp = ba.data();
        char line[1024];
        QString Line;
        int i;
        int j = 0;
        for(i=0;i<t;i++){
            if(tmp[i] == '\n'){
                qDebug()<<"xxx";
                line[j] = '\0';
                Line = QString::fromLocal8Bit(line);
                if(j > 1){
                    emit receivedDataLR(Line);
                }
                //WriteTextAppend(x.LOG_FILE, Line + "***************************************\n");
                qDebug()<< "Line:  "<< Line;
                Code = Line.mid(0,3);

                switch(CheckCodeLora(Code)){
                case 1:// Take temperature and humidity
                    {
                        bool ok;
                        unsigned int td = Line.mid(5,4).toUInt(&ok, 16);
                        unsigned int rhd = Line.mid(9,4).toUInt(&ok, 16);
                        double temp = (double)(td*0.01 - 39.6);
                        double rh_lind = (double)(0.0367*rhd - 0.0000015955*rhd*rhd - 2.0468);
                        double humi = (double)((temp - 25)*(0.01 + 0.00008*rhd) + rh_lind);
                        int mac= Line.mid(3,2).toUInt(&ok);
                        if(humi > 100) humi = 100;
                        if(humi < 0) humi = 0;
                        QTime time;
                        QString t = time.currentTime().toString("hh-mm-ss");
                        QDate date;
                        QString d = date.currentDate().toString("dd/MM/yyyy");
                        QString tmp = Line.mid(3,2) + ":" + Line.mid(3,2) + ":" + QString::number(temp) + ":" + QString::number(humi);

    //                    tmp = "\nThong tin nhiet do, do am tu sensor ";
    //                    tmp += Line.mid(8,2);
    //                    tmp += ", dia chi Ip ";
    //                    tmp += Line.mid(4, 4);
    //                    tmp += "\nNhiet do:        ";
    //                    tmp += QString::number(temp);
    //                    tmp += "\nDo am:           ";
    //                    tmp += QString::number(humi);
    //                    tmp += "\n";

                        DATA::temp= QString::number(temp);
                        DATA::hump= QString::number(humi);
                        emit tempAndHum(tmp);
                        emit sendTandH(mac,temp,humi);
                        QString data = d+":"+t+":"+Line.mid(3,2) + ":" + QString::number(temp) + ":" + QString::number(humi);
                        WriteTextAppend(x.HISTORY_FILE, data);
                        break;
                    }
                case 2://Take Soil Moisture
                    {
                        bool ok;
                        int Mois = Line.mid(3,2).toInt(&ok,10);
                        int mac = Line.mid(5,2).toInt(&ok,10);
                        QString tmp = Line.mid(5,2) + ":" + QString::number(Mois);
                        emit sendMois(mac,Mois);

                        emit completeMois(tmp);
                }
                case 3://node join
                {
                    bool ok;
                    QString mac = Line.mid(3,2);
                    DATA::mac = mac;
                    int tmp = mac.toInt(&ok, 10);
                    emit nodeJoinLR(tmp);
                }
                case 4: //take dust
                {
                    bool ok;
                    int Dust_mesure = Line.mid(3,3).toInt(&ok,10);
                    int mac = Line.mid(6,2).toInt(&ok,10);
					double Vo = (double)(5.00*Dust_mesure)/1024.00;
					double dustDensity = (double)(Vo-0.6)*0.2*1000;
                    QString tmp = Line.mid(6,2) + ":" + QString::number(dustDensity);
                    emit sendDust(mac,dustDensity);
                    emit completeDust(tmp);
					qDebug()<<"dust mesure :"<<Dust_mesure;
					//qDebug()<<Dust<<" : "<<mac;
                }
                default : break;
            }
                j = 0;
            }
            else{
                line[j] = tmp[i];
                j++;
            }
        }
    }
}



