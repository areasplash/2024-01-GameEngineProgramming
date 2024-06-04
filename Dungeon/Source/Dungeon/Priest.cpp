#include "Priest.h"
#include "Ghost.h"
#include "Weapon.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

APriest::APriest() {
	defaultGroup  = Group::Friendly;
	defaultHealth = 20.0f;
	defaultTag += static_cast<uint8>(Tag::Interactability);
}

// =============================================================================================================
// Spawn
// =============================================================================================================

void APriest::OnStart() {
	Super::OnStart();
}
void APriest::OnSpawn() {
	Super::OnSpawn();
}
void APriest::OnDespawn() {
	Super::OnDespawn();
}





// =============================================================================================================
// Hitbox
// =============================================================================================================

void APriest::OnInteract(AEntity* entity) {
	Super::OnInteract(entity);

	if (entity) entity->AddTag(Tag::Interactability);
	RemoveTag(Tag::Interactability);
	AddTag(Tag::Player);
	AddTag(Tag::PlayerParty);
}





// =============================================================================================================
// Sprite
// =============================================================================================================

void APriest::UpdateSprite(float DeltaTime) {
	Super::UpdateSprite(DeltaTime);

	switch (GetSprite()) {
	case Action::Idle:   SetSpriteIndex(nullptr,  0 + int32(GetLifeTime()  * 2) % 4); break;
	case Action::Move:   SetSpriteIndex(nullptr,  4 + int32(GetLifeTime() * 10) % 6); break;
	case Action::Jump:   SetSpriteIndex(nullptr, FMath::Min(10 + int32(GetSpriteDelay() * 10), 13)); break;
	case Action::Dash:   SetSpriteIndex(nullptr, FMath::Min(14 + int32(GetSpriteDelay() * 10), 19)); break;
	case Action::Attack: break;
	case Action::Defend: break;
	case Action::Defeat: SetSpriteIndex(nullptr, FMath::Min(20 + int32(GetSpriteDelay() * 10), 23)); break;
	}
}

// =============================================================================================================
// Action
// =============================================================================================================

bool APriest::VerifyAction(Action value) {
	if (!Super::VerifyAction(value)) return false;

	if (value == Action::Idle || value == Action::Move || value == Action::Defeat) return true;
	switch (GetAction()) {
	case Action::Idle: return true;
	case Action::Move: return true;
	case Action::Jump:
		switch (value) {
		case Action::Dash:   return false;
		case Action::Attack: return true;
		case Action::Defend: return true;
		}
		break;
	case Action::Dash:
		switch (value) {
		case Action::Jump:   return false;
		case Action::Attack: return false;
		case Action::Defend: return false;
		}
		break;
	case Action::Attack:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Defend: return (HasWeapon() ? true : false);
		}
		break;
	case Action::Defend:
		switch (value) {
		case Action::Jump:   return true;
		case Action::Dash:   return true;
		case Action::Attack: return (HasWeapon() ? true : false);
		}
		break;
	}
	return false;
}
void APriest::UpdateAction(float DeltaTime) {
	Super::UpdateAction(DeltaTime);

	switch (GetAction()) {
	case Action::Idle:
		SetSprite(Action::Idle);
		if (HasTarget()) SetLookDirection(GetTarget()->GetActorLocation() - GetActorLocation());
		break;
	case Action::Move:
		SetSprite(Action::Move);
		AddMovementInput(GetMoveDirection());
		SetLookDirection(GetMoveDirection());
		if (HasTarget()) SetLookDirection(GetTarget()->GetActorLocation() - GetActorLocation());
		break;
	case Action::Jump:
		SetSprite(Action::Jump);
		AddMovementInput(GetMoveDirection());
		if (HasTarget()) SetLookDirection(GetTarget()->GetActorLocation() - GetActorLocation());
		if (0.1f <= GetActionDelay() && GetActionDelay() - DeltaTime < 0.1f) Jump();
		if (0.2f <= GetActionDelay() && !IsFalling()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Jump, 0.2f);
		}
		break;
	case Action::Dash:
		SetSprite(Action::Dash);
		if (GetActionDelay() - DeltaTime == 0.0f) {
			if (HasWeapon()) GetWeapon()->SetAction(Action::Idle);
		}
		if (0.6f <= GetActionDelay() && GetActionDelay() - DeltaTime < 0.6f) {
			for (int32 i = 0; i < sensorArray.Num(); i++) {
				if (sensorArray[i] == nullptr || sensorArray[i] == this) continue;
				if (sensorArray[i]->HasTag(Tag::Invulnerability)) continue;
				if (sensorArray[i]->GetAction() == Action::Defeat) continue;
				if (GetGroup() == Group::None || sensorArray[i]->GetGroup() != GetGroup()) continue;
				sensorArray[i]->AddEffect(Effect::HealthBoost, 10.0f, 5.0f);
				sensorArray[i]->AddEffect(Effect::DamageBoost, 10.0f, 5.0f);
				sensorArray[i]->AdjustHealth(10.0f);
			}
		}
		if (1.2f <= GetActionDelay()) {
			SetAction(Action::Idle);
			SetActionCooldown(Action::Dash,   9.4f);
			SetActionCooldown(Action::Attack, 0.1f);
			SetActionCooldown(Action::Defend, 0.1f);
		}
		break;
	case Action::Attack:
		if (HasWeapon() && GetActionDelay() - DeltaTime == 0 && GetWeapon()->SetAction(Action::Attack));
		else {
			if (SetAction(GetSprite())) SetActionDelay(GetSpriteDelay());
			SetActionCooldown(Action::Attack, 0.25f);
		}
		break;
	case Action::Defend:
		if (HasWeapon() && GetActionDelay() - DeltaTime == 0 && GetWeapon()->SetAction(Action::Defend));
		else {
			if (SetAction(GetSprite())) SetActionDelay(GetSpriteDelay());
			SetActionCooldown(Action::Defend, 0.25f);
		}
		break;
	case Action::Defeat:
		SetSprite(Action::Defeat);
		break;
	}
}