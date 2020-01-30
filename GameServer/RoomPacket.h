#pragma once
#ifndef __GAMESERVER_ROOM_PACKET_H__
#define __GAMESERVER_ROOM_PACKET_H__

#include <string>
#include <list>
#include "BasePacket.h"
#include "Player.h"

//1byte
enum PacketTypeRoom : char {
	packetTypeRoomNone = 0,

	packetTypeRoomMakeRoomRequest, //Client -> GameServer
	packetTypeRoomMakeRoomResponse, //GameServer -> Client

	packetTypeRoomRoomListRequest, //Client -> GameServer
	packetTypeRoomRoomListResponse, //GameServer -> Client

	packetTypeRoomEnterRoomRequest, //Client -> GameServer

	packetTypeRoomRoomInfo, //GameServer -> Client Broadcast

	packetTypeRoomCount,
};

struct BaseRoomPacket : public BasePacket{
	BaseRoomPacket(PacketTypeRoom packetType) : BasePacket(BasePacketType::basePacketTypeRoom), m_packetTypeRoom(packetType){}
protected:
	PacketTypeRoom m_packetTypeRoom = PacketTypeRoom::packetTypeRoomNone;
};

// Make Room
struct RoomPacketMakeRoomRequest : public BaseRoomPacket {
	RoomPacketMakeRoomRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomMakeRoomRequest) {}

	std::string m_roomName;
	int16 m_maxPlayer = 0;
	int16 m_password = 0;

	virtual char* Serialize() override {
		this->TypeSerial(this->GetBasePacketType());
		this->TypeSerial(m_packetTypeRoom);

		this->StringSerial(m_roomName.c_str());
		this->IntSerial(m_maxPlayer);
		this->IntSerial(m_password);

		return buf;
	}
	virtual void Deserialize(char* _buf) override {
		if (nullptr == _buf)
			return;

		this->StringDeserial(_buf, m_roomName);
		this->IntDeserial(_buf, m_maxPlayer);
		this->IntDeserial(_buf, m_password);
	}
};
struct RoomPacketMakeRoomResponse : public BaseRoomPacket {
	RoomPacketMakeRoomResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomMakeRoomResponse) {}

	bool m_success;
	int32 m_roomNumber;

	virtual char* Serialize() override {
		this->BoolSerial(m_success);
		this->IntSerial(m_roomNumber);
		
		return buf;
	}
	virtual void Deserialize(char* _buf) override {
		if (nullptr == buf)
			return;

		this->BoolDeserial(_buf, m_success);
		this->IntDeserial(_buf, m_roomNumber);
	}
};

// Room list
struct RoomPacketRoomListRequest : public BaseRoomPacket {
	RoomPacketRoomListRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomRoomListRequest) {}

	virtual char* Serialize() override {
	}
	virtual void Deserialize(char* _buf) override {
	}
};
struct RoomPacketRoomListResponse : public BaseRoomPacket {
	RoomPacketRoomListResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomRoomListResponse) {}

	struct RoomInfo {
		int number = 0;
		int playerCount = 0;
		int maxPlayerCount = 0;
		bool password = false;
		std::string name;

	};

	std::list<RoomInfo> m_roomList;
	virtual char* Serialize() override {

	}
	virtual void Deserialize(char* _buf) override {

	}
};

#endif // !__GAMESERVER_ROOM_PACKET_H__
