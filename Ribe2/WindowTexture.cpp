// WindowTexture.cpp: WindowTexture �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "WindowTexture.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

WindowTexture::WindowTexture()
{

}

WindowTexture::~WindowTexture()
{

}

// �E�C���h�E��`��RECT�擾
RECT WindowTexture::GetRECT( LPCTSTR name ) const
{
	RECT ret;
	std::map<String,Plane>::const_iterator itr = Planes.find(name);
	if( itr == Planes.end() ){AllZero(ret);return ret;}

	ret.left = itr->second.x;
	ret.top = itr->second.y;
	ret.right = ret.left + itr->second.Width;
	ret.bottom = ret.top + itr->second.Height;

	return ret;
}

// �e�N�X�`���ǂݏo��
HRESULT WindowTexture::SetTexture( DISPDEVICE* pDevice, int no )
{
	std::map<long,String>::const_iterator itr = FileMap.find(no);
	if( itr == FileMap.end() )itr = FileMap.begin();
	if( itr == FileMap.end() )return E_FAIL;

	return LoadTexture( pDevice, itr->second );
}

// �V�X�e���t�@�C���ݒ�
bool WindowTexture::LoadSystemXML( const XMLPaser& SystemXML )
{
	XMLList FileList;
	FileList.Query( SystemXML, TAG::FILE );
	long Count = FileList.GetCount();
	for(int i = 0; i < Count; i++ )
	{
		FileMap[i] = FileList.GetItem(i).AttributeString(TAG::PATH);
	}
	return (Count>0);
}

// �e�N�X�`���ǂݏo��
HRESULT WindowTexture::Load( DISPDEVICE* pDevice, int no )
{
	return Load( pDevice, _T("System/WindowTexture.xml"), no );
}

// �e�N�X�`���ǂݏo��
HRESULT WindowTexture::Load( DISPDEVICE* pDevice, LPCTSTR FileName, int no )
{
	Planes.clear();

	XMLPaser DefineFile;
	HRESULT ret = DefineFile.Load( FileName );
	if( FAILED(ret) )return ret;

	ret = SetTexture( pDevice, no );
	if( FAILED(ret) )
	{
		ret = SetTexture( pDevice, 0 );
		if( FAILED(ret) )return ret;
	}

	XMLList XMLPlanes;
	ret = XMLPlanes.Query( DefineFile, TAG::PLANE );
	if( FAILED(ret) )return ret;

	int Count = XMLPlanes.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadPlane( XMLPlanes.GetItem(i) );
	}

	return S_OK;
}

HRESULT WindowTexture::ReadPlane( const XMLPaser& PlaneXML )
{
	Plane ReadData;

	String name = PlaneXML.AttributeString( TAG::NAME );
	if( name.length() <= 0 )return E_FAIL;

	ReadData.x = PlaneXML.AttributeLong( TAG::X );
	ReadData.y = PlaneXML.AttributeLong( TAG::Y );
	ReadData.Width = PlaneXML.AttributeLong( TAG::W );
	ReadData.Height = PlaneXML.AttributeLong( TAG::H );

	Planes[name] = ReadData;

	return S_OK;
}
