#pragma once
#ifndef __GAMESERVER_ROOM_MANAGER_H__
#define __GAMESERVER_ROOM_MANAGER_H__
#include <list>
#include <stack>
#include <memory>

#include "Room.h"
#include "RoomPacket.h"

class Player;

class RoomManager{
public:
	~RoomManager();

	void MakeRoom(size_t maxPlayer, Player* master);

	void Initialize();
	std::shared_ptr<Room> FindRoom(Player* pplayer);
	void RemoveRoom(std::shared_ptr<Room> room);

	size_t GetRoomCount() const { m_roomList.size(); }

	//Packet Handling
	
private:
	void ClearDeactivatedRoom();
	std::thread* m_roomWatcher = nullptr;

	std::list<std::shared_ptr<Room> > m_roomList;
	
	std::stack<int> m_roomNumberList;
	int m_maxRoomNumber = 100; //#DesignData

	size_t m_minPlayer = 2;	//#DesignData
	size_t m_maxPlayer = 6;	//#DesignData
};

#endif // !__GAMESERVER_ROOM_MANAGER_H__
