// AirForce.h
//   started coding on April 21 2017
//
//
#pragma once

#include "stdafx.h"
#include <string>
#include <list>
#include <memory>
#include <strsafe.h>
#include <dwrite.h>

#include <Wincodec.h>

#include <CommCtrl.h>

#include "BaseWindow.h"
//#include "AirForceData.h"

#define MAX_AIRCRAFTMODELNUMBER 6
#define MAX_INT  2147483647 
#define MIN_INT  -2147483648  

#define MAX_NAMELENGTH 64
#define MAX_ALTINDEX 16
#define MAX_SPDINDEX 16

using namespace std;

LPCTSTR liststrNationality[]
	= {	L"German", L"Britain", L"U.S.A."};

enum nationality {
	GERMAN = 0, 
	BRITAIN = 1, 
	USA = 2
};

enum spdIncTblEntry {
	STAL = 0, 
	MANV = 1, 
	LEVL = 2, 
	DIVE = 3, 
	NA = -1
};

enum gunType {
	gT_Non 	= 0,
	gT_MG 	= 1,
	gT_CN 	= 2
};

enum gunPosition {
	gP_Non = 0,
	gP_FF = 1,
	gP_F  = 2,
	gP_FH = 3, 
	gP_FL = 4
};

struct aircraftCompo {
	int	wing;
	int	fuselage;
	int 	cockpit[2];
	int	engine[4];
	int	gun[8];
	int	fueltank;
};

struct weapon {
	int	cannon;
	int	mg;
};

struct aircraftModel
{
	int		id;
	nationality	nation;
	wchar_t		name[MAX_NAMELENGTH];
	wchar_t		bmpFileName[MAX_NAMELENGTH];
	wchar_t		bmpMaskFileName[MAX_NAMELENGTH];
	float		altIndex[MAX_ALTINDEX];
	spdIncTblEntry	spdIncTbl[2][MAX_ALTINDEX][MAX_SPDINDEX]; //notLoaded, loaded
	int		manuvReqTbl[3][MAX_ALTINDEX][10];
	float		altChgTblClimb[3][MAX_ALTINDEX][10];
	float		altChgTblDive[MAX_ALTINDEX][10];
	int		speedChgTbl[2][MAX_ALTINDEX];
	aircraftCompo	damageTolerance;
	weapon		ammoPayload;
	int		gunType[8]; //leftx3 centerx1 rightx3 Fx1
	int		gunPosition[8];
	int		gunRange[8];
	int		gunPower[8];
	int		fgunPower[4][6]; // Low 2, 4, 6, 8, 10, 12 o'clock
	                                // Med 2, 4, 6, 8, 10, 12 o'clock
					// Hih 2, 4, 6, 8, 10, 12 o'clock
					// above, below, na, na, na, na
};


enum GameMode {
	GM_UNDEF	= -1,
	GM_NOP		= 0,
	GM_FIRE		= 1, 
	GM_BOMB		= 2, 
	GM_BAILOUT	= 3, 
	GM_SPOT		= 4,
	GM_DET_ADV	= 5, 
	GM_PLOT_MOVE	= 6, 
	GM_DET_STAT	= 7,
	GM_FLAKS	= 8,
	GM_MOVE		= 9,
	GM_RECORD	=10,
	GM_DISPATCH	= 101 
};

LPCTSTR listStrGamePhase[]
= {
	L"Deploy",	// 0
	L"Fire",	// 1
	L"Bomb",	// 2
	L"BailOut",	// 3
	L"Spot",	// 4 
	L"Det.Adv.",	// 5
	L"Plot",	// 6
	L"Det.Stat",	// 7
	L"Flaks",	// 8
	L"Move",	// 9
	L"Record"	// 10
};
// declaration above cannot be within class statement.
// I dont know why.
//


enum regStat {
	UNDEF = -1,
	NOTREG = 0, 
	TMPREG = 1,
	OFCREG = 2
};

enum AircraftStat {
	UNDEFINED	= 0,
	ON_TRAY		= 1,
	ON_MAP		= 2,
	DISPATCHED 	= 3,
	DEPLOYED 	= 4
};

enum CommandToX {
	TAIL			= -1, // last array element of returned form. 
	DISPATCH		= 1,
	GET_DISPATCHED 		= 2,
	REPAINT			= 3,
	GET_AC			= 4,
	GET_LIMIT		= 5,
	GET_HIT			= 6,
	SET_ACSTAT		= 7,
	SET_AC_SELECTED 	= 8,
	GET_MANUVABLE		= 9,
	APPEND_MANUV		= 10,
	DELETE_MANUV		= 11,
	SET_PLAYER_SELECTED	= 12,
	GET_FIRE_RANGE		= 13
};

#define SELECTED_PLAYER -1
#define ALL_PLAYERS	-2

#define SELECTED_AC	-1
#define ALL_AC		-2

#define HEADING_N	0.0f
#define HEADING_NE	60.0f
#define HEADING_SE	120.0f
#define HEADING_S	180.0f
#define HEADING_SW	240.0f
#define HEADING_NW	300.0f

#define BANK_IR		0
#define BANK_BR		1
#define BANK_LV		2
#define BANK_BL		3
#define BANK_IL		4
#define BANK_IV		5

#define NOSE_UP		0
#define NOSE_LV		1
#define NOSE_DN		2

#define	MANUV_TL	-1	//Turn Left
#define	MANUV_TR	-2  	//Turn Right
#define	MANUV_BL	-3	//Bank Left
#define	MANUV_BR	-4	//Bank Right
#define	MANUV_SL	-5	//Slip Left
#define	MANUV_SR	-6	//Slit Right
#define	MANUV_RL	-7	//Roll Left
#define	MANUV_RR	-8	//Roll Right
#define	MANUV_LC	-9	//Loop Climb
#define	MANUV_LD	-10	//Loop Dive
#define	MANUV_DP	-11	//Deploy(means infinite straight move)
//--------------------------------------------------------------------
#define	MANUV_PW	-12	//Power
#define	MANUV_BK	-13	//Break
#define	MANUV_DB	-14	//Drip Bomb
#define	MANUV_FR	-15	//Fire Rocket
#define	MANUV_FG	-16	//Fire Gun
#define	MANUV_EN	-19	//end of manuv array
// Natural Number less than 20
// 	is # of hexes for straight maneuver
// Natural Number more than or equal to 20 
// 	is climb in feet
// Negative Integer less than or equal to -20
//  	is dive in feet

struct maneuverable {
	int	turnLeft;
	int	turnRight;
	int	bankLeft;
	int	bankRight;
	int	slipLeft;
	int	slipRight;
	int	rollLeft;
	int	rollRight;
	int	loopClimb;
	int	loopDive;
	float	maxClimb; // feet (positive number)
	float	maxDive;  // feet (positive number)
	int	maxPower;
	int	maxBreak;
	bool	dropBomb;
	bool	fireRocket;
	bool	fire;
	int	remainingMP;
	int	maxDiveSpeed; // -1 means "prohibit Dive Speed"
	bool	mustJettson;
	bool	destroyed;
};

struct firingArcRange {
	int	rangeFFmg;
	int	rangeFFcannon;
	bool	rangeSameHexFF;
	int	rangeFH;
	bool	rangeSameHexFH;
	int	rangeFL;
	bool	rangeSameHexFL;
	int	rangeF[8]; 	// 2, 4, 6, 8, 10, 12Å@o'clock, above, below
};

struct gunPower {
	int	gunPowerFFmg;
	int	gunPowerFFcannon;
	int	gunPowerFH;
	int	gunPowerFL;
	int	gunPowerF[4][6]; 	// H:2, 4, 6, 8, 10, 12 o'clock
					// M:2, 4, 6, 8, 10, 12 o'clock
					// L:2, 4, 6, 8, 10, 12 o'clock
					// above, low, na, na, na, na
};

struct hitResult {
	int	wing;
	int	fuselage;
	int	cockpit;
	int	engine;
	int	gun;
	int	fuel;
};

struct firingEntry {
	int	attackerACID;
	int	targetACID;
	int	gunType;	// 1: ffMg
				// 2: ffCanon
				// 3: ffMg+Cannon
				// 4: FH
				// 5: FL
				// 6: F
	int	dieRoll;
	hitResult	result;
};

struct cmdForm {
	CommandToX	command;
	int		playerID;
	int		selectedAircraft;
	int		aircraftModel;
	int		pilotModel;
	int		aircraftID;
	wchar_t		aircraftName[64];
	AircraftStat	acStatus;
	int		virCorX;
	int		virCorY;
	float		heading;
	int		speed;
	float		alt;
	int		bank;
	int		nose;
	int 		manuv[80];
	int		maxSpeedAtAnyAlt;
	float		maxAlt;
	int		stackNum;
	maneuverable	manuvable;
	int		virCorXmanuv;
	int		virCorYmanuv;
	struct		damage;
	firingArcRange	firingRange;
	gunPower	gunPower;
	firingEntry	firingEntry;
};

struct damage {
	struct aircraftCompo hit;
	struct aircraftCompo tolerance;
};

struct ammo {
	struct weapon fired;
	struct weapon payload;
};

struct expandHistory {
	int	leftCnt;
	int	centerCnt;
	int	rightCnt;
};
// i tried to put this declaration of expandHistory
// in the MapAirForce class,
// but cannot compile.
// i dont know why.   2018/02/20


///////////////////////////////////////////////////////////////////////////
//
//	MapAirForce.h
//
///////////////////////////////////////////////////////////////////////////

LPCTSTR listStrHeading[]
	= {
		L"N  (North)",
		L"NE (North East)",
		L"SE (South East)",
		L"S  (South)",
		L"SW (South West)",
		L"NW (North West)"
	};

LPCTSTR listStrSpeed[]
	= {
		L"0",
		L"1",
		L"2",
		L"3",
		L"4",
		L"5",
		L"6",
		L"7",
		L"8",
		L"9",
		L"10",
		L"11",
		L"12",
		L"13",
		L"14",
		L"15",
		L"16",
		L"17",
		L"18",
		L"19",
		L"20",
		L"21",
		L"22",
		L"23",
		L"24",
		L"25",
		L"26",
		L"27",
		L"28",
		L"29",
		L"30"
	};


LPCTSTR listStrBank[]
	= {
		L"Inverted Right",
		L"Bank Right",
		L"Level",
		L"Bank Left",
		L"Inverted Left",
		L"Inverted"
	};

LPCTSTR listStrBankShort[]
	= {
		L"IR",
		L"BR",
		L"LV",
		L"BL",
		L"IL",
		L"IV"
	};

LPCTSTR listStrNose[]
	= {
		L"Nose Up",
		L"Nose Level",
		L"Nose Down"
	};

class MapAirForce : public MainWindow {
protected:

	enum MapStat {
		UNDEFINED	= -1,
		NOT_SELECTED	= 1,
		SELECTED	= 2,
		MAP_DEPLOY_DRAG = 3,
		MAP_FIRE_AIM	= 4
	};

	struct ResultHitTestHex {
		int virCorX;
		int virCorY;
		int realCorX;
		int realCorY;
		bool even;
	};


  //------------------- protected member variables ---------------------
  	
	IDWriteFactory		*mPtrDWriteFactory;
	IDWriteTextFormat	*mPtrTextFormat;
	int			mHexVirCorOrgX = -2;
	int			mHexVirCorOrgY = -4;
	int			m_hexCntX;
	int			m_hexCntY;
	float			m_offsetX;
	float			m_offsetY;
	IWICImagingFactory	*m_p_imagingFactory;
	ID2D1Bitmap		*m_pBitmap;
	int			m_realCorSelectedPrevX, m_realCorSelectedPrevY;
	bool			m_evenSelectedPrev;
	int			m_realCorSelectedX, m_realCorSelectedY;
	bool			m_evenSelected;
	int			m_virCorSelectedX, m_virCorSelectedY;
	INITCOMMONCONTROLSEX	m_initComCon;
	MapStat			m_mapStat;
	firingArcRange		m_selectedFireArc;

  //------------------- protected member functions ---------------------
	void parseManuvModifyVirCorSlipRollNorth(cmdForm *p_form, int manuv);
	void parseManuvModifyVirCorSlipRollNorthEast(cmdForm *p_form, int manuv);
	void parseManuvModifyVirCorSlipRollSouthEast(cmdForm *p_form, int manuv);
	void parseManuvModifyVirCorSlipRollSouth(cmdForm *p_form, int manuv);
	void parseManuvModifyVirCorSlipRollSouthWest(cmdForm *p_form, int manuv);
	void parseManuvModifyVirCorSlipRollNorthWest(cmdForm *p_form, int manuv);
	void parseManuvModifyVirCorSlipRoll(cmdForm *p_form, int manuv);
	void parseManuvTL(cmdForm *p_form, int *p_mp);
	void parseManuvTR(cmdForm *p_form, int *p_mp);
	void parseManuvBL(cmdForm *p_form, int *p_mp);
	void parseManuvBR(cmdForm *p_form, int *p_mp);
	void parseManuvSL(cmdForm *p_form, int *p_mp);
	void parseManuvSR(cmdForm *p_form, int *p_mp);
	void parseManuvRL(cmdForm *p_form, int *p_mp);
	void parseManuvRR(cmdForm *p_form, int *p_mp);
	void parseManuvLC(cmdForm *p_form, int *p_mp);
	void parseManuvLD(cmdForm *p_form, int *p_mp);
	void parseManuvPW(cmdForm *p_form, int *p_mp);
	void parseManuvBK(cmdForm *p_form, int *p_mp);
	void parseManuvDB(cmdForm *p_form, int *p_mp);
	void parseManuvFR(cmdForm *p_form, int *p_mp);
	void parseManuvFG(cmdForm *p_form, int *p_mp);
	void parseManuvModifyVirCorFwdNorth(cmdForm *p_form);
	void parseManuvModifyVirCorFwdNorthEast(cmdForm *p_form);
	void parseManuvModifyVirCorFwdSouthEast(cmdForm *p_form);
	void parseManuvModifyVirCorFwdSouth(cmdForm *p_form);
	void parseManuvModifyVirCorFwdSouthWest(cmdForm *p_form);
	void parseManuvModifyVirCorFwdNorthWest(cmdForm *p_form);
	void parseManuvMoveFwdOneHex(cmdForm *p_form, int *p_mp);
	void parseManuvMoveFwd(cmdForm *p_form, int *p_mp, int fwd);
	void parseManuvClimb(cmdForm *p_form, int *p_mp, int climb);
	void parseManuvDive(cmdForm *p_form, int *p_mp, int dive);
	void parseManuvMoveOneHexSpecifiedDirection(cmdForm *p_form, int intHeading);
	void parseManuvMoveOneHexClockRef(cmdForm *p_form, int clockRef);
	int parseManuv(cmdForm *p_form);

  	D2D1_POINT_2F	virCorToCenterF(int virCorX, int virCorY);
	void	NumerateStackNum(cmdForm *p_rtn);
	void 	DrawLeaderLine(D2D1_POINT_2F center, cmdForm *p_form);
	void 	DrawManuvPath(cmdForm form);
	HRESULT PaintHex(int virCorX, int virCorY);
	HRESULT DrawPiece(
//		ID2D1RenderTarget *p_renderTgt,
//		ID2D1SolidColorBrush *pBrush,
//		IWICImagingFactory *p_factory,
		cmdForm form
		);
	void 	OnPaintBitmap();
	void 	paintAndExpand(
			cmdForm		form,
			int		range,
			int		clockRef,
			bool		includeLeft,
			bool		includeRight,
			expandHistory	history);
	void 	paintFireArcClockCond (
			cmdForm form, 
			int range, 
			int clockRef,
			bool includeLeft, 
			bool includeRight);
	void 	paintFireArcClockRef(cmdForm form, int clockRef, int range);
	void 	paintFireArc(cmdForm form);
	void 	OnPaintGM_Fire();
	void 	OnPaint();
	HRESULT CreateDWriteTextFormat();
	void HandleDlgDeploySetAcstat(HWND hDlgWnd);
	LRESULT HandleDlgDeploy(HWND hDlgWnd,
				UINT msg,
				WPARAM wParam,
				LPARAM lParam);
	void 	HitTestHex(float dipX, float dipY, ResultHitTestHex *result);
	void	ShowDialogDeploy();
	void 	SetHexSelected(ResultHitTestHex result);
	int 	SelectAircraftFromStack
			(int pixelX, int pixelY, DWORD flags, cmdForm *p_form);
	int 	SetFirstAircraftSelected
			(int pixelX, int pixelY, DWORD flags, cmdForm *p_form);
	int 	SelectGun(int pixelX, int pixelY, DWORD flags, cmdForm *p_form);
	void 	setSelectedFireArc(cmdForm *p_form, int selectedGun);
	void 	OnLButtonDownGM_Fire(int pixelX, int pixelY, DWORD flags);
	void 	OnLButtonDown(int pixelX, int pixelY, DWORD flags);
	int 	OnLButtonDownSetAcSelected(int pixelX, int pixelY, DWORD flags);
	void 	OnRButtonDown(int pixelX, int pixelY, DWORD flags);
	void 	OnLButtonDoubleClicks(int pixelX, int pixelY, DWORD flags);
	int 	selectTarget(int pixelX, int pixelY, DWORD flags);
	void 	OnLButtonUp(int pixelX, int pixelY, DWORD flags);
	int 	getManuvMenuSelItemClimb(int selID, cmdForm *p_formRtnGetManuvable);
	int 	getManuvMenuSelItemDive(int selID, cmdForm *p_formRtnGetManuvable);
	void 	ShowManuvMenuSelItem(int selID, int distance, cmdForm *p_form);
	void 	ShowManuvMenuClimb(HMENU *p_hSubmenu, cmdForm *p_form);
	void 	ShowManuvMenuDive(HMENU *p_hSubmenu, cmdForm *p_form);
	int	ShowManuvMenu(int pixelX, int pixelY, DWORD flags,
			cmdForm *p_form, int distance);
	int 	isHexReachableFwdPath(ResultHitTestHex *p_hex, 
			cmdForm *p_form, int remainingMP);
	void modifyManeuverableByParsedManuv(cmdForm *form);
	void 	MapAirForce::OnMouseMove
			(int pixelX, int pixelY, DWORD flags);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
public:
  //------------------- public member variables ---------------------
	int			mHexSize;
	LONG_PTR		mp_ownerGame;
	cmdForm			m_formFromMap;

  //------------------- public member functions ---------------------
	MapAirForce::MapAirForce() {
		mPtrDWriteFactory = NULL;
		mPtrTextFormat = NULL;
		mHexVirCorOrgX = -2;
		mHexVirCorOrgY = -4;
		m_offsetX = 0.0f;
		m_offsetY = 0.0f;
		m_realCorSelectedPrevX = MAX_INT;
		m_realCorSelectedPrevY = MAX_INT;
		m_evenSelectedPrev = TRUE;
		m_realCorSelectedX = MIN_INT;
		m_realCorSelectedY = MIN_INT;
		m_evenSelected = TRUE;
		m_virCorSelectedX = MAX_INT;
		m_virCorSelectedY = MAX_INT;
		mp_ownerGame = NULL;
		m_mapStat = NOT_SELECTED;
	};
	PCWSTR  ClassName() const;
	void SetMapStat(MapStat stat);
	HRESULT LoadResourceBitmap(
		ID2D1RenderTarget *pRenderTarget,
		IWICImagingFactory *pIWICFactory,
		PCWSTR resourceName,
		PCWSTR resourceType,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap **ppBitmap
		);

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle
		);
	HRESULT MapAirForce::DrawPieces(
		ID2D1RenderTarget *p_renderTgt,
		ID2D1SolidColorBrush *pBrush,
		IWICImagingFactory *p_factory
		);
	static LRESULT CALLBACK MapAirForce::MyDlgProcDeploy(HWND hDlgWnd,
				UINT msg,
				WPARAM wParm,
				LPARAM	lParam);
//	/*
//		DWORD dwExStyle = 0,
//		int x = CW_USEDEFAULT,
//		int y = CW_USEDEFAULT,
//		int nWidth = CW_USEDEFAULT,
//		int nHeight = CW_USEDEFAULT,
//		HWND hWndParent = 0,
//		HMENU hMenu = 0
//	);
//	*/

};

/*
class MapAirForce : public MainWindow {
protected:
	int			mHexSize;
	IDWriteFactory		*mPtrDWriteFactory;
	IDWriteTextFormat	*mPtrTextFormat;
	int			mHexVirCorOrgX = -2;
	int			mHexVirCorOrgY = -4;

	void 	OnPaint();
	HRESULT CreateDWriteTextFormat();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
public:
	PCWSTR  ClassName() const;
};
*/

///////////////////////////////////////////////////////////////////////////
// 
// 	Aircraft.h
//
///////////////////////////////////////////////////////////////////////////

class Aircraft {
protected:
  //------------------- protected member variables ---------------------
	int	m_manuvIndex;
	list<shared_ptr<cmdForm>>	m_logs;
	bool	m_loaded;
	int	m_pilotVision;
	int 	m_pilotReflex;
	int	m_pilotTraining;
	int	m_pilotExperience;
	int 	m_maxDiveSpeed;
  //------------------- protected member functions ---------------------
	int GetAltTblIndex();
	float GetMaxAltChgClimb(int spdIncIndex);
	float GetMaxAltChgDive(int spdIncIndex);
	int GetMaxPower();
	int GetMaxBreak();
	int GetMaxDiveSpeed();
	void modifyManeuverableByBank(maneuverable *rtn);
	void getPrevTwoManuv(int *last, int *secondLast);
	void modifyManeuverableByPrevManuv(maneuverable *rtn);
	int getMP();
	maneuverable ReferManuvReqTblSpdInc(int spdIncIndex);
	void modifyManeuverableByDamageWing(maneuverable *p_manuvable);
	void modifyManeuverableByDamage2Engines
		(maneuverable *p_manuvable, int *engineDamage);
	void modifyManeuverableByDamage4Engines
		(maneuverable *p_manuvable, int *engineDamage);
	void modifyManeuverableByDamageEngine(maneuverable *p_manuvable);
	int getDamageCockpit();

	int ReferFiringRangeACModelGunPosition(int acm, int gunPosition, int gunType);
	void ReferFiringRangeACModelGunF(int acm, firingArcRange *p_range);
	firingArcRange Aircraft::ReferFiringRangeACModel(int acm);

	int ReferGunPowerACModelGunPosition(int acm, int gunPosition, int gunType);
	void ReferGunPowerACModelGunF(int acm, gunPower *p_gunPower);
	gunPower ReferGunPowerACModel(int acm);
	void ModifyGunPowerByBank(gunPower *p_gunPower);
	void ModifyGunPowerByAmmo(gunPower *p_gunPower);
	void ModifyGunPowerByDamage(gunPower *p_gunPower);


public:

  //------------------- public member variables ---------------------
	int	m_id;
	int	mAircraftModel;
	int	mPilotModel;
	wchar_t	mAircraftName[64];
	regStat	mAircraftRegStat;

	ID2D1Bitmap *m_p_bitmap;
	ID2D1Bitmap *m_p_bitmask;
	ID2D1BitmapBrush *m_p_bitmapBrush;
	ID2D1BitmapBrush *m_p_bitmaskBrush;

	AircraftStat	m_stat;
	bool	m_hilight;
	float	m_trayPixelX;
	float	m_trayPixelY;
	int	m_virCorX, m_virCorY;
	float	m_heading;
	int	m_speed;
	float	m_alt;
	int	m_bank;
	int	m_nose;
	int	m_manuv[80];
	damage 	m_damage;
	ammo	m_ammo;

	UINT_PTR mp_owner; // pointer to the owner player

  //------------------- public member functions ---------------------
	Aircraft::Aircraft() {
		m_id = -1;
		m_manuv[0] = MANUV_DP;
		m_manuv[1] = MANUV_DP;
		m_manuv[2] = MANUV_EN;
		m_loaded = FALSE;
		mAircraftRegStat = UNDEF;
		m_stat = UNDEFINED;
		mp_owner = NULL;
		m_damage.hit.wing = 0;
		m_damage.hit.fuselage = 0;
		m_damage.hit.cockpit[0] = 0;
		m_damage.hit.cockpit[1] = 0;
		m_damage.hit.engine[0] = 0;
		m_damage.hit.engine[1] = 0;
		m_damage.hit.engine[2] = 0;
		m_damage.hit.engine[3] = 0;
		m_damage.hit.gun[0] = 0;
		m_damage.hit.gun[1] = 0;
		m_damage.hit.gun[2] = 0;
		m_damage.hit.gun[3] = 0;
		m_damage.hit.gun[4] = 0;
		m_damage.hit.gun[5] = 0;
		m_damage.hit.gun[6] = 0;
		m_damage.hit.gun[7] = 0;
		m_damage.hit.fueltank = 0;
		m_damage.tolerance.wing = -1;
		m_damage.tolerance.fuselage = -1;
		m_damage.tolerance.cockpit[0] = -1;
		m_damage.tolerance.cockpit[1] = -1;
		m_damage.tolerance.engine[0] = -1;
		m_damage.tolerance.engine[1] = -1;
		m_damage.tolerance.engine[2] = -1;
		m_damage.tolerance.engine[3] = -1;
		m_damage.tolerance.gun[0] = -1;
		m_damage.tolerance.gun[1] = -1;
		m_damage.tolerance.gun[2] = -1;
		m_damage.tolerance.gun[3] = -1;
		m_damage.tolerance.gun[4] = -1;
		m_damage.tolerance.gun[5] = -1;
		m_damage.tolerance.gun[6] = -1;
		m_damage.tolerance.gun[7] = -1;
		m_damage.tolerance.fueltank = -1;
		m_ammo.fired.cannon = 0;
		m_ammo.fired.mg = 0;
		m_ammo.payload.cannon = -1;
		m_ammo.payload.mg = -1;
		// initializing array in a constructor is not supported in C++
		m_hilight = FALSE;
		m_pilotVision = 0;
	  	m_pilotReflex = 0;
	 	m_pilotTraining = 0;
	 	m_pilotExperience = 0;
		m_maxDiveSpeed = MAX_INT;
	};

	BOOL Aircraft::HitTest(float x, float y);

	HRESULT Aircraft::Draw(
		ID2D1RenderTarget *p_renderTgt,
		ID2D1SolidColorBrush *pBrush,
		IWICImagingFactory *p_factory,
		float x,
		float y,
		float width,
		float height,
		float heading
		);

	void Aircraft::GetLimit(int acm, cmdForm *p_rtn);
	spdIncTblEntry ReferSpeedIncTbl();
	maneuverable ReferManuvReqTbl();
	void modifyManeuverableByDamage(maneuverable  *p_manuvable);
	void modifyManeuverableByPilot(maneuverable *p_manuvable);

	firingArcRange ReferFiringRange();
	gunPower ReferGunPower();
};

///////////////////////////////////////////////////////////////////////////
// 
// 	PlayerAirForce.h
//
///////////////////////////////////////////////////////////////////////////

class PlayerAirForce : public MainWindow{
protected:
  //------------------- protected member variables ---------------------
	IDWriteFactory		*mPtrDWriteFactory;
	IDWriteTextFormat	*mPtrTextFormat;
	IWICImagingFactory	*m_p_imagingFactory;
	D2D1_POINT_2F		m_ptMouse;

  //------------------- protected member functions ---------------------
  	void cmdToPlayerSetAcstat(cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerGetManuvable(
		cmdForm form, 
		cmdForm *p_rtn, 
		list<std::shared_ptr<Aircraft>>::iterator itr);
	void cmdToPlayerGetManuvables(cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerAppendManuv(cmdForm form, 
		list<std::shared_ptr<Aircraft>>::iterator itr);
	void cmdToPlayerAppendManuvs(cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerDeleteManuv(cmdForm form, 
		list<std::shared_ptr<Aircraft>>::iterator itr);
	void cmdToPlayerDeleteManuvs(cmdForm form, cmdForm *p_rtn);
	void OnPaint();
	bool HitTestAircrafts(float dipX, float dipY);
	void OnLButtonDown(int pixelX, int pixelY, DWORD flags);
	void OnLButtonUp(int pixelX, int pixelY, DWORD flags);
	HRESULT CreateDWriteTextFormat();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void cmdToPlayerGetFireRange(
		cmdForm form, 
		cmdForm *p_rtn, 
		list<std::shared_ptr<Aircraft>>::iterator itr);
	void cmdToPlayerGetFireRanges(cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerDispatch(int cmd, cmdForm form, cmdForm *p_rtn);

public:
  //------------------- public member variables ---------------------
	int	mPlayerID;
	regStat	mPlayerRegStat;
	list<shared_ptr<Aircraft>>::iterator m_ItrSelectedAircraft;
	list<shared_ptr<Aircraft>> mAircrafts;
	LONG_PTR		mp_ownerGame;

  //------------------- public member functions ---------------------
	void cmdToPlayer(int cmd, cmdForm form, cmdForm *p_rtn);
	PlayerAirForce::PlayerAirForce() {
		mPtrDWriteFactory = NULL;
		mPtrTextFormat = NULL;
		m_p_imagingFactory = NULL;
		mp_ownerGame = NULL;
	};

	void WhereToDraw(float *x, float *y);

};

///////////////////////////////////////////////////////////////////////////
// 
// 	GameAirForce.h
//
///////////////////////////////////////////////////////////////////////////
class GameAirForce : public MainWindow
{
protected:
  //------------------- protected member variables ---------------------
	IDWriteFactory		*mPtrDWriteFactory;
	IDWriteTextFormat	*mPtrTextFormat;
	IWICImagingFactory	*mp_imagingFactory;
	list<shared_ptr<MapAirForce>>::iterator mItrMaps;
	list<shared_ptr<MapAirForce>> mMaps;
	list<shared_ptr<PlayerAirForce>>::iterator mItrPlayers;
	list<shared_ptr<PlayerAirForce>> mPlayers;
	static int	mNewPlayerID;
	regStat		mNewPlayerRegStat;
	static int aircraftID[MAX_AIRCRAFTMODELNUMBER];
	list<shared_ptr<firingEntry>>::iterator m_firingEntries;
	int			m_gameTurn;

  //------------------- protected member functions ---------------------
  	HRESULT CreateDWriteTextFormat();
	LRESULT GameAirForce::HandleMessageNewMap(HWND hDlgWnd,
						UINT msg,
					  	WPARAM wParam,
				  	  	LPARAM lParam);
	LRESULT GameAirForce::HandleDlgNewPlayer(HWND hDlgWnd,
						UINT msg,
					  	WPARAM wParam,
				  	  	LPARAM lParam);
	void cmdGetDispatched(cmdForm *p_rtnForms);
	void GameAirForce::addPlayerAndCraftIfNeeded(int acmIndex, int pmIndex, wchar_t *name);
	void OnPaint();
	void OnButtonProceed();
	void GameAirForce::OnEditNewMapDialog();
public:
  //------------------- public member variables ---------------------
	GameMode m_gameMode;
	list<shared_ptr<PlayerAirForce>>::iterator mItrSelectedPlayer;
	static int GameAirForce::numberAircraft(int acmID);

  //------------------- public member functions ---------------------
  	GameAirForce::GameAirForce() {
		m_gameTurn = 1;
		m_gameMode = GM_NOP;
	};
	void setGameMode(GameMode mode);
	void cmdToGame(int cmd, cmdForm form, cmdForm *p_rtnForms);
	static LRESULT CALLBACK GameAirForce::MyDlgProcNewPlayer(HWND hDlgWnd,
		                                         UINT msg,
							 WPARAM wParam,
							 LPARAM lParam);
	static LRESULT CALLBACK GameAirForce::MyDlgProcNewMap(HWND hDlgWnd,
		                               UINT msg,
					       WPARAM wParam,
				  	       LPARAM lParam);
// CALLBACK function needs to be static. I dont understand why.
// otherwse you have compile error of cannot convert.
// In addition specify static in .h file not .cppÅ@file.
// otherwise you have compile error.

	LRESULT GameAirForce::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


	void OnEditShowDialog();
	void addNewPlayer () {
		shared_ptr<PlayerAirForce> ptrNewPlayer(new PlayerAirForce);
		mPlayers.push_front(ptrNewPlayer);
	};

};
// DO NOT FORGET !!! to
// instanciate static member variables
// and do not forget to add namespace!!
	int GameAirForce::mNewPlayerID; 
//	regStat GameAirForce::mNewPlayerRegStat;
	int GameAirForce::aircraftID[MAX_AIRCRAFTMODELNUMBER];
//	list<shared_ptr<MapAirForce>>::iterator GameAirForce::mItrMaps;
//	list<shared_ptr<MapAirForce>> GameAirForce::mMaps;
//	list<shared_ptr<PlayerAirForce>>::iterator GameAirForce::mItrSelectedPlayer;
//	list<shared_ptr<PlayerAirForce>> GameAirForce::mPlayers;




