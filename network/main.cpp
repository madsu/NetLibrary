#include "net/EventLoop.h"
#include "net/TCPServer.h"
#include "EchoServer.h"
#include <conio.h>

bool running = true;
void thread_task()
{
	EventLoop loop;
	EchoServer server(&loop, 7002);
	server.Start();

	while (running)
	{
		loop.Loop();
	}
}

int main()
{
	std::thread t(thread_task);
	
	char szCMD[256] = {};
	while (true)
	{
		if (_kbhit())
		{
			scanf_s("%s", szCMD, 256);
			if (!_stricmp("stop", szCMD)) 
			{
				running = false;
				t.join();
				break;
			}
		}

		Sleep(200);
	}

	return 0;
}