#include "Interactor.h"
#include "Creature.h"
#include "Indicator.h"

#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"





// =============================================================================================================
// Initialization
// =============================================================================================================

AInteractor::AInteractor() {
	defaultTag += static_cast<uint8>(Tag::Floating);
	defaultTag += static_cast<uint8>(Tag::Piercing);

	nameComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Name"));
	nameComponent->SetRelativeRotation(FRotator(48.590382f, 180.0f, 0.0f));
	nameComponent->SetupAttachment(RootComponent);

	SetFont(nameComponent, FontType::Galmuri7);
	nameComponent->SetTextRenderColor(FColor(0, 0, 0, 255));
	nameComponent->SetText(TEXT("Interactor"));
	nameComponent->SetWorldSize(32.0f);
	nameComponent->SetHorizontalAlignment(EHTA_Center);
	nameComponent->SetVerticalAlignment(EVRTA_TextBottom);
}
void AInteractor::BeginPlay() {
	Super::BeginPlay();

	SetCollisionProfileName(TEXT("Particle"));
	SetSpriteIndex(nullptr, 63);
	SetActive(false);
	
}





// =============================================================================================================
// Interactor
// =============================================================================================================

bool AInteractor::GetActive() {
	return active;
}
void AInteractor::SetActive(bool value) {
	active = value;
	SetSpriteIndex(nullptr, value ? 0 : 63);
	nameComponent->SetText(value ? ToFString(parent->GetIdentifier()) : TEXT(""));
	if (parent) UpdateRelativeLocation();
}

void AInteractor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (active) UpdateRelativeLocation();
}
void AInteractor::UpdateRelativeLocation() {
	float height = parent->GetHitboxHeight() * 0.5f + 64.0f;
	if (parent->IsA(ACreature::StaticClass())) {
		if (static_cast<ACreature*>(parent)->GetIndicator()->GetActive()) {
			height += !parent->HasTag(Tag::Leader) ? 96.0f : 148.0f;
		}
	}
	SetActorLocation(parent->GetActorLocation() + FVector(0.0f, 0.0f, height));
}





// =============================================================================================================
// Action
// =============================================================================================================

bool AInteractor::OnInteract(AEntity* entity) {
	if (!Super::OnInteract(entity)) return false;
	parent = entity;
	UpdateRelativeLocation();
	AttachToComponent(parent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	return true;
}