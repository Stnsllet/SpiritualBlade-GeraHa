// MusicPortSelecter.h: MusicPortSelecter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSICPORTSELECTER_H__F4C7F8CA_ED82_452C_B1A4_57176DC4F959__INCLUDED_)
#define AFX_MUSICPORTSELECTER_H__F4C7F8CA_ED82_452C_B1A4_57176DC4F959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MusicPortSelecter  
{
public:
	struct Caps : public DMUS_PORTCAPS
	{
		Caps(){AllZero(*this);dwSize = sizeof(DMUS_PORTCAPS);}
	};
	MusicPortSelecter();
	virtual ~MusicPortSelecter();

	bool SetMusicObject( IDirectMusic* pMusic );
	bool Check();
	bool Check( IDirectMusic* pMusic );
	HRESULT GetUsePort( IDirectMusicPort** ppOutput ) const;
	const GUID& RefPortGuid() const;

protected:
	bool OverProperty( const DMUS_PORTCAPS& Ref ) const;
	bool IsOutput( const DMUS_PORTCAPS& Ref ) const;
	bool IsHard( const DMUS_PORTCAPS& Ref ) const;
	long GetGxLevel( const DMUS_PORTCAPS& Ref ) const;
	long GetDnsLevel( const DMUS_PORTCAPS& Ref ) const;

	CComPtr<IDirectMusic> pDevice;
	Caps Using;
};

#endif // !defined(AFX_MUSICPORTSELECTER_H__F4C7F8CA_ED82_452C_B1A4_57176DC4F959__INCLUDED_)
