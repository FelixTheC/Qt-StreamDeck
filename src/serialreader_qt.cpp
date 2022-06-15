#include "serialreader_qt.h"
#include "qdebug.h"

SerialReader_QT::SerialReader_QT()
{
    initSerialReading();
}

SerialReader_QT::SerialReader_QT(const QString &port_name)
{
    usb_port = port_name;
    initSerialReading();
}

SerialReader_QT::~SerialReader_QT()
{
    serial_port->close();
}

void
SerialReader_QT::initSerialReading()
{
    serial_port = new QSerialPort(usb_port);
    // first close the port, makes it easier to re-open
    serial_port->close();

    connect(serial_port, &QSerialPort::readyRead, this, &SerialReader_QT::read_line);

    if (serial_port->open(QIODevice::ReadWrite))
    {
        serial_port->setBaudRate(QSerialPort::BaudRate::Baud9600);
        serial_port->setParity(QSerialPort::NoParity);
        serial_port->setDataBits(QSerialPort::Data7);   // receiving ASCII data
        open = true;
    }
}

void
SerialReader_QT::read_line()
{
    QByteArray raw_bytes = serial_port->readLine();

    auto data = QString::fromStdString(raw_bytes.toStdString());
    if (data[0].isLetterOrNumber() || data[0] == '#' || data[0] == '*')
    {
        serial_port->flush();
        auto str_data = data.toStdString();
        emit serialValueReceived(str_data);
    }
}

void
SerialReader_QT::changeBaudrate(qint64 &val)
{
    if (serial_port != nullptr && serial_port->isOpen())
    {
        serial_port->setBaudRate(val);
    }
}

bool
SerialReader_QT::isOpen()
{
    return open;
}
