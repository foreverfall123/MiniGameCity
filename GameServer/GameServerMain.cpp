#include "GameServer.h"
/*
int count = 0;
void* operator new(size_t size) {
	++count;
	printf("new - Size: %d, count: %d\n", size, count);

	return malloc(size);
}
void operator delete(void* ptr, size_t size) {
	--count;
	printf("delete - Size: %d, count: %d\n", size, count);
	free(ptr);
}
/*/
//*/
int main(int argc, char* argv[]) {
	GameServer* pServer = GameServer::CreateServer();

	if (0 < argc) 
		pServer->SetPortNum(atoi(argv[1]));
	else
		pServer->SetPortNum(Util::GetConfigToInt("GameServer.ini", "Network", "Port", 10000));
	pServer->SetServerName("GameServer");

	pServer->InitializeBaseServer();
	pServer->InitializeGameServer();

	pServer->RunServer();

	
	return 0;
}
