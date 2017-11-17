# Microsoft Developer Studio Project File - Name="Ribe2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Ribe2 - Win32 Debug2
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Ribe2.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Ribe2.mak" CFG="Ribe2 - Win32 Debug2"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Ribe2 - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Ribe2 - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "Ribe2 - Win32 Debug2" ("Win32 (x86) Application" 用)
!MESSAGE "Ribe2 - Win32 HiTrace" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ribe2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /machine:I386 /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Ribe2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Ribe2 - Win32 Debug2"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Ribe2___Win32_Debug2"
# PROP BASE Intermediate_Dir "Ribe2___Win32_Debug2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Ribe2___Win32_Debug2"
# PROP Intermediate_Dir "Ribe2___Win32_Debug2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_POINTER_CHECHK" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Ribe2 - Win32 HiTrace"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Ribe2___Win32_HiTrace"
# PROP BASE Intermediate_Dir "Ribe2___Win32_HiTrace"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "HiTrace"
# PROP Intermediate_Dir "HiTrace"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__HI_TRACE__" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /machine:I386 /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /machine:I386 /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Ribe2 - Win32 Release"
# Name "Ribe2 - Win32 Debug"
# Name "Ribe2 - Win32 Debug2"
# Name "Ribe2 - Win32 HiTrace"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AllWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\ArmyFormation.cpp
# End Source File
# Begin Source File

SOURCE=.\ArmyStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\AttackObjectDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoBuyWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\BackSwapper.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\BatComWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleDataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleSystemStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleTagetSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\BGMMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\BuyWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ComBaseWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DamegeSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DataConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DataUpdater.cpp
# End Source File
# Begin Source File

SOURCE=.\DDObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultPlanes.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultRenderState.cpp
# End Source File
# Begin Source File

SOURCE=.\Device.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectAttacker.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DischargeWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\DragItemWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Draws.cpp
# End Source File
# Begin Source File

SOURCE=.\DummyWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\EmployWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\EquipSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\EquipStatusWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ExCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\ExPatyEditWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ExSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceCache.cpp
# End Source File
# Begin Source File

SOURCE=.\File.cpp
# End Source File
# Begin Source File

SOURCE=.\FileLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\FomationSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\InfomationWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemCache.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemListWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\JobCache.cpp
# End Source File
# Begin Source File

SOURCE=.\JobEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\LandBase.cpp
# End Source File
# Begin Source File

SOURCE=.\LandEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\LandMapData.cpp
# End Source File
# Begin Source File

SOURCE=.\LandMoveType.cpp
# End Source File
# Begin Source File

SOURCE=.\LandScape.cpp
# End Source File
# Begin Source File

SOURCE=.\LandType.cpp
# End Source File
# Begin Source File

SOURCE=.\LandWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\LearningSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\LevelUpSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniStatusWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MoneyWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveAlgo.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveLiner.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBaseWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiWave.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicPortSelecter.cpp
# End Source File
# Begin Source File

SOURCE=.\NewsChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\NewsListWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\NewsWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\PasonCache.cpp
# End Source File
# Begin Source File

SOURCE=.\PathTexCache.cpp
# End Source File
# Begin Source File

SOURCE=.\PatyEditWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\PatyHpWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyCache.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyMaster.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyStation.cpp
# End Source File
# Begin Source File

SOURCE=.\PopUpMsgWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\PoweredWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\RbActStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\RbAttackAlgo.cpp
# End Source File
# Begin Source File

SOURCE=.\RbBattlePaty.cpp
# End Source File
# Begin Source File

SOURCE=.\RbBattleStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\RbBGMMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\RbCache.cpp
# End Source File
# Begin Source File

SOURCE=.\RbConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\RbDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\RbEventSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\RbFileMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\RbGameData.cpp
# End Source File
# Begin Source File

SOURCE=.\RbGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\RbMember.cpp
# End Source File
# Begin Source File

SOURCE=.\RbParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\RbPaty.cpp
# End Source File
# Begin Source File

SOURCE=.\RbPlaneCache.cpp
# End Source File
# Begin Source File

SOURCE=.\RbPoly.cpp
# End Source File
# Begin Source File

SOURCE=.\RbPolyPoly.cpp
# End Source File
# Begin Source File

SOURCE=.\RbPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\RbRect.cpp
# End Source File
# Begin Source File

SOURCE=.\RbSound.cpp
# End Source File
# Begin Source File

SOURCE=.\RbSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\RbStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\RbTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\RbTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\RbTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\RbUserData.cpp
# End Source File
# Begin Source File

SOURCE=.\RbWindowSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Ribe2.cpp
# End Source File
# Begin Source File

SOURCE=.\Ribe2.rc
# End Source File
# Begin Source File

SOURCE=.\SaveWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\SelCharaWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectMsgWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\SellWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopListWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillCache.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite2.cpp
# End Source File
# Begin Source File

SOURCE=.\SPWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\SquareTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBaseWinow.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StockMemberWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\StrCommander.cpp
# End Source File
# Begin Source File

SOURCE=.\StrParamCom.cpp
# End Source File
# Begin Source File

SOURCE=.\TacticsAlgo.cpp
# End Source File
# Begin Source File

SOURCE=.\TAG.cpp
# End Source File
# Begin Source File

SOURCE=.\TagetAlgo.cpp
# End Source File
# Begin Source File

SOURCE=.\TexRender.cpp
# End Source File
# Begin Source File

SOURCE=.\TextFont.cpp
# End Source File
# Begin Source File

SOURCE=.\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureCache.cpp
# End Source File
# Begin Source File

SOURCE=.\TimerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiAngelDust.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiArrow.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiArrowRain.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiAttackSpreadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBall.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBeast.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBlastEdge.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBlink.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBlinkBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBlueWave.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBomb.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBurnFlare.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBurst.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiBurstBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiChainBurst.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiChainFlare.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiComet.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiCrimson.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDamege.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDark.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDarkBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDarkBlazeBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDarkBlazeGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDarkIraption.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDarkRainBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDarkRainGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDeadlySlash.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiDeathSickle.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExAllHit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExAround.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExArrow.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExBurst.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExCircle.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExCircleBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExHoming.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExLiner.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExRndHit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExSimple.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiExSpread.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiFeather.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiFenix.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiFire.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiFlameBirdGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiFlare.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiFrameCircleBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiFrameCircleGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiFullRecover.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiGeneratorBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiGenocide.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHado.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHalfMoon.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHealMist.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHeatWave.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHellFireBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHellFireGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHightArrow.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHopper.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiHpRecover.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiIce.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiImage.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiIwasi.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiKnife.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiLaser.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiLightning.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiLinerBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiMagicBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiMagicSword.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiMeteorGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiMindDowner.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiMissleBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiMpRecover.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiNoahBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiNoahGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiObjectBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiPlanes.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiPunishWave.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiRandom.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiRandomBall.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiRecoverBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiRoket.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiRuncherGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiSelfBomb.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiShockWave.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiSingleBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiSoulBurst.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiSplash.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiSpreadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiStar.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiStarLightBlade.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiSwordDance.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiThunder.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWater.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWaterBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWaterBitGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWaterSpread.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWhiteBlowBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWhiteBlowGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWhiteNoise.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWindBit.cpp
# End Source File
# Begin Source File

SOURCE=.\TobiWindThrustGene.cpp
# End Source File
# Begin Source File

SOURCE=.\TrapSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\TutorialWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveCache.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveFileData.cpp
# End Source File
# Begin Source File

SOURCE=.\WeponSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLCache.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLEditer.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLList.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLPaser.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLTranser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActLocker.h
# End Source File
# Begin Source File

SOURCE=.\AllTobiObject.h
# End Source File
# Begin Source File

SOURCE=.\AllWindow.h
# End Source File
# Begin Source File

SOURCE=.\AllWorld.h
# End Source File
# Begin Source File

SOURCE=.\AngleChecker.h
# End Source File
# Begin Source File

SOURCE=.\ArmyFormation.h
# End Source File
# Begin Source File

SOURCE=.\ArmyStatus.h
# End Source File
# Begin Source File

SOURCE=.\AttackObjectDefine.h
# End Source File
# Begin Source File

SOURCE=.\AutoBuyWindow.h
# End Source File
# Begin Source File

SOURCE=.\BackReader.h
# End Source File
# Begin Source File

SOURCE=.\BackSwapper.h
# End Source File
# Begin Source File

SOURCE=.\BaseWindow.h
# End Source File
# Begin Source File

SOURCE=.\BatComWindow.h
# End Source File
# Begin Source File

SOURCE=.\BattleDataManager.h
# End Source File
# Begin Source File

SOURCE=.\BattleEffect.h
# End Source File
# Begin Source File

SOURCE=.\BattleStatus.h
# End Source File
# Begin Source File

SOURCE=.\BattleSystem.h
# End Source File
# Begin Source File

SOURCE=.\BattleSystemStatus.h
# End Source File
# Begin Source File

SOURCE=.\BattleTagetSystem.h
# End Source File
# Begin Source File

SOURCE=.\BGMMaker.h
# End Source File
# Begin Source File

SOURCE=.\BillbordPlane.h
# End Source File
# Begin Source File

SOURCE=.\BuyWindow.h
# End Source File
# Begin Source File

SOURCE=.\ComBaseWindow.h
# End Source File
# Begin Source File

SOURCE=.\CriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\D3DObject.h
# End Source File
# Begin Source File

SOURCE=.\DamegeSystem.h
# End Source File
# Begin Source File

SOURCE=.\DataUpdater.h
# End Source File
# Begin Source File

SOURCE=.\DDObject.h
# End Source File
# Begin Source File

SOURCE=.\DefaultPlanes.h
# End Source File
# Begin Source File

SOURCE=.\DefaultRenderState.h
# End Source File
# Begin Source File

SOURCE=.\Device.h
# End Source File
# Begin Source File

SOURCE=.\DirectAttacker.h
# End Source File
# Begin Source File

SOURCE=.\DirectEvent.h
# End Source File
# Begin Source File

SOURCE=.\DischargeWindow.h
# End Source File
# Begin Source File

SOURCE=.\Display.h
# End Source File
# Begin Source File

SOURCE=.\DispStocker.h
# End Source File
# Begin Source File

SOURCE=.\DivisionWindow.h
# End Source File
# Begin Source File

SOURCE=.\DragItemWindow.h
# End Source File
# Begin Source File

SOURCE=.\Draws.h
# End Source File
# Begin Source File

SOURCE=.\DummyWindow.h
# End Source File
# Begin Source File

SOURCE=.\EmployWindow.h
# End Source File
# Begin Source File

SOURCE=.\EquipSkill.h
# End Source File
# Begin Source File

SOURCE=.\EquipStatusWindow.h
# End Source File
# Begin Source File

SOURCE=.\ExCreator.h
# End Source File
# Begin Source File

SOURCE=.\ExPatyEditWindow.h
# End Source File
# Begin Source File

SOURCE=.\ExpSystem.h
# End Source File
# Begin Source File

SOURCE=.\ExSprite.h
# End Source File
# Begin Source File

SOURCE=.\ExtraFliper.h
# End Source File
# Begin Source File

SOURCE=.\FaceCache.h
# End Source File
# Begin Source File

SOURCE=.\File.h
# End Source File
# Begin Source File

SOURCE=.\FileLoader.h
# End Source File
# Begin Source File

SOURCE=.\FomationSystem.h
# End Source File
# Begin Source File

SOURCE=.\GameSystem.h
# End Source File
# Begin Source File

SOURCE=.\GlobalWindow.h
# End Source File
# Begin Source File

SOURCE=.\HelpWindow.h
# End Source File
# Begin Source File

SOURCE=.\InfomationWindow.h
# End Source File
# Begin Source File

SOURCE=.\ItemCache.h
# End Source File
# Begin Source File

SOURCE=.\ItemListWindow.h
# End Source File
# Begin Source File

SOURCE=.\JobCache.h
# End Source File
# Begin Source File

SOURCE=.\JobEvent.h
# End Source File
# Begin Source File

SOURCE=.\JobEventBase.h
# End Source File
# Begin Source File

SOURCE=.\KeyEvents.h
# End Source File
# Begin Source File

SOURCE=.\LandBase.h
# End Source File
# Begin Source File

SOURCE=.\LandEdit.h
# End Source File
# Begin Source File

SOURCE=.\LandMapData.h
# End Source File
# Begin Source File

SOURCE=.\LandMoveType.h
# End Source File
# Begin Source File

SOURCE=.\LandScape.h
# End Source File
# Begin Source File

SOURCE=.\LandType.h
# End Source File
# Begin Source File

SOURCE=.\LandWindow.h
# End Source File
# Begin Source File

SOURCE=.\LearningSkill.h
# End Source File
# Begin Source File

SOURCE=.\LevelUpSystem.h
# End Source File
# Begin Source File

SOURCE=.\LoadWindow.h
# End Source File
# Begin Source File

SOURCE=.\Material.h
# End Source File
# Begin Source File

SOURCE=.\MemberCallBacks.h
# End Source File
# Begin Source File

SOURCE=.\MemoryChecker.h
# End Source File
# Begin Source File

SOURCE=.\MiniStatusWindow.h
# End Source File
# Begin Source File

SOURCE=.\MoneyWindow.h
# End Source File
# Begin Source File

SOURCE=.\MoveAlgo.h
# End Source File
# Begin Source File

SOURCE=.\MoveLiner.h
# End Source File
# Begin Source File

SOURCE=.\MoveList.h
# End Source File
# Begin Source File

SOURCE=.\MsgBaseWindow.h
# End Source File
# Begin Source File

SOURCE=.\MsgWindow.h
# End Source File
# Begin Source File

SOURCE=.\MultiWave.h
# End Source File
# Begin Source File

SOURCE=.\MusicPortSelecter.h
# End Source File
# Begin Source File

SOURCE=.\MyHead.h
# End Source File
# Begin Source File

SOURCE=.\NewsChecker.h
# End Source File
# Begin Source File

SOURCE=.\NewsListWindow.h
# End Source File
# Begin Source File

SOURCE=.\NewsWindow.h
# End Source File
# Begin Source File

SOURCE=.\NormalEditer.h
# End Source File
# Begin Source File

SOURCE=.\PaformanceChecker.h
# End Source File
# Begin Source File

SOURCE=.\ParamXML.h
# End Source File
# Begin Source File

SOURCE=.\PasonCache.h
# End Source File
# Begin Source File

SOURCE=.\Path.h
# End Source File
# Begin Source File

SOURCE=.\PathTexCache.h
# End Source File
# Begin Source File

SOURCE=.\PatyEditWindow.h
# End Source File
# Begin Source File

SOURCE=.\PatyHpWindow.h
# End Source File
# Begin Source File

SOURCE=.\Plane.h
# End Source File
# Begin Source File

SOURCE=.\PolyCache.h
# End Source File
# Begin Source File

SOURCE=.\PolyMaster.h
# End Source File
# Begin Source File

SOURCE=.\PolyStation.h
# End Source File
# Begin Source File

SOURCE=.\PopUpMsgWindow.h
# End Source File
# Begin Source File

SOURCE=.\PoweredWindow.h
# End Source File
# Begin Source File

SOURCE=.\RbActStatus.h
# End Source File
# Begin Source File

SOURCE=.\RbAttackAlgo.h
# End Source File
# Begin Source File

SOURCE=.\RbBattlePaty.h
# End Source File
# Begin Source File

SOURCE=.\RbBattleStatus.h
# End Source File
# Begin Source File

SOURCE=.\RbBGMMaker.h
# End Source File
# Begin Source File

SOURCE=.\RbBuffer.h
# End Source File
# Begin Source File

SOURCE=.\RbCache.h
# End Source File
# Begin Source File

SOURCE=.\RbConfig.h
# End Source File
# Begin Source File

SOURCE=.\RbDate.h
# End Source File
# Begin Source File

SOURCE=.\RbDisplay.h
# End Source File
# Begin Source File

SOURCE=.\RbEventSystem.h
# End Source File
# Begin Source File

SOURCE=.\RbFileMaker.h
# End Source File
# Begin Source File

SOURCE=.\RbGameData.h
# End Source File
# Begin Source File

SOURCE=.\RbGlobal.h
# End Source File
# Begin Source File

SOURCE=.\RbMember.h
# End Source File
# Begin Source File

SOURCE=.\RbParameter.h
# End Source File
# Begin Source File

SOURCE=.\RbPaty.h
# End Source File
# Begin Source File

SOURCE=.\RbPlaneCache.h
# End Source File
# Begin Source File

SOURCE=.\RbPoint.h
# End Source File
# Begin Source File

SOURCE=.\RbPoly.h
# End Source File
# Begin Source File

SOURCE=.\RbPolyPoly.h
# End Source File
# Begin Source File

SOURCE=.\RbPosition.h
# End Source File
# Begin Source File

SOURCE=.\RbRect.h
# End Source File
# Begin Source File

SOURCE=.\RbSound.h
# End Source File
# Begin Source File

SOURCE=.\RbSprite.h
# End Source File
# Begin Source File

SOURCE=.\RbStatus.h
# End Source File
# Begin Source File

SOURCE=.\RbTexture.h
# End Source File
# Begin Source File

SOURCE=.\RbTimer.h
# End Source File
# Begin Source File

SOURCE=.\RbTransform.h
# End Source File
# Begin Source File

SOURCE=.\RbUserData.h
# End Source File
# Begin Source File

SOURCE=.\RbWindowSystem.h
# End Source File
# Begin Source File

SOURCE=.\RefObject.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ResultWindow.h
# End Source File
# Begin Source File

SOURCE=.\Ribe2.h
# End Source File
# Begin Source File

SOURCE=.\SaveWindow.h
# End Source File
# Begin Source File

SOURCE=.\ScrollWindow.h
# End Source File
# Begin Source File

SOURCE=.\SelCharaWindow.h
# End Source File
# Begin Source File

SOURCE=.\SelectMsgWindow.h
# End Source File
# Begin Source File

SOURCE=.\SellWindow.h
# End Source File
# Begin Source File

SOURCE=.\ShopListWindow.h
# End Source File
# Begin Source File

SOURCE=.\ShopSystem.h
# End Source File
# Begin Source File

SOURCE=.\SkillCache.h
# End Source File
# Begin Source File

SOURCE=.\SplashWindow.h
# End Source File
# Begin Source File

SOURCE=.\Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Sprite2.h
# End Source File
# Begin Source File

SOURCE=.\SPWindow.h
# End Source File
# Begin Source File

SOURCE=.\SquareTexture.h
# End Source File
# Begin Source File

SOURCE=.\StatusBaseWinow.h
# End Source File
# Begin Source File

SOURCE=.\StatusWindow.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StockMemberWindow.h
# End Source File
# Begin Source File

SOURCE=.\StrCommander.h
# End Source File
# Begin Source File

SOURCE=.\String.h
# End Source File
# Begin Source File

SOURCE=.\StringIDMap.h
# End Source File
# Begin Source File

SOURCE=.\StringSplitter.h
# End Source File
# Begin Source File

SOURCE=.\StrParamCom.h
# End Source File
# Begin Source File

SOURCE=.\SystemMessage.h
# End Source File
# Begin Source File

SOURCE=.\TacticsAlgo.h
# End Source File
# Begin Source File

SOURCE=.\TAG.h
# End Source File
# Begin Source File

SOURCE=.\TagetAlgo.h
# End Source File
# Begin Source File

SOURCE=.\TexRender.h
# End Source File
# Begin Source File

SOURCE=.\TextFont.h
# End Source File
# Begin Source File

SOURCE=.\Texture.h
# End Source File
# Begin Source File

SOURCE=.\TextureCache.h
# End Source File
# Begin Source File

SOURCE=.\Timer.h
# End Source File
# Begin Source File

SOURCE=.\TimerWindow.h
# End Source File
# Begin Source File

SOURCE=.\TitleWindow.h
# End Source File
# Begin Source File

SOURCE=.\TobiAngelDust.h
# End Source File
# Begin Source File

SOURCE=.\TobiArrow.h
# End Source File
# Begin Source File

SOURCE=.\TobiArrowRain.h
# End Source File
# Begin Source File

SOURCE=.\TobiAttackSpreadBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiBall.h
# End Source File
# Begin Source File

SOURCE=.\TobiBeast.h
# End Source File
# Begin Source File

SOURCE=.\TobiBlastEdge.h
# End Source File
# Begin Source File

SOURCE=.\TobiBlink.h
# End Source File
# Begin Source File

SOURCE=.\TobiBlinkBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiBlueWave.h
# End Source File
# Begin Source File

SOURCE=.\TobiBomb.h
# End Source File
# Begin Source File

SOURCE=.\TobiBurnFlare.h
# End Source File
# Begin Source File

SOURCE=.\TobiBurst.h
# End Source File
# Begin Source File

SOURCE=.\TobiBurstBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiChainBurst.h
# End Source File
# Begin Source File

SOURCE=.\TobiChainFlare.h
# End Source File
# Begin Source File

SOURCE=.\TobiComet.h
# End Source File
# Begin Source File

SOURCE=.\TobiCreator.h
# End Source File
# Begin Source File

SOURCE=.\TobiCrimson.h
# End Source File
# Begin Source File

SOURCE=.\TobiDamege.h
# End Source File
# Begin Source File

SOURCE=.\TobiDark.h
# End Source File
# Begin Source File

SOURCE=.\TobiDarkBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiDarkBlazeBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiDarkBlazeGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiDarkIraption.h
# End Source File
# Begin Source File

SOURCE=.\TobiDarkRainBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiDarkRainGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiDeadlySlash.h
# End Source File
# Begin Source File

SOURCE=.\TobiDeathSickle.h
# End Source File
# Begin Source File

SOURCE=.\TobiExAllHit.h
# End Source File
# Begin Source File

SOURCE=.\TobiExAround.h
# End Source File
# Begin Source File

SOURCE=.\TobiExArrow.h
# End Source File
# Begin Source File

SOURCE=.\TobiExBurst.h
# End Source File
# Begin Source File

SOURCE=.\TobiExCircle.h
# End Source File
# Begin Source File

SOURCE=.\TobiExCircleBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiExHoming.h
# End Source File
# Begin Source File

SOURCE=.\TobiExLiner.h
# End Source File
# Begin Source File

SOURCE=.\TobiExObjectBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiExRndHit.h
# End Source File
# Begin Source File

SOURCE=.\TobiExSimple.h
# End Source File
# Begin Source File

SOURCE=.\TobiExSpread.h
# End Source File
# Begin Source File

SOURCE=.\TobiFeather.h
# End Source File
# Begin Source File

SOURCE=.\TobiFenix.h
# End Source File
# Begin Source File

SOURCE=.\TobiFire.h
# End Source File
# Begin Source File

SOURCE=.\TobiFlameBirdGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiFlare.h
# End Source File
# Begin Source File

SOURCE=.\TobiFrameCircleBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiFrameCircleGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiFullRecover.h
# End Source File
# Begin Source File

SOURCE=.\TobiGeneratorBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiGenocide.h
# End Source File
# Begin Source File

SOURCE=.\TobiHado.h
# End Source File
# Begin Source File

SOURCE=.\TobiHalfMoon.h
# End Source File
# Begin Source File

SOURCE=.\TobiHealMist.h
# End Source File
# Begin Source File

SOURCE=.\TobiHeatWave.h
# End Source File
# Begin Source File

SOURCE=.\TobiHellFireBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiHellFireGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiHightArrow.h
# End Source File
# Begin Source File

SOURCE=.\TobiHopper.h
# End Source File
# Begin Source File

SOURCE=.\TobiHpRecover.h
# End Source File
# Begin Source File

SOURCE=.\TobiIce.h
# End Source File
# Begin Source File

SOURCE=.\TobiImage.h
# End Source File
# Begin Source File

SOURCE=.\TobiIwasi.h
# End Source File
# Begin Source File

SOURCE=.\TobiKnife.h
# End Source File
# Begin Source File

SOURCE=.\TobiLaser.h
# End Source File
# Begin Source File

SOURCE=.\TobiLightning.h
# End Source File
# Begin Source File

SOURCE=.\TobiLinerBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiMagicBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiMagicSword.h
# End Source File
# Begin Source File

SOURCE=.\TobiMeteorGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiMindDowner.h
# End Source File
# Begin Source File

SOURCE=.\TobiMissleBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiMpRecover.h
# End Source File
# Begin Source File

SOURCE=.\TobiNoahBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiNoahGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiObjectBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiPlanes.h
# End Source File
# Begin Source File

SOURCE=.\TobiPunishWave.h
# End Source File
# Begin Source File

SOURCE=.\TobiRandom.h
# End Source File
# Begin Source File

SOURCE=.\TobiRandomBall.h
# End Source File
# Begin Source File

SOURCE=.\TobiRecoverBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiRoket.h
# End Source File
# Begin Source File

SOURCE=.\TobiRuncherGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiSelfBomb.h
# End Source File
# Begin Source File

SOURCE=.\TobiShockWave.h
# End Source File
# Begin Source File

SOURCE=.\TobiSingleBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiSoulBurst.h
# End Source File
# Begin Source File

SOURCE=.\TobiSplash.h
# End Source File
# Begin Source File

SOURCE=.\TobiSpreadBase.h
# End Source File
# Begin Source File

SOURCE=.\TobiStar.h
# End Source File
# Begin Source File

SOURCE=.\TobiStarLightBlade.h
# End Source File
# Begin Source File

SOURCE=.\TobiSwordDance.h
# End Source File
# Begin Source File

SOURCE=.\TobiSystem.h
# End Source File
# Begin Source File

SOURCE=.\TobiThunder.h
# End Source File
# Begin Source File

SOURCE=.\TobiWater.h
# End Source File
# Begin Source File

SOURCE=.\TobiWaterBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiWaterBitGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiWaterSpread.h
# End Source File
# Begin Source File

SOURCE=.\TobiWhiteBlowBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiWhiteBlowGene.h
# End Source File
# Begin Source File

SOURCE=.\TobiWhiteNoise.h
# End Source File
# Begin Source File

SOURCE=.\TobiWindBit.h
# End Source File
# Begin Source File

SOURCE=.\TobiWindThrustGene.h
# End Source File
# Begin Source File

SOURCE=.\TrapSystem.h
# End Source File
# Begin Source File

SOURCE=.\TutorialWindow.h
# End Source File
# Begin Source File

SOURCE=.\VArray.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=.\VertexDef.h
# End Source File
# Begin Source File

SOURCE=.\WaveCache.h
# End Source File
# Begin Source File

SOURCE=.\WaveFileData.h
# End Source File
# Begin Source File

SOURCE=.\WeponSystem.h
# End Source File
# Begin Source File

SOURCE=.\WindowDefine.h
# End Source File
# Begin Source File

SOURCE=.\WindowHandle.h
# End Source File
# Begin Source File

SOURCE=.\WindowSystem.h
# End Source File
# Begin Source File

SOURCE=.\WindowTexture.h
# End Source File
# Begin Source File

SOURCE=.\XMLCache.h
# End Source File
# Begin Source File

SOURCE=.\XMLEditer.h
# End Source File
# Begin Source File

SOURCE=.\XMLList.h
# End Source File
# Begin Source File

SOURCE=.\XMLLoader.h
# End Source File
# Begin Source File

SOURCE=.\XMLPaser.h
# End Source File
# Begin Source File

SOURCE=.\XMLTranser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\NowLoading.bmp
# End Source File
# Begin Source File

SOURCE=.\Ribe2.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
