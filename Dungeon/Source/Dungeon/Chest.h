#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Chest.generated.h"





UCLASS()
class DUNGEON_API AChest : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AChest();
protected:
	virtual void BeginPlay() override;





	// Action
protected:
	virtual bool VerifyAction(Action value) override;
	virtual bool UpdateAction(float DeltaTime) override;
public:
	virtual void OnInteract(AEntity* value) override;
};
