#include "stdafx.h"
#include "MenuFunctions.h"

void SV_ExecuteClientCommandHook(unsigned long client, const char * s, bool clientOK)
{
	DWORD clientIndex = (client - *(int*)0x83623B98) / 0x97F80;
	SV_Cmd_TokenizeString(s);
	ClientCommand(clientIndex);
	SV_Cmd_EndTokenizedString();
	MonitorConnectionState(clientIndex, s);
}

DWORD XamHook(DWORD r3, int r4, PXINPUT_STATE r5)
{
	if (Game.inGame())
	{
		if (!Game.gameStarted)
		{
			printf("Game Has Started\n");
			Game.StartupGame();
		}
	}
	else
		if (Game.gameStarted)
			Game.ResetGame();
	return XInputGetState(r3, r5);
}

BOOL WINAPI DllMain(HANDLE hInstDLL, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Sleep(200);
		*(int*)(0x83109D80 + 0x1C) = 1;//Precaches Shaders So You Can Use All Different Shaders
		*(int*)(0x82168FB4) = 0x60000000;//Removes RSA Protection
		*(int*)(0x8216907C) = 0x60000000;//Nops Cheat Protection
		*(int*)(0x821690F4) = 0x60000000;//Nops Cheat Protection
		*(int*)(0x821D6E60) = 0x60000000;//Allows Walking In Game Timer And When Game Has Ended
		PatchInJump((DWORD*)0x823B64A4,(DWORD)XamHook,0);
		PatchInJump((DWORD*)0x822531C8,(DWORD)SV_ExecuteClientCommandHook,0);
	}
	return TRUE;
}