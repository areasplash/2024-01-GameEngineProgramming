#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Indicator.generated.h"





UCLASS()
class DUNGEON_API AIndicator : public AEntity {
	GENERATED_BODY()
	




	// Initialization
public:
	AIndicator();
protected:
	virtual void BeginPlay() override;

	// Object Pool
protected:
	virtual void OnSpawn  () override;
	virtual void OnDespawn() override;





	// Hitbox
public:
	virtual void OnInteract(AEntity* entity) override;





	// Indicator
private:
	UPROPERTY() class UStaticMeshComponent* lBorderComponent;
	UPROPERTY() class UStaticMeshComponent* rBorderComponent;
	UPROPERTY() class UStaticMeshComponent* lHealthComponent;
	UPROPERTY() class UStaticMeshComponent* lHBoostComponent;
	UPROPERTY() class UStaticMeshComponent* rHealthComponent;
	UPROPERTY() class UStaticMeshComponent* lShieldComponent;
	UPROPERTY() class UStaticMeshComponent* rShieldComponent;
	UPROPERTY() class UStaticMeshComponent* lEnergeComponent;
	UPROPERTY() class UStaticMeshComponent* rEnergeComponent;
	UPROPERTY() class UStaticMeshComponent* iShieldComponent;
	UPROPERTY() class UStaticMeshComponent* iLeaderComponent;
	UFUNCTION() void SetupComponent(UStaticMeshComponent* component);
private:
	UPROPERTY() class ACreature* parent;
	float width;
	float health, healthMax;
	float shield, shieldMax;
	float energe, energeMax;
	float hboost;
	Group group;
	bool  leader;
	void SetWidth();
	void SetColor();
	void SetLeader();
private:
	bool active = false;
public:
	bool GetActive();
	void SetActive(bool value);
	virtual void Tick(float DeltaTime) override;
};
