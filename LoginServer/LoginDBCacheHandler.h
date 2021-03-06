#ifndef __LOGIN_DBCACHE_HANDLER_H__
#define __LOGIN_DBCACHE_HANDLER_H__

#include <memory>
#include "LoginPacket.h"
#include "BaseServerHandler.h"
#include <chrono>
#include <mutex>

class LoginServer;

//class Player;

class LoginDBCacheHandler : public BaseServerHandler {
public:
	void Initialize();

	void HandleLoginPacket(Buffer& buffer, std::shared_ptr<Player> player, LoginPacketType type); //Client to server

	virtual void RegisterToServer() override {}
	virtual void HandlePacket(Buffer& buffer) override;
private:
	// login server to db cache
	void HandlePacketLoginRequest(ClientLoginPacketTypeLoginRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketLogoutRequest(ClientLoginPacketTypeLogoutRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketSignupRequest(ClientLoginPacketTypeSignupRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketDeleteRequest(ClientLoginPacketTypeDeleteRequest& packet, std::shared_ptr<Player> player);

	// login server to client
	void HandlePacketLoginResponse(LoginDBCachePacketTypeLoginResponse& packet, std::shared_ptr<Player> player);
	void HandlePacketSignupResponse(LoginDBCachePacketTypeSignupResponse& packet, std::shared_ptr<Player> player);
	void HandlePacketDeleteResponse(LoginDBCachePacketTypeDeleteResponse& packet, std::shared_ptr<Player> player);

	// check
	bool CheckIDPW(std::string userId, std::string userPw);
	bool CheckNick(std::string userNick);

	LoginServer* m_loginServer;
	
	std::mutex mtx; // mutex for session id token.
};

#endif // __LOGIN_MANAGER_HANDLER_H__
