// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->bCastDynamicShadow = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());

	GetMesh()->SetRelativeLocationAndRotation(FVector(-9.f, 0.f, -29.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->CastShadow = false;
	GetMesh()->bCastDynamicShadow = false;
	PlayerMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Character/CharacterAsset/Hands.Hands'")).Object;
	if (PlayerMesh)
		GetMesh()->SetSkeletalMesh(PlayerMesh);
	static ConstructorHelpers::FObjectFinder<UClass> AnimationBP(TEXT("AnimBlueprint'/Game/Character/CharacterAsset/animations/AnimBP.AnimBP_C'"));
	if (AnimationBP.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimationBP.Object);
	GetMesh()->SetupAttachment(FirstPersonCamera);

	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(GetMesh(), "HipsSocket");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SwordMesh(TEXT("StaticMesh'/Game/Character/Weapons/Sword/Sword.Sword'"));
	if (SwordMesh.Succeeded())
		Sword->SetStaticMesh(SwordMesh.Object);
	Sword->CastShadow = false;
	Sword->bCastDynamicShadow = false;

	AttackAnim = ConstructorHelpers::FObjectFinder<UAnimMontage>
		(TEXT("AnimMontage'/Game/Character/CharacterAsset/animations/Slash_Montage.Slash_Montage'")).Object;
	DrawnSwordAnim = ConstructorHelpers::FObjectFinder<UAnimMontage>
		(TEXT("AnimMontage'/Game/Character/CharacterAsset/animations/DrawnSword_Montage.DrawnSword_Montage'")).Object;
	HideSwordAnim = ConstructorHelpers::FObjectFinder<UAnimMontage>
		(TEXT("AnimMontage'/Game/Character/CharacterAsset/animations/HideSword_Montage.HideSword_Montage'")).Object;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->SetWalkableFloorAngle(60);

	bAttackCooldown = bWeaponEquiped = bWeaponSwitching = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction("DrawnSword", IE_Pressed, this, &AMyCharacter::DrawnOrHideWeapon);
}

void AMyCharacter::MoveForward(float Axis)
{
	if (Controller != NULL && Axis != 0.f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(FRotator(0, Rotation.Yaw, 0)).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void AMyCharacter::MoveRight(float Axis)
{
	if (Controller != NULL && Axis != 0.f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(FRotator(0, Rotation.Yaw, 0)).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void AMyCharacter::Attack()
{
	if (bWeaponEquiped && !bAttackCooldown && AttackAnim)
		PlayAnimMontage(AttackAnim);
}

void AMyCharacter::DrawnOrHideWeapon()
{
	if (bWeaponSwitching || bAttackCooldown)
		return;
	if (bWeaponEquiped)
		PlayAnimMontage(HideSwordAnim);
	else
		PlayAnimMontage(DrawnSwordAnim);
}