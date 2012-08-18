#ifndef GLOBINETWORK_H
#define GLOBINETWORK_H

#include "globilibs_global.h"
#include <QtCore>
#include <QtNetwork>

#define AUTO_CONNECT_INTERVAL 5000

namespace GlobiNetwork
{
	// Returns a globistandard network packet as an array of bytes from the given packet type and content
	GLOBILIBS_EXPORT QByteArray generateNetworkPacket(quint16 packetType, const QVariant & content);

	// Class that handles basic client-side tcp networking by subclassing QTcpSocket and making it more high-level
	class GLOBILIBS_EXPORT GLobiTcpSocket : public QTcpSocket
	{
		Q_OBJECT

		public :
			GLobiTcpSocket(QObject * parent = 0);
			virtual ~GLobiTcpSocket();

			bool isConnected() const;

			// Enable / disable auto-retry when connecting fails every given interval in ms
			void setAutoConnect(bool autoConnect, quint32 interval = AUTO_CONNECT_INTERVAL);
			void setHost(const QString & address, quint16 port);
			
			// Wrapper, generates a packet and send it using write
			void sendPacket(quint16 packetType, const QVariant & content);

			virtual void handlePacket(quint16, const QVariant &){}

		private slots :
			void onConnected();
			void onDisconnected();

			void onReadyRead();

			void onAutoTimerTimeOut();

		signals :
			void packetReceived(quint16, QVariant);

		private :
			quint16 m_packetSize;
			bool m_isConnected;
			QHostAddress m_hostAddress;
			quint16 m_hostPort;

			QTimer * m_autoConnectTimer;
			bool m_isAutoConnect;
	};

	// Class for handeling basic server-side tcp networking
	class GLOBILIBS_EXPORT GlobiTcpServer : public QTcpServer
	{
		Q_OBJECT

		public :
			GlobiTcpServer(QObject * parent = 0);
			virtual ~GlobiTcpServer();

			quint16 nextAvailableId();

			// Wrapper, send a message on every connected socket using write
			void sendPacketToAll(quint16 packetType, const QVariant & content);
			void sendToAllExcept(quint16 packetType, const QVariant & content, const QTcpSocket * except);
			const QMap <quint16, QTcpSocket *> sockets() const;

			virtual void handlePacket(quint16, const QVariant &, QTcpSocket *){}

			void kick(QTcpSocket * client);
			QTcpSocket * socketFromIp(const QString & ip);

		private slots :
			void onNewSocketConnected();
			void onSocketDisconnected();

			void onReadyRead();

		signals :
			void packetReceived(quint16, const QVariant &, QTcpSocket *);
			void socketConnected(QTcpSocket *);
			void socketDisconnected(QTcpSocket *);

		private :
			quint16 m_packetSize;
			QMap <quint16, QTcpSocket *> * m_sockets;

	};
}

#endif