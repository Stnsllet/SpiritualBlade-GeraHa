// Display.h: Display クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAY_H__F7F38016_E038_476A_A2CE_681B787652A3__INCLUDED_)
#define AFX_DISPLAY_H__F7F38016_E038_476A_A2CE_681B787652A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class D3DObject;
#include "Device.h"
#include "RbPosition.h"
#include "RbTransform.h"

class Display  : public Device
{
public:
	Display();
	virtual ~Display();
	DISPDEVICE* operator ->()const{return pDevice;}
	operator DISPDEVICE*() const{return pDevice;}
	const RbPosition& GetViewPos() const{return DispInfo.Scroll;}

	virtual HRESULT SetRenderState( const D3DCAPS8& HalCaps );

	virtual HRESULT SetViewPos( float x, float y, float z=0.0f);
	virtual HRESULT ScrollView( float x, float y, float z=0.0f);
	HRESULT SetLight( float x=-1, float y=-1, float z=-1 );
	HRESULT UpView( float Length, float Angle );
	HRESULT SetProjection( float aspect =1.0f );
	bool SetFishEye( float angle );
	float GetDisplayScale( const D3DXVECTOR3& Pos ) const;
	RbPosition MouseToView( int x, int y, float DestZ=0.0f ) const;
	RbPosition DispToView( const D3DXVECTOR3& DispPos ) const;
	RbPosition ViewToDisp( const D3DXVECTOR3& Pos ) const;
	RbPosition BillBordPos( const D3DVECTOR& BordPos, const D3DVECTOR& BasePos ) const;

	const D3DMATERIAL8& RefMaterial() const{return DispMaterial;}
	const D3DMATERIAL8& RefPlaneMaterial() const{return PlaneMaterial;}
	const RbTransform& UnViewMat() const{return DispInfo.UnViewMat;}
	const RbTransform& RefAllMat() const{return DispInfo.AllMat;}

	const bool& ZBiasOK() const{return bZbias;}
	const float& RefZposRate() const{return ZPosRate;}
protected:
	RbTransform GetAllMat() const;
	virtual HRESULT ResetAllMat( HRESULT hr );
	virtual HRESULT _RestoreDevice();

public:
	struct _DispInfo
	{
		RbPosition Scroll;
		struct UpViewInfo
		{
			float Length, Angle;
			RbPosition Pos;
		}UpView;
		float FishEye;

		RbTransform WorldMat;
		RbTransform ViewMat;
		RbTransform ProjectionMat;
		RbTransform AllMat;
		RbTransform AllReverseMat;
		RbTransform UnViewMat;

		RbPosition LightVector;
	};
	const _DispInfo& RefDispInfo() const{return DispInfo;}

protected:
	_DispInfo DispInfo;
	D3DLIGHT8 DispLight;
	D3DMATERIAL8 DispMaterial;
	D3DMATERIAL8 PlaneMaterial;
	bool bZbias;
	float ZPosRate;
};

#endif // !defined(AFX_DISPLAY_H__F7F38016_E038_476A_A2CE_681B787652A3__INCLUDED_)
