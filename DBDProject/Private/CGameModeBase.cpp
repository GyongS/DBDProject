#include "DBDProject.h"
#include "CGameModeBase.h"
#include "PPawn.h"
#include "CCharacter.h"
#include "C_Killer.h"
#include "CPlayerController.h"
#include "..\Public\CGameModeBase.h"

ACGameModeBase::ACGameModeBase()
{
	// 게임을 시작 할 때 폰을 지정
	//DefaultPawnClass = ACCharacter::StaticClass();	
	//DefaultPawnClass = AC_Killer::StaticClass();	

	// 플레이어 컨트롤러의 속성을 APCPlayerController 클래스의 값으로 변경.
	PlayerControllerClass = ACPlayerController::StaticClass();
}

void ACGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}