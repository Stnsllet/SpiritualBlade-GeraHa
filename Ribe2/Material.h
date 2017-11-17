
#ifndef __RB_MATERIAL__
#define __RB_MATERIAL__

#include "XMLPaser.h"

struct Material : public D3DMATERIAL8
{
	Material()
	{
		AllZero( *this );
		Diffuse.r = Ambient.r = 1.0f;
		Diffuse.g = Ambient.g = 1.0f;
		Diffuse.b = Ambient.b = 1.0f;
		Diffuse.a = Ambient.a = 1.0f;
	}
	Material( const D3DCOLORVALUE& Default, float SpecularPow=0 )
	{
		Diffuse = Ambient = Emissive = Specular = Default;
		Power = SpecularPow;
	}
	void AbientDiffuse( const float& A, const float& R, const float& G, const float& B )
	{
		Diffuse.a = A;
		Ambient.r = R;
		Ambient.g = G;
		Ambient.b = B;
	}

	bool SetXML( const XMLPaser& ReadXML )
	{
		bool bRead = false;
		bRead |= ReadColorXML( ReadXML, _T("Diffuse"), Diffuse );
		bRead |= ReadColorXML( ReadXML, _T("Ambient"), Ambient );
		bRead |= ReadColorXML( ReadXML, _T("Emissive"), Emissive );

		XMLPaser SpecularXML;
		if( !FAILED(ReadXML.Find(_T("Specular"),SpecularXML)) )
		{
			Specular = ReadColorXML( SpecularXML );
			Power = SpecularXML.AttributeFloat(_T("pow"));
			bRead |= true;
		}
		return bRead;
	}
	bool ReadColorXML( const XMLPaser& ReadXML, LPCTSTR Name, D3DCOLORVALUE& Output ) const
	{
		XMLPaser Find;
		if( FAILED(ReadXML.Find(Name,Find)) )return false;
		Output = ReadColorXML( Find );
		return true;
	}

	D3DCOLORVALUE ReadColorXML( const XMLPaser& ReadXML ) const
	{
		D3DCOLORVALUE Col;
		Col.a = ReadXML.AttributeFloat(_T("a"));
		Col.r = ReadXML.AttributeFloat(_T("r"));
		Col.g = ReadXML.AttributeFloat(_T("g"));
		Col.b = ReadXML.AttributeFloat(_T("b"));
		return Col;
	}
};

#endif // __RB_MATERIAL__
