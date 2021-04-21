#include "DBDProject.h"
#include "CGameModeBase.h"
#include "PPawn.h"
#include "CCharacter.h"
#include "C_Killer.h"
#include "CPlayerController.h"
#include "..\Public\CGameModeBase.h"

ACGameModeBase::ACGameModeBase()
{
	// ������ ���� �� �� ���� ����
	//DefaultPawnClass = ACCharacter::StaticClass();	
	//DefaultPawnClass = AC_Killer::StaticClass();	

	// �÷��̾� ��Ʈ�ѷ��� �Ӽ��� APCPlayerController Ŭ������ ������ ����.
	PlayerControllerClass = ACPlayerController::StaticClass();
}

void ACGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}