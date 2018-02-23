#include "stdafx.h"
#include "BaseWindow.h"
#include "AirForce.h"
#include "resource1.h"
#include "AirForceData.h"
#include <windows.h>
#include <Windowsx.h>
#include <d2d1.h>
#include <list>
#include <memory>
#include <time.h>

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <ShObjIdl.h>

#include <Winuser.h>

#include <strsafe.h>
#include <dwrite.h>

#include <wincodec.h>
#include <wincodecsdk.h>
#include <Objbase.h>

#include <Commctrl.h>
#pragma comment(lib, "Comctl32")

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
// you need to add this to avoid link error "LNK2019 unresolved external symbol"
#pragma comment(lib, "windowscodecs")
// you need to add this to avoid link error "LNK2001 unresolved external symbol"
#pragma comment(lib, "Comctl32")

#include <D2D1_1.h>

#define MAX_LOADSTRING 100


using namespace std;


HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget *pRenderTarget,
		IWICImagingFactory *pIWICFactory,
		LPCWSTR fileName,
		ID2D1Bitmap **ppBitmap
		)
{
	IWICBitmapDecoder *p_decoder = NULL;
	IWICBitmapFrameDecode *p_frame = NULL;
	IWICFormatConverter *p_converter = NULL;
	IWICPalette *p_palette = NULL;
	BOOL hasAlpha = FALSE;

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr)) {
		hr = pIWICFactory->CreateDecoderFromFilename(
			fileName,
			NULL, // GUID for preferred decoder vendor
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&p_decoder
			);
	} else { return E_FAIL;}

	if (SUCCEEDED(hr)) {
		hr =p_decoder->GetFrame(0, &p_frame);
	} else { return E_FAIL;}


	if (SUCCEEDED(hr)) {
		hr =pIWICFactory->CreatePalette(&p_palette);
	} else { return E_FAIL;}
	if (SUCCEEDED(hr)) {
		hr =p_palette->InitializeFromBitmap(
			p_frame,
			255,
			TRUE);
	} else { return E_FAIL;}
	if (SUCCEEDED(hr)) {
		hr =p_palette->HasAlpha(&hasAlpha);
	} else { return E_FAIL;}
	if (!hasAlpha) {
		MessageBox(NULL, 
			L"Palette does not have alpha transparent color.\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);

	}

	if (SUCCEEDED(hr)) {
		hr = pIWICFactory->CreateFormatConverter(&p_converter);
	} else { return E_FAIL;}
	if (SUCCEEDED(hr)) {
		hr = p_converter->Initialize(
			p_frame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			p_palette, // IWICPalette *pIPalette
			50.f, // alpha Threshold percent
//			0.f,
			WICBitmapPaletteTypeMedianCut
			);
	} else { return E_FAIL;}

	D2D1_PIXEL_FORMAT pixel_fmt
		= D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED);

	D2D1_BITMAP_PROPERTIES bmpProp; 
	bmpProp.pixelFormat = pixel_fmt;
	bmpProp.dpiX = 0.0f;
	bmpProp.dpiY = 0.0f;

	if (SUCCEEDED(hr)) {
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			p_converter,
			&bmpProp,
			ppBitmap
			);
	} else { return E_FAIL;}
	SafeRelease(&p_decoder);
	SafeRelease(&p_frame);
	SafeRelease(&p_converter);

	return hr;
}

float InnerProduct2D(D2D1_VECTOR_2F a, D2D1_VECTOR_2F b) 
{
	float result;

	result = a.x * b.x + a.y * b.y;

	return result;
}

D2D1_VECTOR_2F NormalizeVector2D(D2D1_VECTOR_2F v)
{
	D2D1_VECTOR_2F result;

	float l = sqrt(v.x * v.x + v.y * v.y);
	result.x = v.x / l;
	result.y = v.y / l;

	return result;
}

void DrawArrowHead(ID2D1RenderTarget *p_renderTarget,
			ID2D1SolidColorBrush *p_Brush,
			D2D1_POINT_2F headPt,
			float headLength,
			float width,
			float angle_head,
			float angle)        // angle : degree
		
{
	float pi = 3.1415;
	D2D1_POINT_2F defHeadTailLeftPt;
	D2D1_POINT_2F defHeadTailRihtPt;
	D2D1_POINT_2F headTailLeftPt;
	D2D1_POINT_2F headTailRihtPt;

	angle = angle * (-1.0f);
	// because Windows' Y-axis system is inverted from ususal Math system
	// you need to negate the angle degree.

	defHeadTailLeftPt.x = (-1.0f) * headLength * cos(angle_head * pi / 180.0f);
	defHeadTailLeftPt.y = (-1.0f) * headLength * sin(angle_head * pi / 180.0f);
	defHeadTailRihtPt.x = (-1.0f) * headLength * cos(angle_head * pi / 180.0f);
	defHeadTailRihtPt.y = ( 1.0f) * headLength * sin(angle_head * pi / 180.0f);

	headTailLeftPt.x = headPt.x + cos(angle * pi / 180.0f) * defHeadTailLeftPt.x
       		 	            - sin(angle * pi / 180.0f) * defHeadTailLeftPt.y;
	headTailLeftPt.y = headPt.y + sin(angle * pi / 180.0f) * defHeadTailLeftPt.x
       		                    + cos(angle * pi / 180.0f) * defHeadTailLeftPt.y;
	headTailRihtPt.x = headPt.x + cos(angle * pi / 180.0f) * defHeadTailRihtPt.x
       		 	            - sin(angle * pi / 180.0f) * defHeadTailRihtPt.y;
	headTailRihtPt.y = headPt.y + sin(angle * pi / 180.0f) * defHeadTailRihtPt.x
       		                    + cos(angle * pi / 180.0f) * defHeadTailRihtPt.y;
	p_renderTarget->DrawLine(headPt, headTailLeftPt, p_Brush, width);
	p_renderTarget->DrawLine(headPt, headTailRihtPt, p_Brush, width);
}

void DrawArrow(ID2D1RenderTarget *p_renderTarget,
		ID2D1SolidColorBrush *p_brush,
		D2D1_POINT_2F tailPt,
		D2D1_POINT_2F headPt,
		float headLength,
		float width)
{
	float pi = 3.1415;
	D2D1_VECTOR_2F unitVector ;
		unitVector.x = 1.0f;
		unitVector.y = 0.0f;

	D2D1_VECTOR_2F d;
		d.x = headPt.x - tailPt.x;
		d.y = headPt.y - tailPt.y;

	D2D1_VECTOR_2F v;
		v = NormalizeVector2D(d);

	float angleRadian;
		angleRadian = acos( InnerProduct2D(unitVector, v));

	p_renderTarget->DrawLine(tailPt, headPt, p_brush, width);
	DrawArrowHead(p_renderTarget,
			p_brush,
			headPt,
			headLength,
			width,
			15.0f,
			angleRadian / pi * 180.0f);
}

/////////////////////////////////////////////////////////////////////////////////
//
//	Aircraft.cpp
//
/////////////////////////////////////////////////////////////////////////////////

BOOL Aircraft::HitTest(float x, float y) {
	const float centerX = m_trayPixelX + 20.0f;
	const float centerY = m_trayPixelY + 20.0f;

	const float alpha = x - centerX;
	const float beta = y - centerY;

	const float d = (alpha * alpha) / (20.0f * 20.0F)
		      + (beta * beta) / (20.0f * 20.0f);

	return d <= 1.0f;
}

HRESULT Aircraft::Draw(
		ID2D1RenderTarget *p_renderTgt,
		ID2D1SolidColorBrush *pBrush,
		IWICImagingFactory *p_factory,
		float x,
		float y,
		float width,
		float height,
		float heading
		)
{
	HRESULT hr;
	wchar_t tmpBitmapFileName[MAX_NAMELENGTH];
	wchar_t tmpBitmaskFileName[MAX_NAMELENGTH];
	ID2D1Bitmap *p_bitmap;
	ID2D1Bitmap *p_bitmask;
	ID2D1BitmapBrush *p_bitmapBrush;
	ID2D1BitmapBrush *p_bitmaskBrush;
	IWICBitmapScaler *p_scaler;
	D2D1_SIZE_F size;

	x = m_trayPixelX;
	y = m_trayPixelY;

	D2D1_RECT_F rcBrushRect = D2D1::RectF(
		x,
		y, 
		x + width,
		y + height);

	p_renderTgt->SetTransform(D2D1::Matrix3x2F::Identity());

	wsprintf(tmpBitmapFileName, L"%s", aircraftModels[mAircraftModel].bmpFileName);
	LPCWSTR bitmapFileName = tmpBitmapFileName;

	wsprintf(tmpBitmaskFileName, L"%s", aircraftModels[mAircraftModel].bmpMaskFileName);
	LPCWSTR bitmaskFileName = tmpBitmaskFileName;

	hr = LoadBitmapFromFile(
		p_renderTgt,
		p_factory,
		bitmapFileName,
		&m_p_bitmap
		);
	if (FAILED(hr)) {
		return -1;
	}

/*
	hr = LoadBitmapFromFile(
		p_renderTgt,
		p_factory,
		bitmaskFileName,
		&m_p_bitmask
		);
 	if (FAILED(hr)) {
		return -1;
	}
 */

	float actHeight;
	float actWidth;
	float actX;
	float actY;
	float opacity = 1.0f;

	size = m_p_bitmap->GetSize();
	D2D1_RECT_F rcSourceRect = D2D1::RectF(
		0,
		0, 
		size.width,
		size.height);

	if (height / size.height < width / size.width) {
		actHeight = height;
		actWidth = size.width * height / size.height;
		actX = x + (width - actWidth) / 2;
		actY = y;
	} else {
		actHeight = size.height * width / size.width;
		actWidth = width;
		actX = x;
		actY = y + (height - actHeight) / 2;
	}

	 p_renderTgt->SetTransform(
		D2D1::Matrix3x2F::Rotation(
			heading,
			D2D1::Point2F(
				actX + actWidth / 2,
				actY + actHeight /2
			)
		)
	);

	if ((m_stat == DISPATCHED) 
	||  (m_stat == DEPLOYED)) {
		opacity = 0.2f;
	} else {
		opacity = 1.0f;
	}

	p_renderTgt->DrawBitmap(
		m_p_bitmap,
		D2D1::RectF(
			actX,
			actY,
			actX + actWidth,
			actY + actHeight 
		),
		opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		NULL
	);	
	if (m_hilight) {
		p_renderTgt->DrawRectangle(
			D2D1::RectF(
				actX,
				actY,
				actX + actWidth,
				actY + actHeight
			),
			pBrush,
			3.0f,
			NULL
		);
	}
 
	return hr;
}

void Aircraft::GetLimit(int acm, cmdForm *p_rtn)
{
	int i, j;
	int maxSpeed = 0;
	float maxAlt = 0.0f;

	for (i = 0; aircraftModels[acm].altIndex[i] >= 0.0f; i++) {
		if (m_loaded) {
			for (j = 0; aircraftModels[acm].spdIncTbl[1][i][j] != NA; j++) {
				if (j > maxSpeed) {
					maxSpeed = j;
				}
			}
		} else {
			for (j = 0; aircraftModels[acm].spdIncTbl[0][i][j] != NA; j++) {
				if (j > maxSpeed) {
					maxSpeed = j;
				}
			}
		}
	}
	for (i =0; aircraftModels[acm].altIndex[i] >= 0.0f; i++) {
		if (maxAlt < aircraftModels[acm].altIndex[i]) {
			maxAlt = aircraftModels[acm].altIndex[i];
		}
	}

	p_rtn->maxSpeedAtAnyAlt = maxSpeed;
	p_rtn->maxAlt = maxAlt;

}

int Aircraft::GetAltTblIndex()
{
	int i;

	for (i = 0; 
		aircraftModels[mAircraftModel].altIndex[i] > 0;
		i++) {
		if (aircraftModels[mAircraftModel].altIndex[i] >= m_alt) {
			return i;
		}
	}

	return -1;
}

spdIncTblEntry Aircraft::ReferSpeedIncTbl()
{
	int altTblIndex = GetAltTblIndex();

	if (altTblIndex < 0) {
		MessageBox(NULL, 
  			L"Error: ReferSpeedIncTbl: failed to get altTblIndex..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}

	if (m_loaded) {
		return aircraftModels[mAircraftModel].spdIncTbl[1][altTblIndex][m_speed];
	} else {
		return aircraftModels[mAircraftModel].spdIncTbl[0][altTblIndex][m_speed];
	}
}

float Aircraft::GetMaxAltChgClimb(int spdIncIndex)
{
	int altTblIndex = GetAltTblIndex();
	int i;
	float maxClimb = 0.0f;

	for (i = 0; 
	     aircraftModels[mAircraftModel].altChgTblClimb[spdIncIndex][altTblIndex][i] > 0;
	     i++) {
		maxClimb = aircraftModels[mAircraftModel]
			.altChgTblClimb[spdIncIndex][altTblIndex][i];
	}
	return maxClimb;
}

float Aircraft::GetMaxAltChgDive(int spdIncIndex)
{
	int altTblIndex = GetAltTblIndex();
	int i;
	float maxDive = 0.0f;

	for (i = 0; 
	     aircraftModels[mAircraftModel].altChgTblDive[altTblIndex][i] > 0;
	     i++) {
		maxDive = aircraftModels[mAircraftModel]
			.altChgTblDive[altTblIndex][i];
	}
	return maxDive;
}

int Aircraft::GetMaxPower()
{
	int altTblIndex = GetAltTblIndex();

	return aircraftModels[mAircraftModel].speedChgTbl[0][altTblIndex];
}

int Aircraft::GetMaxBreak()
{
	int altTblIndex = GetAltTblIndex();

	return aircraftModels[mAircraftModel].speedChgTbl[1][altTblIndex];
}

int Aircraft::GetMaxDiveSpeed()
{
	int altTblIndex = GetAltTblIndex();

	int i;

	for (i = 0; 
		aircraftModels[mAircraftModel].spdIncTbl[m_loaded][altTblIndex][i] >= STAL;
	       	i++) {
	}

	return i;
}

void Aircraft::modifyManeuverableByBank(maneuverable *rtn)
{
	switch (m_bank) {
		case BANK_IR:
			rtn->turnLeft = -1;
			break;
		case BANK_BR:
			rtn->turnLeft = -1;
			break;
		case BANK_LV:
			rtn->turnLeft = -1;
			rtn->turnRight = -1;
			rtn->slipLeft = -1;
			rtn->slipRight = -1;
			break;
		case BANK_BL:
			rtn->turnRight = -1;
			break;
		case BANK_IL:
			rtn->turnRight = -1;
			break;
		case BANK_IV:
			rtn->turnLeft = -1;
			rtn->turnRight = -1;
			rtn->slipLeft = -1;
			rtn->slipRight = -1;
			break;
		default:
			break;
	}
}

void Aircraft::getPrevTwoManuv(int *last, int *secondLast)
{
	int i = 0;

	*last = MANUV_EN;
	*secondLast = MANUV_EN;

	for (i = 0; m_manuv[i] != MANUV_EN; i++) {
		if (((m_manuv[i] < 0) && (m_manuv[i] > MANUV_PW)) 
		||  ((m_manuv[i] >= 0) && (m_manuv[i] < 20))){
			*last = m_manuv[i];
	        	*secondLast = *last;	
		}
	}

	if (*secondLast == MANUV_EN) { // need to go back to log data of previous GameTurns
		list<shared_ptr<cmdForm>>::iterator itr;

		for (itr = m_logs.begin(); itr != m_logs.end(); itr++) {
			for (i = 0; (*itr)->manuv[i] != MANUV_EN; i++) {
				if ((((*itr)->manuv[i] < 0) && ((*itr)->manuv[i] > MANUV_PW)) 
				||  (((*itr)->manuv[i] >= 0) && ((*itr)->manuv[i] < 20))){
					*last = m_manuv[i];
	        			*secondLast = *last;	
				}
				if (*secondLast != MANUV_EN) {
					return;
				}
			}
		}
	}
}

void Aircraft::modifyManeuverableByPrevManuv(maneuverable *rtn) 
{
	int lastManuv;
	int secondLastManuv;

	getPrevTwoManuv(&lastManuv, &secondLastManuv);

	if (lastManuv == MANUV_SL) {
		if (rtn->turnRight >= 0) {
			rtn->turnRight = 0;
		}
	}
	if (lastManuv == MANUV_SR) {
		if (rtn->turnLeft >= 0) {
			rtn->turnLeft = 0;
		}
	}
	if ((secondLastManuv == MANUV_SR) 
	&&  (lastManuv == MANUV_TL)){
		if (rtn->turnLeft >= 0) {
			rtn->turnLeft = 0;
		}
	}
	if ((secondLastManuv == MANUV_SL) 
	&&  (lastManuv == MANUV_TR)){
		if (rtn->turnRight >= 0) {
			rtn->turnRight = 0;
		}
	}
	if ((secondLastManuv == MANUV_DP)
	&&  (lastManuv == MANUV_DP)) {
		if (rtn->turnLeft >= 0) {
			rtn->turnLeft = 0;
		}
		if (rtn->turnRight >= 0) {
			rtn->turnRight = 0;
		}
		if (rtn->bankLeft >= 0) {
			rtn->bankLeft = 0;
		}
		if (rtn->bankRight >= 0) {
			rtn->bankRight = 0;
		}
		if (rtn->slipLeft >= 0) {
			rtn->slipLeft = 0;
		}
		if (rtn->slipRight >= 0) {
			rtn->slipRight = 0;
		}
		if (rtn->rollLeft >= 0) {
			rtn->rollLeft = 0;
		}
		if (rtn->rollRight >= 0) {
			rtn->rollRight = 0;
		}
		if (rtn->loopClimb >= 0) {
			rtn->loopClimb = 0;
		}
		if (rtn->loopDive >= 0) {
			rtn->loopDive = 0;
		}
	}
	if ((0 < lastManuv) && (lastManuv < 20)) {
		int straight = 0;
		int tmp;
		if ((0 < secondLastManuv) && (secondLastManuv)) {
			straight = lastManuv + secondLastManuv;
		} else {
			straight = lastManuv;
		}			

		if (rtn->turnLeft >= 0) {
			tmp = rtn->turnLeft - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->turnLeft = tmp;
		}
		if (rtn->turnRight >= 0) {
			tmp = rtn->turnRight - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->turnRight = tmp;
		}
		if (rtn->bankLeft >= 0) {
			tmp = rtn->bankLeft - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->bankLeft = tmp;
		}
		if (rtn->bankRight >= 0) {
			tmp = rtn->bankRight - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->bankRight = tmp;
		}
		if (rtn->slipLeft >= 0) {
			tmp = rtn->slipLeft - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->slipLeft = tmp;
		}
		if (rtn->slipRight >= 0) {
			tmp = rtn->slipRight - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->slipRight = tmp;
		}
		if (rtn->rollLeft >= 0) {
			tmp = rtn->rollLeft - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->rollLeft = tmp;
		}
		if (rtn->rollRight >= 0) {
			tmp = rtn->rollRight - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->rollRight = tmp;
		}
		if (rtn->loopClimb >= 0) {
			tmp = rtn->loopClimb - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->loopClimb = tmp;
		}
		if (rtn->loopDive >= 0) {
			tmp = rtn->loopDive - straight;
			if (tmp < 0) {
				tmp = 0;
			}
			rtn->loopDive = tmp;
		}
	}
}


int Aircraft::getMP()
{
	int mp;

	if (mPilotModel == 3) { // Random
		mp = m_speed + m_pilotReflex;
	} else {
		mp = m_speed ;
	}

	return mp;
}

maneuverable Aircraft::ReferManuvReqTblSpdInc(int spdIncIndex)
{
	int altTblIndex = GetAltTblIndex();
	maneuverable	rtn;
	int loaded;

	if (m_loaded) {
		loaded = 5;
	} else {
		loaded = 0;
	}

	if (spdIncIndex == -1) { // spcInc = STAL
		rtn.turnLeft = -1;
		rtn.turnRight = -1;
		rtn.bankLeft = -1;
		rtn.bankRight = -1;
		rtn.slipLeft = -1;
		rtn.slipRight = -1;
		rtn.rollLeft = -1;
		rtn.rollRight = -1;
		rtn.loopClimb = -1;
		rtn.loopDive = -1;
		rtn.maxClimb = 0.0f;
		rtn.maxDive = 0.0f;
		rtn.maxPower = 0;
		rtn.maxBreak = 0;
		rtn.dropBomb = false;
		rtn.fireRocket = false;
		rtn.fire= false;
		rtn.remainingMP = getMP();
		rtn.maxDiveSpeed = GetMaxDiveSpeed();
		rtn.mustJettson = false;
		rtn.destroyed = false;
	} else if (spdIncIndex == 0    // spdInc = MANV
		|| spdIncIndex == 1    // spdInc = LEVL
		|| spdIncIndex == 2) { // spdInc = DIVE
		rtn.turnLeft = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][0+loaded];
		rtn.turnRight = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][0+loaded];
		rtn.bankLeft = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][1+loaded];
		rtn.bankRight = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][1+loaded];
		rtn.slipLeft = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][2+loaded];
		rtn.slipRight = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][2+loaded];
		rtn.rollLeft = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][3+loaded];
		rtn.rollRight = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][3+loaded];
		rtn.loopClimb = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][4+loaded];
		rtn.loopDive = aircraftModels[mAircraftModel]
			.manuvReqTbl[spdIncIndex][altTblIndex][4+loaded];
		rtn.maxClimb = GetMaxAltChgClimb(spdIncIndex);
		rtn.maxDive = GetMaxAltChgDive(spdIncIndex);
		rtn.maxPower = GetMaxPower();
		rtn.maxBreak = GetMaxBreak();
		rtn.dropBomb = false;
		rtn.fireRocket = false;
		rtn.fire= false;
		rtn.remainingMP = getMP();
		rtn.maxDiveSpeed = GetMaxDiveSpeed();
		rtn.mustJettson = false;
		rtn.destroyed = false;
	} else {                       // spdInc = else
		MessageBox(NULL, 
  			L"Error: ReferManuvReqTblSpdInc: illegal speed increments : NA..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}

//	modifyManeuverableByBank(&rtn);
//	 
	modifyManeuverableByPrevManuv(&rtn);

	return rtn;
}

maneuverable Aircraft::ReferManuvReqTbl()
{
	spdIncTblEntry spdInc = ReferSpeedIncTbl();
	int altTblIndex = GetAltTblIndex();
	int i;
	maneuverable	rtn;

	switch (spdInc) {
		case STAL:
			i = -1;
			break;
		case MANV:
			i = 0;
			break;
		case LEVL:
			i = 1;
			break;
		case DIVE:
			i = 2;
			break;
		case NA:
			MessageBox(NULL, 
  				L"Error: ReferManuvReqTbl: illegal speed increments : NA..\n",
				NULL,
				MB_OKCANCEL | MB_ICONSTOP
			);
			break;
		default:
			MessageBox(NULL, 
  				L"Error: ReferManuvReqTbl: illegal speed increments : default..\n",
				NULL,
				MB_OKCANCEL | MB_ICONSTOP
			);
			break;
	}

	return ReferManuvReqTblSpdInc(i);
}	

void Aircraft::modifyManeuverableByDamageWing(maneuverable *p_manuvable) 
{
	int numEngine = 0;
	int i;
	for (i = 0; i < 4; i++) {
//		if (m_damage.tolerance.engine[i] >= 0) {
		if (aircraftModels[mAircraftModel].damageTolerance.engine[i] >= 0) {
			numEngine ++;
		}
	}

	if (numEngine == 1) { // single engine
		int x = m_damage.hit.wing / 2;
		p_manuvable->maxDiveSpeed -= x;
	} else if (numEngine >= 2) { // multiple engines
		int x = m_damage.hit.wing / 3;
		p_manuvable->maxDiveSpeed -= x;
	} else {
		MessageBox(NULL, 
 			L"Error: illeal damageTolerance.engine.: modifyManeuverableByDamageWing..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
  
}

void Aircraft::modifyManeuverableByDamage2Engines(maneuverable *p_manuvable, int *engineDamage)
{
	if ((engineDamage[0] >= 10) 
	&&  (engineDamage[1] >= 10)) {
		(p_manuvable -> maxPower) = 0;
		(p_manuvable -> mustJettson) = true;
		(p_manuvable -> destroyed) = true;
	} else if 
	   ((engineDamage[0] >= 10) 
	||  (engineDamage[1] >= 10)) {
		(p_manuvable -> maxPower) = 0;
		(p_manuvable -> mustJettson) = true;
	} else if 
	   ((engineDamage[0] >= 2)
	&&  (engineDamage[1] >= 2)) {
		(p_manuvable -> maxPower) -= 2;
	} else if
	   ((engineDamage[0] >= 1)
	&&  (engineDamage[1] >= 1)) {
		(p_manuvable -> maxPower) -= 1;
	}
}

void Aircraft::modifyManeuverableByDamage4Engines(maneuverable *p_manuvable, int *engineDamage)
{
	int i;
	int numLeftDestroyed = 0;
	int numLeftHit = 0;
	int numRightDestroyed = 0;
	int numRightHit = 0;

	for (i = 0; i < 2; i++) {
		if (engineDamage[i] >= 10) {
			numLeftDestroyed ++;
		} else if (engineDamage[i] > 0) {
			numLeftHit ++;
		}
	}
	for (i = 2; i < 4; i++) {
		if (engineDamage[i] >= 10) {
			numRightDestroyed ++;
		} else if (engineDamage[i] > 0) {
			numRightHit ++;
		}
	}

	if ((numLeftDestroyed >= 2)
	||  (numRightDestroyed >= 2)) {
		(p_manuvable -> maxPower) = 0;
		(p_manuvable -> mustJettson) = true;
		(p_manuvable -> destroyed) = true;
	} else if
	   ((numLeftDestroyed >= 1)
	&&  (numLeftHit >=1)
	&&  (numRightDestroyed >= 1)
	&&  (numRightHit >= 1)) {
		(p_manuvable -> maxPower) = 0;
		(p_manuvable -> mustJettson) = true;
		(p_manuvable -> destroyed) = true;
	} else if 
	   ((numLeftDestroyed >= 1)
	&&  (numRightDestroyed >= 1)) {
		(p_manuvable -> maxPower) -= 2;
		(p_manuvable -> mustJettson) = true;
	} else if 
	   ((numLeftDestroyed >= 1)
	||  (numRightDestroyed >= 1)) {
		(p_manuvable -> maxPower) -= 1;
	}
}

void Aircraft::modifyManeuverableByDamageEngine(maneuverable *p_manuvable)
{
	int numEngine = 0;
	int i = 0;
	int j = 0;
	int engine[4] = { -1, -1, -1, -1};

	for (i = 0; i < 4; i++) {
//		if (m_damage.tolerance.engine[i] >= 0) {
		if (aircraftModels[mAircraftModel].damageTolerance.engine[i] >= 0) {
//			if (m_damage.hit.engine[i] >= m_damage.tolerance.engine[i]) {
			if (m_damage.hit.engine[i] >= aircraftModels[mAircraftModel].damageTolerance.engine[i]) {
				engine[j] = 100;
			} else if (m_damage.hit.engine[i] >= 2) {
				engine[j] = 2;
			} else if (m_damage.hit.engine[i] >= 1) {
				engine[j] = 1;
			} else {
				engine[j] = 0;
			}
			numEngine ++;
			j++;

		}
	}

	switch (numEngine) {
		case 1:
			(p_manuvable -> maxPower) -= engine[0];
			break;
		case 2:
			modifyManeuverableByDamage2Engines(p_manuvable, engine);
			break;
		case 4:
			modifyManeuverableByDamage4Engines(p_manuvable, engine);
			break;
	}

}

int Aircraft::getDamageCockpit()
{

//	if ((m_damage.tolerance.cockpit[0] >= 0)
	if ((aircraftModels[mAircraftModel].damageTolerance.cockpit[0] >= 0)
//	&&  (m_damage.tolerance.cockpit[1] >= 0)) { // dual cockpit
	&& (aircraftModels[mAircraftModel].damageTolerance.cockpit[1] >= 0)) { // dual cockpit
		if (aircraftModels[mAircraftModel].damageTolerance.cockpit[0] <= m_damage.hit.cockpit[0]) {
			return m_damage.hit.cockpit[1];
		} 
		if (aircraftModels[mAircraftModel].damageTolerance.cockpit[1] <= m_damage.hit.cockpit[1]) {
			return m_damage.hit.cockpit[0];
		} 
		return 0;
	} else if (aircraftModels[mAircraftModel].damageTolerance.cockpit[0] >= 0) { // single cockpit[0]
		return m_damage.hit.cockpit[0];
	} else if (aircraftModels[mAircraftModel].damageTolerance.cockpit[1] >= 0) { // single cockpit[1]
		return m_damage.hit.cockpit[1];
	} else {
		MessageBox(NULL, 
 			L"Error: illeal m_damage.tolerance.cockpit.: getDamageCockpit..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
	return -1;	
}

void Aircraft::modifyManeuverableByDamage(maneuverable  *p_manuvable)
{
	modifyManeuverableByDamageWing(p_manuvable);

	int j = getDamageCockpit();
	if (j >= 1) {
		p_manuvable -> loopClimb = -1;
		p_manuvable -> loopDive = -1;
		p_manuvable -> rollLeft = -1;
		p_manuvable -> rollRight = -1;
	}
	if (j >= 2) {
		p_manuvable -> slipLeft = -1;
		p_manuvable -> slipRight = -1;
		p_manuvable -> maxDiveSpeed = -1;
	}

	modifyManeuverableByDamageEngine(p_manuvable);

}

void Aircraft::modifyManeuverableByPilot(maneuverable *p_manuvable)
{
	if (mPilotModel == 0) { //  Novice
		if (p_manuvable->turnLeft >= 0) {
			p_manuvable->turnLeft ++;
		}
		if (p_manuvable->turnRight >= 0) {
			p_manuvable->turnRight ++;
		}
		if (p_manuvable->bankLeft >= 0) {
			p_manuvable->bankLeft ++;
		}
		if (p_manuvable->bankRight >= 0) {
			p_manuvable->bankRight ++;
		}
		if (p_manuvable->slipLeft >= 0) {
			p_manuvable->slipLeft ++;
		}
		if (p_manuvable->slipRight >= 0) {
			p_manuvable->slipRight ++;
		}
		if (p_manuvable->rollLeft >= 0) {
			p_manuvable->rollLeft ++;
		}
		if (p_manuvable->rollRight >= 0) {
			p_manuvable->rollRight ++;
		}
		if (p_manuvable->loopClimb >= 0) {
			p_manuvable->loopClimb ++;
		}
		if (p_manuvable->loopDive >= 0) {
			p_manuvable->loopDive ++;
		}
	}

	if (mPilotModel == 3) { // Random
		p_manuvable->remainingMP += m_pilotReflex;
	}
}

int Aircraft::ReferFiringRangeACModelGunPosition(int acm, int gunPosition, int gunType)
{
	int rtn = -1;
	int i;

	for (i = 0; i < 8; i++) {
		if (aircraftModels[acm].gunPosition[i] == gunPosition) {
			if (aircraftModels[acm].gunPosition[i] == gP_FF) {
				if (aircraftModels[acm].gunType[i] == gunType) {
					if (aircraftModels[acm].gunRange[i] > rtn) {
						rtn = aircraftModels[acm].gunRange[i];
					}
				}
			} else {
				if (aircraftModels[acm].gunRange[i] > rtn) {
					rtn = aircraftModels[acm].gunRange[i];
				}
			}
		}
	}
	return rtn;
}

void Aircraft::ReferFiringRangeACModelGunF(int acm, firingArcRange *p_range)
{
	int i, j;

	for (i = 0; i < 8; i++) {
		p_range->rangeF[i] = -1; 		// initialize to -1
	}

	for (i = 0; i < 6; i++) {			// 2, 4, 6, 8, 10, 12 oclock
		for (j = 0; j < 3; j++) {
			if (aircraftModels[acm].fgunPower[j][i] > 0) {
				p_range->rangeF[i] = ReferFiringRangeACModelGunPosition
					     (acm, gP_F, gT_Non);
			}
		}
	}
	if (aircraftModels[acm].fgunPower[3][0] > 0) {	// above
		p_range->rangeF[6] = ReferFiringRangeACModelGunPosition(acm, gP_F, gT_Non);
	}
	if (aircraftModels[acm].fgunPower[3][1] > 0) {	// below
		p_range->rangeF[7] = ReferFiringRangeACModelGunPosition(acm, gP_F, gT_Non);
	}
}

firingArcRange Aircraft::ReferFiringRangeACModel(int acm)
{
	firingArcRange	rtn;

	rtn.rangeFFmg = ReferFiringRangeACModelGunPosition(acm, gP_FF, gT_MG);
	rtn.rangeFFcannon = ReferFiringRangeACModelGunPosition(acm, gP_FF, gT_CN);
	rtn.rangeSameHexFF = false;
	rtn.rangeFH = ReferFiringRangeACModelGunPosition(acm, gP_FH, gT_Non);
	rtn.rangeSameHexFH = false;
	rtn.rangeFL = ReferFiringRangeACModelGunPosition(acm, gP_FL, gT_Non);
	rtn.rangeSameHexFL = false;

	ReferFiringRangeACModelGunF(acm, &rtn);

	return rtn;
}

firingArcRange Aircraft::ReferFiringRange()
{
	firingArcRange	rtn;
	int		acm = mAircraftModel;

	rtn = ReferFiringRangeACModel(acm);

	return rtn;
}	

int Aircraft::ReferGunPowerACModelGunPosition(int acm, int gunPosition, int gunType)
{
	int rtn = 0;
	int i;

	for (i = 0; i < 8; i++) {
		if (aircraftModels[acm].gunPosition[i] == gunPosition) {
			if (aircraftModels[acm].gunPosition[i] == gP_FF) {
				if (aircraftModels[acm].gunType[i] == gunType) {
					rtn += aircraftModels[acm].gunPower[i];
				}
			} else {
				rtn = aircraftModels[acm].gunPower[i];
			}
		}
	}
	return rtn;
}

void Aircraft::ReferGunPowerACModelGunF(int acm, gunPower *p_gunPower)
{
	int i, j;

	for (i = 0; i < 4; i++) {			// 2, 4, 6, 8, 10, 12 oclock
		for (j = 0; j < 6; j++) {
			p_gunPower->gunPowerF[i][j] 
				= aircraftModels[acm].fgunPower[i][j];
		}
	}
}

gunPower Aircraft::ReferGunPowerACModel(int acm)
{
	gunPower	rtn;

	rtn.gunPowerFFmg = ReferGunPowerACModelGunPosition(acm, gP_FF, gT_MG);
	rtn.gunPowerFFcannon = ReferGunPowerACModelGunPosition(acm, gP_FF, gT_CN);
	rtn.gunPowerFH = ReferGunPowerACModelGunPosition(acm, gP_FH, gT_Non);
	rtn.gunPowerFL = ReferGunPowerACModelGunPosition(acm, gP_FL, gT_Non);

	ReferGunPowerACModelGunF(acm, &rtn);

	return rtn;
}

void Aircraft::ModifyGunPowerByBank(gunPower *p_gunPower)
{
	switch (m_bank) {
		case BANK_IV:
		case BANK_IL:
		case BANK_IR:
			p_gunPower->gunPowerFH = 0;
			p_gunPower->gunPowerFL = 0;
			break;
	}
}

void Aircraft::ModifyGunPowerByAmmo(gunPower *p_gunPower)
{
	int 	acm = mAircraftModel;
	int	i;

	if (m_ammo.fired.cannon >= m_ammo.payload.cannon) {
		p_gunPower->gunPowerFFcannon = 0;

		for (i =0; i < 8; i++) {
			if (aircraftModels[acm].gunType[i] == gT_CN) {
				if (aircraftModels[acm].gunPosition[i] == gP_FH) {
					p_gunPower->gunPowerFH = 0;
				}
				if (aircraftModels[acm].gunPosition[i] == gP_FL) {
					p_gunPower->gunPowerFL = 0;
				}
			}
		}
	}
	if (m_ammo.fired.mg >= m_ammo.payload.mg) {
		p_gunPower->gunPowerFFmg = 0;

		for (i =0; i < 8; i++) {
			if (aircraftModels[acm].gunType[i] == gT_MG) {
				if (aircraftModels[acm].gunPosition[i] == gP_FH) {
					p_gunPower->gunPowerFH = 0;
				}
				if (aircraftModels[acm].gunPosition[i] == gP_FL) {
					p_gunPower->gunPowerFL = 0;
				}
			}
		}
	}
}

void Aircraft::ModifyGunPowerByDamage(gunPower *p_gunPower)
{
	int 	acm = mAircraftModel;
	int	i;

	for (i =0; i < 8; i++) {
		switch (aircraftModels[acm].gunPosition[i]) {
			case gP_FF:
				if (aircraftModels[acm].gunType[i] == gT_CN) {
					if (m_damage.hit.gun[i] > 0) {
						p_gunPower->gunPowerFFcannon
							-= aircraftModels[acm].gunPower[i];
					}
				}
				if (aircraftModels[acm].gunType[i] == gT_MG) {
					if (m_damage.hit.gun[i] > 0) {
						p_gunPower->gunPowerFFmg
							-= aircraftModels[acm].gunPower[i];
					}
				}
				break;
			case gP_FH:
				if (m_damage.hit.gun[i] > 0) {
					p_gunPower->gunPowerFH
						-= aircraftModels[acm].gunPower[i];
				}
				break;
			case gP_FL:
				if (m_damage.hit.gun[i] > 0) {
					p_gunPower->gunPowerFL
						-= aircraftModels[acm].gunPower[i];
				}
				break;
			case gP_F:
				if (m_damage.hit.gun[i] > 0) {
					int	j, k;
					for (j = 0; j < 4; j++) {
						for (k = 0; k < 6; k++) {
							p_gunPower->gunPowerF[j][k]
								-= m_damage.hit.gun[i];
						}
					}	
				}
				break;
		}
	}
}

gunPower Aircraft::ReferGunPower()
{
	gunPower	rtn;
	int		acm = mAircraftModel;

	rtn = ReferGunPowerACModel(acm);
	ModifyGunPowerByBank(&rtn);
	ModifyGunPowerByAmmo(&rtn);
	ModifyGunPowerByDamage(&rtn);

	return rtn;
}	

//////////////////////////////////////////////////////////////////////////////
//
//	PlayerAirForce.cpp
//
//////////////////////////////////////////////////////////////////////////////

void PlayerAirForce::cmdToPlayerSetAcstat(cmdForm form, cmdForm *p_rtn)
{
	std::list<std::shared_ptr<Aircraft>>::iterator itr;

	if (form.selectedAircraft == SELECTED_AC) {
		(*m_ItrSelectedAircraft)->m_stat = form.acStatus;
		(*m_ItrSelectedAircraft)->m_heading = form.heading;
		(*m_ItrSelectedAircraft)->m_speed = form.speed;
		(*m_ItrSelectedAircraft)->m_alt = form.alt;
		(*m_ItrSelectedAircraft)->m_bank = form.bank;
		(*m_ItrSelectedAircraft)->m_nose = form.nose;
	} else if (form.selectedAircraft == ALL_AC) {
		for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
			(*itr)->m_stat = form.acStatus;
			(*itr)->m_heading = form.heading;
			(*itr)->m_speed = form.speed;
			(*itr)->m_alt = form.alt;
			(*itr)->m_bank = form.bank;
			(*itr)->m_nose = form.nose;
		}
	} else {
	}

}


void PlayerAirForce::cmdToPlayerGetManuvable(
	cmdForm form, 
	cmdForm *p_rtn, 
	list<std::shared_ptr<Aircraft>>::iterator itr)
{
	maneuverable manuv;
	manuv = (*itr)->ReferManuvReqTbl();
	(*itr)->modifyManeuverableByDamage(&manuv);
	(*itr)->modifyManeuverableByPilot(&manuv);
	p_rtn->manuvable = manuv;

	p_rtn->virCorX = (*itr)->m_virCorX;
	p_rtn->virCorY = (*itr)->m_virCorY;
	p_rtn->heading = (*itr)->m_heading;
	p_rtn->speed = (*itr)->m_speed;
	p_rtn->alt = (*itr)->m_alt;
	p_rtn->bank = (*itr)->m_bank;
	p_rtn->nose = (*itr)->m_nose;
	int i;
	for (i = 0; i < 80; i++) {
		p_rtn->manuv[i] = (*itr)->m_manuv[i];
	}

	return;
}


void PlayerAirForce::cmdToPlayerGetManuvables(cmdForm form, cmdForm *p_rtn)
{
	std::list<std::shared_ptr<Aircraft>>::iterator itr;
	int i = 0;

	if (form.selectedAircraft == SELECTED_AC) {
		cmdToPlayerGetManuvable(form, p_rtn, m_ItrSelectedAircraft);
		i++;
		p_rtn[i].command = TAIL;
	} else if (form.selectedAircraft == ALL_AC) {
		for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
			cmdToPlayerGetManuvable(form, &(p_rtn[i]), itr);
			i++;
		}
		p_rtn[i].command = TAIL;
	}
}

void PlayerAirForce::cmdToPlayerAppendManuv(cmdForm form, 
	list<std::shared_ptr<Aircraft>>::iterator itr)
{
	int i;
	for (i = 0; (*itr)->m_manuv[i] != MANUV_EN; i++) {
	}
//	i = i -1;

	int j;
	for (j = 0; form.manuv[j] != MANUV_EN; j++) {
		(*itr)->m_manuv[i] = form.manuv[j];
		i++;
	}
	(*itr)->m_manuv[i] = MANUV_EN;
}

void PlayerAirForce::cmdToPlayerAppendManuvs(cmdForm form, cmdForm *p_rtn)
{
	std::list<std::shared_ptr<Aircraft>>::iterator itr;
	int i = 0;

	if (form.selectedAircraft == SELECTED_AC) {
		cmdToPlayerAppendManuv(form, m_ItrSelectedAircraft);
	} else if (form.selectedAircraft == ALL_AC) {
		for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
			cmdToPlayerAppendManuv(form, itr);
		}
	}
}

void PlayerAirForce::cmdToPlayerDeleteManuv(cmdForm form, 
	list<std::shared_ptr<Aircraft>>::iterator itr)
{
	int i;
	for (i = 0; (*itr)->m_manuv[i] != MANUV_EN; i++) {
	}

	// as for the "for" statement, when the loop condition is not met,
	// then i++ is not executed.
	if (i <= 0) {
		return;
	} else {
		i = i - 1;
	}

	if ((*itr)->m_manuv[i] == MANUV_DP) {
		return;
	} else {
		(*itr)->m_manuv[i] = MANUV_EN;
	}
}

void PlayerAirForce::cmdToPlayerDeleteManuvs(cmdForm form, cmdForm *p_rtn)
{
	std::list<std::shared_ptr<Aircraft>>::iterator itr;
	int i = 0;

	if (form.selectedAircraft == SELECTED_AC) {
		cmdToPlayerDeleteManuv(form, m_ItrSelectedAircraft);
	} else if (form.selectedAircraft == ALL_AC) {
		for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
			cmdToPlayerDeleteManuv(form, itr);
		}
	}
}

void PlayerAirForce::cmdToPlayerGetFireRange(
	cmdForm form, 
	cmdForm *p_rtn, 
	list<std::shared_ptr<Aircraft>>::iterator itr)
{
	firingArcRange	range;
	gunPower	gPower;

	range = (*itr)->ReferFiringRange();
	p_rtn->firingRange = range;

	gPower = (*itr)->ReferGunPower();
	p_rtn->gunPower = gPower;

	return;
}


void PlayerAirForce::cmdToPlayerGetFireRanges(cmdForm form, cmdForm *p_rtn)
{
	std::list<std::shared_ptr<Aircraft>>::iterator itr;
	int i = 0;

	if (form.selectedAircraft == SELECTED_AC) {
		cmdToPlayerGetFireRange(form, p_rtn, m_ItrSelectedAircraft);
		i++;
		p_rtn[i].command = TAIL;
	} else if (form.selectedAircraft == ALL_AC) {
		for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
			cmdToPlayerGetFireRange(form, &(p_rtn[i]), itr);
			i++;
		}
		p_rtn[i].command = TAIL;
	}
}

void PlayerAirForce::cmdToPlayerDispatch(int cmd, cmdForm form, cmdForm *p_rtn) 
{
	if (m_ItrSelectedAircraft != mAircrafts.end()) {
		int acm = (*m_ItrSelectedAircraft)->mAircraftModel;
		int i;

		(*m_ItrSelectedAircraft)->m_stat = form.acStatus;
		(*m_ItrSelectedAircraft)->m_virCorX = form.virCorX;
		(*m_ItrSelectedAircraft)->m_virCorY = form.virCorY;
		(*m_ItrSelectedAircraft)->m_heading = form.heading;
		(*m_ItrSelectedAircraft)->m_speed = form.speed;
		(*m_ItrSelectedAircraft)->m_alt = form.alt;
		(*m_ItrSelectedAircraft)->m_bank = form.bank;
		(*m_ItrSelectedAircraft)->m_nose = form.nose;

		(*m_ItrSelectedAircraft)->m_ammo.payload.cannon 
			= aircraftModels[acm].ammoPayload.cannon;
		(*m_ItrSelectedAircraft)->m_ammo.payload.mg 
			= aircraftModels[acm].ammoPayload.mg;

		(*m_ItrSelectedAircraft)->m_damage.tolerance.wing 
			= aircraftModels[acm].damageTolerance.wing;
		(*m_ItrSelectedAircraft)->m_damage.tolerance.fuselage 
			= aircraftModels[acm].damageTolerance.fuselage;
		for (i = 0; i < 2; i++) {
			(*m_ItrSelectedAircraft)->m_damage.tolerance.cockpit[i] 
				= aircraftModels[acm].damageTolerance.cockpit[i];
		}
		for (i = 0; i < 4; i++) {
			(*m_ItrSelectedAircraft)->m_damage.tolerance.engine[i] 
				= aircraftModels[acm].damageTolerance.engine[i];
		}
		for (i = 0; i < 8; i++) {
			(*m_ItrSelectedAircraft)->m_damage.tolerance.gun[i] 
				= aircraftModels[acm].damageTolerance.gun[i];
		}
		(*m_ItrSelectedAircraft)->m_damage.tolerance.fueltank 
			= aircraftModels[acm].damageTolerance.fueltank;

	} else {
		MessageBox(NULL, 
      			L"Error: no aircraft selected.: cmdToPlayer..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
}

void PlayerAirForce::cmdToPlayer(int cmd, cmdForm form, cmdForm *p_rtn)
{

	std::list<std::shared_ptr<Aircraft>>::iterator itr;
	int i = 0;

	switch (cmd) {
		case DISPATCH:
			cmdToPlayerDispatch(DISPATCH, form, p_rtn); 

//			if (m_ItrSelectedAircraft != mAircrafts.end()) {
//				(*m_ItrSelectedAircraft)->m_stat = form.acStatus;
//				(*m_ItrSelectedAircraft)->m_virCorX = form.virCorX;
//				(*m_ItrSelectedAircraft)->m_virCorY = form.virCorY;
//				(*m_ItrSelectedAircraft)->m_heading = form.heading;
//				(*m_ItrSelectedAircraft)->m_speed = form.speed;
//				(*m_ItrSelectedAircraft)->m_alt = form.alt;
//				(*m_ItrSelectedAircraft)->m_bank = form.bank;
//				(*m_ItrSelectedAircraft)->m_nose = form.nose;
//			} else {
//				MessageBox(NULL, 
//	       				L"Error: no aircraft selected.: cmdToPlayer..\n",
//					NULL,
//					MB_OKCANCEL | MB_ICONSTOP
//				);
//			}
			break;

		case GET_DISPATCHED:
		case GET_AC:
		case GET_HIT:
			if ((form.selectedAircraft == SELECTED_AC) 
			&& ( ((cmd == GET_DISPATCHED)
				   && ((*m_ItrSelectedAircraft)->m_stat == DISPATCHED
				     ||(*m_ItrSelectedAircraft)->m_stat == DEPLOYED 
				      )
			     )
			   || (cmd == GET_AC) 
			   ||((cmd == GET_HIT)
				   && form.virCorX == (*m_ItrSelectedAircraft)->m_virCorX
				   && form.virCorY == (*m_ItrSelectedAircraft)->m_virCorY
			     )
			   )
			)
			{
				p_rtn[i].command = GET_DISPATCHED;
				p_rtn[i].playerID = mPlayerID;
				p_rtn[i].aircraftModel = (*m_ItrSelectedAircraft)->mAircraftModel;
				p_rtn[i].pilotModel = (*m_ItrSelectedAircraft)->mPilotModel;
//				Syntax:  wcscpy(p_destination, p_source)
				wcscpy(p_rtn[i].aircraftName, (*m_ItrSelectedAircraft)->mAircraftName);
				p_rtn[i].acStatus = (*m_ItrSelectedAircraft)->m_stat;
				p_rtn[i].virCorX = (*m_ItrSelectedAircraft)->m_virCorX;
				p_rtn[i].virCorY = (*m_ItrSelectedAircraft)->m_virCorY;
				p_rtn[i].heading = (*m_ItrSelectedAircraft)->m_heading;
				p_rtn[i].speed = (*m_ItrSelectedAircraft)->m_speed;
				p_rtn[i].alt = (*m_ItrSelectedAircraft)->m_alt;
				p_rtn[i].bank = (*m_ItrSelectedAircraft)->m_bank;
				p_rtn[i].nose = (*m_ItrSelectedAircraft)->m_nose;
				int j;
				for (j = 0; j < 80; j++) {
					p_rtn[i].manuv[j] = (*m_ItrSelectedAircraft)->m_manuv[j];
				}
				i++;
				p_rtn[i].command = TAIL;
				
			} else if (form.selectedAircraft == ALL_AC) {
				for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
					if (((cmd == GET_DISPATCHED) 
						&& ((*itr)->m_stat == DISPATCHED
						||  (*itr)->m_stat == DEPLOYED)	
					   	)
					   || (cmd == GET_AC)
					   ||((cmd == GET_HIT)
						   && form.virCorX == (*itr)->m_virCorX
						   && form.virCorY == (*itr)->m_virCorY
					     )
					   )
					 {
						p_rtn[i].command = GET_DISPATCHED;
						p_rtn[i].playerID = mPlayerID;
						p_rtn[i].aircraftModel = (*itr)->mAircraftModel;
						p_rtn[i].pilotModel = (*itr)->mPilotModel;
//						Syntax:  wcscpy(p_destination, p_source)
						wcscpy(p_rtn[i].aircraftName, (*itr)->mAircraftName);
						p_rtn[i].acStatus = (*itr)->m_stat;
						p_rtn[i].virCorX = (*itr)->m_virCorX;
						p_rtn[i].virCorY = (*itr)->m_virCorY;
						p_rtn[i].heading = (*itr)->m_heading;
						p_rtn[i].speed = (*itr)->m_speed;
						p_rtn[i].alt = (*itr)->m_alt;
						p_rtn[i].bank = (*itr)->m_bank;
						p_rtn[i].nose = (*itr)->m_nose;
						int j;
						for (j = 0; j < 80; j++) {
							p_rtn[i].manuv[j] = (*itr)->m_manuv[j];
						}
	
						i++;
					}
				}
				p_rtn[i].command = TAIL;
			} else {
			}
			break;

		case REPAINT:
	        	InvalidateRect(m_hwnd, NULL, FALSE);
			break;
		case GET_LIMIT:
			if (form.selectedAircraft == SELECTED_AC) {
				int	acm;
				acm = (*m_ItrSelectedAircraft)->mAircraftModel;
				(*m_ItrSelectedAircraft)->GetLimit(acm, p_rtn) ;
				i++;
				p_rtn[i].command = TAIL;
			} else if (form.selectedAircraft == ALL_AC) {
				for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
					(*itr)->GetLimit((*itr)->mAircraftModel,
							&p_rtn[i]);
					i++;
				}
				p_rtn[i].command = TAIL;
			} else {
			}
			break;

		case SET_ACSTAT:
			cmdToPlayerSetAcstat(form, p_rtn);
			break;

		case SET_AC_SELECTED:
			for (itr = mAircrafts.begin(); itr != mAircrafts.end(); itr++) {
				if (wcscmp(form.aircraftName, (*itr)->mAircraftName) == 0) {
					shared_ptr<Aircraft> ptrAircraft = (*itr);
					mAircrafts.remove(ptrAircraft);
					mAircrafts.push_front(ptrAircraft);
					m_ItrSelectedAircraft = mAircrafts.begin();
					break;
					// This break is important!!
					// After removing and pushing the element,
					// control goes back to the "for (itr = mAircrafs.begin(); itr != .....
					// statement. But something is wrong with itr and
					// error occurs. To avoid another itr test, break is necessary.
					// 17/07/17
				}
			}
			break;

		case GET_MANUVABLE:
			cmdToPlayerGetManuvables(form, p_rtn);
			break;

		case APPEND_MANUV:
			cmdToPlayerAppendManuvs(form, p_rtn);
			break;

		case DELETE_MANUV:
			cmdToPlayerDeleteManuvs(form, p_rtn);
			break;
		case GET_FIRE_RANGE:
			cmdToPlayerGetFireRanges(form, p_rtn);
			break;
	}
}


HRESULT PlayerAirForce::CreateDWriteTextFormat()
{
    static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 8;
    HRESULT hr;

        // Create a DirectWrite factory.
       hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(mPtrDWriteFactory),
            reinterpret_cast<IUnknown **>(&mPtrDWriteFactory)
            );
    if (SUCCEEDED(hr))
    {
        // Create a DirectWrite text format object.
        hr = mPtrDWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"", //locale
            &mPtrTextFormat
            );
    }
    if (SUCCEEDED(hr))
    {
        // Center the text horizontally and vertically.
        mPtrTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

        mPtrTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
       

    }

    return hr;
}


void PlayerAirForce::WhereToDraw(float *x, float *y)
{
	static float drawX = 30.0f;
	static float drawY = 30.0f;
	const float pitchX = 0.0f;
	const float pitchY = 60.0f;

	*x = drawX + pitchX;
	*y = drawY + pitchY;

	drawX = drawX + pitchX;
	drawY = drawY + pitchY;
}

void PlayerAirForce::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
	float offsetX = 30.0f;
	float offsetY = 20.0f;
	float interval = 50.0f;
	int i = 0;


        BeginPaint(m_hwnd, &ps);
     
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::WhiteSmoke) );

	std::list<std::shared_ptr<Aircraft>>::iterator itr;

	for (itr = mAircrafts.begin(); itr != mAircrafts.end(); ++itr) {
		(*itr)->Draw(pRenderTarget, pBrush, m_p_imagingFactory,
				offsetX, offsetY + interval * i,
				40.0f, 40.0f, 60.0f);
		i++;
	}

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

bool PlayerAirForce::HitTestAircrafts(float dipX, float dipY)
{
	list<shared_ptr<Aircraft>>::iterator itr;
	bool found = FALSE;

	m_ItrSelectedAircraft = mAircrafts.end();

	for (itr = mAircrafts.begin(); itr != mAircrafts.end(); ++itr) {
		if (!found && (*itr)->HitTest(dipX, dipY)) {
			m_ItrSelectedAircraft = itr;
			(*itr)->m_hilight = TRUE;
			found = TRUE;
		} else {
			(*itr)->m_hilight = FALSE;
		}
	}
	return found;
}

void PlayerAirForce::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	m_ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = m_ptMouse.x;
	const float dipY = m_ptMouse.y;

	const int playerID = mPlayerID;
	cmdForm form;
	form.command = SET_PLAYER_SELECTED;
	form.playerID = playerID;
	((GameAirForce*)mp_ownerGame)->cmdToGame(SET_PLAYER_SELECTED, form, NULL);
	// you need parenthsis like above!!

	POINT pt = {pixelX, pixelY};

	if (DragDetect(m_hwnd, pt)) {
//		SetCapture(m_hwnd);
		((GameAirForce*)mp_ownerGame)->setGameMode(GM_DISPATCH);
	} else {
		if (HitTestAircrafts(dipX, dipY)) {
//			(*m_ItrSelectedAircraft)->m_hilight = TRUE;
		}
	        InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void PlayerAirForce::OnLButtonUp(int pixelX, int pixelY, DWORD flags)
{
	m_ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = m_ptMouse.x;
	const float dipY = m_ptMouse.y;

	POINT pt = {pixelX, pixelY};

	if (((GameAirForce*)mp_ownerGame)->m_gameMode == GM_DISPATCH) {
		((GameAirForce*)mp_ownerGame)->setGameMode(GM_NOP);
	}

}

//void PlayerAirForce::OnCopyData(
//	GET_X_LPARAM(lParam), 
//	GET_Y_LPARAM(lParam), 
//	(DWORD)wParam)
//{
//	PCOPYDATASTRUCT	p_cds;
//
//
//}



LRESULT PlayerAirForce::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO tmpScrollInfo;
	static int sbPosX;
	static int sbPosY;

	switch (uMsg)
    	{
	    case WM_CREATE:
	        if (FAILED(D2D1CreateFactory(
	                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
	        {
	            return -1;  // Fail CreateWindowEx.
	        }
		if (FAILED(CreateDWriteTextFormat())) {
			return -1; // failed to create DWriteFactory or TextFormat
		}

		CoInitialize(NULL);
		if (FAILED(CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&m_p_imagingFactory))) {
			return -1; //failed to create WICImagig factory
		}
	        return 0;

	    case WM_DESTROY:
	        DiscardGraphicsResources();
	        SafeRelease(&pFactory);
	        PostQuitMessage(0);
	        return 0;

	    case WM_PAINT:
	        OnPaint();
	        return 0;

	    case WM_SIZE:
		{
	        Resize();
	        return 0;
		}

	    case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_LBUTTONUP:
		OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDM_FILEEXIT:
	        		DiscardGraphicsResources();
			        SafeRelease(&pFactory);
	       			PostQuitMessage(0);
				break;
		}
		return 0;

	    case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT) {
			SetCursor(hCursor);
			return TRUE;
		}
		break;
	    case WM_HSCROLL:
		break;
			
	    case WM_VSCROLL:
		break;
	    case WM_COPYDATA:
		MessageBox(NULL, 
	           L"I got a copydata..\n",
		   NULL,
		   MB_OKCANCEL | MB_ICONSTOP
		);
//		OnLCopyData(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);

		break;
	}
	    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
//
//	MapAirForce.cpp
//
/////////////////////////////////////////////////////////////////////////////

PCWSTR  MapAirForce::ClassName() const 
{ 
	return L"MapAirForce Class"; 
}

void MapAirForce::SetMapStat(MapStat stat)
{
	m_mapStat = stat;
}

void MapAirForce::parseManuvModifyVirCorSlipRollNorth(cmdForm *p_form, int manuv)
{
	if ((manuv == MANUV_SR) || (manuv == MANUV_RR)) {
		if ((p_form->virCorX) % 2 == 0) {
			p_form->virCorX ++;
			p_form->virCorY --;
		} else {
			p_form->virCorX ++;
		}
	} else if ((manuv == MANUV_SL) || (manuv == MANUV_RL)) {
		if ((p_form->virCorX) % 2 == 0) {
			p_form->virCorX --;
			p_form->virCorY --;
		} else {
			p_form->virCorX --;
		}
	} else {
		MessageBox(NULL, 
			L"parseManuvModifyVirCorSlipRollNorth: illeagal manuv..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
}

void MapAirForce::parseManuvModifyVirCorSlipRollNorthEast(cmdForm *p_form, int manuv)
{
	if ((manuv == MANUV_SR) || (manuv == MANUV_RR)) {
		if ((p_form->virCorX) % 2 == 0) {
			p_form->virCorX ++;
		} else {
			p_form->virCorX ++;
			p_form->virCorY ++;
		}
	} else if ((manuv == MANUV_SL) || (manuv == MANUV_RL)) {
		if ((p_form->virCorX) % 2 == 0) {
//			p_form->virCorX --;
			p_form->virCorY --;
		} else {
//			p_form->virCorX --;
			p_form->virCorY --;
		}
	} else {
		MessageBox(NULL, 
			L"parseManuvModifyVirCorSlipRollNorthEast: illeagal manuv..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
}

void MapAirForce::parseManuvModifyVirCorSlipRollSouthEast(cmdForm *p_form, int manuv)
{
	if ((manuv == MANUV_SR) || (manuv == MANUV_RR)) {
		if ((p_form->virCorX) % 2 == 0) {
//			p_form->virCorX ++;
			p_form->virCorY ++;
		} else {
//			p_form->virCorX ++;
			p_form->virCorY ++;
		}
	} else if ((manuv == MANUV_SL) || (manuv == MANUV_RL)) {
		if ((p_form->virCorX) % 2 == 0) {
			p_form->virCorX ++;
			p_form->virCorY --;
		} else {
			p_form->virCorX ++;
//			p_form->virCorY ++;
		}
	} else {
		MessageBox(NULL, 
			L"parseManuvModifyVirCorSlipRollNorthEast: illeagal manuv..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
}

void MapAirForce::parseManuvModifyVirCorSlipRollSouth(cmdForm *p_form, int manuv)
{
	if ((manuv == MANUV_SR) || (manuv == MANUV_RR)) {
		if ((p_form->virCorX) % 2 == 0) {
			p_form->virCorX --;
//			p_form->virCorY ++;
		} else {
			p_form->virCorX --;
			p_form->virCorY ++;
		}
	} else if ((manuv == MANUV_SL) || (manuv == MANUV_RL)) {
		if ((p_form->virCorX) % 2 == 0) {
			p_form->virCorX ++;
//			p_form->virCorY --;
		} else {
			p_form->virCorX ++;
			p_form->virCorY ++;
		}
	} else {
		MessageBox(NULL, 
			L"parseManuvModifyVirCorSlipRollNorthEast: illeagal manuv..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
}

void MapAirForce::parseManuvModifyVirCorSlipRollSouthWest(cmdForm *p_form, int manuv)
{
	if ((manuv == MANUV_SR) || (manuv == MANUV_RR)) {
		if ((p_form->virCorX) % 2 == 0) {
			p_form->virCorX --;
			p_form->virCorY --;
		} else {
			p_form->virCorX --;
//			p_form->virCorY ++;
		}
	} else if ((manuv == MANUV_SL) || (manuv == MANUV_RL)) {
		if ((p_form->virCorX) % 2 == 0) {
//			p_form->virCorX ++;
			p_form->virCorY ++;
		} else {
//			p_form->virCorX ++;
			p_form->virCorY ++;
		}
	} else {
		MessageBox(NULL, 
			L"parseManuvModifyVirCorSlipRollNorthEast: illeagal manuv..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
}

void MapAirForce::parseManuvModifyVirCorSlipRollNorthWest(cmdForm *p_form, int manuv)
{
	if ((manuv == MANUV_SR) || (manuv == MANUV_RR)) {
		if ((p_form->virCorX) % 2 == 0) {
//			p_form->virCorX --;
			p_form->virCorY --;
		} else {
//			p_form->virCorX --;
			p_form->virCorY --;
		}
	} else if ((manuv == MANUV_SL) || (manuv == MANUV_RL)) {
		if ((p_form->virCorX) % 2 == 0) {
			p_form->virCorX --;
//			p_form->virCorY ++;
		} else {
			p_form->virCorX --;
			p_form->virCorY --;
		}
	} else {
		MessageBox(NULL, 
			L"parseManuvModifyVirCorSlipRollNorthEast: illeagal manuv..\n",
			NULL,
			MB_OKCANCEL | MB_ICONSTOP
		);
	}
}

void MapAirForce::parseManuvModifyVirCorSlipRoll(cmdForm *p_form, int manuv)
{
	switch ((int) p_form->heading) {
		case 0: // North
			parseManuvModifyVirCorSlipRollNorth(p_form, manuv);
			break;
		case 60: // North East
			parseManuvModifyVirCorSlipRollNorthEast(p_form, manuv);
			break;
		case 120: // South East
			parseManuvModifyVirCorSlipRollSouthEast(p_form, manuv);
			break;
		case 180: // South
			parseManuvModifyVirCorSlipRollSouth(p_form, manuv);
			break;
		case 240: // South West
			parseManuvModifyVirCorSlipRollSouthWest(p_form, manuv);
			break;
		case 300: // North West
			parseManuvModifyVirCorSlipRollNorthWest(p_form, manuv);
			break;
		default: // else
			MessageBox(NULL, 
				L"parseManuvModifyVirCorSlipRoll: illeagal heading..\n",
				NULL,
				MB_OKCANCEL | MB_ICONSTOP
			);
			break;

	}
}

void MapAirForce::parseManuvTL(cmdForm *p_form, int *p_mp)
{
	p_form->speed--;
	(*p_mp)--;
	(p_form->manuvable.remainingMP) --;

	int int_heading = (int)(p_form->heading);
	int_heading = (int_heading - 60) % 360;
	p_form->heading = (float)int_heading;
}

void MapAirForce::parseManuvTR(cmdForm *p_form, int *p_mp)
{
	p_form->speed--;
	(*p_mp)--;
	(p_form->manuvable.remainingMP) --;

	int int_heading = (int)(p_form->heading);
	int_heading = (int_heading + 60) % 360;
	p_form->heading = (float)int_heading;
}

void MapAirForce::parseManuvBL(cmdForm *p_form, int *p_mp)
{
	p_form->bank = (p_form->bank + 1) % 6;
}

void MapAirForce::parseManuvBR(cmdForm *p_form, int *p_mp)
{
	p_form->bank = (p_form->bank + 7) % 6;
}

void MapAirForce::parseManuvSL(cmdForm *p_form, int *p_mp)
{
	p_form->speed -= 2;
	(*p_mp)--;
	(p_form->manuvable.remainingMP) --;

	parseManuvModifyVirCorSlipRoll(p_form, MANUV_SL);
}

void MapAirForce::parseManuvSR(cmdForm *p_form, int *mp)
{
	p_form->speed -= 2;
	(*mp)--;
	(p_form->manuvable.remainingMP) --;

	parseManuvModifyVirCorSlipRoll(p_form, MANUV_SR);
}

void MapAirForce::parseManuvRL(cmdForm *p_form, int *p_mp)
{
	p_form->speed -= 1;
	(*p_mp)--;
	(p_form->manuvable.remainingMP) --;
	p_form->bank = (p_form->bank + 3) % 6;

	parseManuvModifyVirCorSlipRoll(p_form, MANUV_RL);
}

void MapAirForce::parseManuvRR(cmdForm *p_form, int *p_mp)
{
	p_form->speed -= 1;
	(*p_mp)--;
	(p_form->manuvable.remainingMP) --;
	p_form->bank = (p_form->bank + 3) % 6;
	// % may return negative value.
	// therefore bank - 3 was changed to bank + 3 
	// so that return value of % is positive value.
	// surprisingly listStringBankShort[-1] did not cause errors
	// and returned the adjacent array of listStringBank value.
	//   '2017/09/25

	parseManuvModifyVirCorSlipRoll(p_form, MANUV_RR);
}

void MapAirForce::parseManuvLC(cmdForm *p_form, int *p_mp)
{
	p_form->speed -= 2;
	(*p_mp)--;
	(p_form->manuvable.remainingMP) --;
	p_form->bank = (p_form->bank + 3) % 6;

	int int_heading = (int)(p_form->heading);
	int_heading = (int_heading + 180) % 360;
	p_form->heading = (float)int_heading;
}

void MapAirForce::parseManuvLD(cmdForm *p_form, int *p_mp)
{
	p_form->speed -= 2;
	(*p_mp)--;
	(p_form->manuvable.remainingMP) --;
	p_form->bank = (p_form->bank + 3) % 6;

	int int_heading = (int)(p_form->heading);
	int_heading = (int_heading + 180) % 360;
	p_form->heading = (float)int_heading;
}

void MapAirForce::parseManuvPW(cmdForm *p_form, int *p_mp)
{
	(p_form->speed)++;
	((p_form->manuvable).maxPower)--;
}

void MapAirForce::parseManuvBK(cmdForm *p_form, int *p_mp)
{
	(p_form->speed)--;
	((p_form->manuvable).maxBreak)--;
}

void MapAirForce::parseManuvDB(cmdForm *p_form, int *p_mp)
{
}

void MapAirForce::parseManuvFR(cmdForm *p_form, int *p_mp)
{
}

void MapAirForce::parseManuvFG(cmdForm *p_form, int *p_mp)
{
}

void MapAirForce::parseManuvModifyVirCorFwdNorth(cmdForm *p_form)
{
	if ((p_form->virCorX) % 2 == 0) {
//		p_form->virCorX ++;
		p_form->virCorY --;
	} else {
//		p_form->virCorX ++;
		p_form->virCorY --;
	}
}

void MapAirForce::parseManuvModifyVirCorFwdNorthEast(cmdForm *p_form)
{
	if ((p_form->virCorX) % 2 == 0) {
		p_form->virCorX ++;
		p_form->virCorY --;
	} else {
		p_form->virCorX ++;
//		p_form->virCorY --;
	}
}

void MapAirForce::parseManuvModifyVirCorFwdSouthEast(cmdForm *p_form)
{
	if ((p_form->virCorX) % 2 == 0) {
		p_form->virCorX ++;
//		p_form->virCorY --;
	} else {
		p_form->virCorX ++;
		p_form->virCorY ++;
	}
}

void MapAirForce::parseManuvModifyVirCorFwdSouth(cmdForm *p_form)
{
	if ((p_form->virCorX) % 2 == 0) {
//		p_form->virCorX ++;
		p_form->virCorY ++;
	} else {
//		p_form->virCorX ++;
		p_form->virCorY ++;
	}
}

void MapAirForce::parseManuvModifyVirCorFwdSouthWest(cmdForm *p_form)
{
	if ((p_form->virCorX) % 2 == 0) {
		p_form->virCorX --;
//		p_form->virCorY ++;
	} else {
		p_form->virCorX --;
		p_form->virCorY ++;
	}
}

void MapAirForce::parseManuvModifyVirCorFwdNorthWest(cmdForm *p_form)
{
	if ((p_form->virCorX) % 2 == 0) {
		p_form->virCorX --;
		p_form->virCorY --;
	} else {
		p_form->virCorX --;
//		p_form->virCorY ++;
	}
}

void MapAirForce::parseManuvMoveFwdOneHex(cmdForm *p_form, int *p_mp)
{
	(*p_mp)--;
	(p_form->manuvable.remainingMP) --;

	switch ((int) p_form->heading) {
		case 0: // North
			parseManuvModifyVirCorFwdNorth(p_form);
			break;
		case 60: // North East
			parseManuvModifyVirCorFwdNorthEast(p_form);
			break;
		case 120: // South East
			parseManuvModifyVirCorFwdSouthEast(p_form);
			break;
		case 180: // South
			parseManuvModifyVirCorFwdSouth(p_form);
			break;
		case 240: // South West
			parseManuvModifyVirCorFwdSouthWest(p_form);
			break;
		case 300: // North West
			parseManuvModifyVirCorFwdNorthWest(p_form);
			break;
		default: // else
			MessageBox(NULL, 
				L"parseManuvModifyVirCorFwdOneHex: illeagal heading..\n",
				NULL,
				MB_OKCANCEL | MB_ICONSTOP
			);
			break;

	}
}

void MapAirForce::parseManuvMoveFwd(cmdForm *p_form, int *p_mp, int fwd)
{
	int i;

	for (i = 0; i < fwd; i++) {
		parseManuvMoveFwdOneHex(p_form, p_mp);
	}

}

void MapAirForce::parseManuvClimb(cmdForm *p_form, int *p_mp, int climb)
{
// climb; feet
	p_form->alt += (float)(climb) / 1000.0f;
}

void MapAirForce::parseManuvDive(cmdForm *p_form, int *p_mp, int dive)
{
// dive; negative integer in feet
	p_form->alt += (float)(dive) / 1000.0f;
}

void MapAirForce::parseManuvMoveOneHexSpecifiedDirection(cmdForm *p_form, int intHeading)
{
	intHeading = intHeading % 360;

	switch (intHeading) {
		case 0:
			parseManuvModifyVirCorFwdNorth(p_form);
			break;
		case 60:
			parseManuvModifyVirCorFwdNorthEast(p_form);
			break;
		case 120:
			parseManuvModifyVirCorFwdSouthEast(p_form);
			break;
		case 180:
			parseManuvModifyVirCorFwdSouth(p_form);
			break;
		case 240:
			parseManuvModifyVirCorFwdSouthWest(p_form);
			break;
		case 300:
			parseManuvModifyVirCorFwdNorthWest(p_form);
			break;
		default:
			MessageBox(NULL, 
				L"parseManuvMoveOneHexSpecifiedDirection: illeagal heading..\n",
				NULL,
				MB_OKCANCEL | MB_ICONSTOP
			);
			break;
	}
}

void MapAirForce::parseManuvMoveOneHexClockRef(cmdForm *p_form, int clockRef)
{
	int	intHeading = (int)p_form->heading;

	switch (clockRef) {
		case 2:			// 2 o'oclock
			intHeading += 60;
			intHeading = intHeading % 360;
			parseManuvMoveOneHexSpecifiedDirection(p_form, intHeading);
			break;
		case 4:			// 4 o'oclock
			intHeading += 120;
			intHeading = intHeading % 360;
			parseManuvMoveOneHexSpecifiedDirection(p_form, intHeading);
			break;
		case 6:			// 6 o'oclock
			intHeading += 180;
			intHeading = intHeading % 360;
			parseManuvMoveOneHexSpecifiedDirection(p_form, intHeading);
			break;
		case 8:			// 8 o'oclock
			intHeading += 240;
			intHeading = intHeading % 360;
			parseManuvMoveOneHexSpecifiedDirection(p_form, intHeading);
			break;
		case 10:		// 10 o'oclock
			intHeading += 300;
			intHeading = intHeading % 360;
			parseManuvMoveOneHexSpecifiedDirection(p_form, intHeading);
			break;
		case 12:		// 12 o'oclock
			intHeading += 0;
			intHeading = intHeading % 360;
			parseManuvMoveOneHexSpecifiedDirection(p_form, intHeading);
			break;
		case 14:		// above
			break;
		case 16:		// below
			break;
		default:
			break;
			
	}
}

int MapAirForce::parseManuv(cmdForm *p_form) 
{
	int mp = p_form->speed;
	int i;

	for (i = 0; p_form->manuv[i] != MANUV_EN; i++) {
		switch (p_form->manuv[i]) {
			case MANUV_TL:
				parseManuvTL(p_form, &mp);
				break;
			case MANUV_TR:
				parseManuvTR(p_form, &mp);
				break;
			case MANUV_BL:
				parseManuvBL(p_form, &mp);
				break;
			case MANUV_BR:
				parseManuvBR(p_form, &mp);
				break;
			case MANUV_SL:
				parseManuvSL(p_form, &mp);
				break;
			case MANUV_SR:
				parseManuvSR(p_form, &mp);
				break;
			case MANUV_RL:
				parseManuvRL(p_form, &mp);
				break;
			case MANUV_RR:
				parseManuvRR(p_form, &mp);
				break;
			case MANUV_LC:
				parseManuvLC(p_form, &mp);
				break;
			case MANUV_LD:
				parseManuvLD(p_form, &mp);
				break;
			case MANUV_DP:
				break;
			case MANUV_PW:
				parseManuvPW(p_form, &mp);
				break;
			case MANUV_BK:
				parseManuvBK(p_form, &mp);
				break;
			case MANUV_DB:
				break;
			case MANUV_FR:
				break;
			case MANUV_FG:
				break;
			default:
				if ((0 <= p_form->manuv[i])
			 	&&  (p_form->manuv[i] < 20)) { // move forward
					parseManuvMoveFwd(p_form, &mp, p_form->manuv[i]);
				} else if (p_form->manuv[i] >= 20) { // climb
					parseManuvClimb(p_form, &mp, p_form->manuv[i]);
				} else if (p_form->manuv[i] <= -20) { // dive
					parseManuvDive(p_form, &mp, p_form->manuv[i]);
				}
		}
	}

	return mp;
}


HRESULT MapAirForce::LoadResourceBitmap(
		ID2D1RenderTarget *pRenderTarget,
		IWICImagingFactory *pIWICFactory,
		PCWSTR resourceName,
		PCWSTR resourceType,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap **ppBitmap
		)
{
	IWICBitmapDecoder *p_decoder = NULL;
	IWICBitmapFrameDecode *p_frame = NULL;
	IWICFormatConverter *p_converter = NULL;
	IWICBitmapScaler *p_scaler = NULL;

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void *p_imageFile = NULL;
	DWORD imageFileSize = 0;
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr)) {
		hr = pIWICFactory->CreateDecoderFromFilename(
			L"bitmapFiles\\Ju87.bmp",
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&p_decoder
			);
	} else { return E_FAIL;}
	if (SUCCEEDED(hr)) {
		hr =p_decoder->GetFrame(0, &p_frame);
	} else { return E_FAIL;}
	if (SUCCEEDED(hr)) {
		hr = pIWICFactory->CreateFormatConverter(&p_converter);
	} else { return E_FAIL;}
	if (SUCCEEDED(hr)) {
		hr = p_converter->Initialize(
			p_frame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL, 
			0.f,
			WICBitmapPaletteTypeMedianCut
			);
	} else { return E_FAIL;}
	if (SUCCEEDED(hr)) {
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			p_converter,
			NULL,
			ppBitmap
			);
	} else { return E_FAIL;}
	SafeRelease(&p_decoder);
	SafeRelease(&p_frame);
	SafeRelease(&p_converter);
	SafeRelease(&p_scaler);

	return hr;
}

BOOL MapAirForce::Create(
		PCWSTR lpWindowName,
		DWORD dwStyle
	)
{
		DWORD dwExStyle = 0;
		int x = CW_USEDEFAULT;
		int y = CW_USEDEFAULT;
		int nWidth = CW_USEDEFAULT;
		int nHeight = CW_USEDEFAULT;
		HWND hWndParent = 0;
		HMENU hMenu = 0;
	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAP);
	wc.lpszClassName = ClassName();
	wc.style = CS_DBLCLKS;

	RegisterClass(&wc);

	m_hwnd = CreateWindowEx(
		dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
		nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
	);

	return (m_hwnd ? TRUE : FALSE);
}


D2D1_POINT_2F MapAirForce::virCorToCenterF(int virCorX, int virCorY)
{
	D2D1_POINT_2F center;

	const int unitA = mHexSize;
	const int root3A = (int) ((float)unitA * 1.732050);
	const float unitAF = (float) unitA;
	const float root3AF = (float) root3A;
	const float offsetX = m_offsetX;
	const float offsetY = m_offsetY;
	int i, j;
	bool	even;

	i = virCorX - mHexVirCorOrgX;
	j = virCorY - mHexVirCorOrgY;

	if (i % 2 == 0) {
		i = i / 2;
		center.x = i * unitAF * 6 + offsetX;
		center.y = j * root3AF  * 2 + offsetY;
	} else {
		i = (i -1) / 2;
		center.x = i * unitAF * 6 + offsetX + unitAF * 3;
		center.y = j * root3AF  * 2 + offsetY + root3AF;
	}

	return center;
}

void MapAirForce::NumerateStackNum(cmdForm *p_rtn)
{
	int id0 = 0;
	int id1 = 0;
	int x = MAX_INT;
	int y = MAX_INT;
	int stack = 0;

	for (id0 = 0; p_rtn[id0].command != TAIL; id0++) {
		p_rtn[id0].stackNum = 0;
	}

	for (id0 = 0; p_rtn[id0].command != TAIL; id0++) {
		if (p_rtn[id0].stackNum == 0) {
			stack = 1;
			p_rtn[id0].stackNum = stack;
			x = p_rtn[id0].virCorX;
			y = p_rtn[id0].virCorY;
			for (id1 = id0 + 1; p_rtn[id1].command != TAIL; id1++) {
				if ((x == p_rtn[id1].virCorX) 
				&&  (y == p_rtn[id1].virCorY)) {
					stack++;
					p_rtn[id1].stackNum = stack;
				}
			}
		}
	}
}

void MapAirForce::DrawLeaderLine(D2D1_POINT_2F center, cmdForm *p_form)
{
	const int unitA = mHexSize;
	const int root3A = (int) ((float)unitA * 1.732050);
	const float unitAF = (float) unitA;
	const float root3AF = (float) root3A;

	const float sin60 = 1.732050 / 2.0;
	const float cos60 = 0.500000;

	const float start = 0.5;
	const float end = 1.3;

	D2D1_POINT_2F startPt;
	D2D1_POINT_2F endPt;

	D2D1_COLOR_F originalColor = pBrush->GetColor();

	switch (p_form->stackNum % 6) {
		case 1:
			startPt.x = center.x + root3AF * sin60 * start;
		      	startPt.y = center.y + root3AF * cos60 * start;
			endPt.x = center.x + root3AF * sin60 * end;
			endPt.y = center.y + root3AF * cos60 * end;
			break;
		case 2:
			startPt.x = center.x + root3AF * sin60 * start;
		      	startPt.y = center.y - root3AF * cos60 * start;
			endPt.x = center.x + root3AF * sin60 * end;
			endPt.y = center.y - root3AF * cos60 * end;
			break;
		case 3:
			startPt.x = center.x;
		      	startPt.y = center.y - root3AF * start;
			endPt.x = center.x;
			endPt.y = center.y - root3AF * end;
			break;
		case 4:
			startPt.x = center.x - root3AF * sin60 * start;
		      	startPt.y = center.y - root3AF * cos60 * start;
			endPt.x = center.x - root3AF * sin60 * end;
			endPt.y = center.y - root3AF * cos60 * end;
			break;
		case 5:
			startPt.x = center.x - root3AF * sin60 * start;
		      	startPt.y = center.y + root3AF * cos60 * start;
			endPt.x = center.x - root3AF * sin60 * end;
			endPt.y = center.y + root3AF * cos60 * end;
			break;
		case 0:
			startPt.x = center.x;
		      	startPt.y = center.y + root3AF * start;
			endPt.x = center.x;
			endPt.y = center.y + root3AF * end;
			break;

	}

	pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	pRenderTarget->DrawLine(startPt, endPt, pBrush, 0.7f);

	wchar_t str_acInfo[80];
	swprintf(str_acInfo, L"%s, %2.1f, %d, %s\n", 
		p_form->aircraftName,
		p_form->alt,
		p_form->speed,
		listStrBankShort[p_form->bank]);
	pRenderTarget->DrawText(
		str_acInfo,
		wcslen(str_acInfo) ,
		mPtrTextFormat,
		D2D1::RectF(endPt.x, endPt.y, 
			endPt.x + 160.0f, endPt.y + 5.0f),
		pBrush
	);
	pBrush->SetColor(originalColor);
}

void MapAirForce::DrawManuvPath(cmdForm form) 
{
	D2D1_POINT_2F srcPt;
	D2D1_POINT_2F desPt;

	int i;
	cmdForm tmpForm = form;
	int srcVirCorX = tmpForm.virCorX;
	int srcVirCorY = tmpForm.virCorY;
	int desVirCorX;
	int desVirCorY;
	int mp = tmpForm.speed;
	
	for (i = 0; form.manuv[i] != MANUV_EN; i++) {
		switch (form.manuv[i]) {
			case MANUV_PW:
				parseManuvPW(&tmpForm, &mp);
				break;
			case MANUV_LC:
				parseManuvLC(&tmpForm, &mp); 
				break;
			case MANUV_RR:
				parseManuvRR(&tmpForm, &mp); 
				break;
			case MANUV_SR:
				parseManuvSR(&tmpForm, &mp); 
				break;
			case MANUV_TR:
				parseManuvTR(&tmpForm, &mp); 
				break;
			case MANUV_BR:
				parseManuvBR(&tmpForm, &mp); 
				break;
			case MANUV_BL:
				parseManuvBL(&tmpForm, &mp); 
				break;
			case MANUV_TL:
				parseManuvTL(&tmpForm, &mp); 
				break;
			case MANUV_SL:
				parseManuvSL(&tmpForm, &mp); 
				break;
			case MANUV_RL:
				parseManuvRL(&tmpForm, &mp); 
				break;
			case MANUV_LD:
				parseManuvLD(&tmpForm, &mp); 
				break;
			case MANUV_BK:
				parseManuvBK(&tmpForm, &mp);
				break;
			default:
				if ((0 < form.manuv[i]) 
				&&  (form.manuv[i] < 20)) {
					parseManuvMoveFwd(&tmpForm, &mp, form.manuv[i]);
				} else if (20 <= form.manuv[i]) {
					parseManuvClimb(&tmpForm, &mp, form.manuv[i]);
				} else if (form.manuv[i] <= -20) {
					parseManuvDive(&tmpForm, &mp, form.manuv[i]);
				}
				break;
		}
		desVirCorX = tmpForm.virCorX;
		desVirCorY = tmpForm.virCorY;
		srcPt = virCorToCenterF(srcVirCorX, srcVirCorY);
		desPt = virCorToCenterF(desVirCorX, desVirCorY);
		if ((srcVirCorX != desVirCorX)
		||  (srcVirCorY != desVirCorY)) {
			pRenderTarget->DrawLine(srcPt, desPt, pBrush, 1.7f);
			srcVirCorX = desVirCorX;
			srcVirCorY = desVirCorY;
		}
	}
	DrawLeaderLine(desPt, &tmpForm);
	DrawPiece(tmpForm);

}

HRESULT MapAirForce::PaintHex(int virCorX, int virCorY)
{
	const int unitA = mHexSize;
	const int root3A = (int) ((float)unitA * 1.732050);
	const float unitAF = (float) unitA;
	const float root3AF = (float) root3A;

	D2D1_POINT_2F center;
	float neX, neY;
	float nwX, nwY;
	float eX, eY;
	float seX, seY;
	float swX, swY;
	float wX, wY;

	center = virCorToCenterF(virCorX, virCorY);
	neX = center.x + unitAF;
	neY = center.y - root3AF;
	nwX = center.x - unitAF;
	nwY = center.y - root3AF;
	eX = center.x + unitAF * 2;
	eY = center.y;
	seX = center.x + unitAF;
	seY = center.y + root3AF;
	swX = center.x - unitAF;
	swY = center.y + root3AF;
	wX = center.x - unitAF * 2;
	wY = center.y;

	ID2D1PathGeometry	*p_pathGeometryHex;
	HRESULT hr = S_OK;

	hr = pFactory->CreatePathGeometry(&p_pathGeometryHex);
    	if (SUCCEEDED(hr)) {
		ID2D1GeometrySink	*p_sink = NULL;

		hr = p_pathGeometryHex->Open(&p_sink);
		if (SUCCEEDED(hr)) {
			p_sink->SetFillMode(D2D1_FILL_MODE_WINDING);
			p_sink->BeginFigure(
				D2D1::Point2F(neX, neY),
				D2D1_FIGURE_BEGIN_FILLED
			);
			D2D1_POINT_2F points[6] = {
				D2D1::Point2F(eX, eY),
				D2D1::Point2F(seX, seY),
				D2D1::Point2F(swX, swY),
				D2D1::Point2F(wX, wY),
				D2D1::Point2F(nwX, nwY),
				D2D1::Point2F(neX, neY),
			};
			p_sink->AddLines(points, ARRAYSIZE(points));
			p_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = p_sink->Close();
		SafeRelease(&p_sink);
	}
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	D2D1_COLOR_F originalColor = pBrush->GetColor();
	pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::YellowGreen, 1.f));

	pRenderTarget->FillGeometry(p_pathGeometryHex, pBrush);

	pBrush->SetColor(originalColor);

	return hr;
}

HRESULT MapAirForce::DrawPiece(
//		ID2D1RenderTarget *p_renderTgt,
//		ID2D1SolidColorBrush *pBrush,
//		IWICImagingFactory *p_factory,
		cmdForm form
		)
{
	const int unitA = mHexSize;
	const int root3A = (int) ((float)unitA * 1.732050);
	const float unitAF = (float) unitA;
	const float root3AF = (float) root3A;

	HRESULT hr = S_OK;
	wchar_t tmpBitmapFileName[MAX_NAMELENGTH];
	ID2D1Bitmap *p_bitmap;
	ID2D1BitmapBrush *p_bitmapBrush;
	D2D1_SIZE_F size;

	float actHeight;
	float actWidth;
	float actX;
	float actY;

	int i = 0;
	D2D1_POINT_2F center;
	D2D1_RECT_F rcBrushRect;
	
//	p_renderTgt->SetTransform(D2D1::Matrix3x2F::Identity());
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	center = virCorToCenterF(form.virCorX, form.virCorY);
	rcBrushRect = D2D1::RectF(
		center.x - unitAF * 1.5f,
		center.y - root3AF / 2.0f,
		center.x + unitAF * 1.5f,
		center.y + root3AF / 2.0f
	);
	wsprintf(tmpBitmapFileName, L"%s", aircraftModels[form.aircraftModel].bmpFileName);
	LPCWSTR bitmapFileName = tmpBitmapFileName;
	hr = LoadBitmapFromFile(
//		p_renderTgt,
		pRenderTarget,
//		p_factory,
		m_p_imagingFactory,
		bitmapFileName,
		&p_bitmap
	);
	if (FAILED(hr)) {
		return -1;
	}
	size = p_bitmap->GetSize();
	D2D1_RECT_F rcSourceRect = D2D1::RectF(
		0,
		0, 
		size.width,
		size.height
	);

	float height = root3AF;
	float width = unitAF * 3.0f;
	float x = center.x - unitAF * 1.5f;
	float y = center.y - root3AF / 2.0f;

	if (height / size.height < width / size.width) {
		actHeight = height;
		actWidth = size.width * height / size.height;
		actX = x + (width - actWidth) / 2;
		actY = y;
	} else {
		actHeight = size.height * width / size.width;
		actWidth = width;
		actX = x;
		actY = y + (height - actHeight) / 2;
	}
//	 p_renderTgt->SetTransform(
	 pRenderTarget->SetTransform(
		D2D1::Matrix3x2F::Rotation(
			form.heading,
			D2D1::Point2F(                // you can change this to "center"
			actX + actWidth / 2,
			actY + actHeight /2
			)
		)
	);
//	p_renderTgt->DrawBitmap(
	pRenderTarget->DrawBitmap(
		p_bitmap,
		D2D1::RectF(
			actX,
			actY,
			actX + actWidth,
			actY + actHeight 
		),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		NULL
	);	
//	p_renderTgt->SetTransform(D2D1::Matrix3x2F::Identity());
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	
	return hr;
}


HRESULT MapAirForce::DrawPieces(
		ID2D1RenderTarget *p_renderTgt,
		ID2D1SolidColorBrush *pBrush,
		IWICImagingFactory *p_factory
		)
{
	const int unitA = mHexSize;
	const int root3A = (int) ((float)unitA * 1.732050);
	const float unitAF = (float) unitA;
	const float root3AF = (float) root3A;
	const float offsetX = m_offsetX;
	const float offsetY = m_offsetY;
	bool	even;

	HRESULT hr = S_OK;
	
	cmdForm	rtn[20];
	cmdForm dummy;

	dummy.command = GET_DISPATCHED;
	dummy.playerID = ALL_PLAYERS;
	dummy.selectedAircraft = ALL_AC;

	float actHeight;
	float actWidth;
	float actX;
	float actY;


	if (mp_ownerGame) {
		((GameAirForce*)mp_ownerGame)->cmdToGame(GET_DISPATCHED, dummy, rtn);
	}

	NumerateStackNum(rtn);

	int i = 0;
	D2D1_POINT_2F center;
	D2D1_RECT_F rcBrushRect;
	
	p_renderTgt->SetTransform(D2D1::Matrix3x2F::Identity());

	for (i = 0; rtn[i].command != TAIL; i++) {
//		hr = DrawPiece(p_renderTgt, pBrush, p_factory, rtn[i]);
		hr = DrawPiece(rtn[i]);
		center = virCorToCenterF(rtn[i].virCorX, rtn[i].virCorY);
		rcBrushRect = D2D1::RectF(
				center.x - unitAF * 1.5f,
				center.y - root3AF / 2.0f,
				center.x + unitAF * 1.5f,
				center.y + root3AF / 2.0f
		);

		DrawLeaderLine(center, &rtn[i]);
		DrawManuvPath(rtn[i]);
	}
	return hr;
}


void MapAirForce::OnPaintBitmap()
{
	HRESULT hr;
	cmdForm	rtn[20];
	cmdForm dummy;

	dummy.command = GET_DISPATCHED;
	dummy.playerID = ALL_PLAYERS;
	dummy.selectedAircraft = ALL_AC;

/*	hr = LoadResourceBitmap(
		pRenderTarget,
		m_p_imagingFactory,
//		L"Ju87a",
	       MAKEINTRESOURCE(IDB_BITMAP_JU87), 
//		L"Image",
	       MAKEINTRESOURCE(RT_BITMAP), 
		200,
		0,
		&m_pBitmap
		);
 */		
	hr = LoadBitmapFromFile(
		pRenderTarget,
		m_p_imagingFactory,
		L"bitmapFiles\\Ju87.bmp",
		&m_pBitmap
		);

//	hr = CreateDeviceResources();

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

//	D2D1_SIZE_F size = m_pBitmap->GetSize();
	D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(100.f, 10.f);

	pRenderTarget->DrawBitmap(
		m_pBitmap,
		D2D1::RectF(
			upperLeftCorner.x,
			upperLeftCorner.y,
//			upperLeftCorner.x + size.width,
//			upperLeftCorner.y + size.height
			upperLeftCorner.x + 30,
			upperLeftCorner.y + 30 
		)
	);	

	if (mp_ownerGame) {
		((GameAirForce*)mp_ownerGame)->cmdToGame(GET_DISPATCHED, dummy, rtn);
	}

	int i = 0;
	D2D1_POINT_2F center;
	
	for (i = 0; rtn[i].command != TAIL; i++) {
		center = virCorToCenterF(rtn[i].virCorX, rtn[i].virCorY);


		MessageBox(NULL, 
	           L"Found a dispatched aircraft..\n",
		   NULL,
		   MB_OKCANCEL | MB_ICONSTOP
	   );
	}

}

void MapAirForce::paintAndExpand(
	cmdForm		form,
	int		range,
	bool		includeLeft,
	bool		includeRight,
	expandHistory	history)
{
	cmdForm	formLeft = form;
	cmdForm formRight = form;
	cmdForm formCenter = form;

	if (range > 0) {
		PaintHex(form.virCorX, form.virCorY);
	
		if (includeLeft) {
			if (history.centerCnt - history.leftCnt >= 1) {
				parseManuvMoveOneHexClockRef(&formLeft, 10);
				history.leftCnt ++;
				paintAndExpand
					(formLeft,
					 range -= 1,
					 includeLeft,
					 includeRight,
					 history);
			}
		} else {
			if (history.centerCnt - history.leftCnt >= 2) {
				parseManuvMoveOneHexClockRef(&formLeft, 10);
				history.leftCnt ++;
				paintAndExpand
					(formLeft,
					 range -= 1,
					 includeLeft,
					 includeRight,
					 history);
			}
		}
		if (includeRight) {
			if (history.centerCnt - history.rightCnt >= 1) {
				parseManuvMoveOneHexClockRef(&formRight, 2);
				history.rightCnt ++;
				paintAndExpand
					(formRight,
					 range -= 1,
					 includeLeft,
					 includeRight,
					 history);
			}
		} else {
			if (history.centerCnt - history.rightCnt >= 2) {
				parseManuvMoveOneHexClockRef(&formRight, 2);
				history.rightCnt ++;
				paintAndExpand
					(formRight,
					 range -= 1,
					 includeLeft,
					 includeRight,
					 history);
			}
		}
		parseManuvMoveOneHexClockRef(&formCenter, 12);
		history.centerCnt ++;
		paintAndExpand
			(formCenter,
			 range -= 1,
			 includeLeft,
			 includeRight,
			 history);
	}
}


void MapAirForce::paintFireArcClockCond
	(cmdForm form, int range, bool includeLeft, bool includeRight)
{
	expandHistory	history;
	history.leftCnt	= 0;
	history.centerCnt = 0;
	history.rightCnt = 0;


	paintAndExpand(form, range, includeLeft, includeRight, history);

}


void MapAirForce::paintFireArcClockRef(cmdForm form, int clockRef, int range)
{
	int 	intHeading = (int) form.heading;

	switch (clockRef) {
		case 2:
			paintFireArcClockCond(form, range, false, true);
			break;
		case 4: 
			paintFireArcClockCond(form, range, false, false);
			break;
		case 6:
			paintFireArcClockCond(form, range, true, true);
			break;
		case 8:
			paintFireArcClockCond(form, range, false, false);
			break;
		case 10:
			paintFireArcClockCond(form, range, true, false);
			break;
		case 12: 
			paintFireArcClockCond(form, range, true, true);
			break;
		case 14:		// above
			break;
		case 16:		// below
			break;
		default:
			break;
	}
}

void MapAirForce::paintFireArc(cmdForm form)
{
	int i;

	if (m_selectedFireArc.rangeFFmg > 0) {
		paintFireArcClockRef(form, 12, m_selectedFireArc.rangeFFmg);
	}
	if (m_selectedFireArc.rangeFFcannon > 0) {
		paintFireArcClockRef(form, 12, m_selectedFireArc.rangeFFcannon);
	}
	if (m_selectedFireArc.rangeFH > 0) {
		paintFireArcClockRef(form, 12, m_selectedFireArc.rangeFH);
	}
	if (m_selectedFireArc.rangeFL > 0) {
		paintFireArcClockRef(form, 12, m_selectedFireArc.rangeFL);
	}
	for (i = 0; i < 8; i++) {
		if (m_selectedFireArc.rangeF[i] > 0) {
			paintFireArcClockRef(form, i * 2, m_selectedFireArc.rangeF[i]);
		}
	}

}

void MapAirForce::OnPaintGM_Fire()
{
	cmdForm	rtn[20];
	cmdForm form;
	form.command = GET_AC;
	form.playerID = SELECTED_PLAYER;
	form.selectedAircraft = SELECTED_AC;


	if (mp_ownerGame) {
		((GameAirForce*)mp_ownerGame)->cmdToGame(GET_AC, form, rtn);
	}
	if (rtn[0].command != TAIL) {
		paintFireArc(rtn[0]);
	}
}

void MapAirForce::OnPaint()
{
	const int unitA = mHexSize;
	const int root3A = (int) ((float)unitA * 1.732050);
	const float unitAF = (float) unitA;
	const float root3AF = (float) root3A;
	const float offsetX = m_offsetX;
	const float offsetY = m_offsetY;
	int i, j;

	float centerX, centerY;
	float neX, neY;
	float nwX, nwY;
	float eX, eY;
	float seX, seY;
	float swX, swY;
	float wX, wY;



	
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
	D2D1_SIZE_F size = pRenderTarget->GetSize();
	const float RenderTargetSizeX = size.width;
	const float RenderTargetSizeY = size.height;
	const int hexCntX = (int) (RenderTargetSizeX / (unitAF * 6.0f)) + 1;
	m_hexCntX = hexCntX;
	const int hexCntY = (int) (RenderTargetSizeY / (root3AF * 2.0f)) + 1;
	m_hexCntY = hexCntY;
	wchar_t strHexCor[16];
	int	hexVirCorX;
	int	hexVirCorY;

        BeginPaint(m_hwnd, &ps);
     
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::SkyBlue) );

	std::list<std::shared_ptr<MyEllipse>>::iterator itr;

	for (i = 0; i < hexCntX; i++) {
		for (j = 0; j < hexCntY; j++) {
			centerX = offsetX + unitAF * 6 * i;
			centerY = offsetY + root3AF * 2 * j;

			neX = centerX + unitAF;
			neY = centerY - root3AF;

			nwX = centerX - unitAF;
			nwY = centerY - root3AF;

			eX = centerX + unitAF * 2;
			eY = centerY;

			seX = centerX + unitAF;
			seY = centerY + root3AF;

			swX = centerX - unitAF;
			swY = centerY + root3AF;

			wX = centerX - unitAF * 2;
			wY = centerY;


			hexVirCorX = mHexVirCorOrgX + i * 2;
			hexVirCorY = mHexVirCorOrgY + j;

			wsprintf(strHexCor, L"%d, %d\n", hexVirCorX, hexVirCorY);

			if ((m_evenSelected == TRUE)
			&&  (m_realCorSelectedX == i)
			&&  (m_realCorSelectedY == j)) {
				pRenderTarget->DrawLine(
					D2D1::Point2F(nwX, nwY),
					D2D1::Point2F(neX, neY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(neX, neY),
					D2D1::Point2F(eX, eY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(eX, eY),
					D2D1::Point2F(seX, seY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(seX, seY),
					D2D1::Point2F(swX, swY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(swX, swY),
					D2D1::Point2F(wX, wY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(wX, wY),
					D2D1::Point2F(nwX, nwY),
					pBrush,
					3.5f
				);
			} else {
				pRenderTarget->DrawLine(
					D2D1::Point2F(nwX, nwY),
					D2D1::Point2F(neX, neY),
					pBrush,
					0.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(neX, neY),
					D2D1::Point2F(eX, eY),
					pBrush,
					0.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(eX, eY),
					D2D1::Point2F(seX, seY),
					pBrush,
					0.5f
				);
			}
			pRenderTarget->DrawText(
				strHexCor,
				wcslen(strHexCor) ,
				mPtrTextFormat,
				D2D1::RectF(swX, swY-unitAF, 
					    seX, seY),
				pBrush
			);
		}
		for (j = 0; j < hexCntY; j++) {
			centerX = offsetX + unitAF * 6 * i + unitAF * 3;
			centerY = offsetY + root3AF * 2 * j + root3AF ;

			neX = centerX + unitAF;
			neY = centerY - root3AF;

			nwX = centerX - unitAF;
			nwY = centerY - root3AF;

			eX = centerX + unitAF * 2;
			eY = centerY;

			seX = centerX + unitAF;
			seY = centerY + root3AF;

			swX = centerX - unitAF;
			swY = centerY + root3AF;

			wX = centerX - unitAF * 2;
			wY = centerY;

			hexVirCorX = mHexVirCorOrgX + i * 2 + 1;
			hexVirCorY = mHexVirCorOrgY + j;

			wsprintf(strHexCor, L"%d, %d\n", hexVirCorX, hexVirCorY);

			if ((m_evenSelected == FALSE)
			&&  (m_realCorSelectedX == i)
			&&  (m_realCorSelectedY == j)) {
				pRenderTarget->DrawLine(
					D2D1::Point2F(nwX, nwY),
					D2D1::Point2F(neX, neY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(neX, neY),
					D2D1::Point2F(eX, eY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(eX, eY),
					D2D1::Point2F(seX, seY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(seX, seY),
					D2D1::Point2F(swX, swY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(swX, swY),
					D2D1::Point2F(wX, wY),
					pBrush,
					3.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(wX, wY),
					D2D1::Point2F(nwX, nwY),
					pBrush,
					3.5f
				);
			} else {
				pRenderTarget->DrawLine(
					D2D1::Point2F(nwX, nwY),
					D2D1::Point2F(neX, neY),
					pBrush,
					0.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(neX, neY),
					D2D1::Point2F(eX, eY),
					pBrush,
					0.5f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2F(eX, eY),
					D2D1::Point2F(seX, seY),
					pBrush,
					0.5f
				);
			}
			pRenderTarget->DrawText(
				strHexCor,
				wcslen(strHexCor) ,
				mPtrTextFormat,
				D2D1::RectF(swX, swY-unitAF, 
					    seX, seY),
				pBrush
			);
		}
	}

	GameMode	gameMode = ((GameAirForce*)mp_ownerGame)->m_gameMode;
	switch (gameMode) {
		case GM_FIRE:
			OnPaintGM_Fire();
			break;
	}

//	OnPaintBitmap();
	hr = DrawPieces(
		pRenderTarget,
		pBrush,
		m_p_imagingFactory
		);
	if (FAILED(hr)) {
		MessageBox(NULL, 
	           L"DrawPieces Failed..\n",
		   NULL,
		   MB_OKCANCEL | MB_ICONSTOP
	   	);
		return;
	}


        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void MapAirForce::HitTestHex(float dipX, float dipY, ResultHitTestHex *result)
{
	const int unitA = mHexSize;
	const int root3A = (int) ((float)unitA * 1.732050);
	const float unitAF = (float) unitA;
	const float root3AF = (float) root3A;
	const float offsetX = m_offsetX;
	const float offsetY = m_offsetY;

	float iF_even, jF_even;
	float iF_odd, jF_odd;
	float iF_Delta_even, jF_Delta_even;
	float iF_Delta_odd, jF_Delta_odd;
	float iF, jF;

	int	hexVirCorX;
	int	hexVirCorY;
	
	int i, j;
	bool	even;

	iF_even = (dipX - offsetX) / (unitAF * 6);
	jF_even = (dipY - offsetY) / (root3AF * 2);
	iF_Delta_even = iF_even - (float)((int)(iF_even + 0.5f));
	jF_Delta_even = jF_even - (float)((int)(jF_even + 0.5f));
	iF_odd = (dipX - offsetX - unitAF * 3) / (unitAF * 6);
	jF_odd = (dipY - offsetY - root3AF) / (root3AF * 2);
	iF_Delta_odd = iF_odd - (float)((int)(iF_odd + 0.5f));
	jF_Delta_odd = jF_odd - (float)((int)(jF_odd + 0.5f));

	if ((iF_Delta_even * iF_Delta_even) + (jF_Delta_even * jF_Delta_even) 
		< (iF_Delta_odd * iF_Delta_odd) + (jF_Delta_odd * jF_Delta_odd)) {
		iF = iF_even;
		jF = jF_even;
		i = (int) (iF + 0.5f); // 0.5f is added for rounding.
		j = (int) (jF + 0.5f);
		hexVirCorX = mHexVirCorOrgX + i * 2;
		hexVirCorY = mHexVirCorOrgY + j;
		even = TRUE;
	} else {
		iF = iF_odd;
		jF = jF_odd;
		i = (int) (iF + 0.5f);
		j = (int) (jF + 0.5f);
		hexVirCorX = mHexVirCorOrgX + i * 2 + 1;
		hexVirCorY = mHexVirCorOrgY + j;
		even = FALSE;
	}

	result->virCorX = hexVirCorX;
	result->virCorY = hexVirCorY;
	result->realCorX = i;
	result->realCorY = j;
	result->even = even;
}


	

HRESULT MapAirForce::CreateDWriteTextFormat()
{
    static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 8;
    HRESULT hr;

    // Create a Direct2D factory.
//    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

 //   if (SUCCEEDED(hr))
  //  {
        
        // Create a DirectWrite factory.
       hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(mPtrDWriteFactory),
            reinterpret_cast<IUnknown **>(&mPtrDWriteFactory)
            );
   // }
    if (SUCCEEDED(hr))
    {
        // Create a DirectWrite text format object.
        hr = mPtrDWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"", //locale
            &mPtrTextFormat
            );
    }
    if (SUCCEEDED(hr))
    {
        // Center the text horizontally and vertically.
        mPtrTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

        mPtrTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
       

    }

    return hr;
}

void MapAirForce::HandleDlgDeploySetAcstat(HWND hDlgWnd)
{
	LRESULT headingIdx;
	float heading;
	headingIdx = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_HEADING),
		CB_GETCURSEL,
		0,
		0);
	heading = (float) headingIdx * 60.0f;

	LRESULT speed;
	speed = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_SPEED),
		CB_GETCURSEL,
		0,
		0);

	wchar_t altStr[20];
	*(WORD *)altStr = sizeof(altStr);
	// EM_GETLINE needs to set the buffer size in the first word of the buffer
	float	alt;
	SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT),
		EM_GETLINE,
		0,
		(LPARAM)altStr);
	alt = wcstof(altStr, NULL);
	
	LRESULT bank;
	bank = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_BANK),
		CB_GETCURSEL,
		0,
		0);

	LRESULT nose;
	nose = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_NOSE),
		CB_GETCURSEL,
		0,
		0);

	cmdForm form;
	form.command = SET_ACSTAT;
	form.acStatus = DEPLOYED;
	form.heading = (float) heading;
	form.speed = (int) speed;
	form.alt = alt;
	form.bank = (int) bank;
	form.nose = (int) nose;
	form.playerID = SELECTED_PLAYER;
	form.selectedAircraft = SELECTED_AC;
	((GameAirForce*)mp_ownerGame)->cmdToGame(SET_ACSTAT, form, NULL);
	// you need parenthsis like above!!

}

LRESULT MapAirForce::HandleDlgDeploy(HWND hDlgWnd,
       					UINT msg,
					WPARAM wParam,
					LPARAM lParam)
{
	int	i;

	switch (msg) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					HandleDlgDeploySetAcstat(hDlgWnd);
					EndDialog(hDlgWnd, IDOK);
					break;
				case IDCANCEL:
					EndDialog(hDlgWnd, IDOK);
					break;
				case IDC_DEPLOY_ALT:
					if (HIWORD(wParam) == EN_CHANGE) {
//				MessageBox(NULL, 
//			           L"EN_CHANGE detected..\n",
//				   NULL,
//				   MB_OKCANCEL | MB_ICONSTOP
//				   );
					}
					break;
			}
			break;

		case WM_INITDIALOG:
			{
		bool result;
		m_initComCon.dwSize = sizeof(INITCOMMONCONTROLSEX);
		m_initComCon.dwICC = ICC_STANDARD_CLASSES;
		result = InitCommonControlsEx(&m_initComCon);

			for (i = 0; i < 6; i++) {
				SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_HEADING), 
						       CB_ADDSTRING, 
						       0,
						       (LPARAM)listStrHeading[i]);
			}

			cmdForm	rtn[20];

			cmdForm form;
			form.command = GET_AC;
			form.playerID = SELECTED_PLAYER;
			form.selectedAircraft = SELECTED_AC;

			if (mp_ownerGame) {
				((GameAirForce*)mp_ownerGame)->cmdToGame(GET_AC, form, rtn);

				if (rtn[0].command != TAIL) {
					wchar_t  x[32];

					int	acModel = rtn[0].aircraftModel;
					wsprintf(x, L"%s\n", 
						aircraftModels[acModel].name);
					Static_SetText(GetDlgItem(hDlgWnd, IDC_DEPLOY_ACMODEL),
					   (LPTSTR)x);

					int	pilotModel = rtn[0].pilotModel;
					wsprintf(x, L"%s\n", 
						pilotModels[pilotModel].name);
					Static_SetText(GetDlgItem(hDlgWnd, IDC_DEPLOY_PILOTMODEL),
					   (LPTSTR)x);

//					Syntax:  wcscpy(p_destination, p_source)
					wcscpy(x, rtn[0].aircraftName);
					Static_SetText(GetDlgItem(hDlgWnd, IDC_DEPLOY_ACNAME),
					   (LPTSTR)x);

					int	corX = rtn[0].virCorX;
					wsprintf(x, L"%d\n", corX );
					Static_SetText(GetDlgItem(hDlgWnd, IDC_DEPLOY_COR_X),
					   (LPTSTR)x);

					int	corY = rtn[0].virCorY;
					wsprintf(x, L"%d\n", corY );
					Static_SetText(GetDlgItem(hDlgWnd, IDC_DEPLOY_COR_Y),
					   (LPTSTR)x);

					float heading = rtn[0].heading;
					switch ((int)heading) {
					// switch must be integer constant.
					// So you need to cast from float to integer
					// 17/06/06
						case 0:
							SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_HEADING), 
						       		CB_SETCURSEL, 
						       		0,
						       		0);
							break;
						case 60:
							SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_HEADING), 
						       		CB_SETCURSEL, 
						       		1,
						       		0);
							break;
						case 120:
							SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_HEADING), 
						       		CB_SETCURSEL, 
						       		2,
						       		0);
							break;
						case 180:
							SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_HEADING), 
						       		CB_SETCURSEL, 
						       		3,
						       		0);
							break;
						case 240:
							SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_HEADING), 
						       		CB_SETCURSEL, 
						       		4,
						       		0);
							break;
						case 300:
							SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_HEADING), 
						       		CB_SETCURSEL, 
						       		5,
						       		0);
							break;
					}

					//------------ Speed ------------
					int speed = rtn[0].speed;
					cmdForm form1;
					cmdForm rtn1[20];
					form1.command = GET_LIMIT;
					form1.playerID = SELECTED_PLAYER;
					form1.selectedAircraft = SELECTED_AC;
					if (mp_ownerGame) {
						((GameAirForce*)mp_ownerGame)->cmdToGame(GET_LIMIT, form1, rtn1);
					}

					int maxSpeedAtAnyAlt = rtn1[0].maxSpeedAtAnyAlt;
					for (i = 0; i <= maxSpeedAtAnyAlt; i++) {
						SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_SPEED), 
								       CB_ADDSTRING, 
								       0,
								       (LPARAM)listStrSpeed[i]);
					}
					SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_SPEED), 
					       		CB_SETCURSEL, 
					       		speed,
					       		0);

					//------------ Alt ------------
					float alt = rtn[0].alt;
					float maxAlt = rtn1[0].maxAlt;
					UDACCEL udaccel;
					udaccel.nSec = 1;
					udaccel.nInc = -1;
					// if nInc = 1, alititude up-arrow decreases.
					// I dont know why.
					// So nInc = -1 to avoide this problem.
					
		m_initComCon.dwSize = sizeof(INITCOMMONCONTROLSEX);
		m_initComCon.dwICC = ICC_UPDOWN_CLASS;
		result = InitCommonControlsEx(&m_initComCon);

					HWND handle = GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT_UD);
					HWND handle1 = GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT);

					if (result = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT_UD), 
						UDM_SETBUDDY, 
						(WPARAM)GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT), 
						(LPARAM)0
					   	)) {
					}
					if (result = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT_UD), 
						UDM_SETBASE, 
						10,
						0
						)) {
					}
//					if (result = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT_UD), 
//						UDM_SETRANGE, 
//						MAKELPARAM((WORD) (INT)(maxAlt * 10), 0),
//						0
//						)) {
//					}
					if (result = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT_UD), 
						UDM_SETRANGE32, 
						(WPARAM) (INT)(maxAlt * 10),
						0
						)) {
					}
					if (result = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT_UD), 
						UDM_SETPOS, 
						0,
						(LPARAM)(INT)(alt * 10)
						)) {
					}
					if (result = SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT_UD), 
						UDM_SETACCEL, 
						(WPARAM)1,
						(LPARAM)(&udaccel)
						)) {
					}
					swprintf(x, L"%2.1f\n", alt );
					 // you need to use swprintf, not wsprintf
					 // because wsprintf does not support floating number.
					 // 2017/07/08
					Edit_SetText(
						GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT), 
						x
					);
					//------------ Bank ------------
					int bank = rtn[0].bank;
					for (i = 0; i < 6; i++) {
						SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_BANK), 
								       CB_ADDSTRING, 
								       0,
								       (LPARAM)listStrBank[i]);
					}
					SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_BANK), 
					       		CB_SETCURSEL, 
					       		bank,
					       		0);

					//------------ Nose ------------
					int nose = rtn[0].nose;
					for (i = 0; i < 3; i++) {
						SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_NOSE), 
								       CB_ADDSTRING, 
								       0,
								       (LPARAM)listStrNose[i]);
					}
					SendMessage(GetDlgItem(hDlgWnd, IDC_DEPLOY_NOSE), 
					       		CB_SETCURSEL, 
					       		nose,
					       		0);

				} else {
					return FALSE;
				}
			} else {
				return FALSE;
			}
			break;

		case WM_VSCROLL:
			{
			wchar_t  x[32];
			int	position;
			if ((HWND)lParam == GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT_UD)) {
					position = HIWORD(wParam);
					float newAlt = ((float)position)/10.0f;
					swprintf(x, L"%2.1f\n", newAlt); 
					Edit_SetText(
						GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT), 
						x
					);
			}
 			// When you click an updown arrow, WM_VSCROLL event is evoked,
			// but unlike scroll-bars, LOWORD(wParam) is not valid.
			// So you cannot expect LOWORD(wParam) == SB_LINEUP, etc.

			break;
			}

		case WM_NOTIFY:
			{
			wchar_t  x[32];
//			if (wParam == (WPARAM)ID_UPDOWN) {
				LPNMUPDOWN	lpnmud = (LPNMUPDOWN)lParam;
				if (lpnmud->hdr.code == UDN_DELTAPOS){
					int altx100ft = (lpnmud->iPos) + (lpnmud->iDelta);
					float newAlt = (float)altx100ft / 10;
					swprintf(x, L"%2.1f\n", newAlt); 
					Edit_SetText(
						GetDlgItem(hDlgWnd, IDC_DEPLOY_ALT), 
						x
					);
				}
//			}
			}

		default:
			return FALSE;
		// this default statement is very important.
		// dlgProc must return FALSE if it did not process the message.
		// Otherwise dialog manager in the system cannot perform the default
		// dialog operation and it behaves very strange such as
		// the dialog draws controls twice.
		// '17/07/03
		}
	}
	return TRUE;
					   
}

LRESULT CALLBACK MapAirForce::MyDlgProcDeploy(HWND hDlgWnd,
		                               UINT msg,
					       WPARAM wParam,
				  	       LPARAM lParam)
{
	MapAirForce *pThis = NULL;

	HWND hWinMap = GetWindow(hDlgWnd, GW_OWNER);
	pThis = (MapAirForce*)GetWindowLongPtr(hWinMap, GWLP_USERDATA);

	return pThis->HandleDlgDeploy(hDlgWnd, msg, wParam, lParam);
}


void MapAirForce::ShowDialogDeploy()
{
	int	id;

	id = DialogBox((HINSTANCE)GetWindowLong( 
					((GameAirForce*)mp_ownerGame)->Window(), 
					GWLP_HINSTANCE), 
		       MAKEINTRESOURCE(IDD_DIALOG_DEPLOY), 
			m_hwnd,
		       (DLGPROC)MyDlgProcDeploy
	); 
        InvalidateRect(m_hwnd, NULL, TRUE);

}

void MapAirForce::SetHexSelected(ResultHitTestHex result)
{
	m_realCorSelectedPrevX = m_realCorSelectedX;
	m_realCorSelectedPrevY = m_realCorSelectedY;
	m_evenSelectedPrev = m_evenSelected;
	m_realCorSelectedX = result.realCorX;
	m_realCorSelectedY = result.realCorY;
	m_evenSelected = result.even;
	m_virCorSelectedX = result.virCorX;
	m_virCorSelectedY = result.virCorY;
}

int MapAirForce::SelectAircraftFromStack(int pixelX, int pixelY, DWORD flags, cmdForm *p_rtn)
{
	POINT pt;
	pt.x = pixelX;
	pt.y = pixelY;

	HMENU hmenu, hSubmenu;

	hmenu = LoadMenu((HINSTANCE)GetWindowLong(m_hwnd, GWLP_HINSTANCE),
			MAKEINTRESOURCE(IDR_MAP_POPUP));
	hSubmenu = GetSubMenu(hmenu, 0);

	int id = 0;
	for  (id = 0; p_rtn[id].command != TAIL; id++) {
		AppendMenu(hSubmenu, MF_ENABLED, 610 + id,
			p_rtn[id].aircraftName);
	}
	RemoveMenu(hSubmenu, MF_BYCOMMAND, 601);
	ClientToScreen(m_hwnd, &pt);

	BOOL selectedID;
	selectedID = TrackPopupMenu(hSubmenu, 
		TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, m_hwnd, NULL);
	if (selectedID >= 610) {
		selectedID = selectedID - 610;
		((GameAirForce*)mp_ownerGame)->cmdToGame(SET_AC_SELECTED, p_rtn[selectedID], p_rtn);
	}
	DestroyMenu(hmenu);

	return 0;
}

int MapAirForce::SetFirstAircraftSelected(int pixelX, int pixelY, DWORD flags, cmdForm *p_rtn)
{
	int id = 0;

//	for  (id = 0; p_rtn[id].command != TAIL; id++) {
//	}
//	id--;
	// when command == TAIL, id++ is executed.
	// So you need to decrement id by 1 to point the last element.
	// 17/07/17

	((GameAirForce*)mp_ownerGame)->cmdToGame(SET_AC_SELECTED, p_rtn[id], p_rtn);

	return 0;
}

int MapAirForce::SelectGun(int pixelX, int pixelY, DWORD flags, cmdForm *p_form)
{
// return
// 	=-1:	none is selected
// 	=1: 	FFmg
//	=2: 	FFcannon
//	=3: 	FFmg+cannon
//	=4: 	FH
//	=5: 	FL
//	=6: 	F

	int	int_selectedID = -1;

	POINT pt;
	pt.x = pixelX;
	pt.y = pixelY;

	HMENU hmenu, hSubmenu;

	int i, j;
	bool	ffmg = false;
	bool	ffcannon = false;
	bool	fh = false;
	bool	fl = false;
	bool	f[6] = {false, false, false, false, false, false};
	bool	fabove = false;
	bool	fbelow = false;
	bool	flex = false;

	int numWeapons = 0;

	if ((p_form->firingRange.rangeFFmg >= 0)
	  && (p_form->gunPower.gunPowerFFmg > 0) ) {
		ffmg = true;
		int_selectedID = 1;
		numWeapons ++;
	}
	if ((p_form->firingRange.rangeFFcannon >= 0)
	  && (p_form->gunPower.gunPowerFFcannon > 0) ) {
		ffcannon = true;
		int_selectedID = 2;
		numWeapons ++;
	}
	if ((p_form->firingRange.rangeFH >= 0)
	  && (p_form->gunPower.gunPowerFH > 0) ) {
		fh = true;
		int_selectedID = 4;
		numWeapons ++;
	}
	if ((p_form->firingRange.rangeFL >= 0)
	  && (p_form->gunPower.gunPowerFL > 0) ) {
		fl = true;
		int_selectedID = 5;
		numWeapons ++;
	}
	
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 3; j++) {
			if ((p_form->firingRange.rangeF[i] >= 0)
			  && (p_form->gunPower.gunPowerF[j][i] > 0) ) {
				f[i] = true;
				flex = true;
			}
		}
	}
	if (flex) {
		int_selectedID = 6;
		numWeapons ++;
	}

	if (numWeapons >= 2) {
		hmenu = LoadMenu((HINSTANCE)GetWindowLong(m_hwnd, GWLP_HINSTANCE),
			MAKEINTRESOURCE(IDR_MAP_POPUP));
		if (hmenu == NULL) {
			return -1;
		}
		hSubmenu = GetSubMenu(hmenu, 0);
		if (hSubmenu == NULL) {
			return -1;
		}
		if (ffmg) {
			if (AppendMenu(hSubmenu, MF_ENABLED, 670 + 1,
				L"FF MG")
			) {} 
			else {
				return -1;
			}
		}
		if (ffcannon) {
			AppendMenu(hSubmenu, MF_ENABLED, 670 + 2,
				L"FF CANNON");
		}
		if (ffmg && ffcannon) {
			AppendMenu(hSubmenu, MF_ENABLED, 670 + 3,
				L"FF MG+CANNON");
		}
		if (fh) {
			AppendMenu(hSubmenu, MF_ENABLED, 670 + 4,
				L"FH");
		}
		if (fl) {
			AppendMenu(hSubmenu, MF_ENABLED, 670 + 5,
				L"FL");
		}
		if (flex) {
			AppendMenu(hSubmenu, MF_ENABLED, 670 + 6,
				L"F");
		}
//		RemoveMenu(hSubmenu, MF_BYCOMMAND, 601);
		ClientToScreen(m_hwnd, &pt);
// Pitfall: 2018/02/23
// this must be "BOOL" not "bool".
// BOOL is originally integer, but bool is just boolean.
// They are different from each other.	
		BOOL selectedID;
		selectedID = TrackPopupMenu(hSubmenu, 
			TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, m_hwnd, NULL);

		int_selectedID = (int)selectedID;
		if (int_selectedID >= 670) {
			int_selectedID = int_selectedID - 670;
//		((GameAirForce*)mp_ownerGame)->cmdToGame(SET_AC_SELECTED, p_rtn[selectedID], p_rtn);
		}
		DestroyMenu(hmenu);
	}

	return int_selectedID;
}
void MapAirForce::setSelectedFireArc(cmdForm *p_form, int selectedGun)
{
// selectedGun
// 	=1: FFmg
//	=2: FFcannon
//	=3: FFmg+cannon
//	=4: FH
//	=5: FL
//	=6: F

	int i;

	m_selectedFireArc.rangeFFmg = -1;
	m_selectedFireArc.rangeFFcannon = -1;
	m_selectedFireArc.rangeSameHexFF = false;
	m_selectedFireArc.rangeFH = -1;
	m_selectedFireArc.rangeFL = -1;
	m_selectedFireArc.rangeSameHexFL = false;
	for (i = 0; i < 8; i++) {
		m_selectedFireArc.rangeF[i] = -1;
	}

	switch (selectedGun) {
		case 1: 	//FFmg
			m_selectedFireArc.rangeFFmg = p_form->firingRange.rangeFFmg;
			break;
		case 2:		//FFcannon
			m_selectedFireArc.rangeFFcannon = p_form->firingRange.rangeFFcannon;
			break;
		case 3:		//FFmg+cannon
			m_selectedFireArc.rangeFFmg = p_form->firingRange.rangeFFmg;
			m_selectedFireArc.rangeFFcannon = p_form->firingRange.rangeFFcannon;
			break;
		case 4:		//FH
			m_selectedFireArc.rangeFH = p_form->firingRange.rangeFH;
			break;
		case 5: 	//FL
			m_selectedFireArc.rangeFL = p_form->firingRange.rangeFL;
			break;
		case 6: 	//F
			for (i = 0; i < 8; i++) {
				m_selectedFireArc.rangeF[i] = p_form->firingRange.rangeF[i];
			}
			break;
	}
	
}

void MapAirForce::OnLButtonDownGM_Fire(int pixelX, int pixelY, DWORD flags)
{
	cmdForm	form;
	form.command = GET_FIRE_RANGE;
	form.playerID = ALL_PLAYERS;
	form.selectedAircraft = SELECTED_AC;
	cmdForm rtn[20];
	((GameAirForce*)mp_ownerGame)->cmdToGame(GET_FIRE_RANGE, form, rtn);

	if (rtn[0].command != TAIL) {
		int	selectedGun = -1;
		selectedGun = SelectGun(pixelX, pixelY, flags, rtn);
		setSelectedFireArc(rtn, selectedGun);
	}

}

void MapAirForce::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	GameMode	gameMode = ((GameAirForce*)mp_ownerGame)->m_gameMode;

	D2D1_POINT_2F ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = ptMouse.x;
	const float dipY = ptMouse.y;

	ResultHitTestHex result;
	HitTestHex(dipX, dipY, &result);

	cmdForm	form;
	form.command = GET_HIT;
	form.playerID = ALL_PLAYERS;
	form.selectedAircraft = ALL_AC;
	form.virCorX = result.virCorX;
	form.virCorY = result.virCorY;
	cmdForm rtn[20];
	((GameAirForce*)mp_ownerGame)->cmdToGame(GET_HIT, form, rtn);

	int id = 0;
	int stackNum = 0;
	for (id = 0; rtn[id].command != TAIL; id++) {
		stackNum++;
	}

	if (stackNum == 0) {
		SetHexSelected(result);
		InvalidateRect(m_hwnd, NULL, FALSE);
	} else if (stackNum == 1) {
		rtn[0].command = SET_AC_SELECTED;
		((GameAirForce*)mp_ownerGame)->cmdToGame(SET_AC_SELECTED, rtn[0], rtn);
		SetHexSelected(result);
		InvalidateRect(m_hwnd, NULL, FALSE);
	} else {
		SetFirstAircraftSelected(pixelX, pixelY, flags, rtn);
		SetHexSelected(result);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
	
	POINT pt = {pixelX, pixelY};

	if (gameMode == GM_NOP
	||  gameMode == GM_DISPATCH) {
		if (DragDetect(m_hwnd, pt)) {
			SetMapStat(MAP_DEPLOY_DRAG);
			SetCapture(m_hwnd);
		}
	} 
//	if (gameMode ==GM_FIRE) {
		if (stackNum > 0 ) {
			OnLButtonDownGM_Fire(pixelX, pixelY, flags);
			InvalidateRect(m_hwnd, NULL, FALSE);
		}
//	}


}

void MapAirForce::OnRButtonDown(int pixelX, int pixelY, DWORD flags)
{
	D2D1_POINT_2F ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = ptMouse.x;
	const float dipY = ptMouse.y;

	ResultHitTestHex result;
	HitTestHex(dipX, dipY, &result);

	cmdForm	form;
	form.command = GET_HIT;
	form.playerID = ALL_PLAYERS;
	form.selectedAircraft = ALL_AC;
	form.virCorX = result.virCorX;
	form.virCorY = result.virCorY;
	cmdForm rtn[20];
	((GameAirForce*)mp_ownerGame)->cmdToGame(GET_HIT, form, rtn);

	int id = 0;
	int stackNum = 0;
	for (id = 0; rtn[id].command != TAIL; id++) {
		stackNum++;
	}

	if (stackNum == 0) {
		SetHexSelected(result);
		InvalidateRect(m_hwnd, NULL, FALSE);
	} else if (stackNum == 1) {
		rtn[0].command = SET_AC_SELECTED;
		((GameAirForce*)mp_ownerGame)->cmdToGame(SET_AC_SELECTED, rtn[0], rtn);
		SetHexSelected(result);
		InvalidateRect(m_hwnd, NULL, FALSE);
	} else {
		SelectAircraftFromStack(pixelX, pixelY, flags, rtn);
		SetHexSelected(result);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void MapAirForce::OnLButtonDoubleClicks(int pixelX, int pixelY, DWORD flags)
{
	D2D1_POINT_2F ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = ptMouse.x;
	const float dipY = ptMouse.y;

	ResultHitTestHex result;
	HitTestHex(dipX, dipY, &result);

	cmdForm	form;
	form.command = GET_HIT;
	form.playerID = ALL_PLAYERS;
	form.selectedAircraft = ALL_AC;
	form.virCorX = result.virCorX;
	form.virCorY = result.virCorY;
	cmdForm rtn[20];
	((GameAirForce*)mp_ownerGame)->cmdToGame(GET_HIT, form, rtn);

	int id = 0;
	int stackNum = 0;
	for (id = 0; rtn[id].command != TAIL; id++) {
		stackNum++;
	}

	if (stackNum == 0) {
		SetHexSelected(result);
		InvalidateRect(m_hwnd, NULL, FALSE);
	} else if (stackNum == 1) {
		rtn[0].command = SET_AC_SELECTED;
		((GameAirForce*)mp_ownerGame)->cmdToGame(SET_AC_SELECTED, rtn[0], rtn);
		SetHexSelected(result);
		ShowDialogDeploy();
		InvalidateRect(m_hwnd, NULL, FALSE);
	} else {
//		SelectAircraftFromStack(pixelX, pixelY, flags, rtn);
		SetHexSelected(result);
		ShowDialogDeploy();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void MapAirForce::OnLButtonUp() 
{
	GameMode	gameMode = ((GameAirForce*)mp_ownerGame)->m_gameMode;
	INT_PTR		id;

	if (gameMode == GM_DISPATCH) {
		((GameAirForce*)mp_ownerGame)->setGameMode(GM_NOP);
		SetMapStat(SELECTED);

		cmdForm form;
		form.command = DISPATCH;
		form.acStatus = DISPATCHED;
		form.virCorX = m_virCorSelectedX;
		form.virCorY = m_virCorSelectedY;
		form.heading = HEADING_N;
		form.speed = 0;
		form.alt = 0.0f;
		form.bank = BANK_LV;
		form.nose = NOSE_LV;
		((GameAirForce*)mp_ownerGame)->cmdToGame(DISPATCH, form, NULL);
		// you need parenthsis like above!!
		InvalidateRect(m_hwnd, NULL, FALSE);
	
		cmdForm form1;
		form1.command = REPAINT;
		form1.playerID = SELECTED_PLAYER;
		((GameAirForce*)mp_ownerGame)->cmdToGame(REPAINT, form1, NULL);
	

		ShowDialogDeploy();
//        	InvalidateRect(m_hwnd, NULL, TRUE);
	}

	if (m_mapStat == MAP_DEPLOY_DRAG) {
		SetMapStat(SELECTED);

		cmdForm form2;
		cmdForm rtn[20];
		form2.command = GET_AC;
		form2.playerID = SELECTED_PLAYER;
		form2.selectedAircraft = SELECTED_AC;
		((GameAirForce*)mp_ownerGame)->cmdToGame(GET_AC, form2, rtn);

		rtn[0].command = DISPATCH;
		rtn[0].acStatus = DEPLOYED;
		rtn[0].virCorX = m_virCorSelectedX;
		rtn[0].virCorY = m_virCorSelectedY;
		((GameAirForce*)mp_ownerGame)->cmdToGame(DISPATCH, rtn[0], NULL);
        	InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

int MapAirForce::getManuvMenuSelItemClimb(int selID, cmdForm *p_formRtnGetManuvable)
{
	if (selID == 620) {
		return (int)(p_formRtnGetManuvable->manuvable.maxClimb);
	} else {
		return (selID - 620) * 100;
	}
}

int MapAirForce::getManuvMenuSelItemDive(int selID, cmdForm *p_formRtnGetManuvable)
{
	if (selID == 640) {
		return ((int)(p_formRtnGetManuvable->manuvable.maxDive)) * (-1);
	} else {
		return (selID - 640) * (-100);
	}
}





void MapAirForce::ShowManuvMenuSelItem(int selID, int distance, cmdForm *p_formRtnGetManuvable)
{
	if (selID == 601) { // cancel previous manuv
		cmdForm form;
		cmdForm rtn[20];
		form.command = DELETE_MANUV;
		form.playerID = SELECTED_PLAYER;
		form.selectedAircraft = SELECTED_AC;
		((GameAirForce*)mp_ownerGame)->cmdToGame(DELETE_MANUV, form, rtn);
		return;
	} else if ((610 <= selID) 
	       &&  (selID <= 679)) {
		cmdForm form;
		cmdForm rtn[20];
		form.command = APPEND_MANUV;
		form.playerID = SELECTED_PLAYER;
		form.selectedAircraft = SELECTED_AC;
	
		int i = 0;
	
		if (distance > 0) {
			form.manuv[i] = distance;
			i++;
		}
	
		switch (selID) {
			case 610: // Loop Climb
				form.manuv[i] = MANUV_LC;
				break;
			case 611: // Roll Right
				form.manuv[i] = MANUV_RR;
				break;
			case 612: // Slip Right
				form.manuv[i] = MANUV_SR;
				break;
			case 613: // Turn Right
				form.manuv[i] = MANUV_TR;
				break;
			case 614: // Bank Right
				form.manuv[i] = MANUV_BR;
				break;
			case 615: // Bank Left
				form.manuv[i] = MANUV_BL;
				break;
			case 616: // Turn Left
				form.manuv[i] = MANUV_TL;
				break;
			case 617: // Slip Left
				form.manuv[i] = MANUV_SL;
				break;
			case 618: // Roll Left
				form.manuv[i] = MANUV_RL;
				break;
			case 619: // Loop Dive
				form.manuv[i] = MANUV_LD;
				break;
			case 660: // Power
				form.manuv[i] = MANUV_PW;
				break;
			case 661: // Break
				form.manuv[i] = MANUV_BK;
				break;
			case 662: // Finalize
				form.manuv[i] = (p_formRtnGetManuvable->manuvable).remainingMP - distance;
				break;
			default:
				if ((620 <= selID) && (selID <= 639)){
					form.manuv[i] = getManuvMenuSelItemClimb(selID, p_formRtnGetManuvable);
				} else if (( 640 <= selID) && (selID <= 659)) {
					form.manuv[i] = getManuvMenuSelItemDive(selID, p_formRtnGetManuvable);
				}
				break;
	
		}
		i++;
		form.manuv[i] = MANUV_EN;
	
		((GameAirForce*)mp_ownerGame)->cmdToGame(APPEND_MANUV, form, rtn);
	
		return;
	} else {
		return;
	}
}

void MapAirForce::ShowManuvMenuClimb(HMENU *p_hSubmenu, cmdForm *p_form)
{
	HMENU hSubmenuClimb = CreatePopupMenu();

	if ((p_form->manuvable).maxClimb > 0) {
		wchar_t climbStr[80];

		AppendMenu(*p_hSubmenu, MF_ENABLED | MF_POPUP, (UINT_PTR)hSubmenuClimb,
			L"Climb");
		if ((p_form->manuvable).maxClimb + 0.001f < 0.1f) {
			swprintf(climbStr, L"%1.3f\n", p_form->manuvable.maxClimb);
			AppendMenu(hSubmenuClimb, MF_ENABLED, 620, 
				climbStr);
		} else {
			int i;
			float alt = 0.1f;
			
			for (i = 1; alt <= p_form->manuvable.maxClimb + 0.001f; i++) {
			// 0.001f is added becasue "maxClimb = 0.9 is actually 0.899999.....
				swprintf(climbStr, L"%1.1f\n", alt);
				AppendMenu(hSubmenuClimb, MF_ENABLED, 620 + i, 
					climbStr);
				alt = alt + 0.1f;
			}
		}
	}
}

void MapAirForce::ShowManuvMenuDive(HMENU *p_hSubmenu, cmdForm *p_form)
{
	HMENU hSubmenuDive = CreatePopupMenu();

	if ((p_form->manuvable).maxDive > 0) {
		wchar_t climbStr[80];

		AppendMenu(*p_hSubmenu, MF_ENABLED | MF_POPUP, (UINT_PTR)hSubmenuDive,
			L"Dive");
		if ((p_form->manuvable).maxDive + 0.001f < 0.1f) {
			swprintf(climbStr, L"%1.3f\n", p_form->manuvable.maxDive);
			AppendMenu(hSubmenuDive, MF_ENABLED, 640, 
				climbStr);
		} else {
			int i;
			float alt = 0.1f;
			
			for (i = 1; alt <= p_form->manuvable.maxDive + 0.001f; i++) {
				swprintf(climbStr, L"%1.1f\n", alt);
				AppendMenu(hSubmenuDive, MF_ENABLED, 640 + i, 
					climbStr);
				alt = alt + 0.1f;
			}
		}
	}
}


int MapAirForce::ShowManuvMenu(int pixelX, int pixelY, DWORD flags, cmdForm *p_form, int distance)
{
	POINT pt;
	pt.x = pixelX;
	pt.y = pixelY;

//	static int i = 0;

	HMENU hmenu, hSubmenu;

//	i++;
//	if (i % 2 == 0) {
//		PostMessage(m_hwnd, WM_KEYDOWN, VK_ESCAPE, 0);
//	}

//	SendMessage(m_hwnd, WM_KEYDOWN, VK_LBUTTON, 0);
//	SendMessage(m_hwnd, WM_KEYDOWN, VK_RETURN, 0);

	hmenu = LoadMenu((HINSTANCE)GetWindowLong(m_hwnd, GWLP_HINSTANCE),
			MAKEINTRESOURCE(IDR_MAP_POPUP));
	hSubmenu = GetSubMenu(hmenu, 0);

	HMENU hSubmenuClimb = CreatePopupMenu();
	
	AppendMenu(hSubmenu, MF_ENABLED, 662,
		L"Finalize");

	ShowManuvMenuClimb(&hSubmenu, p_form);

	if ((p_form->manuvable).maxPower > 0) {
		AppendMenu(hSubmenu, MF_ENABLED, 660,
			L"Power");
	}

	if (((p_form->manuvable).loopClimb <= distance) 
	&&  ((p_form->manuvable).loopClimb >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 0,
			L"Loop Climb");
	}
	if (((p_form->manuvable).rollRight <= distance) 
	&&  ((p_form->manuvable).rollRight >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 1,
			L"Roll Right");
	}
	if (((p_form->manuvable).slipRight <= distance) 
	&&  ((p_form->manuvable).slipRight >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 2,
			L"Slip Right");
	}
	if (((p_form->manuvable).turnRight <= distance)
	&&  ((p_form->manuvable).turnRight >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 3,
			L"Turn Right");
	}
	if (((p_form->manuvable).bankRight <= distance)
	&&  ((p_form->manuvable).bankRight >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 4,
			L"Bank Right");
	}
	if (((p_form->manuvable).bankLeft <= distance)
	&&  ((p_form->manuvable).bankLeft >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 5,
			L"Bank Left");
	}
	if (((p_form->manuvable).turnLeft <= distance)
	&&  ((p_form->manuvable).turnLeft >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 6,
			L"Turn Left");
	}
	if (((p_form->manuvable).slipLeft <= distance)
	&&  ((p_form->manuvable).slipLeft >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 7,
			L"Slip Left");
	}
	if (((p_form->manuvable).rollLeft <= distance)
	&&  ((p_form->manuvable).rollLeft >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 8,
			L"Roll Left");
	}
	if (((p_form->manuvable).loopDive <= distance)
	&&  ((p_form->manuvable).loopDive >= 0)){
		AppendMenu(hSubmenu, MF_ENABLED, 610 + 9,
			L"Loop Dive");
	}

	if ((p_form->manuvable).maxBreak > 0) {
		AppendMenu(hSubmenu, MF_ENABLED, 661,
			L"Break");
	}

	ShowManuvMenuDive(&hSubmenu, p_form);


	int id = 0;
/*	for  (id = 0; p_rtn[id].command != TAIL; id++) {
		AppendMenu(hSubmenu, MF_ENABLED, 610 + id,
			p_rtn[id].aircraftName);
	}
	RemoveMenu(hSubmenu, MF_BYCOMMAND, 601);
 */
	ClientToScreen(m_hwnd, &pt);

	BOOL selectedID;
	selectedID = TrackPopupMenu(hSubmenu, 
		TPM_LEFTALIGN | TPM_VCENTERALIGN | TPM_RETURNCMD, 
		pt.x, pt.y, 0, m_hwnd, NULL);

	ShowManuvMenuSelItem((int)selectedID, distance, p_form);
/*	if (selectedID >= 610) {
		selectedID = selectedID - 610;
		((GameAirForce*)mp_ownerGame)->cmdToGame(SET_AC_SELECTED, p_rtn[selectedID], p_rtn);
	}
 */
	DestroyMenu(hmenu);

	return 0;
}

int MapAirForce::isHexReachableFwdPath(ResultHitTestHex *p_hex, cmdForm *p_form, int remainingMP)
{
	int int_heading = (int)(p_form->heading);
	int i;
	int reachable = -1;

	if ((p_form->virCorX == p_hex->virCorX)
	&&  (p_form->virCorY == p_hex->virCorY)) {
		reachable = 0;
	}
	for (i = 0; i < p_form->manuvable.remainingMP; i++) {
		switch (int_heading) {
			case 0: // North
				parseManuvModifyVirCorFwdNorth(p_form);
				break;
			case 60: // North East
				parseManuvModifyVirCorFwdNorthEast(p_form);
				break;
			case 120: // South East
				parseManuvModifyVirCorFwdSouthEast(p_form);
				break;
			case 180: // South
				parseManuvModifyVirCorFwdSouth(p_form);
				break;
			case 240: // South West
				parseManuvModifyVirCorFwdSouthWest(p_form);
				break;
			case 300: // North West
				parseManuvModifyVirCorFwdNorthWest(p_form);
				break;
		}
		if ((p_form->virCorX == p_hex->virCorX)
		&&  (p_form->virCorY == p_hex->virCorY)) {
			reachable = i + 1;
			break;
		}
	}
	return reachable;
}


void MapAirForce::modifyManeuverableByParsedManuv(cmdForm *form)
{
	switch (form->bank) {
		case BANK_IR:
			(form->manuvable).turnLeft = -1;
			break;
		case BANK_BR:
			(form->manuvable).turnLeft = -1;
			break;
		case BANK_LV:
			(form->manuvable).turnLeft = -1;
			(form->manuvable).turnRight = -1;
			(form->manuvable).slipLeft = -1;
			(form->manuvable).slipRight = -1;
			break;
		case BANK_BL:
			(form->manuvable).turnRight = -1;
			break;
		case BANK_IL:
			(form->manuvable).turnRight = -1;
			break;
		case BANK_IV:
			(form->manuvable).turnLeft = -1;
			(form->manuvable).turnRight = -1;
			(form->manuvable).slipLeft = -1;
			(form->manuvable).slipRight = -1;
			break;
		default:
			break;
	}
}

void MapAirForce::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
/*	if (flags & MK_LBUTTON) {
 *		const D2D1_POINT_2F dips = DPIScale::PixelsToDips(pixelX, pixelY);
 *		const float width = (dips.x - ptMouse.x) / 2;
 *		const float height = (dips.y - ptMouse.y) / 2;
 *		const float x1 = ptMouse.x + width;
 *		const float y1 = ptMouse.y + height;
 *
 *		ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
 *		InvalidateRect(m_hwnd, NULL, FALSE);
 *	}
 */

	GameMode gameMode = ((GameAirForce*)mp_ownerGame)->m_gameMode;
        D2D1_POINT_2F	pnt = DPIScale::PixelsToDips(pixelX, pixelY);
	const float dipX = pnt.x;
	const float dipY = pnt.y;
	int virCorX, virCorY;
	ResultHitTestHex result;


	if ((gameMode == GM_NOP) 
	||  (gameMode == GM_DISPATCH)) {
		if ((flags & MK_LBUTTON) ) {
			HitTestHex(dipX, dipY, &result);
			if ((m_realCorSelectedPrevX != result.virCorX) 
			 || (m_realCorSelectedPrevY != result.virCorY)
			 || (m_evenSelectedPrev != result.even)) {
				SetHexSelected(result);
				InvalidateRect(m_hwnd, NULL, FALSE);
			}			
		}
	}

	if (gameMode == GM_PLOT_MOVE) {
		cmdForm form;
		cmdForm rtn[20];
		form.command = GET_MANUVABLE;
		form.playerID = SELECTED_PLAYER;
		form.selectedAircraft = SELECTED_AC;
		((GameAirForce*)mp_ownerGame)->cmdToGame(GET_MANUVABLE, form, rtn);

		int remainingMP =  parseManuv(rtn);

		modifyManeuverableByParsedManuv(rtn);


		
//	PostMessage(m_hwnd, WM_KEYDOWN, VK_ESCAPE, 0);
//	PostMessage(m_hwnd, WM_KEYDOWN, VK_LBUTTON, 0);
//	PostMessage(m_hwnd, WM_KEYDOWN, VK_RETURN, 0);
//	PostMessage(m_hwnd, WM_CANCELMODE, 0,0);
		static int prevVirCorX;
		static int prevVirCorY;
		HitTestHex(dipX, dipY, &result);
		if ((prevVirCorX != result.virCorX)
		||  (prevVirCorY != result.virCorY)) {
			prevVirCorX = result.virCorX;
			prevVirCorY = result.virCorY;

			int hexReachable;
			cmdForm mouseHex = rtn[0];
			hexReachable = isHexReachableFwdPath(&result, &mouseHex, remainingMP);
			if (hexReachable >= 0) {
				ShowManuvMenu(pixelX, pixelY, flags, rtn, hexReachable);
				InvalidateRect(m_hwnd, NULL, FALSE);
			}
		} else {
//			PostMessage(m_hwnd, WM_KEYDOWN, VK_ESCAPE, 0);
			PostMessage(m_hwnd, WM_CANCELMODE, 0, 0);
		}

	}
/*
	if ((flags &MK_LBUTTON) && Selection()) {
		if (mode == DrawMode) {
			// Resize the ellipse
			const float width = (dipX - ptMouse.x) / 2;
			const float height = (dipY - ptMouse.y) / 2;
			const float x1 = ptMouse.x + width;
			const float y1 = ptMouse.y + height;

			Selection()->ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
		} else if (mode == DragMode) {
			// Move the ellipse
			Selection()->ellipse.point.x = dipX + ptMouse.x;
			Selection()->ellipse.point.y = dipY + ptMouse.y;

		}
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
 */
}


LRESULT MapAirForce::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO tmpScrollInfo;
	static int sbPosX;
	static int sbPosY;

	static INITCOMMONCONTROLSEX initComCon;

	switch (uMsg)
    	{
	    case WM_CREATE:
	        if (FAILED(D2D1CreateFactory(
	                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
	        {
	            return -1;  // Fail CreateWindowEx.
	        }
		if (FAILED(CreateDWriteTextFormat())) {
			return -1; // failed to create DWriteFactory or TextFormat
		}

		CoInitialize(NULL);
		if (FAILED(CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&m_p_imagingFactory))) {
			return -1; //failed to create WICImagig factory
		}
		//------------ updown arrow in alt edit-box of "deploy" dialog -----------
		bool result;
		m_initComCon.dwSize = sizeof(INITCOMMONCONTROLSEX);
		m_initComCon.dwICC = ICC_UPDOWN_CLASS;
		result = InitCommonControlsEx(&m_initComCon);

	        return 0;

	    case WM_DESTROY:
	        DiscardGraphicsResources();
	        SafeRelease(&pFactory);
	        PostQuitMessage(0);
	        return 0;

	    case WM_PAINT:
		tmpScrollInfo.cbSize = sizeof(tmpScrollInfo);
		tmpScrollInfo.fMask = SIF_POS;
		GetScrollInfo(m_hwnd, SB_VERT, &tmpScrollInfo);
		mHexVirCorOrgY = tmpScrollInfo.nPos;

		tmpScrollInfo.cbSize = sizeof(tmpScrollInfo);
		tmpScrollInfo.fMask = SIF_POS;
		GetScrollInfo(m_hwnd, SB_HORZ, &tmpScrollInfo);
		mHexVirCorOrgX = tmpScrollInfo.nPos * 2;

	        OnPaint();
	        return 0;

	    case WM_SIZE:
		{
		const int unitA = mHexSize;
		const int root3A = (int) ((float)unitA * 1.732050);
		const float unitAF = (float) unitA;
		const float root3AF = (float) root3A;

		const float RenderTargetSizeX = LOWORD(lParam);
		const float RenderTargetSizeY = HIWORD(lParam);
		const int hexCntX = (int) (RenderTargetSizeX / (unitAF * 6.0f)) + 1;
		m_hexCntX = hexCntX;
		const int hexCntY = (int) (RenderTargetSizeY / (root3AF * 2.0f)) + 1;
		m_hexCntY = hexCntY;

		tmpScrollInfo.cbSize = sizeof(tmpScrollInfo);
		tmpScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
		tmpScrollInfo.nMin = mHexVirCorOrgX;
		tmpScrollInfo.nMax = mHexVirCorOrgX + m_hexCntX * 2;
		tmpScrollInfo.nPage = m_hexCntX * 2;
		SetScrollInfo(m_hwnd, SB_HORZ, &tmpScrollInfo, TRUE);

		tmpScrollInfo.cbSize = sizeof(tmpScrollInfo);
		tmpScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
		tmpScrollInfo.nMin = mHexVirCorOrgY;
		tmpScrollInfo.nMax = mHexVirCorOrgY + m_hexCntY * 1;
		tmpScrollInfo.nPage = m_hexCntY * 1;
		SetScrollInfo(m_hwnd, SB_VERT, &tmpScrollInfo, TRUE);

	        Resize();
	        return 0;
		}

	    case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_LBUTTONUP:
		OnLButtonUp();
		return 0;

	    case WM_LBUTTONDBLCLK:
		OnLButtonDoubleClicks(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_RBUTTONDOWN:
		OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_MOUSEMOVE:
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;	

//	    case WM_USER + 5:
//		{
//		if (InSendMessage()) {
//			ReplyMessage(TRUE);
//		}
//		std::shared_ptr<MyEllipse> 	 ptrEllipse(new MyEllipse);
//		*ptrEllipse = sentEllipse;
//
//		ellipses.push_front(ptrEllipse);
//
//	        InvalidateRect(m_hwnd, NULL, FALSE);
//		return 0;	
//		}
		
	    case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDM_MAP_EDITAIRCRAFT:
				ShowDialogDeploy();
				break;
			case IDM_FILESAVE:
				if (OnFileSave()) {
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILESAVEAS:
				if (OnFileSaveAs()) {
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILEOPEN:
				if (OnFileOpen()) {
					OnPaint();
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILEEXIT:
	        		DiscardGraphicsResources();
			        SafeRelease(&pFactory);
	       			PostQuitMessage(0);
				break;
			case IDM_POPUPREMOVE:
				OnPopRemove();
				break;
			case IDM_POPUPSEND:
				OnPopSend();
				break;
			case IDM_NEWPLAYER:
				OnEditShowDialog();
				break;
		}
		return 0;

	    case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT) {
			SetCursor(hCursor);
			return TRUE;
		}
		break;
	    case WM_HSCROLL:

		tmpScrollInfo.cbSize = sizeof(tmpScrollInfo);
		tmpScrollInfo.fMask = SIF_ALL;
		GetScrollInfo(m_hwnd, SB_HORZ, &tmpScrollInfo);
		sbPosX = tmpScrollInfo.nPos;

		switch (LOWORD(wParam)) {
			case SB_LINELEFT:
				tmpScrollInfo.nPos -=1;
				break;
			case SB_LINERIGHT:
				tmpScrollInfo.nPos +=1;
				break;
			case SB_PAGELEFT:
				tmpScrollInfo.nPos -=m_hexCntX;
				break;
			case SB_PAGERIGHT:
				tmpScrollInfo.nPos +=m_hexCntX;
				break;
			case SB_THUMBTRACK:
				tmpScrollInfo.nPos =tmpScrollInfo.nTrackPos;
				break;
			default:
				break;
		}
		if (tmpScrollInfo.nPos < tmpScrollInfo.nMin) {
			tmpScrollInfo.nMin = tmpScrollInfo.nPos;
		}
		if (tmpScrollInfo.nPos + m_hexCntX * 2 > tmpScrollInfo.nMax) {
			tmpScrollInfo.nMax = tmpScrollInfo.nPos + m_hexCntX * 2;
		}
		tmpScrollInfo.fMask = SIF_ALL;
		SetScrollInfo(m_hwnd, SB_HORZ, &tmpScrollInfo, TRUE);
		tmpScrollInfo.fMask = SIF_POS;
		GetScrollInfo(m_hwnd, SB_HORZ, &tmpScrollInfo);
		if (tmpScrollInfo.nPos != sbPosX) {
			mHexVirCorOrgX = tmpScrollInfo.nPos * 2;
			ScrollWindowEx(m_hwnd, 
					(sbPosX - tmpScrollInfo.nPos) * mHexSize * 6,
					0,
					NULL,
					NULL,
					NULL,
					NULL,
//					SW_ERASE | SW_INVALIDATE 
//					SW_INVALIDATE | SW_SMOOTHSCROLL
					SW_INVALIDATE 
				       	// need "SW_INVALIDATE", otherwise Hex Cordinate 
					// would not be updated.
					// "SW_SMOOTHSCROO" would not update either. I dont know why.
			);
//			UpdateWindow(m_hwnd);
		}	
		break;
			
	    case WM_VSCROLL:

		const int unitA = mHexSize;
		const int root3A = (int) ((float)unitA * 1.732050);

		tmpScrollInfo.cbSize = sizeof(tmpScrollInfo);
		tmpScrollInfo.fMask = SIF_ALL;
		GetScrollInfo(m_hwnd, SB_VERT, &tmpScrollInfo);
		sbPosY = tmpScrollInfo.nPos;

		switch (LOWORD(wParam)) {
			case SB_TOP:
				break;
			case SB_BOTTOM:
				break;
			case SB_LINEUP:
				tmpScrollInfo.nPos -=1;
				break;
			case SB_LINEDOWN:
				tmpScrollInfo.nPos +=1;
				break;
			case SB_PAGEUP:
				tmpScrollInfo.nPos -=m_hexCntY;
				break;
			case SB_PAGEDOWN:
				tmpScrollInfo.nPos +=m_hexCntY;
				break;
			case SB_THUMBTRACK:
				tmpScrollInfo.nPos =tmpScrollInfo.nTrackPos;
				break;
			default:
				break;
		}
		if (tmpScrollInfo.nPos < tmpScrollInfo.nMin) {
			tmpScrollInfo.nMin = tmpScrollInfo.nPos;
		}
		if (tmpScrollInfo.nPos + m_hexCntY  > tmpScrollInfo.nMax) {
			tmpScrollInfo.nMax = tmpScrollInfo.nPos + m_hexCntY;
		}
		tmpScrollInfo.fMask = SIF_ALL;
		SetScrollInfo(m_hwnd, SB_VERT, &tmpScrollInfo, TRUE);
		tmpScrollInfo.fMask = SIF_POS;
		GetScrollInfo(m_hwnd, SB_VERT, &tmpScrollInfo);
		if (tmpScrollInfo.nPos != sbPosY) {
			mHexVirCorOrgY = tmpScrollInfo.nPos ;
			ScrollWindowEx(m_hwnd, 
					(sbPosY - tmpScrollInfo.nPos) * root3A * 2,
					0,
					NULL,
					NULL,
					NULL,
					NULL,
					SW_INVALIDATE 
				       	// need "SW_INVALIDATE", otherwise Hex Cordinate 
					// would not be updated.
					// "SW_SMOOTHSCROO" would not update either. I dont know why.
			);
	    	}
		break;
	}
	    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//	GameAirForce.cpp
//
/////////////////////////////////////////////////////////////////////////////////////

void GameAirForce::setGameMode(GameMode mode)
{
	m_gameMode = mode;
}

HRESULT GameAirForce::CreateDWriteTextFormat()
{
    static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 16;
    HRESULT hr;

        // Create a DirectWrite factory.
       hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(mPtrDWriteFactory),
            reinterpret_cast<IUnknown **>(&mPtrDWriteFactory)
            );
    if (SUCCEEDED(hr))
    {
        // Create a DirectWrite text format object.
        hr = mPtrDWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"", //locale
            &mPtrTextFormat
            );
    }
    if (SUCCEEDED(hr))
    {
        // Center the text horizontally and vertically.
        mPtrTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

        mPtrTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
       

    }

    return hr;
}


void GameAirForce::cmdGetDispatched(cmdForm *p_rtnForms) 
{
	cmdForm	dummy;
	cmdForm rtn[20];
	int	id = 0;
	int	rtnID = 0;

	dummy.command = GET_DISPATCHED;
	dummy.playerID = ALL_PLAYERS;
	dummy.selectedAircraft = ALL_AC;

	list<std::shared_ptr<PlayerAirForce>>::iterator itrPlayer;

	for (itrPlayer = mPlayers.begin(); itrPlayer != mPlayers.end(); itrPlayer++) {
		(*itrPlayer)->cmdToPlayer(GET_DISPATCHED, dummy, rtn);

		for (id = 0; rtn[id].command != TAIL; id++) {
			p_rtnForms[rtnID] = rtn[id];
			rtnID++;
		}

	}
	p_rtnForms[rtnID].command = TAIL;
	
}

void GameAirForce::cmdToGame(int cmd, cmdForm form, cmdForm *p_rtnForms)
{
	list<std::shared_ptr<PlayerAirForce>>::iterator itrPlayer;
	cmdForm rtn[20];
	int	id = 0;
	int	rtnID = 0;

	switch (cmd) {
		case DISPATCH:
			(*mItrSelectedPlayer)->cmdToPlayer(cmd, form, NULL);
			break;

		case GET_DISPATCHED:
			cmdGetDispatched(p_rtnForms);
			break;

		case REPAINT:
		case GET_AC:
		case GET_LIMIT:
		case GET_HIT:
		case SET_ACSTAT:
		case GET_MANUVABLE:
		case APPEND_MANUV:
		case DELETE_MANUV:
		case GET_FIRE_RANGE:
			if (form.playerID == SELECTED_PLAYER) {
				(*mItrSelectedPlayer)->cmdToPlayer(cmd, form, p_rtnForms);
			} else if (form.playerID == ALL_PLAYERS) {
				for (itrPlayer = mPlayers.begin(); 
					itrPlayer != mPlayers.end(); itrPlayer++) {
						(*itrPlayer)->cmdToPlayer(cmd, form, rtn);
					for (id = 0; rtn[rtnID].command != TAIL; id++) {
						p_rtnForms[rtnID] = rtn[id];
						rtnID++;
					}
				}
				p_rtnForms[rtnID].command = TAIL;
			} else {
				MessageBox(NULL, 
			           L"ERROR: illeagal form..\n",
				   NULL,
				   MB_OKCANCEL | MB_ICONSTOP
				   );
			}
			break;

		case SET_AC_SELECTED:
			if (  (form.playerID == ALL_PLAYERS) 
			    ||(form.playerID == SELECTED_PLAYER)) {
				MessageBox(NULL, 
			           L"ERROR: illeagal form...\n",
				   NULL,
				   MB_OKCANCEL | MB_ICONSTOP
				   );
				return;
			} else {
				for (itrPlayer = mPlayers.begin(); 
				     itrPlayer != mPlayers.end(); itrPlayer++) {
					if ((*itrPlayer)->mPlayerID == form.playerID) {
						(*itrPlayer)->cmdToPlayer(cmd, form, rtn);

						shared_ptr<PlayerAirForce> ptrPlayer = (*itrPlayer);
						mPlayers.remove(ptrPlayer);
						mPlayers.push_front(ptrPlayer);
						mItrSelectedPlayer = mPlayers.begin();
						break;
						// This break is important!!
						// After removing and pushing the element,
						// control goes back to the "for (itr = mAircrafs.begin(); itr != .....
						// statement. But something is wrong with itr and
						// error occurs. To avoid another itr test, break is necessary.
						// 17/07/17
					}
				}

			}
			break;

		case SET_PLAYER_SELECTED:
			if (  (form.playerID == ALL_PLAYERS) 
			    ||(form.playerID == SELECTED_PLAYER)) {
				MessageBox(NULL, 
			           L"ERROR: illeagal form...\n",
				   NULL,
				   MB_OKCANCEL | MB_ICONSTOP
				   );
				return;
			} else {
				for (itrPlayer = mPlayers.begin(); 
				     itrPlayer != mPlayers.end(); itrPlayer++) {
					if ((*itrPlayer)->mPlayerID == form.playerID) {
						mItrSelectedPlayer = itrPlayer;
						break;
						// This break is important!!
						// After removing and pushing the element,
						// control goes back to the "for (itr = mAircrafs.begin(); itr != .....
						// statement. But something is wrong with itr and
						// error occurs. To avoid another itr test, break is necessary.
						// 17/07/17
					}
				}

			}
			break;


	}
}

void GameAirForce::addPlayerAndCraftIfNeeded(int acmIndex, int pmIndex, wchar_t *name)
{
	GameAirForce *pThis = NULL;
	pThis = (GameAirForce*)GetWindowLongPtr(m_hwnd, GWLP_USERDATA);


	if (mNewPlayerRegStat == NOTREG) {
		shared_ptr<PlayerAirForce> ptrPlayer(new PlayerAirForce);
		ptrPlayer->mp_ownerGame = (LONG_PTR)pThis;
		ptrPlayer->mPlayerID = mNewPlayerID;
		ptrPlayer->mPlayerRegStat = TMPREG;
		mPlayers.push_front(ptrPlayer);
		mItrSelectedPlayer = mPlayers.begin();
		mNewPlayerRegStat = TMPREG;
	}

	shared_ptr<Aircraft> ptrAircraft (new Aircraft);
	ptrAircraft->mAircraftModel = acmIndex;
	ptrAircraft->mPilotModel = pmIndex;
	wcscpy(ptrAircraft->mAircraftName,  name);
	ptrAircraft->mAircraftRegStat = TMPREG;
	ptrAircraft->m_stat = ON_TRAY;

	ptrAircraft->mp_owner = (UINT_PTR)((*mItrSelectedPlayer).get());

	float x, y;
//	(ptrAircraft->mp_owner)->WhereToDraw(&x, &y);
	(*mItrSelectedPlayer)->WhereToDraw(&x, &y);
	ptrAircraft->m_trayPixelX = x;
	ptrAircraft->m_trayPixelY = y;

	(*mItrSelectedPlayer)->mAircrafts.push_front(ptrAircraft);

}

LRESULT CALLBACK GameAirForce::MyDlgProcNewPlayer(HWND hDlgWnd,
		                               UINT msg,
					       WPARAM wParam,
				  	       LPARAM lParam)
{
	GameAirForce *pThis = NULL;

	HWND hWinGame = GetWindow(hDlgWnd, GW_OWNER);
	pThis = (GameAirForce*)GetWindowLongPtr(hWinGame, GWLP_USERDATA);

	return pThis->HandleDlgNewPlayer(hDlgWnd, msg, wParam, lParam);

// "pThis->" is required because HandleMessageNewMap is a non-static function
// and therefore it requires real object.	
//  This wrapping structure is very important!!
//  Because of this, i can make HandleDlgNewPlayer function non-static
//  and i can keep many member varialbes non-static.
//

}


LRESULT GameAirForce::HandleDlgNewPlayer(HWND hDlgWnd,
       					UINT msg,
					WPARAM wParam,
					LPARAM lParam)
{
//	GameAirForce *pThis = NULL;
//	pThis = (GameAirForce*)GetWindowLongPtr(m_hwnd, GWLP_USERDATA);
	int i;

	switch (msg) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					mNewPlayerID ++;
					EndDialog(hDlgWnd, IDOK);
					(*mItrSelectedPlayer)->Create(
						L"Player",		//lpWindowName 
						WS_OVERLAPPEDWINDOW, 	//dwStyle
						0,			//dwExStyle
						CW_USEDEFAULT,		//x
						CW_USEDEFAULT,		//y
						100,		//nWidth
						800,		//nHeight
						0,			//hWndParent
						0			//hMenu
						);
					ShowWindow(
						(*mItrSelectedPlayer)->Window(), 
						SW_SHOWDEFAULT
						);
					break;
				case IDCANCEL:
					EndDialog(hDlgWnd, IDOK);
					break;
				case IDC_BUTTON1:  // "add ->" button
					LRESULT resultNation;
					LRESULT result1, result2;
					resultNation = SendMessage(GetDlgItem(hDlgWnd, IDC_COMBO1),
								CB_GETCURSEL,
								0,
								0);
					result1 = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1),
						             LB_GETCURSEL, 
						       	     0,
						             0);	     
					result2 = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST2),
						             LB_GETCURSEL, 
						       	     0,
						             0);	     
					if (resultNation == CB_ERR) {
						MessageBox(NULL, 
						           L"Select Nationality..\n",
							   NULL,
							   MB_OKCANCEL | MB_ICONSTOP
							   );
					} else if (( result1 == LB_ERR) || (result2 == LB_ERR) ){
						MessageBox(NULL, 
						           L"Select Aircraft and Pilot..\n",
							   NULL,
							   MB_OKCANCEL | MB_ICONSTOP
							   );
					} else {
						LRESULT acmIndex, pmIndex;
						int acIndex;
						acmIndex = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1),
						             LB_GETITEMDATA, 
						       	     (int)result1,
						             0);	     
//						pmIndex = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1),
//						             LB_GETITEMDATA, 
//						       	     (int)result2,
//						             0);	     
						acIndex = numberAircraft(acmIndex);
						wchar_t name[MAX_NAMELENGTH];
						wsprintf(name, L"%s\#%03d + %s", 
							 aircraftModels[acmIndex].name,
							 acIndex,
							 pilotModels[(int)result2].name);
						i = (int)result1;
						SendMessage(GetDlgItem(hDlgWnd, IDC_LIST3), 
						       LB_ADDSTRING, 
						       0,
						       (LPARAM)name);
						
						addPlayerAndCraftIfNeeded(acmIndex, (int)result2, name);
					}


					break;
				case IDC_COMBO1: // Nationality Combo Box
					switch (HIWORD(wParam)) {
						case CBN_SELCHANGE:
							LRESULT r, itemCnt;
							r = SendMessage(GetDlgItem(hDlgWnd, IDC_COMBO1), 
									CB_GETCURSEL,
									0,
									0);
							SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
							       LB_RESETCONTENT, 
							       0,
							       0);
							if (r == 0) {
								for (i = 0; i < 6; i++) {
									if (aircraftModels[i].nation == BRITAIN){
										SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_ADDSTRING, 
											       0,
											       (LPARAM)aircraftModels[i].name);
										itemCnt = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_GETCOUNT, 
											       0,
											       0);
										SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_SETITEMDATA, 
											       (WPARAM)(itemCnt - 1),
											       (LPARAM)i);
									}
								}
							}
							if (r == 1) {
								for (i = 0; i < 6; i++) {
									if (aircraftModels[i].nation == GERMAN){
										SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_ADDSTRING, 
											       0,
											       (LPARAM)aircraftModels[i].name);
										itemCnt = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_GETCOUNT, 
											       0,
											       0);
										SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_SETITEMDATA, 
											       (WPARAM)(itemCnt - 1),
											       (LPARAM)i);
									}
								}
							}
							if (r == 2) {
								for (i = 0; i < 6; i++) {
									if (aircraftModels[i].nation == USA){
										SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_ADDSTRING, 
											       0,
											       (LPARAM)aircraftModels[i].name);
										itemCnt = SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_GETCOUNT, 
											       0,
											       0);
										SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), 
											       LB_SETITEMDATA, 
											       (WPARAM)(itemCnt - 1),
											       (LPARAM)i);
									}
								}
							}
					}


				default:
					return FALSE;
			}
			break;
		case WM_INITDIALOG:
			{
			mNewPlayerRegStat = NOTREG;
//			string x = to_string(mNewPlayerID);
			wchar_t  x[32];
			wsprintf(x, L"%03d\n", mNewPlayerID);
			static LPTSTR	lpOut; 


//			StringCbPrintf(lpOut, 512, TEXT("%d"), mNewPlayerID++);
			Static_SetText(GetDlgItem(hDlgWnd, IDC_PLAYERID),
					   (LPTSTR)x);
					   
			for (i = 0; i < 4; i++) {
				SendMessage(GetDlgItem(hDlgWnd, IDC_LIST2), 
						       LB_INSERTSTRING, 
						       (WPARAM)i,
						       (LPARAM)pilotModels[i].name);
			}
			for (i = 0; i < 3; i++) {
				SendMessage(GetDlgItem(hDlgWnd, IDC_COMBO1), 
						       CB_ADDSTRING, 
						       0,
						       (LPARAM)liststrNationality[i]);
			}
			break;
			}
		default:
			return FALSE;
				              
	}
	return TRUE;
}

LRESULT CALLBACK GameAirForce::MyDlgProcNewMap(HWND hDlgWnd,
		                               UINT msg,
					       WPARAM wParam,
				  	       LPARAM lParam)
{
	GameAirForce *pThis = NULL;

//	pThis = GetWindowLongPtr(GameAirForce::m_hwnd, GWLP_USERDATA);
//	return pThis->HandleMessageNewMap(hDlgWnd, msg, wParam, lParam);
//
	HWND hWinGame = GetWindow(hDlgWnd, GW_OWNER);
	pThis = (GameAirForce*)GetWindowLongPtr(hWinGame, GWLP_USERDATA);

	return pThis->HandleMessageNewMap(hDlgWnd, msg, wParam, lParam);
// "pThis->" is required because HandleMessageNewMap is a non-static function
// and therefore it requires real object.	
}



LRESULT GameAirForce::HandleMessageNewMap(HWND hDlgWnd,
					  UINT msg,
					  WPARAM wParam,
				  	  LPARAM lParam)
{
	GameAirForce *pThis = NULL;
	HWND hWinGame = GetWindow(hDlgWnd, GW_OWNER);
	pThis = (GameAirForce*)GetWindowLongPtr(hWinGame, GWLP_USERDATA);

	int i;

	static int tmpHexSize;

	switch (msg) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					{ // this parenthis is added to avoid compile error C2360
					EndDialog(hDlgWnd, IDOK);
					shared_ptr<MapAirForce>	ptrMap(new MapAirForce);
					ptrMap->mHexSize = tmpHexSize;
					ptrMap->mp_ownerGame = (LONG_PTR)pThis;
					mMaps.push_front(ptrMap);
					mItrMaps = mMaps.begin();
					if (!(ptrMap->Create(L"Map", 
							      WS_OVERLAPPEDWINDOW
							    | WS_HSCROLL
							    | WS_VSCROLL)))
					{
						return 0;
					}

					ShowWindow(ptrMap->Window(), SW_SHOWDEFAULT);
					break;
					}
				case IDCANCEL:
					EndDialog(hDlgWnd, IDOK);
					break;
				case IDC_COMBO1: // Hex Size Combo Box
					switch (HIWORD(wParam)) {
						case CBN_SELCHANGE:
							LRESULT r;
							r = SendMessage(GetDlgItem(hDlgWnd, IDC_COMBO1), 
									CB_GETCURSEL,
									0,
									0);
							if (r ==0) { // hexSize = 10
								tmpHexSize = 10;
							} else if (r == 1) { // hexSize = 15
								tmpHexSize = 15;
							} else if (r == 2) { // hexSize = 20
								tmpHexSize = 20;
							} else if (r == 3) { // hexSize = 25
								tmpHexSize = 25;
							} else if (r == 4) { // hexSize = 30
								tmpHexSize = 30;
							} else {
							}
							break;
					}
					break;
				default:
					return FALSE;
			}
			break;
		case WM_INITDIALOG:
			for (i = 0; i < 5; i++) {
				wchar_t c[8];
				wsprintf(c, L"%d\n", 10 + i * 5);
				SendMessage(GetDlgItem(hDlgWnd, IDC_COMBO1), 
						       CB_ADDSTRING, 
						       0,
						       (LPARAM)c);
			}
			tmpHexSize = 20;
			break;
		default:
			return FALSE;
				              
	}
	return TRUE;
}


void GameAirForce::OnEditShowDialog()
{
	int	id;

//	id = DialogBoxW(GetModuleHandle(NULL), 
	id = DialogBox((HINSTANCE)GetWindowLong(m_hwnd, GWLP_HINSTANCE), 
		       MAKEINTRESOURCE(IDD_DIALOG1), 
			m_hwnd,
		       (DLGPROC)MyDlgProcNewPlayer); 
        InvalidateRect(m_hwnd, NULL, TRUE);

}

void GameAirForce::OnEditNewMapDialog()
{
	int	id;

//	id = DialogBoxW(GetModuleHandle(NULL), 
	id = DialogBox((HINSTANCE)GetWindowLong(m_hwnd, GWLP_HINSTANCE), 
		       MAKEINTRESOURCE(IDD_DIALOG_NEWMAP), 
			m_hwnd,
		       (DLGPROC)MyDlgProcNewMap); 
        InvalidateRect(m_hwnd, NULL, TRUE);

}

int GameAirForce::numberAircraft(int acmID)
{
	int i;

	if (acmID > MAX_AIRCRAFTMODELNUMBER) {
		return -1;
	} else {
		i = aircraftID[acmID];
		aircraftID[acmID] = i + 1;
		return i;
	}
}

void GameAirForce::OnPaint()
{
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr)) {
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);

		pRenderTarget->BeginDraw();
		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_COLOR_F originalColor = pBrush->GetColor();
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

		
		D2D1_POINT_2F orgPt;
			orgPt.x = 30.0f;
			orgPt.y = 30.0f;
		float height = 20.0f;
		float width = 100.0f;
		float interval = 110.0f;
		float arrowLength = 20.0f;
		int i;

		D2D1_POINT_2F pt;

		for (i = 0; i < 11; i++) {
			pt.x = orgPt.x + interval * i;
			pt.y = orgPt.y;

			if (m_gameMode != i) {
				pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGray));
			}
			pRenderTarget->DrawText(
				listStrGamePhase[i],
				wcslen(listStrGamePhase[i]) ,
				mPtrTextFormat,
				D2D1::RectF(pt.x,
				            pt.y,
				    	    pt.x + width, 
					    pt.y + height),
				pBrush
			);
			pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	
			if (i == 10) {
				pRenderTarget->DrawLine(
					D2D1::Point2F(pt.x + width,
				       	pt.y + height / 2.0f),
					D2D1::Point2F(pt.x + width + arrowLength,
				       	pt.y + height / 2.0f),
					pBrush, 1.0f
				);
			} else {
				DrawArrow(pRenderTarget,
					pBrush,
					D2D1::Point2F(pt.x + width,
					       	pt.y + height / 2.0f),
					D2D1::Point2F(pt.x + width + arrowLength,
					       	pt.y + height / 2.0f),
					10.0f,
					1.0f);
			}
		}
		pRenderTarget->DrawLine(D2D1::Point2F(pt.x + width + arrowLength, 
						      pt.y + height / 2.0f),
					D2D1::Point2F(pt.x + width + arrowLength,
						      pt.y + height / 2.0f + 50.0f),
				       	pBrush, 1.0f);
		pRenderTarget->DrawLine(D2D1::Point2F(pt.x + width + arrowLength,
						      pt.y + height / 2.0f + 50.0f),
					D2D1::Point2F(orgPt.x + width + arrowLength / 2.0f,
						      pt.y + height / 2.0f + 50.0f),
				       	pBrush, 1.0f);
		DrawArrow(pRenderTarget,
			pBrush,
			D2D1::Point2F(orgPt.x + width + arrowLength / 2.0f,
			       	      pt.y + height / 2.0f + 50.0f),
			D2D1::Point2F(orgPt.x + width + arrowLength / 2.0f,
			       	      orgPt.y + height / 2.0f),
			10.0f,
			1.0f);

		wchar_t strGameTurn[80];
		swprintf (strGameTurn, L"Game Turn = %3d\n", m_gameTurn); 
		pRenderTarget->DrawText(
			strGameTurn,
			wcslen(strGameTurn),
			mPtrTextFormat,
			D2D1::RectF(50, 100, 200, 200),
			pBrush
		);

		pBrush->SetColor(originalColor);

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}

void GameAirForce::OnButtonProceed()
{
	switch (m_gameMode) {
		case GM_NOP:
			m_gameMode = GM_FIRE;
			break;
		case GM_FIRE:
			m_gameMode = GM_BOMB;
			break;
		case GM_BOMB:
			m_gameMode = GM_BAILOUT;
			break;
		case GM_BAILOUT:
			m_gameMode = GM_SPOT;
			break;
		case GM_SPOT:
			m_gameMode = GM_DET_ADV;
			break;
		case GM_DET_ADV:
			m_gameMode = GM_PLOT_MOVE;
			break;
		case GM_PLOT_MOVE:
			m_gameMode = GM_DET_STAT;
			break;
		case GM_DET_STAT:
			m_gameMode = GM_FLAKS;
			break;
		case GM_FLAKS:
			m_gameMode = GM_MOVE;
			break;
		case GM_MOVE:
			m_gameMode = GM_RECORD;
			break;
		case GM_RECORD:
			m_gameMode = GM_FIRE;
			m_gameTurn ++;
			break;
	}
        InvalidateRect(m_hwnd, NULL, TRUE);
}

LRESULT GameAirForce::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND h_buttonProceed;

	switch (uMsg)
    	{
	    case WM_CREATE:
	        if (FAILED(D2D1CreateFactory(
	                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
	        {
	            return -1;  // Fail CreateWindowEx.
	        }

		if (FAILED(CreateDWriteTextFormat())) {
			return -1; // failed to create DWriteFactory or TextFormat
		}
		CoInitialize(NULL);
		if (FAILED(CoCreateInstance(CLSID_WICImagingFactory,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_IWICImagingFactory,
						(LPVOID*) &mp_imagingFactory))) {
			return -1;
		}

		h_buttonProceed = CreateWindow(
			L"BUTTON",
			L"Proceed",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			800, 120, 100, 20,
			m_hwnd,
			(HMENU) IDC_GAME_PROCEED,
			(HINSTANCE)GetWindowLong(m_hwnd, GWLP_HINSTANCE), 
			NULL
		);
		break;

	    case WM_DESTROY:
	        DiscardGraphicsResources();
	        SafeRelease(&pFactory);
	        PostQuitMessage(0);
	        return 0;

	    case WM_PAINT:
	        OnPaint();
	        return 0;


	    case WM_SIZE:
	        Resize();
	        return 0;

	    case WM_LBUTTONDOWN:
//		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_LBUTTONUP:
//		OnLButtonUp();
		return 0;

	    case WM_RBUTTONDOWN:
//		OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	    case WM_MOUSEMOVE:
//		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;	

//	    case WM_USER + 5:
//		{
//		if (InSendMessage()) {
//			ReplyMessage(TRUE);
//		}
//		std::shared_ptr<MyEllipse> 	 ptrEllipse(new MyEllipse);
//		*ptrEllipse = sentEllipse;
//
//		ellipses.push_front(ptrEllipse);
//
//	        InvalidateRect(m_hwnd, NULL, FALSE);
//		return 0;	
//		}
		
	    case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDM_FILESAVE:
				if (OnFileSave()) {
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILESAVEAS:
				if (OnFileSaveAs()) {
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILEOPEN:
				if (OnFileOpen()) {
					OnPaint();
				} else {
					return EXIT_FAILURE;
				}
				break;
			case IDM_FILEEXIT:
	        		DiscardGraphicsResources();
			        SafeRelease(&pFactory);
	       			PostQuitMessage(0);
				break;
			case IDM_POPUPREMOVE:
				OnPopRemove();
				break;
			case IDM_POPUPSEND:
				OnPopSend();
				break;
			case IDM_NEWPLAYER:
				OnEditShowDialog();
				break;
			case IDM_NEWMAP:
				OnEditNewMapDialog();
				break;
			case IDC_GAME_PROCEED:
				OnButtonProceed();
				break;
		}
		return 0;

	    case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT) {
			SetCursor(hCursor);
			return TRUE;
		}
		break;
	    }
	    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////
//
//	Main
//
////////////////////////////////////////////////////////////////////////////////

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	GameAirForce win;


	initRand();

	if (!win.Create(L"Circle", WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);
/*
	GameAirForce subwin;

	if (!subwin.Create(L"Circle", WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}

	ShowWindow(subwin.Window(), nCmdShow);

	win.SetPeerWnd(subwin.Window());
	subwin.SetPeerWnd(win.Window());
 */

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
