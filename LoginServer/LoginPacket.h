#ifndef __LOGIN_PACKET_H__
#define __LOGIN_PACKET_H__

#include "ManagementPacket.h"
#include "LoginDBCachePacket.h"

enum LoginPacketType : char {
	loginPacketTypeNone = 0,

	// client <-> login server
	clientLoginPacketTypeLoginResponse, //(bool)flag(1), (string)userNick(4~8), (int64)token
	clientLoginPacketTypeLoginRequest, //(string)userId(4~8), (string)userPw(8~16)  
	clientLoginPacketTypeLogoutRequest, //nothing
	clientLoginPacketTypeSignupResponse, //(bool)flag
	clientLoginPacketTypeSignupRequest, //(string)userId(4~8), (string)userPw(8~16), (string)userNick(4~8),
	clientLoginPacketTypeDeleteResponse, //(bool)flag
	clientLoginPacketTypeDeleteRequest, //nothing
	clientLoginPacketTypeShowChannelResponse, // (int32)channelSize, (channel(string, int, int)channelName, numberOfPeople, limitOfPeople)
	clientLoginPacketTypeShowChannelRequest, //nothing.
	clientLoginPacketTypeChannelInResponse, //(bool)flag, (string)ip, (int)port
	clientLoginPacketTypeChannelInRequest, //(string)channelName

	loginPacketTypeSize,

};

/*

		  client <-> login

*/

class LoginPacket : public BasePacket {
public:
	LoginPacket(LoginPacketType _loginPacketType) : BasePacket(BasePacketType::basePacketTypeLogin), loginPacketType(_loginPacketType) {
		this->PacketTypeSerial(loginPacketType);
	}
	~LoginPacket() {}
protected:
	LoginPacketType loginPacketType = loginPacketTypeNone;
};

class ClientLoginPacketTypeLoginResponse : public LoginPacket {
public:
	ClientLoginPacketTypeLoginResponse() : LoginPacket(clientLoginPacketTypeLoginResponse) {}
	~ClientLoginPacketTypeLoginResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << userNick;
		buffer << token;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> userNick;
		_buf >> token;
	};

	bool flag = true;
	std::string userNick;
	int64 token = 0;
};

class ClientLoginPacketTypeLoginRequest : public LoginPacket {
public:
	ClientLoginPacketTypeLoginRequest() : LoginPacket(clientLoginPacketTypeLoginRequest) {}
	~ClientLoginPacketTypeLoginRequest() {}

	virtual Buffer& Serialize() override {
		buffer << userId;
		buffer << userPw;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> userId;
		_buf >> userPw;
	};

	std::string userId;
	std::string userPw;
};

class ClientLoginPacketTypeLogoutRequest : public LoginPacket {
public:
	ClientLoginPacketTypeLogoutRequest() : LoginPacket(clientLoginPacketTypeLogoutRequest) {}
	~ClientLoginPacketTypeLogoutRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
	}
};

class ClientLoginPacketTypeSignupResponse : public LoginPacket {
public:
	ClientLoginPacketTypeSignupResponse() : LoginPacket(clientLoginPacketTypeSignupResponse) {}
	~ClientLoginPacketTypeSignupResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
	}

	bool flag = true;
};

class ClientLoginPacketTypeSignupRequest : public LoginPacket {
public:
	ClientLoginPacketTypeSignupRequest() : LoginPacket(clientLoginPacketTypeSignupRequest) {}
	~ClientLoginPacketTypeSignupRequest() {}

	virtual Buffer& Serialize() override {
		buffer << userId;
		buffer << userPw;
		buffer << userNick;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> userId;
		_buf >> userPw;
		_buf >> userNick;
	}

	std::string userId;
	std::string userPw;
	std::string userNick;
};

class ClientLoginPacketTypeDeleteResponse : public LoginPacket {
public:
	ClientLoginPacketTypeDeleteResponse() : LoginPacket(clientLoginPacketTypeDeleteResponse) {}
	~ClientLoginPacketTypeDeleteResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
	}

	bool flag = true;
};

class ClientLoginPacketTypeDeleteRequest : public LoginPacket {
public:
	ClientLoginPacketTypeDeleteRequest() : LoginPacket(clientLoginPacketTypeDeleteRequest) {}
	~ClientLoginPacketTypeDeleteRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};

class ClientLoginPacketTypeShowChannelResponse : public LoginPacket {
public:
	ClientLoginPacketTypeShowChannelResponse() : LoginPacket(clientLoginPacketTypeShowChannelResponse) {}
	~ClientLoginPacketTypeShowChannelResponse() {}

	virtual Buffer& Serialize() override {
		buffer << channelSize;
		for (auto s : channel) {
			buffer << s._channelName;
			buffer << s._currentPeople;
			buffer << s._maximumPeople;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> channelSize;
		Channel tmp;
		for (int i = 0; i < channel.size(); ++i) {
			_buf >> tmp._channelName;
			_buf >> tmp._currentPeople;
			_buf >> tmp._maximumPeople;
			channel.emplace_back(tmp);
		}
	}

	int32 channelSize = 0;
	std::list<Channel> channel;
};

class ClientLoginPacketTypeShowChannelRequest : public LoginPacket {
public:
	ClientLoginPacketTypeShowChannelRequest() : LoginPacket(clientLoginPacketTypeShowChannelRequest) {}
	~ClientLoginPacketTypeShowChannelRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};

class ClientLoginPacketTypeChannelInResponse : public LoginPacket {
public:
	ClientLoginPacketTypeChannelInResponse() : LoginPacket(clientLoginPacketTypeChannelInResponse) {}
	~ClientLoginPacketTypeChannelInResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << ip;
		buffer << port;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> ip;
		_buf >> port;
	}

	bool flag = true;
	std::string ip;
	int16 port = 0;
};

class ClientLoginPacketTypeChannelInRequest : public LoginPacket {
public:
	ClientLoginPacketTypeChannelInRequest() : LoginPacket(clientLoginPacketTypeChannelInRequest) {}
	~ClientLoginPacketTypeChannelInRequest() {}

	virtual Buffer& Serialize() override {
		buffer << channelName;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> channelName;
	}

	std::string channelName;
};

#endif // __LOGIN_PACKET_H__