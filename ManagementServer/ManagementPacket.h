#ifndef __MANAGEMENT_PACKET_H__
#define __MANAGEMENT_PACKET_H__

#include <list>
#include "BasePacket.h"

using GPID = unsigned __int32;
using SessionID = __int64;
enum ManagementPacketType : char {
	managementPacketTypeNone = 0,

	// login server <-> management sserver
	loginManagementPacketTypeShowChannelResponse, // (int32)channelSize (channel(string, int, int)channelName, numberOfPeople, limitOfPeople), (unsigned int)GPID
	loginManagementPacketTypeShowChannelRequest, // (uint32)gpid;
	loginManagementPacketTypeChannelInResponse, // (bool)flag, (string)ip, (int)port, (uint32)gpid
	loginManagementPacketTypeChannelInRequest, //(string)channelName, (unsigned int)GPID, (int64)token

	// game server <-> management server
	registerServerInfo, //Game -> Management // (string)m_ipAddress, (int)port
	preLoadClient, //Management -> Game // (uint32)m_gpid, (int64)token
	updateServerInfo, //Game -> Management // (int32)m_currentPlayer
	transferChannelRequest,
	transferChannelResponse,
	disconnectUserRequest, // management -> game // (uint32) m_gpid;

	managementPacketTypeSize,
};

struct Channel {
	std::string _channelName;
	int32 _currentPeople = 0;
	int32 _maximumPeople = 0;

	Channel() {}
	Channel(std::string channelName, int32 currentPeople, int32 maximumPeople)
		: _channelName(channelName), _currentPeople(currentPeople), _maximumPeople(maximumPeople) {}
};

/*

		  login <-> management

*/

class LoginManagementPacket : public BasePacket {
public:
	LoginManagementPacket(ManagementPacketType _managementPacketType) : BasePacket(BasePacketType::loginPacketTypeManagement), managementPacketType(_managementPacketType) {
		this->PacketTypeSerial(managementPacketType);
	}
	~LoginManagementPacket() {}
protected:
	ManagementPacketType managementPacketType = managementPacketTypeNone;
};

class LoginManagementPacketTypeShowChannelResponse : public LoginManagementPacket {
public:
	LoginManagementPacketTypeShowChannelResponse() : LoginManagementPacket(loginManagementPacketTypeShowChannelResponse) {}
	~LoginManagementPacketTypeShowChannelResponse() {}

	virtual Buffer& Serialize() override {
		buffer << channelSize;
		for (auto s : channel) {
			buffer << s._channelName;
			buffer << s._currentPeople;
			buffer << s._maximumPeople;
		}
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> channelSize;
		Channel tmp;
		for (int i = 0; i < channelSize; ++i) {
			_buf >> tmp._channelName;
			_buf >> tmp._currentPeople;
			_buf >> tmp._maximumPeople;
			channel.emplace_back(tmp);
		}
		_buf >> gpid;
	}

	int32 channelSize = 0;
	std::list<Channel> channel;
	uint32 gpid = 0;
};

class LoginManagementPacketTypeShowChannelRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeShowChannelRequest() : LoginManagementPacket(loginManagementPacketTypeShowChannelRequest) {}
	~LoginManagementPacketTypeShowChannelRequest() {}

	virtual Buffer& Serialize() override {
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> gpid;
	}
	uint32 gpid = 0;
};

class LoginManagementPacketTypeChannelInResponse : public LoginManagementPacket {
public:
	LoginManagementPacketTypeChannelInResponse() : LoginManagementPacket(loginManagementPacketTypeChannelInResponse) {}
	~LoginManagementPacketTypeChannelInResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << ip;
		buffer << port;
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> ip;
		_buf >> port;
		_buf >> gpid;
	}

	bool flag = true;
	std::string ip;
	int16 port = 0;
	uint32 gpid = 0;
};

class LoginManagementPacketTypeChannelInRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeChannelInRequest() : LoginManagementPacket(loginManagementPacketTypeChannelInRequest) {}
	~LoginManagementPacketTypeChannelInRequest() {}

	virtual Buffer& Serialize() override {
		buffer << channelName;
		buffer << gpid;
		buffer << token;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> channelName;
		_buf >> gpid;
		_buf >> token;
	}

	std::string channelName;
	uint32 gpid;
	int64 token;
};

/*

		  game server <-> management server

*/

struct GameToManagementPacket : public BasePacket {
	GameToManagementPacket(ManagementPacketType type) : BasePacket(BasePacketType::basePacketTypeGameToManagementServer), m_packetType(type) {
		this->PacketTypeSerial(m_packetType);
	}
protected:
	ManagementPacketType m_packetType = ManagementPacketType::managementPacketTypeNone;
};

struct GameToManagementRegisterServerInfo : public GameToManagementPacket {
	GameToManagementRegisterServerInfo() : GameToManagementPacket(ManagementPacketType::registerServerInfo) {}

	/*int m_maxPlayer = 0;
	
	virtual Buffer& Serialize() override {
		buffer << m_maxPlayer;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_maxPlayer;
	}*/

	std::string m_ipAddress;
	__int32 m_portNum;

	virtual Buffer& Serialize() override {
		buffer << m_ipAddress;
		buffer << m_portNum;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> m_ipAddress;
		buf >> m_portNum;
	}
};

struct GameToManagementPreLoadRequest : public GameToManagementPacket {
	GameToManagementPreLoadRequest() : GameToManagementPacket(ManagementPacketType::preLoadClient) {}

	unsigned __int32 m_gpid;
	int64 m_token;

	virtual Buffer& Serialize() override {
		buffer << m_gpid;
		buffer << m_token;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_gpid;
		_buf >> m_token;
	}
};

struct GameToManagementUpdateServerInfoRequest : public GameToManagementPacket {
	GameToManagementUpdateServerInfoRequest() : GameToManagementPacket(ManagementPacketType::updateServerInfo) {}

	int m_currentPlayer;

	virtual Buffer& Serialize() override {
		buffer << m_currentPlayer;
		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_currentPlayer;
	}
};

struct GameToManagementTransferChannelRequest : public GameToManagementPacket {
	GameToManagementTransferChannelRequest() : GameToManagementPacket(ManagementPacketType::transferChannelRequest) {}

	std::string m_ipAddress;
	int m_portNum = 0;
	GPID m_gpid = 0;
	SessionID m_session = 0;
	
	virtual Buffer& Serialize() override {
		buffer << m_ipAddress;
		buffer << m_portNum;
		buffer << m_gpid;
		buffer << m_session;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_ipAddress;
		_buf >> m_portNum;
		_buf >> m_gpid;
		_buf >> m_session;

	}
};

struct GameToManagementTransferChannelResponse : public GameToManagementPacket {
	GameToManagementTransferChannelResponse() : GameToManagementPacket(ManagementPacketType::transferChannelResponse) {}

	GPID m_gpid = 0;
	std::string m_channelName;
	bool m_isSuccess = true;

	virtual Buffer& Serialize() override {
		buffer << m_gpid;
		buffer << m_channelName;
		buffer << m_isSuccess;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_gpid;
		_buf >> m_channelName;
		_buf >> m_isSuccess;
	}
};

struct GameToManagementDisconnectUserRequest : public GameToManagementPacket {
	GameToManagementDisconnectUserRequest() : GameToManagementPacket(ManagementPacketType::disconnectUserRequest) {}

	uint32 m_gpid;

	virtual Buffer& Serialize() override {
		buffer << m_gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_gpid;
	}
};

#endif