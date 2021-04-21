#pragma once
#include "DBDProject.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

UCLASS()
class DBDPROJECT_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		// ���� ��� ����
		ACGameModeBase();

public:

	// �÷��̾� ���� Ȯ�ο�
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
