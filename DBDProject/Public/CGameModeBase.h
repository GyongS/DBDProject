#pragma once
#include "DBDProject.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

UCLASS()
class DBDPROJECT_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		// 게임 모드 정의
		ACGameModeBase();

public:

	// 플레이어 설정 확인용
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
