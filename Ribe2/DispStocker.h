
#ifndef __DISP_STOCKER__
#define __DISP_STOCKER__

#include "RbTransform.h"
#include "Display.h"

struct DispStocker
{
	struct Render
	{
		Render( Display& Disp, const D3DRENDERSTATETYPE& SetType)
			: Ref( Disp ), Type(SetType)
		{
			Ref->GetRenderState( Type, &OldValue );
		}
		HRESULT Set( const DWORD& NewValue )
		{
			return Ref->SetRenderState( Type, NewValue );
		}
		~Render()
		{
			Ref->SetRenderState( Type, OldValue );
		}
		Display& Ref;
		D3DRENDERSTATETYPE Type;
		DWORD OldValue;
	};

	struct TexRender
	{
		TexRender( Display& Disp, const D3DTEXTURESTAGESTATETYPE& SetType, long SetStage=0)
			: Ref( Disp ), Type(SetType), Stage(SetStage)
		{
			Ref->GetTextureStageState( Stage, Type, &OldValue );
		}
		HRESULT Set( const DWORD& NewValue )
		{
			return Ref->SetTextureStageState( Stage, Type, NewValue );
		}
		~TexRender()
		{
			Ref->SetTextureStageState( Stage, Type, OldValue );
		}
		Display& Ref;
		long Stage;
		D3DTEXTURESTAGESTATETYPE Type;
		DWORD OldValue;
	};

	struct TexUV
	{
		TexUV( Display& Disp, long SetStage=0 )
			: U(Disp, D3DTSS_ADDRESSU, SetStage)
			, V(Disp, D3DTSS_ADDRESSV, SetStage){}
		HRESULT Set( const DWORD& NewValue )
		{
			U.Set( NewValue );
			return V.Set( NewValue );
		}
		TexRender U;
		TexRender V;
	};

	struct Transform
	{
		Transform( Display& Disp, const D3DTRANSFORMSTATETYPE& SetType )
			: Ref( Disp ), Type(SetType)
		{
			Ref->GetTransform( Type, &OldTrans );
		}
		HRESULT Set( const D3DXMATRIX& NewTrans )
		{
			return Ref->SetTransform( Type, &NewTrans );
		}
		HRESULT AddFront( const D3DXMATRIX& NewTrans )
		{
			return Ref->MultiplyTransform( Type, &NewTrans );
		}
		~Transform()
		{
			Ref->SetTransform( Type, &OldTrans );
		}
		Display& Ref;
		RbTransform OldTrans;
		D3DTRANSFORMSTATETYPE Type;
	};

	struct ViewTransSet
	{
		ViewTransSet( Display& Disp )
			:World( Disp, D3DTS_WORLD )
			,View( Disp, D3DTS_VIEW )
			,Proj( Disp, D3DTS_PROJECTION )
		{
			static RbTransform NoTrans;
			All = World.OldTrans;
			All *= View.OldTrans;
		//	All *= Proj.OldTrans;
			World.Set( NoTrans );
			View.Set( All );
		//	Proj.Set( All );
		}
		Transform World;
		Transform View;
		Transform Proj;
		RbTransform All;
	};


	struct Shaker : public Transform
	{
		Shaker( Display& Disp ) : Transform( Disp,D3DTS_WORLD ){}
		HRESULT Set( float Scale )
		{
			Scale /= 2.0f;
			RbTransform Trans;
			Trans.AddScroll( Rndf2(-Scale,Scale), Rndf2(-Scale,Scale), Rndf2(-Scale,Scale) );

			return Transform::AddFront( Trans );
		}
	};

	struct AllTrans
	{
		AllTrans( Display& Disp )
		{
			D3DMATRIX Get;
			Disp->GetTransform( D3DTS_WORLD, &Get );
			All *= Get;
			Disp->GetTransform( D3DTS_VIEW, &Get );
			All *= Get;
			Disp->GetTransform( D3DTS_PROJECTION, &Get );
			All *= Get;
		}
		RbTransform All;
	};

	struct Material
	{
		Material( Display& Disp )
			:Ref( Disp )
		{
			Ref->GetMaterial( &OldMaterial );
		}
		~Material()
		{
			Ref->SetMaterial( &OldMaterial );
		}
		HRESULT Set( const D3DMATERIAL8& NewMaterial )
		{
			return Ref->SetMaterial( &NewMaterial );
		}
		HRESULT SetAlpha( float Alpha )
		{
			D3DMATERIAL8 NewMaterial = OldMaterial;
			NewMaterial.Diffuse.a = Alpha;
			return Set( NewMaterial );
		}
		Display& Ref;
		D3DMATERIAL8 OldMaterial;
	};

	struct BillBord
	{
		BillBord( Display& Disp )
			:OldMaterial( Disp )
			,ZStock( Disp, D3DRS_ZENABLE )
		{
			OldMaterial.Set( Disp.RefPlaneMaterial() );
			ZStock.Set( FALSE );
		}
		Material OldMaterial;
		Render ZStock;
	};
};

#endif // __DISP_STOCKER__
