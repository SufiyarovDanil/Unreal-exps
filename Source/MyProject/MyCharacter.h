// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS(Blueprintable)
class MYPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties

	UPROPERTY(BlueprintReadWrite, Blueprintable, Category = Camera)
		class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
		class USkeletalMesh* PlayerMesh;

	UPROPERTY(BlueprintReadWrite, Blueprintable, Category = "Weapon")
		class UStaticMeshComponent* Sword;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
		class UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
		class UAnimMontage* DrawnSwordAnim;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
		class UAnimMontage* HideSwordAnim;

	UPROPERTY(BlueprintReadWrite, Blueprintable, Category = "Animation")
		bool bAttackCooldown;

	UPROPERTY(BlueprintReadWrite, Blueprintable, Category = "Animation")
		bool bWeaponEquiped;

	UPROPERTY(BlueprintReadWrite, Blueprintable, Category = "Animation")
		bool bWeaponSwitching;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AMyCharacter();
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void DrawnOrHideWeapon();
	void Attack();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
