// MusicPortSelecter.cpp: MusicPortSelecter �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MusicPortSelecter.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

MusicPortSelecter::MusicPortSelecter()
{

}

MusicPortSelecter::~MusicPortSelecter()
{

}

// �|�[�gGUID���擾����
const GUID& MusicPortSelecter::RefPortGuid() const
{
	return Using.guidPort;
}

// �|�[�g���擾����
HRESULT MusicPortSelecter::GetUsePort( IDirectMusicPort** ppOutput ) const
{
	if( ppOutput == NULL )return E_POINTER;
	if( pDevice == NULL )return E_PENDING;
	if( !IsOutput(Using) )return E_PENDING;

	DMUS_PORTPARAMS8 Params;
	AllZero(Params).dwSize = sizeof(Params);
	HRESULT hr = pDevice->CreatePort( Using.guidPort, &Params,
		ppOutput, NULL );
	if( hr == S_FALSE )return E_FAIL;
	return hr;
}

// �I�u�W�F�N�g�Z�b�g
bool MusicPortSelecter::SetMusicObject( IDirectMusic* pMusic )
{
	if( pMusic == NULL )return false;

	pDevice.Release();

	pMusic->QueryInterface(IID_IDirectMusic8, (void **)&pDevice);
	return (pDevice != NULL);
}

// �Z�b�g & �`�F�b�N����
bool MusicPortSelecter::Check( IDirectMusic* pMusic )
{
	if( !SetMusicObject(pMusic) )return false;
	return Check();
}

// �`�F�b�N����
bool MusicPortSelecter::Check()
{
	if( pDevice == NULL )return false;
	DWORD i = 0;
	while( true )
	{
		Caps GetCaps;
		HRESULT hr = pDevice->EnumPort( i, & GetCaps );
		if( FAILED(hr) )break;
		if( hr == S_FALSE )break;

		i++;
		if( !IsOutput(GetCaps) )continue;

		if( !IsOutput(Using) )
		{
			Using = GetCaps;
			continue;
		}
		if( OverProperty(GetCaps) )Using = GetCaps;
	}
	return IsOutput(Using);
}

// ����ʂ��`�F�b�N
bool MusicPortSelecter::OverProperty( const DMUS_PORTCAPS& Ref ) const
{
	if( !IsHard(Using) && IsHard(Ref) )return true;
	if( IsHard(Using) && !IsHard(Ref) )return false;
	if( GetDnsLevel(Using) < GetDnsLevel(Ref) )return true;
	if( GetGxLevel(Using) < GetGxLevel(Ref) )return true;

	return false;
}


bool MusicPortSelecter::IsHard( const DMUS_PORTCAPS& Ref ) const
{
	return !(Ref.dwFlags & DMUS_PC_SOFTWARESYNTH);
}

bool MusicPortSelecter::IsOutput( const DMUS_PORTCAPS& Ref ) const
{
	return (Ref.dwClass == DMUS_PC_OUTPUTCLASS);
}

long MusicPortSelecter::GetGxLevel( const DMUS_PORTCAPS& Ref ) const
{
	long Value = 0;
	if( Using.dwFlags & DMUS_PC_GMINHARDWARE )Value += 4;
	if( Using.dwFlags & DMUS_PC_GSINHARDWARE )Value += 1;
	if( Using.dwFlags & DMUS_PC_XGINHARDWARE  )Value += 1;
	return Value;
}

long MusicPortSelecter::GetDnsLevel( const DMUS_PORTCAPS& Ref ) const
{
	long Value = 0;
	if( Using.dwFlags & DMUS_PC_DLS )Value += 1;
	if( Using.dwFlags & DMUS_PC_DLS2 )Value += 2;
	return Value;
}
