// Fill out your copyright notice in the Description page of Project Settings.


#include "RGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RGInteractionComponent.h"
//#include "RGAttributeActorComponent.h"

// Sets default values
ARGCharacter::ARGCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	// ��SpringArmComponent�п�����ʹ��Pawn������ת��
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	// ��ȡ����ɫ�ƶ������������������ת�����˶���
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<URGInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<URGAttributeActorComponent>("AttributeComp");
}

// Called when the game starts or when spawned
void ARGCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ARGCharacter::MoveForWard(float Value)
{
	//��ȡ����ĳ���
	FRotator ControlRot = GetControlRotation();
	//RollΪX�ᣬPitchΪY�ᣬYawΪZ�ᣬ XYZ = ǰ����
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ARGCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ARGCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	//��ʱ��
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARGCharacter::PrimaryAttack_TimeElapsed, 0.18f);
	
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);

}

void ARGCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);

	//��ʱ��
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARGCharacter::BlackHoleAttack_TimeElapsed, 0.18f);

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);

}

void ARGCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);

	//��ʱ��
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARGCharacter::Dash_TimeElapsed, 0.18f);

}

void ARGCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ARGCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ARGCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ARGCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashClass);
}

void ARGCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		//��ȡָ����λ��
		FVector RightHandLoc = GetMesh()->GetSocketLocation("Muzzle_01");

		// �˴�������ײ������Ϊ����ʹ��ײҲ�������ɣ���Ϊ�����ڽ�ɫ�м����ɱ�Ȼ��ײ
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		//ɨ����״
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//���Ӳ����� ������Ҫ�����Լ�
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		//ɨ�����
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		//ɨ�迪ʼ�ͽ�����λ��
		FVector TranceStart = CameraComponent->GetComponentLocation();
		FVector TranceEnd = CameraComponent->GetComponentLocation() + (GetActorRotation().Vector() * 5000);

		//ɨ����
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TranceStart, TranceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TranceEnd = Hit.ImpactPoint;
		}
		//��ȡ���䷽��
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TranceEnd - RightHandLoc).Rotator();

		FTransform SpawnTM = FTransform(GetActorRotation(), RightHandLoc);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);

	}
}

void ARGCharacter::OnHealthChanged(AActor* InstigatorActor, URGAttributeActorComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* PC =  Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ARGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ARGCharacter::OnHealthChanged);
}

// Called every frame
void ARGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARGCharacter::MoveForWard);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARGCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARGCharacter::Jump);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARGCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ARGCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ARGCharacter::Dash);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARGCharacter::PrimaryInteract);
}

