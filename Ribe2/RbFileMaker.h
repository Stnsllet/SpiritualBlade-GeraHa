// RbFileMaker.h: RbFileMaker クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBFILEMAKER_H__33DA5BF9_5872_4A44_9363_FC5F55A71A39__INCLUDED_)
#define AFX_RBFILEMAKER_H__33DA5BF9_5872_4A44_9363_FC5F55A71A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 圧縮ディレクトリ作成クラス
class RbFileMaker  
{
public:
	RbFileMaker();
	~RbFileMaker();

private:
	template<class T> class AutoPtr
	{
	public:
		AutoPtr(){p=NULL;}
		AutoPtr( T* pPtr ){p=pPtr;}
		~AutoPtr(){if(p)delete p;}
		T* Detach(){T* t = p;p=NULL;return t;}
		operator T*() const{return p;}
		T** operator &() const{return &p;}
		T* p;
	};
	class FlagFile
	{
	public:
		FlagFile();
		~FlagFile();
		HRESULT ReadFile( LPCTSTR pFileName, BYTE** ppOutput );
	private:
		struct FlagData
		{
			BYTE NestCompress;
			long SourceSize;
			BYTE NumData[256];
			BYTE* pData;
		};
		HRESULT ReadData( const FlagData* pData, BYTE** ppOutput );
		BYTE ReadFlag( const BYTE* pData, long FlagPos, BYTE ReadMask );

		static const BYTE MaskList[8];
		static const long Filter;
		BYTE Compress;
		BYTE DataCount[256];
	};
};

#endif // !defined(AFX_RBFILEMAKER_H__33DA5BF9_5872_4A44_9363_FC5F55A71A39__INCLUDED_)
