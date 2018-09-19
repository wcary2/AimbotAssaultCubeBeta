#include <Windows.h>
#include <string>
#include "Vec.h"


class Entity {
public:
		char pad1[0x4];			//0x0
		float x;				//0x4
		float y;				//0x8
		float z;				//0xC
		char pad2[0x30];		//0x10
		float yaw;				//0x40	
		float pitch;			//0x44
		float roll;				//0x48

		char pad3[0xAC];		//0x4C
		int health;				//0xF8
		int armor;				//0xFC

		char pad4[0x14];		//0x100
		int ammoPistol;			//0x114
		int ammoCarbine;		//0x118
		int ammoShotgun;		//0x11C
		int ammoMachinegun;		//0x120
		int ammoSniper;			//0x124
		int ammoAssaultrifle;	//0x128

		char pad5[0x10];		//0x12C
		int clipPistol;			//0x13C
		int clipCarbine;		//0x140
		int clipShotgun;		//0x144
		int clipMachinegun;		//0x148
		int clipSniper;			//0x14C
		int clipAssaultRifle;	//0x150

		char pad6[0x4];			//0x154
		int grenades;			//0x158

		char pad7[0x1D0];		//0x15C
		int team;

		double calcMagnitude(float x, float y, float z);
		

};

