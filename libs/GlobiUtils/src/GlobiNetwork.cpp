#include "GlobiNetwork.h"

QByteArray GlobiNetwork::generateNetworkPacket(quint16 packetType, const QVariant & content)
{
	QByteArray packet;
	QDataStream out(&packet, QIODevice::WriteOnly);

	out << (quint16)0 << packetType << content;
	out.device()->seek(0);
	out << (quint16)(packet.size() - sizeof(quint16));

	return packet;
}


GlobiNetwork::GLobiTcpSocket::GLobiTcpSocket(QObject * parent) : QTcpSocket(parent),
	m_packetSize(0), m_isConnected(false), m_isAutoConnect(false), 
	m_hostAddress(QHostAddress::Null), m_hostPort(0)
{
	m_autoConnectTimer = new QTimer();

	QObject::connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
	QObject::connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

	QObject::connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

	QObject::connect(m_autoConnectTimer, SIGNAL(timeout()), this, SLOT(onAutoTimerTimeOut()));
}

GlobiNetwork::GLobiTcpSocket::~GLobiTcpSocket()
{
	delete m_autoConnectTimer;
}

bool GlobiNetwork::GLobiTcpSocket::isConnected() const
{
	return m_isConnected;
}

void GlobiNetwork::GLobiTcpSocket::setAutoConnect(bool autoConnect, quint32 interval)
{
	m_isAutoConnect = autoConnect;
	m_autoConnectTimer->setInterval(interval);

	if(!autoConnect)
		m_autoConnectTimer->stop();
	else if(!m_isConnected)
		m_autoConnectTimer->start();
}

void GlobiNetwork::GLobiTcpSocket::setHost(const QString & address, quint16 port)
{
	m_hostAddress.setAddress(address);
	m_hostPort = port;

	this->connectToHost(m_hostAddress, m_hostPort);
}

void GlobiNetwork::GLobiTcpSocket::sendPacket(quint16 packetType, const QVariant & content)
{
	this->write(generateNetworkPacket(packetType, content));
}

void GlobiNetwork::GLobiTcpSocket::onConnected()
{
	m_isConnected = true;
	m_autoConnectTimer->stop();
}

void GlobiNetwork::GLobiTcpSocket::onDisconnected()
{
	m_isConnected = false;
	if(m_isAutoConnect)
		m_autoConnectTimer->start();
}

void GlobiNetwork::GLobiTcpSocket::onReadyRead()
{
	QDataStream in(this);

	if(m_packetSize == 0)
	{
		if(this->bytesAvailable() < sizeof(quint16))
			return;

		in >> m_packetSize;
	}

	if(this->bytesAvailable() >= m_packetSize)
	{
		quint16 type;
		QVariant content;

		in >> type;
		in >> content;

		emit packetReceived(type, content);
		handlePacket(type, content);

		m_packetSize = 0;

		if(this->bytesAvailable())
			onReadyRead();
	}
}

void GlobiNetwork::GLobiTcpSocket::onAutoTimerTimeOut()
{
	if(!m_hostAddress.isNull())
		if(this->state() == QAbstractSocket::UnconnectedState)
			this->connectToHost(m_hostAddress, m_hostPort);
}


GlobiNetwork::GlobiTcpServer::GlobiTcpServer(QObject * parent) : QTcpServer(parent),
	m_packetSize(0)
{
	m_sockets = new QMap <quint16, QTcpSocket *>;

	QObject::connect(this, SIGNAL(newConnection()), this, SLOT(onNewSocketConnected()));
}

GlobiNetwork::GlobiTcpServer::~GlobiTcpServer()
{
	delete m_sockets;
}

quint16 GlobiNetwork::GlobiTcpServer::nextAvailableId()
{
	quint16 id(0);
	while(m_sockets->contains(id)) id ++;
	return id;
}

void GlobiNetwork::GlobiTcpServer::sendPacketToAll(quint16 packetType, const QVariant & content)
{
	foreach(QTcpSocket * clientSocket, m_sockets->values())
	{
		clientSocket->write(generateNetworkPacket(packetType, content));
	}
}

void GlobiNetwork::GlobiTcpServer::sendToAllExcept(quint16 packetType, const QVariant & content, const QTcpSocket * except)
{
	foreach(QTcpSocket * clientSocket, m_sockets->values())
	{
		if(clientSocket != except)
			clientSocket->write(generateNetworkPacket(packetType, content));
	}
}

const QMap <quint16, QTcpSocket *> GlobiNetwork::GlobiTcpServer::sockets() const
{
	return *m_sockets;
}

void GlobiNetwork::GlobiTcpServer::kick(QTcpSocket * client)
{
	if(client)
		client->disconnectFromHost();
}

QTcpSocket * GlobiNetwork::GlobiTcpServer::socketFromIp(const QString & ip)
{
	foreach(QTcpSocket * socket, m_sockets->values())
		if(socket->peerAddress().toString() == ip)
			return socket;
	return NULL;
}

void GlobiNetwork::GlobiTcpServer::onNewSocketConnected()
{
	QTcpSocket * newSocket = this->nextPendingConnection();

	if(newSocket)
	{
		m_sockets->insert(nextAvailableId(), newSocket);
		QObject::connect(newSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
		QObject::connect(newSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		emit socketConnected(newSocket);
	}
}

void GlobiNetwork::GlobiTcpServer::onSocketDisconnected()
{
	QTcpSocket * disconnectedSocket = qobject_cast <QTcpSocket *> (sender());

	if(disconnectedSocket)
	{
		m_sockets->remove(m_sockets->key(disconnectedSocket));
		emit socketDisconnected(disconnectedSocket);
	}

}

void GlobiNetwork::GlobiTcpServer::onReadyRead()
{
	QTcpSocket * socketSender = qobject_cast <QTcpSocket *> (sender());
	
	if(socketSender)
	{
		QDataStream in(socketSender);

		if(m_packetSize == 0)
		{
			if(socketSender->bytesAvailable() < sizeof(quint16))
				return;

			in >> m_packetSize;
		}

		if(socketSender->bytesAvailable() >= m_packetSize)
		{
			quint16 type;
			QVariant content;

			in >> type;
			in >> content;

			emit packetReceived(type, content, socketSender);
			handlePacket(type, content, socketSender);

			m_packetSize = 0;

			if(socketSender->bytesAvailable())
				onReadyRead();
		}
	}
}