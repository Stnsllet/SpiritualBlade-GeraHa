// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂�

#define _WIN32_WINNT 0x0400

// Windows �w�b�_�[ �t�@�C��:
#include <windows.h>
#include <shellapi.h>
#include <atlbase.h>

// C �����^�C�� �w�b�_�[ �t�@�C��
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "TAG.h"
#include "MyHead.h"
#include "SystemMessage.h"
#include "VertexDef.h"

class String;
class GameSystem;
extern GameSystem* g_pSystem;
extern D3DCOLOR MYWHITE;

bool InfoPopup( LPCTSTR Msg );
bool SystemPopup( LPCTSTR Msg );
bool UserOrStr( String& InOut );
bool UserOrStr( CComBSTR& InOut );

// ���[�J�� �w�b�_�[ �t�@�C��

// TODO: �v���O�����ŕK�v�ȃw�b�_�[�Q�Ƃ�ǉ����Ă��������B

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
