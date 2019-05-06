#include "define.h"
#include "qdebug.h"
define::define(){
    QString current_dir = QDir::currentPath().toAscii();
    CONF_MQTT=current_dir+"/WSAN/confMQTT.txt";
    DATA_SUPPORT=current_dir+"/WSAN/support.txt";
    //CONF_MQTT=current_dir.append();
    //qDebug()<<DATA_SUPPORT;

}
