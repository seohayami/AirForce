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

#define MAX_AC_CNT	32
#define MAX_PLAYER_CNT	4

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
	NA = -1		// more than DIVE speed
};

enum gunType {
	gT_Non 	= 0,
	gT_MG 	= 1,
	gT_CN 	= 2,
	gT_MGCN = 3,
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
	gunType		gunType[8]; //leftx3 centerx1 rightx3 Fx1
	int		gunPosition[8];
	int		gunRange[8];
	int		gunPower[8];
	int		fgunPower[4][6]; // Low 2, 4, 6, 8, 10, 12 o'clock
	                                // Med 2, 4, 6, 8, 10, 12 o'clock
					// Hih 2, 4, 6, 8, 10, 12 o'clock
					// above, below, na, na, na, na
	int		silhouette;
	int		fireAccuracy;
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
	GM_DISPATCH	= 101 	// both NOP and DISPATCH are used in Dispatch Mode
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
	DEPLOYED 	= 3, 	// on a tray
	DISPATCHED 	= 4,	// moved from tray to map
	SHOTDOWN	= 5,	// destroyed by attacker firing in the game turn
	REMOVED		= 6,	// destroyed aircraft is removed in the next turn
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
	GET_FIRE_RANGE		= 13,
	APPEND_FIRING		= 14,
	DELETE_FIRING		= 15,
	GET_FIRINGS		= 16,
	GET_ACID		= 17,
	USE_AMMO		= 18,
	MODIFY_DAMAGE		= 19,
	UPDATE_ACSTAT		= 20,
	CLEAR_MANUVS		= 21,
	TAKE_LOGS		= 22,
	WRITE_FILE		= 23,
	REPLICA_AC		= 24,
	REPLICA_LOG		= 25,
// cmdToMap commands
	FINALIZE_MANUV		= 1001,
	REFLECT_ERASE_PLOT	= 1002,
	REPAINT_MAP		= 1003,
};

enum attackSide {
	AS_UNDEF = -1,
	LEFT = 1,
	RIGHT = 2,
	FORWARD = 3,
	BACK = 4,
	ABOVE = 5,
	BELOW = 6,
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

struct firingArcRange {
	int		rangeFFmg;
	int		rangeFFcannon;
	bool		rangeSameHexFF;
	int		rangeFH;
	bool		rangeSameHexFH;
	int		rangeFL;
	bool		rangeSameHexFL;
	int		rangeF[8]; 	// 2, 4, 6, 8, 10, 12Å@o'clock, above, below
	gunPower	gunFactor;
};

struct hitResult {
	int		wing;
	int		fuselage;
	int		cockpit;
	int		engine;
	int		gun;
	int		fuel;
	int		die;
};

struct firingEntry {
	int		gameTurn;
	int		acIDattacker;
	int		acIDtarget;
	firingArcRange	weapon;
	int		distanceH;	// horizontal distance in hex
	int		distanceV;	// vertical distance in hex(per 500ft)
	int		modifierA;	// modifier attacker
	int		modifierT;	// modifier target
	int		modifierD;	// modifier deflection
	int		modifierP;	// modifier attacker pilot
	int		attenuation;
	int		dieRoll;
	wchar_t		result[32];
	int		clockAtoT;	// clock from attacker to target
	int		clockTtoA;	// clock from target to attacker
					// 2, 4, 6, 8, 10, 12 oclock
					// 14 if above
					// 16 if below
					// 18 if same hex, same alt (collision)
};

struct damage {
	struct aircraftCompo hit;
	struct aircraftCompo tolerance;
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
	spdIncTblEntry	speedCat;	//STAL,DIVE,LEVL,MANV,NA
	float		alt;
	int		bank;
	int		nose;
	bool		loaded;
	int 		manuv[80];
	int		maxSpeedAtAnyAlt;
	float		maxAlt;
	int		stackNum;
	maneuverable	manuvable;
	int		virCorXmanuv;
	int		virCorYmanuv;
	damage		dmg;
	firingArcRange	firingRange;
	gunPower	gunPower;
	firingEntry	firingEnt;
	int		silhouette;
	int		fireAccuracy;
	int		*p_aircraft;
	int		gameTurn;
	fstream		*p_file;
	UINT_PTR 	p_ptr; // pointer for general use
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

enum chunkType {
	UNDEF_CHUNK	= -1,
	GAME 		= 1,
	PLAYER 		= 2,
	MAP 		= 3,
	AIRCRAFT 	= 4,
	LOG_AIRCRAFT 	= 5,
	FIRING 		= 6,
};

struct chunkTab{
	chunkType	type;
	int		cnt;
	int		revMain;
	int		revSub;
	int		playerID;
	int		acID;
};

///////////////////////////////////////////////////////////////////////////
// 
// 	MapAirForce.h
//
///////////////////////////////////////////////////////////////////////////

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

	// for inFireArcCheck
	bool			m_inFireArc;
	int			m_virCorX_target, m_virCorY_target;

  //------------------- protected member functions ---------------------
	int checkIfNeedBreaks(cmdForm form);
  	void finalizeManuvByAcID(cmdForm form);
  	void reflectErasePlotByAcID(cmdForm form);
	void repaintMap(cmdForm form);
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
	int getDistanceHex(cmdForm formS, cmdForm fromD);

  	D2D1_POINT_2F	virCorToCenterF(int virCorX, int virCorY);
	void	NumerateStackNum(cmdForm *p_rtn);
	void 	DrawLeaderLine(D2D1_POINT_2F center, cmdForm *p_form);
	void 	DrawManuvPath(cmdForm form);
	HRESULT PaintHex(int virCorX, int virCorY);
	HRESULT drawArrowHexToHex(
		int virCorXsource, int virCorYsource,
		int virCorXdest, int virCorYdest);
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
	void 	drawFiring(cmdForm form);
	void 	drawFirings();
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
	void 	cmdToGameGetAC_acID(cmdForm *p_cmdForm, int acID);
	void 	cmdToGameGetAC_selected(cmdForm *p_cmdForm);
	int 	getFireModifierA(cmdForm formA);
	int 	getFireModifierT(cmdForm formT);
	int 	getFireModifierD(cmdForm formA, cmdForm formT);
	int 	getFireModifierP(cmdForm formA);
	void 	deleteUnusedGunFactorFExcpH(firingEntry *fe, int clock);
	void 	deleteUnusedGunFactorFExcpL(firingEntry *fe, int clock);
	void 	deleteUnusedGunFactorFExcpM(firingEntry *fe, int clock);
	void 	deleteUnusedGunFactorFExcpA(firingEntry *fe, int clock);
	void 	deleteUnusedGunFactorFExcpB(firingEntry *fe, int clock);
	void 	deleteUnusedGunFactorF(firingEntry *p_fe, cmdForm formA, cmdForm formT);
	void 	cmdToGameAppendFiring(int acIDtarget);
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
	MapAirForce::MapAirForce(MapAirForce &src)
	{
	// copy constructor
		mPtrDWriteFactory = NULL;
		mPtrTextFormat = NULL;
		mHexVirCorOrgX = src.mHexVirCorOrgX;
		mHexVirCorOrgY = src.mHexVirCorOrgY;
		m_hexCntX = src.m_hexCntX;
		m_hexCntY = src.m_hexCntY;
		m_offsetX = src.m_offsetX;
		m_offsetY = src.m_offsetY;
		m_p_imagingFactory = NULL;
		m_pBitmap = NULL;
		m_realCorSelectedPrevX = src.m_realCorSelectedPrevX;
		m_realCorSelectedPrevY = src.m_realCorSelectedPrevY;
		m_evenSelectedPrev = src.m_evenSelectedPrev;
		m_realCorSelectedX = src.m_realCorSelectedX; 
		m_realCorSelectedY = src.m_realCorSelectedY;
		m_evenSelected = src.m_evenSelected;
		m_virCorSelectedX = src.m_virCorSelectedX;
		m_virCorSelectedY = src.m_virCorSelectedY;
		m_initComCon = src.m_initComCon;
		m_mapStat = src.m_mapStat;
		m_selectedFireArc = src.m_selectedFireArc;;
		m_inFireArc = src.m_inFireArc;
		m_virCorX_target = src.m_virCorX_target;
		m_virCorY_target = src.m_virCorY_target;

		mHexSize = src.mHexSize;
		mp_ownerGame = NULL;
		m_formFromMap = src.m_formFromMap;
	}

  	void cmdToMap(int cmd, cmdForm form, cmdForm *p_rtn);
	int getClock(cmdForm formS, cmdForm formD);
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
	int	m_pilotVision;
	int 	m_pilotReflex;
	int	m_pilotTraining;
	int	m_pilotExperience;
	int 	m_maxDiveSpeed;
  //------------------- protected member functions ---------------------
	HRESULT makeStrDamageCockpit(wchar_t *a_str);
	HRESULT makeStrDamageEngine(wchar_t *a_str);
	gunType getGunType(int index);
	HRESULT catGunStatToStr(wchar_t *a_str, int index);
	HRESULT makeStrDamageGun(wchar_t *a_str);

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
	bool	m_loaded;
	int	m_manuv[80];
	damage 	m_damage;
	ammo	m_ammo;
	list<shared_ptr<Aircraft>>	m_logs;
	int	m_logGameTurn;		// this variable is used for logging
	UINT_PTR mp_owner; 	// pointer to the owner player if used as Aircraft
			  	// pointer to the owner aircraft if used as Logs

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

	Aircraft::Aircraft(Aircraft &src)
	{
	// copy constructor
		int i;

		m_pilotVision = src.m_pilotVision;
		m_pilotReflex = src.m_pilotReflex;
		m_pilotTraining = src.m_pilotTraining;
		m_pilotExperience = src.m_pilotExperience;
	 	m_maxDiveSpeed = src.m_maxDiveSpeed;

		m_id = src.m_id;
		mAircraftModel = src.mAircraftModel;
		mPilotModel = src.mPilotModel;
		StringCchCopyW(mAircraftName, STRSAFE_MAX_CCH, src.mAircraftName);
		regStat	mAircraftRegStat = src.mAircraftRegStat;

		m_p_bitmap = NULL;
		m_p_bitmask = NULL;
		m_p_bitmapBrush = NULL;
		m_p_bitmaskBrush = NULL;

		m_stat = src.m_stat;
		m_hilight = src.m_hilight;
		m_trayPixelX = src.m_trayPixelX;
		m_trayPixelY = src.m_trayPixelY;
		m_virCorX = src.m_virCorX; 
		m_virCorY = src.m_virCorY;
		m_heading = src.m_heading;
		m_speed = src.m_speed;
		m_alt = src.m_alt;
		m_bank = src.m_bank;
		m_nose = src.m_nose;
		m_loaded = src.m_loaded;
		m_manuv[80];
		for (i = 0; i < 80; i++) {
			m_manuv[i] = src.m_manuv[i];
		}
		m_damage = src.m_damage;
		m_ammo = src.m_ammo;
		m_logs.clear();
		m_logGameTurn = src.m_logGameTurn;
		mp_owner = NULL; // pointer to the owner player
	}

	Aircraft::~Aircraft() {
	// destructor
		list<shared_ptr<Aircraft>>::iterator itr;
		for (itr = m_logs.begin(); itr != m_logs.end(); itr++){
			delete (*itr);
		}
		m_logs.clear();
	}

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
  	void drawInfoName(
		ID2D1RenderTarget *p_renderTgt,
		ID2D1SolidColorBrush *pBrush,
		IWICImagingFactory *p_factory,
		IDWriteTextFormat *p_textFormat,
		float offsetX,
		float offsetY,
		float width,
		float height
		);
	HRESULT drawInfoDamage(
		ID2D1RenderTarget *p_renderTgt,
		ID2D1SolidColorBrush *pBrush,
		IWICImagingFactory *p_factory,
		IDWriteTextFormat *p_textFormat,
		float offsetX,
		float offsetY,
		float width,
		float height
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
	float			m_drawOriginX;
	float			m_drawOriginY;

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
	void cmdToPlayerGetAcAcID(cmdForm form, cmdForm *p_rtn);
	BOOL cmdToPlayerGetACID(
		cmdForm form, 
		cmdForm *p_rtn, 
		list<std::shared_ptr<Aircraft>>::iterator itr);
	void cmdToPlayerGetACIDs(cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerUSE_AMMO(cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerMODIFY_DAMAGE_Ac
		(cmdForm form, cmdForm *p_rtn, shared_ptr<Aircraft> p_ac);
	void cmdToPlayerMODIFY_DAMAGE (cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerUPDATE_ACSTAT_Ac
		(cmdForm form, cmdForm *p_rtn, shared_ptr<Aircraft> p_ac);
	void cmdToPlayerUPDATE_ACSTAT(cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerClearManuv
		(cmdForm form, list<shared_ptr<Aircraft>>::iterator itr);
	void cmdToPlayerCLEAR_MANUVS(cmdForm form, cmdForm *p_rtn);
	void cmdToPlayerTakeLog
		(cmdForm form, list<shared_ptr<Aircraft>>::iterator itr_ac);
	void cmdToPlayerTAKE_LOGS(cmdForm form, cmdForm *p_rtn);
	void writeAircraftLogToFile
		(cmdForm form, list<shared_ptr<Aircraft>>::iterator itr_ac);
	void writeAircraftLogsToFile
		(cmdForm form, list<shared_ptr<Aircraft>>::iterator itr_ac);
	void writeAircraftToFile
		(cmdForm form, list<shared_ptr<Aircraft>>::iterator itr_ac);
	void cmdToPlayerWRITE_FILE(cmdForm form, cmdForm *p_rtn);
	bool cmdToPlayerREPLICA_AC(cmdForm form, cmdForm *p_rtn);
	bool cmdToPlayerREPLICA_LOG(cmdForm form, cmdForm *p_rtn);

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
		m_drawOriginX = 30.0f;
		m_drawOriginY = 30.0f;
		mp_ownerGame = NULL;
		m_ItrSelectedAircraft = mAircrafts.end();
	};

	PlayerAirForce::PlayerAirForce(const PlayerAirForce &src)
	{
	// copy constructor
		mPtrDWriteFactory = src.mPtrDWriteFactory;
		mPtrTextFormat = src.mPtrTextFormat;
		m_p_imagingFactory = src.m_p_imagingFactory;
		m_ptMouse = src.m_ptMouse;
		m_drawOriginX = src.m_drawOriginX;
		m_drawOriginY = src.m_drawOriginY;
	
			mPlayerID = src.mPlayerID;
		mPlayerRegStat = src.mPlayerRegStat;
		//m_ItrSelectedAircraft = src.m_ItrSelectedAircraft;;
		m_ItrSelectedAircraft = mAircrafts.end(); 
		//list<shared_ptr<Aircraft>> mAircrafts;
		mAircrafts.clear();
		mp_ownerGame = src.mp_ownerGame;
	};

	PlayerAirForce::~PlayerAirForce() {
	// desturctor
		list<shared_ptr<Aircraft>>::iterator itr;
		for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
			delete (*itr);
		}
		mAircrafts.clear();
	}

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
	list<shared_ptr<firingEntry>> m_firingEntries;
	static int	mNewPlayerID;
	regStat		mNewPlayerRegStat;
	static int aircraftID[MAX_AIRCRAFTMODELNUMBER];
	int			m_gameTurn;
	HWND m_hwndLV_FiringTable;

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
	BOOL calculateFiringEnt(cmdForm *p_form);
	void cmdAppendFiring(cmdForm form);
	void cmdGetFirings(cmdForm form, cmdForm *p_rtnForms);
	void addPlayerAndCraftIfNeeded(int acmIndex, int pmIndex, wchar_t *name);
	void OnPaint();
	int referHitTableWing(int column, int die);
	int referHitTableFuselage(int column, int die);
	int referHitTableCockpit(int column, int die);
	int referHitTableEngine(int column, int die);
	int referHitTableGun(int column, int die);
	int referHitTableFuel(int column, int die);
	void referHitTable(int column, BOOL concentrate, hitResult *p_hr);
	hitResult resolveFire(int index);
	void makeHitTblStr(hitResult hr, wchar_t *a_str);
	void setFireTableResult(int index, hitResult hr);
	void resolveFires();
	BOOL addResolvedFireToFE
		(wchar_t *nameAttacker, int attenuation, int die, wchar_t *result);
	BOOL reflectResolvedFireToFE(int index);
	void reflectResolvedFireToFEs();
	gunType getGunTypeFromACM(int acmID, int gunPosition);
	gunType getGunTypeFromFE(firingEntry fe);
	void cmdToGameUSE_AMMO(firingEntry fe, int gunType);
	void reflectFireToA(firingEntry fe);
	void reflectFiresToAs();
	attackSide getAttackSideLorR(firingEntry fe);
	void modifyFormDamageFromDamageChrC(cmdForm *p_form, firingEntry fe);
	void modifyFormDamageFromDamageChrE(cmdForm *p_form, firingEntry fe);
	void modifyFormDamageFromDamageChrG(cmdForm *p_form, firingEntry fe);
	void modifyFormDamageFromDamageChr(cmdForm *p_form, firingEntry fe, wchar_t chr);
	void setFormDamageFromFE(cmdForm *p_form, firingEntry fe);
	void reflectFireToT(firingEntry fe);
	void reflectFiresToTs();

	void updateAcStats();
	void onExitGameModeFire();
	void finalizeAcManuv(cmdForm form);
	void finalizeAcManuvs();
	void onExitGameModePlot();
	void reflectAndErasePlot(cmdForm form);
	void reflectAndErasePlots();	
	void logAircrafts(int gameTurn);
	void repaintMap(list<shared_ptr<MapAirForce>>::iterator itr);
	void repaintMaps();
	void onEnterGameModeMove();
	void OnButtonProceed();
	void cmdToGameGetAC_acID(cmdForm *p_cmdForm, int acID);
	int  insertFormAttackerFiringTable(HWND hwndListView, cmdForm *p_form, int index);
	BOOL setFormAttackerFiringTable(HWND hwndListView, cmdForm *p_form, int index);
	void insertFormTargetFiringTable(HWND hwndListView, cmdForm *p_form, int index);
	BOOL setFormTargetFiringTable(HWND hwndListView, cmdForm *p_form, int index);
	BOOL setGunFactorToFireTable(HWND hwndListView, firingEntry *p_firingEnt, int index);
	BOOL setDistanceToFireTable(HWND hwndListView, firingEntry *p_firingEnt, int index);
	BOOL setAttenuationToFireTable(HWND hwndListView, firingEntry *p_firingEnt, int index);
	BOOL setModifierToFireTable(HWND hwndListView, firingEntry *p_firingEnt, int index);
	void insertFiringEntFiringTable(HWND hwndListView, firingEntry *p_firingEnt, int index);
	void setFiringEntFiringTable(HWND hwndListView, firingEntry *p_firingEnt, int index);
	void insertItemsFiringTable(HWND hwndListView);
	void insertColumnsFiringTable(HWND hwndListView);
	HWND createFiringTable();
	void handleWM_NOTIFY_FiringTable(LPARAM lParam);
	void handleWM_NOTIFY(LPARAM lParam);
	//----------------------
	void writeChunkTabGameToFile(fstream *p_file);
	void writeGameToFile(fstream *p_file);
	void writeWholePlayersToFile(fstream *p_file);
	void writeWholeMapsToFile(fstream *p_file);
	void writeFiringEntriesToFile(fstream *p_file);
	void writeWholeGameToFile(fstream *p_file);
	bool onFileSaveAs();
	//----------------------
	void replicaGame(GameAirForce *p_src, GameAirForce *p_des);
	bool readChunksGame(fstream *p_file, GameAirForce *p_bufGame);
	void replicaPlayer(PlayerAirForce *p_player, GameAirForce *p_des);
	bool readChunksPlayers
		(fstream *p_file, chunkTab tab, PlayerAirForce *p_bufPlayer);
	bool readChunksPlayers(fstream *p_file, chunkTab tab);
	void replicaMap(MapAirForce *p_map, GameAirForce *p_des);
	bool readChunksMaps(fstream *p_file, chunkTab tab, MapAirForce *p_bufMap);
	void readChunkAc(fstream *p_file, chunkTab tab, Aircraft *p_bufAircraft);
	bool readChunksAcs(fstream *p_file, chunkTab tab, Aircraft *p_bufAircraft);
	void readChunkLog(fstream *p_file, chunkTab tab, Aircraft *p_bufAircraft);
	bool readChunksLogs(fstream *p_file, chunkTab tab, Aircraft *p_bufAircraft);
	void replicaFiring(firingEntry *p_bufFiring, GameAirForce *p_des);
	bool readChunksFirings(
		fstream *p_file, chunkTab tab, firingEntry *p_bufFiring);
	bool readChunks(
		fstream *p_file, 
	 	chunkTab *p_tab,
		GameAirForce *p_bufGame,
		MapAirForce *p_bufMap,
		PlayerAirForce *p_bufPlayer,
		Aircraft *p_bufAircraft,
		firingEntry *p_bufFiring);
	bool onFileOpenWholeGame(PWSTR pszFilePath);
	bool isGameInProcess();
	bool mayOverwriteWholeGame();
	bool onFileOpen();
	//----------------------
	void OnEditNewMapDialog();
public:
  //------------------- public member variables ---------------------
	GameMode m_gameMode;
	list<shared_ptr<PlayerAirForce>>::iterator mItrSelectedPlayer;
  //------------------- public member functions ---------------------
	static int GameAirForce::numberAircraft(int acmID);

  	GameAirForce::GameAirForce() {
		m_gameTurn = 1;
		m_gameMode = GM_NOP;
		mItrSelectedPlayer = mPlayers.end();
		mItrMaps =mMaps.end();
	};

	GameAirForce::~GameAirForce() {
	// destructor
		list<shared_ptr<MapAirForce>>::iterator itrM;
		for (itrM = mMaps.begin(); itrM != mMaps.end(); itrM++) {
			delete (*itrM);
		}
		mMaps.clear();

		list<shared_ptr<PlayerAirForce>>::iterator itrP;
		for (itrP = mPlayers.begin(); itrP != mPlayers.end(); itrP++) {
			delete (*itrP);
		}
		mPlayers.clear();

		list<shared_ptr<firingEntry>>::iterator itrF;
		for (itrF = m_firingEntries.begin(); itrF != m_firingEntries.end(); itrF++) {
			delete (*itrF);
		}
		m_firingEntries.clear();
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
	int GameAirForce::mNewPlayerID = 1; 
//	regStat GameAirForce::mNewPlayerRegStat;
	int GameAirForce::aircraftID[MAX_AIRCRAFTMODELNUMBER];
//	list<shared_ptr<MapAirForce>>::iterator GameAirForce::mItrMaps;
//	list<shared_ptr<MapAirForce>> GameAirForce::mMaps;
//	list<shared_ptr<PlayerAirForce>>::iterator GameAirForce::mItrSelectedPlayer;
//	list<shared_ptr<PlayerAirForce>> GameAirForce::mPlayers;




