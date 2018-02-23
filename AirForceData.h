// AirForceData.h

/*
#define MAX_NAMELENGTH 64
#define MAX_ALTINDEX 16
#define MAX_SPDINDEX 16
 */

#include "AirForce.h"

aircraftModel aircraftModels[]
= {
	{	0,
		GERMAN,
		L"Ju87B",
		L"bitmapFiles\\Ju87.png",
		L"bitmapFiles\\Ju87_mask.png",
// FillOpacityMask with .bmp file does not work(transparent area becomes opac.
// with .png file, it works fine. I dont know why. 17-06-05
		{4.9, 9.9, 14.9, 19.9, 24.9, 26.3, -0.1},
		{
			//   0,    1,    2,    3,    4,    5,    6,    7,    8,   9,   10
			//   not Loaded
			{
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //4.9
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //9.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //14.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //19.9
			{ STAL, STAL, STAL, LEVL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}, //24.9
			{ STAL, STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}  //26.3
			},
			//   Loaded
			{
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //4.9
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //9.9
			{ STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA}, //14.9
			{ STAL, STAL, STAL, DIVE, DIVE, LEVL, DIVE, DIVE, NA,   NA}, //19.9
			{ STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}, //24.9
			{ STAL, STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}  //26.3
			}
		},
		//Maneuverability Requirements Table
		//						loaded	loaded	loaded	loaded	loaded
		//	turn,  	bank, 	slip, 	roll, 	loop, 	turn, 	bank, 	slip, 	roll, 	loop
		{
			//Maneuver Speed
			{
			{2,	3,	3, 	5,	4,	3,	4,	4,	-1,	-1}, //4.9	
			{2,	3,	3, 	6,	5,	3,	4,	4,	-1,	-1}, //9.9	
			{3,	3,	3, 	7,	6,	4,	4,	4,	-1,	-1}, //14.9
			{3,	4,	4, 	8,	7,	4,	5,	5,	-1,	-1}, //19.9
			{3,	4,	4, 	-1,	-1,	4,	5,	5,	-1,	-1}, //24.9
			{4,	5,	5, 	-1,	-1,	5,	6,	6,	-1,	-1}, //26.3
			},
			//Level Speed
			{
			{3,	4,	4, 	6,	5,	4,	5,	5,	-1,	-1}, //4.9	
			{3,	4,	4, 	7,	6,	4,	5,	5,	-1,	-1}, //9.9	
			{4,	4,	4, 	8,	7,	5,	5,	5,	-1,	-1}, //14.9
			{4,	5,	5, 	9,	8,	5,	6,	6,	-1,	-1}, //19.9
			{4,	5,	5, 	10,	8,	5,	6,	6,	-1,	-1}, //24.9
			{5,	6,	6, 	11,	10,	6,	7,	7,	-1,	-1}, //26.3
			},
			//Dive Speed
			{
			{5,	6,	6, 	8,	7,	6,	7,	7,	-1,	-1}, //4.9	
			{5,	6,	6, 	9,	8,	6,	7,	7,	-1,	-1}, //9.9	
			{6,	6,	6, 	10,	9,	7,	7,	7,	-1,	-1}, //14.9
			{6,	7,	7, 	11,	10,	7,	8,	8,	-1,	-1}, //19.9
			{6,	7,	7, 	12,	11,	7,	8,	8,	-1,	-1}, //24.9
			{7,	8,	8, 	13,	12,	8,	9,	9,	-1,	-1}  //26.3
			}
		},
		//Altitude Change Table Climb
		//	0,	-1,	-2,	-3,	-4,	-5, 	-6,	-7, 	-8,	-9
		{
			//Maneuver Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Level Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Dive Speed
			{
			{0.1,	0.3, 	0.4,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			}
		},
		//Altitude Change Table Dive
		//	0,	+1,	+2,	+3,	+4,	+5, 	+6,	+7, 	+8,	+9
		{
			{0.1,	0.3, 	0.5,	0.7,	0.90,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//24.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1} //26.3	
		},
		//Speed Change Table
		//	4.9,	9.9,	14.9,	19.9,	24.9, 	26.3
		{
			{1,	1,	1,	1,	1,	0}, //Power
			{5,	5,	4,	4,	3,	3}  //Break
		},
		//Damage Tolerance
		{
			6, //Wing
			6, //Fuselage
			{3, -1}, // Cockpit
			{3, -1, -1, -1}, // Engines
			{1, -1, -1, -1, -1, -1,  1,  1}, // Guns
			3 // Fule Tank
		},
		// Ammo Payload
		{
			-1,	// cannon
			10,	// machineGun
		},
		// gunType
		{
			gT_MG,	gT_Non,	gT_Non,	gT_Non,	gT_Non,	gT_Non,	gT_MG,	gT_MG
		},
		// gunPosition
		{
			gP_FF,	gP_Non,	gP_Non,	gP_Non,	gP_Non,	gP_Non, gP_FF,	gP_F
		},
		// gunRange
		{
			4,	-1,	-1,	-1,	-1,	-1,	4,	5
		},
		// gunPower
		{
			1, 	0,	0, 	0, 	0, 	0, 	1, 	0
		},
		// fgunPower
		{
			{0,	0, 	0,	0,	0,	0}, // Low 2, 4, 6, 8, 10, 12 o'clock
			{0,	1, 	1,	1,	0,	0}, // Med 2, 4, 6, 8, 10, 12 o'clock
			{0,	1, 	1,	1,	0,	0}, // Hih 2, 4, 6, 8, 10, 12 o'clock
			{1,	0, 	0,	0,	0,	0}  // above, below, na, na, na, na
		}
	},
	{	1,
		GERMAN,
		L"Ju87D",
		L"bitmapFiles\\Ju87.png",
		L"bitmapFiles\\Ju87_mask.png",
		{4.9, 9.9, 14.9, 19.9, 24.9, 26.3, -0.1},
		{
			//   0,    1,    2,    3,    4,    5,    6,    7,    8,   9,   10
			//   not Loaded
			{
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //4.9
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //9.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //14.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //19.9
			{ STAL, STAL, STAL, LEVL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}, //24.9
			{ STAL, STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}  //26.3
			},
			//   Loaded
			{
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //4.9
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //9.9
			{ STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA}, //14.9
			{ STAL, STAL, STAL, DIVE, DIVE, LEVL, DIVE, DIVE, NA,   NA}, //19.9
			{ STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}, //24.9
			{ STAL, STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}  //26.3
			}
		},
		//Maneuverability Requirements Table
		//						loaded	loaded	loaded	loaded	loaded
		//	turn,  	bank, 	slip, 	roll, 	loop, 	turn, 	bank, 	slip, 	roll, 	loop
		{
			//Maneuver Speed
			{
			{2,	3,	3, 	5,	4,	3,	4,	4,	-1,	-1}, //4.9	
			{2,	3,	3, 	6,	5,	3,	4,	4,	-1,	-1}, //9.9	
			{3,	3,	3, 	7,	6,	4,	4,	4,	-1,	-1}, //14.9
			{3,	4,	4, 	8,	7,	4,	5,	5,	-1,	-1}, //19.9
			{3,	4,	4, 	-1,	-1,	4,	5,	5,	-1,	-1}, //24.9
			{4,	5,	5, 	-1,	-1,	5,	6,	6,	-1,	-1}, //26.3
			},
			//Level Speed
			{
			{3,	4,	4, 	6,	5,	4,	5,	5,	-1,	-1}, //4.9	
			{3,	4,	4, 	7,	6,	4,	5,	5,	-1,	-1}, //9.9	
			{4,	4,	4, 	8,	7,	5,	5,	5,	-1,	-1}, //14.9
			{4,	5,	5, 	9,	8,	5,	6,	6,	-1,	-1}, //19.9
			{4,	5,	5, 	10,	8,	5,	6,	6,	-1,	-1}, //24.9
			{5,	6,	6, 	11,	10,	6,	7,	7,	-1,	-1}, //26.3
			},
			//Dive Speed
			{
			{5,	6,	6, 	8,	7,	6,	7,	7,	-1,	-1}, //4.9	
			{5,	6,	6, 	9,	8,	6,	7,	7,	-1,	-1}, //9.9	
			{6,	6,	6, 	10,	9,	7,	7,	7,	-1,	-1}, //14.9
			{6,	7,	7, 	11,	10,	7,	8,	8,	-1,	-1}, //19.9
			{6,	7,	7, 	12,	10,	7,	8,	8,	-1,	-1}, //24.9
			{7,	8,	8, 	13,	12,	8,	9,	9,	-1,	-1}  //26.3
			}
		},
		//Altitude Change Table Climb
		//	0,	-1,	-2,	-3,	-4,	-5, 	-6,	-7, 	-8,	-9
		{
			//Maneuver Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Level Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Dive Speed
			{
			{0.1,	0.3, 	0.4,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	0.1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			}
		},
		//Altitude Change Table Dive
		//	0,	+1,	+2,	+3,	+4,	+5, 	+6,	+7, 	+8,	+9
		{
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//24.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1} //26.3	
		},
		//Speed Change Table
		//	4.9,	9.9,	14.9,	19.9,	24.9, 	26.3
		{
			{1,	1,	1,	1,	1,	0}, //Power
			{5,	5,	4,	4,	3,	3}  //Break
		},
		//Damage Tolerance
		{
			6, //Wing
			7, //Fuselage
			{3, -1}, // Cockpit
			{3, -1, -1, -1}, // Engines
			{1, -1, -1, -1, -1, -1,  1,  1}, // Guns
			3 // Fule Tank
		},
		// Ammo Payload
		{
			-1,
			10,
		},
		// gunType
		{
			gT_MG,	gT_Non,	gT_Non,	gT_Non,	gT_Non,	gT_Non,	gT_MG,	gT_MG
		},
		// gunPosition
		{
			gP_FF,	gP_Non,	gP_Non,	gP_Non,	gP_Non,	gP_Non, gP_FF,	gP_F
		},
		// gunRange
		{
			4,	-1,	-1,	-1,	-1,	-1,	4,	5
		},
		// gunPower
		{
			1, 	0,	0, 	0, 	0, 	0, 	1, 	0
		},
		// fgunPower
		{
			{0,	0, 	0,	0,	0,	0}, // Low 2, 4, 6, 8, 10, 12 o'clock
			{0,	2, 	2,	2,	0,	0}, // Med 2, 4, 6, 8, 10, 12 o'clock
			{0,	2, 	2,	2,	0,	0}, // Hih 2, 4, 6, 8, 10, 12 o'clock
			{2,	0, 	0,	0,	0,	0}  // above, below, na, na, na, na
		}
	},
	{	2,
		GERMAN,
		L"Ju87D-5",
		L"bitmapFiles\\Ju87.png",
		L"bitmapFiles\\Ju87_mask.png",
		{4.9, 9.9, 14.9, 19.9, 24.9, 26.3, -0.1},
		{
			//   0,    1,    2,    3,    4,    5,    6,    7,    8,   9,   10
			//   not Loaded
			{
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //4.9
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //9.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //14.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //19.9
			{ STAL, STAL, STAL, LEVL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}, //24.9
			{ STAL, STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}  //26.3
			},
			//   Loaded
			{
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //4.9
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //9.9
			{ STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA}, //14.9
			{ STAL, STAL, STAL, DIVE, DIVE, LEVL, DIVE, DIVE, NA,   NA}, //19.9
			{ STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}, //24.9
			{ STAL, STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}  //26.3
			}
		},
		//Maneuverability Requirements Table
		//						loaded	loaded	loaded	loaded	loaded
		//	turn,  	bank, 	slip, 	roll, 	loop, 	turn, 	bank, 	slip, 	roll, 	loop
		{
			//Maneuver Speed
			{
			{2,	3,	3, 	5,	4,	3,	4,	4,	-1,	-1}, //4.9	
			{2,	3,	3, 	6,	5,	3,	4,	4,	-1,	-1}, //9.9	
			{3,	3,	3, 	7,	6,	4,	4,	4,	-1,	-1}, //14.9
			{3,	4,	4, 	8,	7,	4,	5,	5,	-1,	-1}, //19.9
			{3,	4,	4, 	-1,	-1,	4,	5,	5,	-1,	-1}, //24.9
			{4,	5,	5, 	-1,	-1,	5,	6,	6,	-1,	-1}, //26.3
			},
			//Level Speed
			{
			{3,	4,	4, 	6,	5,	4,	5,	5,	-1,	-1}, //4.9	
			{3,	4,	4, 	7,	6,	4,	5,	5,	-1,	-1}, //9.9	
			{4,	4,	4, 	8,	7,	5,	5,	5,	-1,	-1}, //14.9
			{4,	5,	5, 	9,	8,	5,	6,	6,	-1,	-1}, //19.9
			{4,	5,	5, 	10,	8,	5,	6,	6,	-1,	-1}, //24.9
			{5,	6,	6, 	11,	10,	6,	7,	7,	-1,	-1}, //26.3
			},
			//Dive Speed
			{
			{5,	6,	6, 	8,	7,	6,	7,	7,	-1,	-1}, //4.9	
			{5,	6,	6, 	9,	8,	6,	7,	7,	-1,	-1}, //9.9	
			{6,	6,	6, 	10,	9,	7,	7,	7,	-1,	-1}, //14.9
			{6,	7,	7, 	11,	10,	7,	8,	8,	-1,	-1}, //19.9
			{6,	7,	7, 	12,	10,	7,	8,	8,	-1,	-1}, //24.9
			{7,	8,	8, 	13,	12,	8,	9,	9,	-1,	-1}  //26.3
			}
		},
		//Altitude Change Table Climb
		//	0,	-1,	-2,	-3,	-4,	-5, 	-6,	-7, 	-8,	-9
		{
			//Maneuver Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Level Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Dive Speed
			{
			{0.1,	0.3, 	0.4,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	0.1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			}
		},
		//Altitude Change Table Dive
		//	0,	+1,	+2,	+3,	+4,	+5, 	+6,	+7, 	+8,	+9
		{
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//24.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1} //26.3	
		},
		//Speed Change Table
		//	4.9,	9.9,	14.9,	19.9,	24.9, 	26.3
		{
			{1,	1,	1,	1,	1,	0}, //Power
			{5,	5,	4,	4,	3,	3}  //Break
		},
		//Damage Tolerance
		{
			6, //Wing
			7, //Fuselage
			{3, -1}, // Cockpit
			{3, -1, -1, -1}, // Engines
			{1, -1, -1, -1, -1, -1,  1,  1}, // Guns
			3 // Fule Tank
		},
		// Ammo Payload
		{
			-1, // Cannons
			 5, // Machine Guns
		},
		// gunType
		{
			gT_MG,	gT_MG,	gT_Non,	gT_Non,	gT_Non,	gT_MG,	gT_MG,	gT_MG
		},
		// gunPosition
		{
			gP_FF,	gP_FF,	gP_Non,	gP_Non,	gP_Non,	gP_FF, gP_FF,	gP_F
		},
		// gunRange
		{
			4,	4,	-1,	-1,	-1,	4,	4,	5
		},
		// gunPower
		{
			6, 	1,	0, 	0, 	0, 	1, 	6, 	0
		},
		// fgunPower
		{
			{0,	0, 	0,	0,	0,	0}, // Low 2, 4, 6, 8, 10, 12 o'clock
			{0,	2, 	2,	2,	0,	0}, // Med 2, 4, 6, 8, 10, 12 o'clock
			{0,	2, 	2,	2,	0,	0}, // Hih 2, 4, 6, 8, 10, 12 o'clock
			{1,	0, 	0,	0,	0,	0}  // above, below, na, na, na, na
		}
	},
	{	3,
		BRITAIN,
		L"Spitfire I",
		L"bitmapFiles\\spitfireMKI.png",
		L"bitmapFiles\\Ju87_mask.png",
		{4.9, 9.9, 14.9, 19.9, 24.9, 29.9, 31.9, -0.1},
		{
			//   0,    1,    2,    3,    4,    5,    6,    7,    8,   9,   10
			//   not Loaded
			{
			{ STAL, STAL, STAL, MANV, MANV, MANV, LEVL, DIVE, DIVE, NA}, //4.9
			{ STAL, STAL, STAL, MANV, MANV, MANV, LEVL, DIVE, DIVE, NA}, //9.9
			{ STAL, STAL, STAL, MANV, MANV, MANV, LEVL, DIVE, DIVE, DIVE, NA}, //14.9
			{ STAL, STAL, STAL, MANV, MANV, LEVL, LEVL, LEVL, DIVE, DIVE, NA}, //19.9
			{ STAL, STAL, STAL, STAL, MANV, LEVL, LEVL, LEVL, DIVE, DIVE, NA}, //24.9
			{ STAL, STAL, STAL, STAL, STAL, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //29.9
			{ STAL, STAL, STAL, STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA}  //31.9
			},
			//   Loaded
			{
			{ STAL, STAL, STAL, MANV, MANV, MANV, LEVL, DIVE, DIVE, NA}, //4.9
			{ STAL, STAL, STAL, MANV, MANV, MANV, LEVL, DIVE, DIVE, NA}, //9.9
			{ STAL, STAL, STAL, MANV, MANV, MANV, LEVL, DIVE, DIVE, DIVE, NA}, //14.9
			{ STAL, STAL, STAL, MANV, MANV, LEVL, LEVL, LEVL, DIVE, DIVE, NA}, //19.9
			{ STAL, STAL, STAL, STAL, MANV, LEVL, LEVL, LEVL, DIVE, DIVE, NA}, //24.9
			{ STAL, STAL, STAL, STAL, STAL, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //29.9
			{ STAL, STAL, STAL, STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA}  //31.9
			}
		},
		//Maneuverability Requirements Table
		//						loaded	loaded	loaded	loaded	loaded
		//	turn,  	bank, 	slip, 	roll, 	loop, 	turn, 	bank, 	slip, 	roll, 	loop
		{
			//Maneuver Speed
			{
			{1,	3,	1, 	5,	4,	3,	4,	4,	-1,	-1}, //4.9	
			{2,	3,	2, 	6,	5,	3,	4,	4,	-1,	-1}, //9.9	
			{2,	3,	2, 	7,	5,	4,	4,	4,	-1,	-1}, //14.9
			{2,	4,	2, 	7,	6,	4,	5,	5,	-1,	-1}, //19.9
			{2,	5,	2, 	8,	7,	4,	5,	5,	-1,	-1}, //24.9
			{3,	5,	3, 	-1,	8,	4,	5,	5,	-1,	-1}, //29.9
			{3,	6,	3, 	-1,	-1,	5,	6,	6,	-1,	-1}, //31.9
			},
			//Level Speed
			{
			{2,	4,	2, 	6,	5,	4,	5,	5,	-1,	-1}, //4.9	
			{3,	4,	3, 	7,	6,	4,	5,	5,	-1,	-1}, //9.9	
			{3,	4,	3, 	8,	6,	5,	5,	5,	-1,	-1}, //14.9
			{3,	5,	3, 	8,	7,	5,	6,	6,	-1,	-1}, //19.9
			{3,	6,	3, 	9,	8,	5,	6,	6,	-1,	-1}, //24.9
			{4,	6,	4, 	10,	9,	5,	6,	6,	-1,	-1}, //29.9
			{4,	7,	4, 	11,	10,	6,	7,	7,	-1,	-1}, //31.9
			},
			//Dive Speed
			{
			{3,	5,	3, 	7,	6,	6,	7,	7,	-1,	-1}, //4.9	
			{4,	5,	4, 	8,	7,	6,	7,	7,	-1,	-1}, //9.9	
			{4,	5,	4, 	9,	8,	7,	7,	7,	-1,	-1}, //14.9
			{4,	6,	4, 	9,	8,	7,	8,	8,	-1,	-1}, //19.9
			{4,	7,	4, 	10,	9,	7,	8,	8,	-1,	-1}, //24.9
			{5,	7,	5, 	11,	10,	7,	8,	8,	-1,	-1}, //29.9
			{5,	8,	5, 	12,	11,	8,	9,	9,	-1,	-1}  //31.9
			}
		},
		//Altitude Change Table Climb
		//	0,	-1,	-2,	-3,	-4,	-5, 	-6,	-7, 	-8,	-9
		{
			//Maneuver Speed
			{
			{0.2,	0.4, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.2,	0.4, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.2,	0.4, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.2,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.2,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//29.9	
			{0.05,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//31.9
			},
			//Level Speed
			{
			{0.2,	0.4, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.2,	0.4, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.2,	0.4, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.2,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.2,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//29.9	
			{0.05,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//31.9
			},
			//Dive Speed
			{
			{0.2,	0.5, 	0.6,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.2,	0.5, 	0.6,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.2,	0.5, 	0.6,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.2,	0.5, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.2,	0.4, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.2,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//29.9	
			{0.2,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//31.9	
			}
		},
		//Altitude Change Table Dive
		//	0,	+1,	+2,	+3,	+4,	+5, 	+6,	+7, 	+8,	+9
		{
			{0.3,	0.7, 	0.9,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.3,	0.7, 	1.0,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.3,	0.7, 	1.0,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.3,	0.7, 	1.0,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.3,	0.7, 	1.1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.3,	0.7, 	1.1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//29.9	
			{0.3,	0.7, 	1.1,	-1,	-1,	-1,	-1,	-1,	-1,	-1} //31.9	
		},
		//Speed Change Table
		//	4.9,	9.9,	14.9,	19.9,	24.9, 	29.9	31.9
		{
			{1,	1,	1,	1,	1,	1,	0}, //Power
			{2,	2,	2,	2,	1,	1,	1}  //Break
		},
		//Damage Tolerance
		{
			5, //Wing
			5, //Fuselage
			{3, -1}, // Cockpit
			{3, -1, -1, -1}, // Engines
			{1,  1, -1, -1, -1,  1,  1, -1}, // Guns
			3 // Fule Tank
		},
		// Ammo Payload
		{
			-1,
			4,
		},
		// gunType
		{
			gT_MG,	gT_MG,	gT_Non,	gT_Non,	gT_Non,	gT_MG,	gT_MG,	gT_Non
		},
		// gunPosition
		{
			gP_FF,	gP_FF,	gP_Non,	gP_Non,	gP_Non,	gP_FF, gP_FF,	gP_Non
		},
		// gunRange
		{
			4,	4,	-1,	-1,	-1,	4,	4,	-1
		},
		// gunPower
		{
			2, 	2,	0, 	0, 	0, 	2, 	2, 	0
		},
		// fgunPower
		{
			{0,	0, 	0,	0,	0,	0}, // Low 2, 4, 6, 8, 10, 12 o'clock
			{0,	0, 	0,	0,	0,	0}, // Med 2, 4, 6, 8, 10, 12 o'clock
			{0,	0, 	0,	0,	0,	0}, // Hih 2, 4, 6, 8, 10, 12 o'clock
			{0,	0, 	0,	0,	0,	0}  // above, below, na, na, na, na
		}
	},
	{	4,
		BRITAIN,
		L"Tempest V",
		L"bitmapFiles\\Ju87.png",
		L"bitmapFiles\\Ju87_mask.png",
		{4.9, 9.9, 14.9, 19.9, 24.9, 26.3, -0.1},
		{
			//   0,    1,    2,    3,    4,    5,    6,    7,    8,   9,   10
			//   not Loaded
			{
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //4.9
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //9.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //14.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //19.9
			{ STAL, STAL, STAL, LEVL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}, //24.9
			{ STAL, STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}  //26.3
			},
			//   Loaded
			{
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //4.9
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //9.9
			{ STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA}, //14.9
			{ STAL, STAL, STAL, DIVE, DIVE, LEVL, DIVE, DIVE, NA,   NA}, //19.9
			{ STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}, //24.9
			{ STAL, STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}  //26.3
			}
		},
		//Maneuverability Requirements Table
		//						loaded	loaded	loaded	loaded	loaded
		//	turn,  	bank, 	slip, 	roll, 	loop, 	turn, 	bank, 	slip, 	roll, 	loop
		{
			//Maneuver Speed
			{
			{2,	3,	3, 	5,	4,	3,	4,	4,	-1,	-1}, //4.9	
			{2,	3,	3, 	6,	5,	3,	4,	4,	-1,	-1}, //9.9	
			{3,	3,	3, 	7,	6,	4,	4,	4,	-1,	-1}, //14.9
			{3,	4,	4, 	8,	7,	4,	5,	5,	-1,	-1}, //19.9
			{3,	4,	4, 	-1,	-1,	4,	5,	5,	-1,	-1}, //24.9
			{4,	5,	5, 	-1,	-1,	5,	6,	6,	-1,	-1}, //26.3
			},
			//Level Speed
			{
			{3,	4,	4, 	6,	5,	4,	5,	5,	-1,	-1}, //4.9	
			{3,	4,	4, 	7,	6,	4,	5,	5,	-1,	-1}, //9.9	
			{4,	4,	4, 	8,	7,	5,	5,	5,	-1,	-1}, //14.9
			{4,	5,	5, 	9,	8,	5,	6,	6,	-1,	-1}, //19.9
			{4,	5,	5, 	10,	8,	5,	6,	6,	-1,	-1}, //24.9
			{5,	6,	6, 	11,	10,	6,	7,	7,	-1,	-1}, //26.3
			},
			//Dive Speed
			{
			{5,	6,	6, 	8,	7,	6,	7,	7,	-1,	-1}, //4.9	
			{5,	6,	6, 	9,	8,	6,	7,	7,	-1,	-1}, //9.9	
			{6,	6,	6, 	10,	9,	7,	7,	7,	-1,	-1}, //14.9
			{6,	7,	7, 	11,	10,	7,	8,	8,	-1,	-1}, //19.9
			{6,	7,	7, 	12,	10,	7,	8,	8,	-1,	-1}, //24.9
			{7,	8,	8, 	13,	12,	8,	9,	9,	-1,	-1}  //26.3
			}
		},
		//Altitude Change Table Climb
		//	0,	-1,	-2,	-3,	-4,	-5, 	-6,	-7, 	-8,	-9
		{
			//Maneuver Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Level Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Dive Speed
			{
			{0.1,	0.3, 	0.4,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	0.1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			}
		},
		//Altitude Change Table Dive
		//	0,	+1,	+2,	+3,	+4,	+5, 	+6,	+7, 	+8,	+9
		{
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//24.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1} //26.3	
		},
		//Speed Change Table
		//	4.9,	9.9,	14.9,	19.9,	24.9, 	26.3
		{
			{1,	1,	1,	1,	1,	0}, //Power
			{5,	5,	4,	4,	3,	3}  //Break
		},
		//Damage Tolerance
		{
			6, //Wing
			6, //Fuselage
			{3, -1}, // Cockpit
			{3, -1, -1, -1}, // Engines
			{1, -1, -1, -1, -1, -1, -1,  1}, // Guns
			3 // Fule Tank
		},
		// Ammo Payload
		{
			-1,
			10,
		},
		// gunType
		{
			gT_MG,	gT_Non,	gT_Non,	gT_Non,	gT_Non,	gT_Non,	gT_MG,	gT_MG
		},
		// gunPosition
		{
			gP_FF,	gP_Non,	gP_Non,	gP_Non,	gP_Non,	gP_Non, gP_FF,	gP_F
		},
		// gunRange
		{
			4,	-1,	-1,	-1,	-1,	-1,	4,	5
		},
		// gunPower
		{
			1, 	0,	0, 	0, 	0, 	0, 	1, 	0
		},
		// fgunPower
		{
			{0,	0, 	0,	0,	0,	0}, // Low 2, 4, 6, 8, 10, 12 o'clock
			{0,	1, 	1,	1,	0,	0}, // Med 2, 4, 6, 8, 10, 12 o'clock
			{0,	1, 	1,	1,	0,	0}, // Hih 2, 4, 6, 8, 10, 12 o'clock
			{1,	0, 	0,	0,	0,	0}  // above, below, na, na, na, na
		}
	},
	{	5,
		USA,
		L"P-47D",
		L"bitmapFiles\\Ju87.png",
		L"bitmapFiles\\Ju87_mask.png",
		{4.9, 9.9, 14.9, 19.9, 24.9, 26.3, -0.1},
		{
			//   0,    1,    2,    3,    4,    5,    6,    7,    8,   9,   10
			//   not Loaded
			{
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //4.9
			{ STAL, STAL, MANV, MANV, LEVL, DIVE, DIVE, DIVE, DIVE, NA}, //9.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //14.9
			{ STAL, STAL, STAL, MANV, LEVL, LEVL, DIVE, DIVE, DIVE, NA}, //19.9
			{ STAL, STAL, STAL, LEVL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}, //24.9
			{ STAL, STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, DIVE, DIVE, NA}  //26.3
			},
			//   Loaded
			{
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //4.9
			{ STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA,   NA}, //9.9
			{ STAL, STAL, STAL, LEVL, DIVE, DIVE, DIVE, NA,   NA,   NA}, //14.9
			{ STAL, STAL, STAL, DIVE, DIVE, LEVL, DIVE, DIVE, NA,   NA}, //19.9
			{ STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}, //24.9
			{ STAL, STAL, STAL, STAL, DIVE, DIVE, DIVE, DIVE, NA,   NA,   NA}  //26.3
			}
		},
		//Maneuverability Requirements Table
		//						loaded	loaded	loaded	loaded	loaded
		//	turn,  	bank, 	slip, 	roll, 	loop, 	turn, 	bank, 	slip, 	roll, 	loop
		{
			//Maneuver Speed
			{
			{2,	3,	3, 	5,	4,	3,	4,	4,	-1,	-1}, //4.9	
			{2,	3,	3, 	6,	5,	3,	4,	4,	-1,	-1}, //9.9	
			{3,	3,	3, 	7,	6,	4,	4,	4,	-1,	-1}, //14.9
			{3,	4,	4, 	8,	7,	4,	5,	5,	-1,	-1}, //19.9
			{3,	4,	4, 	-1,	-1,	4,	5,	5,	-1,	-1}, //24.9
			{4,	5,	5, 	-1,	-1,	5,	6,	6,	-1,	-1}, //26.3
			},
			//Level Speed
			{
			{3,	4,	4, 	6,	5,	4,	5,	5,	-1,	-1}, //4.9	
			{3,	4,	4, 	7,	6,	4,	5,	5,	-1,	-1}, //9.9	
			{4,	4,	4, 	8,	7,	5,	5,	5,	-1,	-1}, //14.9
			{4,	5,	5, 	9,	8,	5,	6,	6,	-1,	-1}, //19.9
			{4,	5,	5, 	10,	8,	5,	6,	6,	-1,	-1}, //24.9
			{5,	6,	6, 	11,	10,	6,	7,	7,	-1,	-1}, //26.3
			},
			//Dive Speed
			{
			{5,	6,	6, 	8,	7,	6,	7,	7,	-1,	-1}, //4.9	
			{5,	6,	6, 	9,	8,	6,	7,	7,	-1,	-1}, //9.9	
			{6,	6,	6, 	10,	9,	7,	7,	7,	-1,	-1}, //14.9
			{6,	7,	7, 	11,	10,	7,	8,	8,	-1,	-1}, //19.9
			{6,	7,	7, 	12,	10,	7,	8,	8,	-1,	-1}, //24.9
			{7,	8,	8, 	13,	12,	8,	9,	9,	-1,	-1}  //26.3
			}
		},
		//Altitude Change Table Climb
		//	0,	-1,	-2,	-3,	-4,	-5, 	-6,	-7, 	-8,	-9
		{
			//Maneuver Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Level Speed
			{
			{0.1,	0.3, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	-1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
			//Dive Speed
			{
			{0.1,	0.3, 	0.4,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.2, 	0.3,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.2, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//24.9	
			{0.025,	0.1, 	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1},//26.3	
			},
		},
		//Altitude Change Table Dive
		//	0,	+1,	+2,	+3,	+4,	+5, 	+6,	+7, 	+8,	+9
		{
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//4.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	-1,	-1,	-1,	-1,	-1},//9.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//14.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//19.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1},//24.9	
			{0.1,	0.3, 	0.5,	0.7,	0.9,	1.0,	-1,	-1,	-1,	-1} //26.3	
		},
		//Speed Change Table
		//	4.9,	9.9,	14.9,	19.9,	24.9, 	26.3
		{
			{1,	1,	1,	1,	1,	0}, //Power
			{5,	5,	4,	4,	3,	3}  //Break
		},
		//Damage Tolerance
		{
			6, //Wing
			6, //Fuselage
			{3, -1}, // Cockpit
			{3, -1, -1, -1}, // Engines
			{1, -1, -1, -1, -1, -1, -1,  1}, // Guns
			3 // Fule Tank
		},
		// Ammo Payload
		{
			-1,
			10,
		},
		// gunType
		{
			gT_MG,	gT_Non,	gT_Non,	gT_Non,	gT_Non,	gT_Non,	gT_MG,	gT_MG
		},
		// gunPosition
		{
			gP_FF,	gP_Non,	gP_Non,	gP_Non,	gP_Non,	gP_Non, gP_FF,	gP_F
		},
		// gunRange
		{
			4,	-1,	-1,	-1,	-1,	-1,	4,	5
		},
		// gunPower
		{
			1, 	0,	0, 	0, 	0, 	0, 	1, 	0
		},
		// fgunPower
		{
			{0,	0, 	0,	0,	0,	0}, // Low 2, 4, 6, 8, 10, 12 o'clock
			{0,	1, 	1,	1,	0,	0}, // Med 2, 4, 6, 8, 10, 12 o'clock
			{0,	1, 	1,	1,	0,	0}, // Hih 2, 4, 6, 8, 10, 12 o'clock
			{1,	0, 	0,	0,	0,	0}  // above, below, na, na, na, na
		}
	}
};


struct pilotModel
{
	int	id;
	wchar_t name[MAX_NAMELENGTH];
	int	firing;
	int	maneuverability;
	int	experience;
	int	training;
	int	reflex;
	int 	vision;
};

pilotModel pilotModels[]
	= {
		{ 0,
		  L"Novice", 
		  -2, 1, 0, 0, 0, 0
		},
		{ 1,
		  L"Average", 
		  0, 0, 0, 0, 0, 0
		},
		{ 2,
		  L"Ace", 
		  1, 0, 0, 0, 0, 0
		},
		{ 3,
		  L"Random", 
		  0, 0, 0, 0, 0, 0
		},
	  };

