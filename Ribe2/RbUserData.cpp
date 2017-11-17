// RbUserData.cpp: RbUserData �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"
#include "StringSplitter.h"
#include "XMLList.h"
#include "RbMember.h"
#include "RbPaty.h"
#include "RbCache.h"
#include "RbUserData.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RbUserData::RbUserData() : SaveVersion( 1,0,0 )
{
	AllClear();
	DataUpDateID = 0;
	ExPatyNum = -5;
}

long RbUserData::UpDate()
{
	return ::InterlockedIncrement( &DataUpDateID );
}

RbUserData::~RbUserData()
{
}

long RbUserData::GetPasonID( const String& PasonName ) const
{
	return GetCache().Pason.GetPasonID( PasonName );
}

HRESULT RbUserData::ReadGlobalFlg()
{
	return ReadGlobalFlg( _T("System/GlobalFlg.xml") );
}
// �O���[�o���t���O�ǂݍ���
HRESULT RbUserData::ReadGlobalFlg( LPCTSTR FileName )
{
	XMLPaser AllFlg;
	HRESULT ret = AllFlg.Load( FileName );
	if( FAILED(ret) )return ret;

	XMLList FlgList;
	FlgList.Query( AllFlg, _T("Flg") );
	int Count = FlgList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		XMLPaser Flg;
		FlgList.GetPaser( i, Flg );
		long no = Flg.AttributeLong(TAG::NO);
		if( no == 0 )continue;
		String Name = Flg.AttributeString(TAG::NAME);
		if( Name.empty() )continue;
		ATLASSERT( GlobalFlgID.GetID(Name) < 0 );
		GlobalFlgID.AddID( Name, no );
	}
	return S_OK;
}

// �t���O�Z�b�g
bool RbUserData::SetFlg( const String& Name, bool Flg )
{
	long ID = GlobalFlgID.GetID( Name );
	if( ID > 0 )return SetFlg( GrobalData.EventFlag, ID, Flg );
	return SetFlg( HotFlg, HotFlgID.GetID(Name), Flg );
}

// �t���O�Q�b�g
bool RbUserData::GetFlg( const String& Name ) const
{
	long ID = GlobalFlgID.GetID( Name );
	if( ID > 0 )return GetFlg( GrobalData.EventFlag, ID );
	return GetFlg( HotFlg, HotFlgID.GetID(Name) );
}

// �t���O�`�F�b�N
bool RbUserData::CheckFlgs( LPCTSTR Names, bool bFlg ) const
{
	StringSplitter<> Split( Names );
	int Count = Split.Count();
	for( int i = 0; i < Count; i++ )
	{
		if( GetFlg(Split[i]) != bFlg )return false;
	}
	return true;
}

// �ق��ƃt���O�N���A
bool RbUserData::ClearHotFlg()
{
	AllZero( HotFlg );
	return true;
}

// �p�����[�^�擾
CComVariant RbUserData::GetUserParam( const String& ParamName ) const
{
	std::map<String,RbParameter>::const_iterator itr = UserParams.find( ParamName );
	if( itr == UserParams.end() )return CComVariant();
	return itr->second;
}

// �A�C�e�����擾
long RbUserData::GetItemCount( long ItemNo ) const
{
	std::map<long,HaveItems>::const_iterator itr = HaveItemList.find( ItemNo );
	if( itr == HaveItemList.end() )return 0;
	return itr->second.Count;
}

// �A�C�e��������
bool RbUserData::AddItem( long ItemNo, long Count )
{
	if( ItemNo <= 0 )return false;
	std::map<long,HaveItems>::iterator itr = HaveItemList.find( ItemNo );
	HaveItems& Ref = (itr==HaveItemList.end())
		?(AllZero(HaveItemList[ItemNo])):(itr->second);
	Ref.id = ItemNo;
	Ref.Count += Count;
	Ref.Count = Trim( Ref.Count, (long)99, (long)0 );
	if( Ref.Count <= 0 )
	{
		HaveItemList.erase( HaveItemList.find( ItemNo ) );
	}
	return true;
}

// ���[�_�[�擾
bool RbUserData::GetLeader( LPCTSTR Name, MemberStatus& Output ) const
{
	long PatyID = GetPatyID( Name );
	if( PatyID < 0 )return GetMember( Name, Output );
	return GetMember( PatyID, 0, Output );
}

// �����o�[�`�F�b�N
bool RbUserData::InMember( const String& Member ) const
{
	return InMember( GetPasonID(Member) );
}

// �����o�[�`�F�b�N
bool RbUserData::InMember( long PasonID ) const
{
	return ( MemberList.find( PasonID ) != MemberList.end() );
}

// �ҋ@�����o�[���擾
long RbUserData::GetStockCount() const
{
	std::map<long,MemberStatus>::const_iterator itr = MemberList.begin();
	long Count = 0;
	while( itr != MemberList.end() )
		if( 0 > GetPatyID((itr++)->second.PasonName) )++Count;
	return Count;
}

// �����o�[�擾
bool RbUserData::GetMember( const String& Name, MemberStatus& Output ) const
{
	return GetMember( GetPasonID(Name), Output );
}

// �����o�[�擾
bool RbUserData::GetMember( long PasonID, MemberStatus& Output ) const
{
	std::map<long, MemberStatus>::const_iterator itr = MemberList.find( PasonID );
	if( itr == MemberList.end() )return false;

	Output = itr->second;
	return true;
}

// �����o�[�擾
bool RbUserData::GetMember( long PatyNumber, long MemberNumber, MemberStatus& Output ) const
{
	PatyStatus Paty;
	if( !GetPaty( PatyNumber, Paty ) )return false;
	RbPaty State( Paty );
	if( MemberNumber >= State.MemberCount() )return false;
	return GetMember( Paty.Member[MemberNumber].id, Output );
}

// �p�[�e�B�擾
bool RbUserData::GetPaty( long PatyNumber, PatyStatus& Output ) const
{
	if( PatyNumber == ExPatyNum )
	{
		Output = ExPaty;
		return true;
	}
	if( PatyNumber < 0 )return false;
	if( PatyNumber >= PatyList.size() )return false;

	Output = PatyList[PatyNumber];
	return true;
}

// �p�[�e�B�擾
long RbUserData::GetPatyID( LPCTSTR Member ) const
{
	if( Member == NULL )return -1;

	for( int i = 0; i < PatyList.size(); i++ )
	{
		RbPaty Paty(PatyList[i]);
		if( Paty.InMember(Member) )return i;
	}
	return -1;
}

// ����p�[�e�B�H
bool RbUserData::InExPaty( LPCTSTR Member ) const
{
	return RbPaty(ExPaty).InMember(Member);
}

// ������
HRESULT RbUserData::InitData()
{
	AllClear();
	ReadGlobalFlg();

	return S_OK;
}

// ���ׂď���
void RbUserData::AllClear()
{
	AllZero( GrobalData );
	AllZero( HotFlg );
	RbPaty(ExPaty).Create();
	GrobalData.Money = 0;
	HaveItemList.clear();
	PatyList.clear();
	MemberList.clear();
	UserParams.clear();
	SYSTEMTIME Systime;
	GetLocalTime( &Systime );
	SystemTimeToFileTime( &Systime, &FileTime );
	UpDate();
}

// ���݈ʒu�ύX
bool RbUserData::SetHousePos( LPCTSTR House )
{
	if( House == NULL )return false;
	GrobalData.NowPos = House;
//	UpDate();
	return true;
}

long RbUserData::Money( long Add )
{
	GrobalData.Money += Add;
	GrobalData.Money = Trim( GrobalData.Money, (long)99999999, (long)0 );
	UpDate();
	return GrobalData.Money;
}
