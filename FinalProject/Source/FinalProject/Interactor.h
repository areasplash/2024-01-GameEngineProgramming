#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Interactor.generated.h"





UCLASS()
class FINALPROJECT_API AInteractor : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AInteractor();
protected:
	virtual void BeginPlay() override;





	// Interactor
private:
	UPROPERTY(EditAnywhere) class UTextRenderComponent* nameComponent;
	//UPROPERTY(EditAnywhere) class UStaticMeshComponent* lBorderComponent;
	UPROPERTY() class AEntity* parent;
	bool active = false;
public:
	bool GetActive();
	void SetActive(bool value);
	virtual void Tick(float DeltaTime) override;
private:
	void UpdateRelativeLocation();





	// Action
public:
	virtual bool OnInteract(AEntity* entity) override;
};