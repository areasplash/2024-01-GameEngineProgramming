#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"





UENUM(BlueprintType)
enum class MeshType : uint8 {
	Plane			= 0,
	Sphere			,
	Length			UMETA(Hidden),
};

UENUM(BlueprintType)
enum class FontType : uint8 {
	Galmuri7		= 0,
	Galmuri9		,
	Galmuri11		,
	Galmuri11Bold	,
	Length			UMETA(Hidden),
};





UENUM(BlueprintType)
enum class Identifier : uint8 {
	Default			= 0,
	Hero			,
	Sword			,
	Chest			,
	Money			,
	Wand			,
	Interactor		,
	Indicator		,
	Dust			,
	Flame			,
	Twinkle			,
	Length			UMETA(Hidden),
};

UENUM(BlueprintType)
enum class Action : uint8 {
	Idle			= 0,
	Move			,
	Jump			,
	Dash			,
	Attack			,
	Defend			,
	Defeat			,
	Length			UMETA(Hidden),
};

UENUM(BlueprintType)
enum class Group : uint8 {
	None			= 0,
	Friendly		,
	Enemy			,
	Length			UMETA(Hidden),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class Tag : uint8 {
	None			= 0 UMETA(Hidden),
	Floating		= 1 << 0,
	Piercing        = 1 << 1,
	Invulnerability	= 1 << 2,
	Interactability	= 1 << 3,
	Collectable		= 1 << 4,
	Player			= 1 << 5,
	Leader			= 1 << 6,
	Length			= 7 UMETA(Hidden),
};
ENUM_CLASS_FLAGS(Tag);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class Effect : uint8 {
	None			= 0 UMETA(Hidden),
	HealthBoost		= 1 << 0,
	DamageBoost		= 1 << 1,
	Resistance		= 1 << 2,
	Speed			= 1 << 3,
	Burn			= 1 << 4,
	Stun			= 1 << 5,
	Freeze			= 1 << 6,
	Length			= 7 UMETA(Hidden),
};
ENUM_CLASS_FLAGS(Effect);





UCLASS(HideCategories = ("Actor Tick", Character, Camera, Pawn, Replication, Rendering, Collision,
	Actor, Input, LOD, Cooking))
class DUNGEON_API AEntity : public ACharacter {
	GENERATED_BODY()

	// =========================================================================================================
	// -
	// =========================================================================================================
	
	// -
public:
	bool operator==(const AEntity& other) const;

	FRotator ToRotator   (FVector  value);
	FVector  ToVector    (FRotator value);
	FVector  RotateVector(FVector  value);

	template<typename TEnum> uint8   GetIndex(TEnum   value);
	template<typename TEnum> FString ToString(TEnum   value);
	template<typename TEnum> TEnum   ToEnum  (FString value);

	UStaticMesh* GetStaticMesh(MeshType value);
	UMaterialInstanceDynamic* GetMaterialInstanceDynamic(Identifier value);
	UFont*     GetFont        (FontType value);
	UMaterial* GetFontMaterial(FontType value);

	AEntity* Spawn(Identifier value, FVector location = FVector::ZeroVector);
	void     Despawn();

	bool IsAttached();
	void Attach(AEntity* entity);
	void Detach();





	// =========================================================================================================
	// Setup
	// =========================================================================================================
	
	// Initialization
public:
	AEntity();
protected:
	virtual void BeginPlay() override;

	// Object Pool
protected:
	virtual void OnSpawn  ();
	virtual void OnDespawn();

	// Update
	#define DefaultGravityScale     3.0f
	#define FallSpeedMax        -1024.0f
	#define VoidZAxis           -1024.0f
	#define DustThreshold        -500.0f
protected:
	UPROPERTY(EditAnywhere) float defaultSpeed;
private:
	float speed;
protected:
	bool  isFalling;
	float fallSpeed;
public:
	virtual void Tick(float DeltaTime) override;



private:
	FVector lookDirection;
	FVector moveDirection;
public:
	FVector GetLookDirection();
	FVector GetMoveDirection();
protected:
	void    SetLookDirection(FVector value);
	void    SetMoveDirection(FVector value);





	// =========================================================================================================
	// Components
	// =========================================================================================================
	
	// Hitbox
protected:
	UPROPERTY(EditAnywhere) float     defaultHitboxRadius;
	UPROPERTY(EditAnywhere) float     defaultHitboxHeight;
	UPROPERTY(EditAnywhere) FVector2D defaultHandLocation;
private:
	UPROPERTY() class UCapsuleComponent* hitboxComponent;
	float hitboxRadius;
	float hitboxHeight;
	FVector handLocation;
protected:
	virtual void OnHitboxChanged();
public:
	float GetHitboxRadius();
	float GetHitboxHeight();
	void  SetHitboxRadius(float value);
	void  SetHitboxHeight(float value);
	void  SetHitbox(float radius, float height);
	void  SetCollisionProfileName(FName value);
	virtual FVector GetHandLocation();
	virtual FVector GetFootLocation();

	UFUNCTION() void OnHit(
		UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);
	UFUNCTION() void OnHitboxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnCollision(AEntity* entity);
	virtual void OnInteract (AEntity* entity);

	// Sprite
private:
	UPROPERTY() class USceneComponent*      anchorComponent;
	UPROPERTY() class UStaticMeshComponent* spriteComponent;
	int32   spriteIndex;
	bool    spriteXFlip;
	FVector spriteColor;
	float   spriteAngle;
	float   spriteIntensity;
public:
	int32   GetSpriteIndex();
	bool    GetSpriteXFlip();
	FVector GetSpriteColor();
	float   GetSpriteAngle();
	float   GetSpriteIntensity();
protected:
	UFUNCTION() USceneComponent* GetAnchorComponent();
	UFUNCTION() void SetSpriteIndex    (UStaticMeshComponent* comp, int32   value = 0);
	UFUNCTION() void SetSpriteXFlip    (UStaticMeshComponent* comp, bool    value = false);
	UFUNCTION() void SetSpriteColor    (UStaticMeshComponent* comp, FVector value = FVector(0, 0, 0));
	UFUNCTION() void SetSpriteAngle    (UStaticMeshComponent* comp, float   value = 0);
	UFUNCTION() void SetSpriteIntensity(UStaticMeshComponent* comp, float   value = 0);

	// Shadow
private:
	UPROPERTY() class UStaticMeshComponent* shadowComponent;

	// Interactor
private:
	UPROPERTY() class AInteractor* interactor;
	void CreateInteractor();
	void RemoveInteractor();
public:
	UFUNCTION() AInteractor* GetInteractor();





	// =========================================================================================================
	// AI
	// =========================================================================================================
	
	// Input
private:
	UPROPERTY() class AGhost* ghost;
public:
	UFUNCTION() AGhost* GetGhost();
protected:
	bool GetInput(Action value);
	FVector GetInputDirection();

	// Action
private:
	Action action;
	float  actionCooldown[static_cast<uint8>(Action::Length)];
protected:
	float  actionDelay;
	Action GetAction();
	bool   SetAction(Action value);
	float  GetActionCooldown(Action value);
	void   SetActionCooldown(Action value, float cooldown);
	virtual bool VerifyAction(Action value);
	virtual bool UpdateInputs(float DeltaTime);
	virtual bool UpdateAction(float DeltaTime);





	// =========================================================================================================
	// Properties
	// =========================================================================================================
	
	// Identifier
private:
	Identifier identifier;
public:
	Identifier GetIdentifier();

	// Group
protected:
	UPROPERTY(EditAnywhere) Group defaultGroup;
private:
	Group group;
public:
	Group        GetGroup();
	virtual void SetGroup(Group value);

	// Tag
protected:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Tag)) uint8 defaultTag;
private:
	uint8 tag;
public:
	bool  HasTag(Tag value);
	virtual bool AddTag   (Tag value);
	virtual bool RemoveTag(Tag value);

	// Effect
	#define EffectStrengthMax 9999.0f
	#define EffectDurationMax 9999.0f
protected:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 defaultEffect;
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = Effect)) uint8 defaultEffectImmunity;
private:
	uint8 effect;
	uint8 effectImmunity;
	float effectStrength[static_cast<uint8>(Effect::Length)];
	float effectDuration[static_cast<uint8>(Effect::Length)];
	float hit;
	bool  updateColor = false;
	bool  updateSpeed = false;
protected:
	void Hit(float value);
	virtual bool UpdateEffect(float DeltaTime);
public:
	bool  HasEffect(Effect value);
	virtual bool AddEffect   (Effect value, float strength = 1.0f, float duration = EffectDurationMax);
	virtual bool RemoveEffect(Effect value);

	float GetEffectStrength(Effect value);
	float GetEffectDuration(Effect value);
	float AdjustEffectStrength(Effect value, float strength);
	float AdjustEffectDuration(Effect value, float duration);
};