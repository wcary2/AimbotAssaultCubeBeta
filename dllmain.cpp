// dllmain.cpp : Defines the entry point for the DLL application.
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include "Player.h"
#include "DirectXMath.h"
#include <math.h>
int main();


void vecSubtract(Vec vector1, Vec vector2, Vec& output);
double calcMagnitude(Vec& vect);
float calcYaw(Vec vect);
float calcPitch(Vec vect, double mag);


double pi = 4 * atan(1);



using namespace std;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  dwReason,
                       LPVOID lpReserved
                     )
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int main() {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	
	cout << "Success: Aimbot Loaded" << endl;
	
	bool aimbot = false;
	
	DWORD* playerCountAddress = (DWORD*)0x50F500;
	DWORD* localPlayer = (DWORD*)0x509B74;
	DWORD* otherPlayers = (DWORD*)0x50F4F8;			//Address points to a pointer
	DWORD* otherPOffSet1 = (DWORD*)(*otherPlayers);	//Dereference the first pointer, leaving a second pointer
	//Assign the second pointer to a pointer I can read
	//Then in the for loop add an offset to that pointer
	//This way it points to each enemies address

	/*
		Pointer 1: P->[Pointer of Player Array]
		= Pointer 2: P->[Pointer of Player in Array]
		Add offsets to retrieve player info eg position, name, health,

		1 0x50F4F8 = p->[Player Array]				[ address of pointer to other pointers (pointer to other pointers)
			---*									[ Dereferencing pointer leads to other pointers
			2 (P->Player address) + 0x4				[ These pointers are the players each pointer the players is offset by 0x4
				3 Player address					[ dereferencing these pointers leads to the player address
				OFFSET F8 = health					[ adding offsets to these addresses show information about the player
			(P->Player Address) + 0x8
			(P->Player Address) + 0xC
			(P->Player Address) + 0x10
			...

		So DWORD* otherPlayers is a pointer to an address which is it'self a pointer to another address
		That address it points to can be accessed by dereferencing the pointer
		Meaning DWORD* otherPOffset1 is what the pointer otherPlayers points to. 
		This address is the beginning of an array of pointers and adding 0x4 is the address of another pointer
		This pointer points to the player addresses.
		So in the for loop DWORD* entRef is the pointer to a player's address.
		This pointer is dereferenced and that address is assigned to the Entity pointer "ent".
		Adding an offset and dereferencing it provides the address of player specific values like health.

		This could be done with a double pointer as in DWORD** because dereferencing this once would be the equivalent of entRef
		Dereferencing that would be the address of the player, and this could be assigned to an Entity pointer (Entity*)
	*/

	Entity* player = (Entity*)(*localPlayer); 

	int* playerCount = (int*)playerCountAddress; //This is kind of extraneous since you could just typcast the original address to the a int pointer 


	cout << "INFO: " << endl << "Player Count: " << *playerCount << endl;
	system("pause");
	cout << "My Health: " << player->health << endl;
	cout << "My Address: " << player << endl << endl;

	
	
	for (int i = 1; i < *playerCount; i++) {
		DWORD* entRef = (DWORD*)(*otherPlayers + 0x4 * i);
		Entity* ent = (Entity*)(*entRef);
		cout << hex << *otherPlayers + 0x4 * i <<  " : " << hex << entRef <<
			" : " << ent << dec << " : " << ent->health <<
			" : " << ent->team << endl;
		
	}

	while (true) {
		if (GetAsyncKeyState(0x5A)) {
			aimbot = (aimbot == false) ? true : false;
		}
		if (aimbot == true) {
			for (int i = 1; i < *playerCount; i++) {
				DWORD* entRef = (DWORD*)(*otherPlayers + 0x4 * i);
				Entity* ent = (Entity*)(*entRef);
				if (ent->team != player->team && ent->health >= 15) {
					Vec entVec(ent->x, ent->y, ent->z);
					Vec playerVec(player->x, player->y, player->z);
					Vec headVec(0, 0, 0);
					vecSubtract(playerVec, entVec, headVec); //subtracts eye position from enemy head
					cout << endl << "HeadVec: x: " << headVec.x << " y: " << headVec.y << " z: " << headVec.z << endl;
					
					double mag = calcMagnitude(headVec);
					cout << "mag: " << mag << endl;

					player->yaw = calcYaw(headVec) * 180/pi + 180;
					player->pitch = calcPitch(headVec, mag) * 180/pi;
					aimbot = false;
					break;
				}
			}
		}
	}


	return 0;
}

void vecSubtract(Vec vector1, Vec vector2, Vec& output) { //Subtracts vector 1 from 2 (2 - 1)
	float xCoord = vector2.x - vector1.x;
	float zCoord = vector2.z - vector1.z;
	float yCoord = vector2.y - vector1.y;

	output.x = xCoord;
	output.y = yCoord;
	output.z = zCoord;
	return;
}

double calcMagnitude(Vec& vect) {
	double magnitude = sqrt(vect.x*vect.x + vect.y * vect.y + vect.z * vect.z);
	return magnitude;
}

float calcYaw(Vec vect) {
	float temp = (vect.y < 0) ? vect.y * 1 : vect.y * -1;
	float tempx = (vect.x < 0) ? vect.x * 1 : vect.x * -1;
	float yaw = atan(tempx / (temp));
	cout << endl << "Yaw: " << yaw << endl;
	return yaw;
}

float calcPitch(Vec vect, double magnitude) {
	float pitch = asin(vect.z / magnitude);
	cout << "Pitch: " << pitch << endl;
	return pitch;
}