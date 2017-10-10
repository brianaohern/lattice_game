// Copyright 

#include "LatticeWeapon.h"
#include "LatticeGame.h"
// #include "Particles/ParticleSystemComponent.h"
// #include "Bots/LatticeAIController.h"
// #include "Online/LatticePlayerState.h"
// #include "UI/LatticeHUD.h"

ALatticeWeapon::ALatticeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Mesh1P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh1P"));
	// Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	// Mesh1P->bReceivesDecals = false;
	// Mesh1P->CastShadow = false;
	// Mesh1P->SetCollisionObjectType(ECC_WorldDynamic);
	// Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = Mesh1P;

	Mesh3P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh3P"));
	// Mesh3P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	// Mesh3P->bReceivesDecals = false;
	// Mesh3P->CastShadow = true;
	// Mesh3P->SetCollisionObjectType(ECC_WorldDynamic);
	// Mesh3P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Mesh3P->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Mesh3P->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	// Mesh3P->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	// Mesh3P->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);
	Mesh3P->SetupAttachment(Mesh1P);

// 	bLoopedMuzzleFX = false;
// 	bLoopedFireAnim = false;
// 	bPlayingFireAnim = false;
	bIsEquipped = false;
	bWantsToFire = false;
	bPendingReload = false;
	bPendingEquip = false;
	CurrentState = EWeaponState::Idle;

	CurrentAmmo = 0;
	CurrentAmmoInClip = 0;
	BurstCounter = 0;
	LastFireTime = 0.0f;

// 	PrimaryActorTick.bCanEverTick = true;
// 	PrimaryActorTick.TickGroup = TG_PrePhysics;
// 	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
// 	bReplicates = true;
// 	bNetUseOwnerRelevancy = true;
}

// void ALatticeWeapon::PostInitializeComponents()
// {
// 	Super::PostInitializeComponents();

// 	if (WeaponConfig.InitialClips > 0)
// 	{
// 		CurrentAmmoInClip = WeaponConfig.AmmoPerClip;
// 		CurrentAmmo = WeaponConfig.AmmoPerClip * WeaponConfig.InitialClips;
// 	}

// 	DetachMeshFromPawn();
// }

// void ALatticeWeapon::Destroyed()
// {
// 	Super::Destroyed();

// 	StopSimulatingWeaponFire();
// }

//////////////////////////////////////////////////////////////////////////
// Inventory

void ALatticeWeapon::OnEquip(const ALatticeWeapon* LastWeapon)
{
	AttachMeshToPawn();

	bPendingEquip = true;
	DetermineWeaponState();

	// // Only play animation if last weapon is valid
	// if (LastWeapon)
	// {
	// 	float Duration = PlayWeaponAnimation(EquipAnim);
	// 	if (Duration <= 0.0f)
	// 	{
	// 		// failsafe
	// 		Duration = 0.5f;
	// 	}
	// 	EquipStartedTime = GetWorld()->GetTimeSeconds();
	// 	EquipDuration = Duration;

	// 	GetWorldTimerManager().SetTimer(TimerHandle_OnEquipFinished, this, &ALatticeWeapon::OnEquipFinished, Duration, false);
	// }
	// else
	// {
		OnEquipFinished();
	// }

	// if (MyPawn && MyPawn->IsLocallyControlled())
	// {
	// 	PlayWeaponSound(EquipSound);
	// }
}

void ALatticeWeapon::OnEquipFinished()
{
	AttachMeshToPawn();

	bIsEquipped = true;
	bPendingEquip = false;

	// Determine the state so that the can reload checks will work
	DetermineWeaponState(); 
	
	if (MyPawn)
	{
		// try to reload empty clip
		if (MyPawn->IsLocallyControlled() &&
			CurrentAmmoInClip <= 0 &&
			CanReload())
		{
			StartReload();
		}
	}

	
}

void ALatticeWeapon::OnUnEquip()
{
	DetachMeshFromPawn();
	bIsEquipped = false;
	StopFire();

	if (bPendingReload)
	{
		// StopWeaponAnimation(ReloadAnim);
		bPendingReload = false;

		// GetWorldTimerManager().ClearTimer(TimerHandle_StopReload);
		// GetWorldTimerManager().ClearTimer(TimerHandle_ReloadWeapon);
	}

	if (bPendingEquip)
	{
		// StopWeaponAnimation(EquipAnim);
		bPendingEquip = false;

		// GetWorldTimerManager().ClearTimer(TimerHandle_OnEquipFinished);
	}

	DetermineWeaponState();
}

void ALatticeWeapon::OnEnterInventory(ALatticeGameCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
}

void ALatticeWeapon::OnLeaveInventory()
{
	if (Role == ROLE_Authority)
	{
		SetOwningPawn(NULL);
	}

	if (IsAttachedToPawn())
	{
		OnUnEquip();
	}
}

void ALatticeWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		// Remove and hide both first and third person meshes
		DetachMeshFromPawn();

		// For locally controller players we attach both weapons and let the bOnlyOwnerSee, bOwnerNoSee flags deal with visibility.
		FName AttachPoint = MyPawn->GetWeaponAttachPoint();
		if( MyPawn->IsLocallyControlled() == true )
		{
			USkeletalMeshComponent* PawnMesh1p = MyPawn->GetSpecifcPawnMesh(true);
			USkeletalMeshComponent* PawnMesh3p = MyPawn->GetSpecifcPawnMesh(false);
			Mesh1P->SetHiddenInGame( false );
			Mesh3P->SetHiddenInGame( false );
			Mesh1P->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
			Mesh3P->AttachToComponent(PawnMesh3p, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
		}
		else
		{
			USkeletalMeshComponent* UseWeaponMesh = GetWeaponMesh();
			USkeletalMeshComponent* UsePawnMesh = MyPawn->GetPawnMesh();
			UseWeaponMesh->AttachToComponent(UsePawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
			UseWeaponMesh->SetHiddenInGame( false );
		}
	}
}

void ALatticeWeapon::DetachMeshFromPawn()
{
	Mesh1P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh1P->SetHiddenInGame(true);

	Mesh3P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh3P->SetHiddenInGame(true);
}


//////////////////////////////////////////////////////////////////////////
// Input

void ALatticeWeapon::StartFire()
{
	if (Role < ROLE_Authority)
	{
		ServerStartFire();
	}

	if (!bWantsToFire)
	{
		bWantsToFire = true;
		DetermineWeaponState();
	}
}

void ALatticeWeapon::StopFire()
{
	if (Role < ROLE_Authority)
	{
		ServerStopFire();
	}

	if (bWantsToFire)
	{
		bWantsToFire = false;
		DetermineWeaponState();
	}
}

void ALatticeWeapon::StartReload(bool bFromReplication)
{
	if (!bFromReplication && Role < ROLE_Authority)
	{
		ServerStartReload();
	}

	if (bFromReplication || CanReload())
	{
		bPendingReload = true;
		DetermineWeaponState();

		// float AnimDuration = PlayWeaponAnimation(ReloadAnim);		
		// if (AnimDuration <= 0.0f)
		// {
		// 	AnimDuration = WeaponConfig.NoAnimReloadDuration;
		// }

		// GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &ALatticeWeapon::StopReload, AnimDuration, false);
		// if (Role == ROLE_Authority)
		// {
		// 	GetWorldTimerManager().SetTimer(TimerHandle_ReloadWeapon, this, &ALatticeWeapon::ReloadWeapon, FMath::Max(0.1f, AnimDuration - 0.1f), false);
		// }
		
		// if (MyPawn && MyPawn->IsLocallyControlled())
		// {
		// 	PlayWeaponSound(ReloadSound);
		// }
	}
}

void ALatticeWeapon::StopReload()
{
	if (CurrentState == EWeaponState::Reloading)
	{
		bPendingReload = false;
		DetermineWeaponState();
		// StopWeaponAnimation(ReloadAnim);
	}
}

// bool ALatticeWeapon::ServerStartFire_Validate()
// {
// 	return true;
// }

// void ALatticeWeapon::ServerStartFire_Implementation()
// {
// 	StartFire();
// }

// bool ALatticeWeapon::ServerStopFire_Validate()
// {
// 	return true;
// }

// void ALatticeWeapon::ServerStopFire_Implementation()
// {
// 	StopFire();
// }

// bool ALatticeWeapon::ServerStartReload_Validate()
// {
// 	return true;
// }

// void ALatticeWeapon::ServerStartReload_Implementation()
// {
// 	StartReload();
// }

// bool ALatticeWeapon::ServerStopReload_Validate()
// {
// 	return true;
// }

// void ALatticeWeapon::ServerStopReload_Implementation()
// {
// 	StopReload();
// }

// void ALatticeWeapon::ClientStartReload_Implementation()
// {
// 	StartReload();
// }

// //////////////////////////////////////////////////////////////////////////
// // Control

bool ALatticeWeapon::CanFire() const
{
	bool bCanFire = MyPawn && MyPawn->CanFire();
	bool bStateOKToFire = ( ( CurrentState ==  EWeaponState::Idle ) || ( CurrentState == EWeaponState::Firing) );	
	return (( bCanFire == true ) && ( bStateOKToFire == true ) && ( bPendingReload == false ));
}

bool ALatticeWeapon::CanReload() const
{
	bool bCanReload = (!MyPawn || MyPawn->CanReload());
	bool bGotAmmo = ( CurrentAmmoInClip < WeaponConfig.AmmoPerClip) && (CurrentAmmo - CurrentAmmoInClip > 0 || HasInfiniteClip());
	bool bStateOKToReload = ( ( CurrentState ==  EWeaponState::Idle ) || ( CurrentState == EWeaponState::Firing) );
	return ( ( bCanReload == true ) && ( bGotAmmo == true ) && ( bStateOKToReload == true) );	
}


// //////////////////////////////////////////////////////////////////////////
// // Weapon usage

// void ALatticeWeapon::GiveAmmo(int AddAmount)
// {
// 	const int32 MissingAmmo = FMath::Max(0, WeaponConfig.MaxAmmo - CurrentAmmo);
// 	AddAmount = FMath::Min(AddAmount, MissingAmmo);
// 	CurrentAmmo += AddAmount;

// 	ALatticeAIController* BotAI = MyPawn ? Cast<ALatticeAIController>(MyPawn->GetController()) : NULL;
// 	if (BotAI)
// 	{
// 		BotAI->CheckAmmo(this);
// 	}
	
// 	// start reload if clip was empty
// 	if (GetCurrentAmmoInClip() <= 0 &&
// 		CanReload() &&
// 		MyPawn->GetWeapon() == this)
// 	{
// 		ClientStartReload();
// 	}
// }

void ALatticeWeapon::UseAmmo()
{
	if (!HasInfiniteAmmo())
	{
		CurrentAmmoInClip--;
	}

	if (!HasInfiniteAmmo() && !HasInfiniteClip())
	{
		CurrentAmmo--;
	}

	// ALatticeAIController* BotAI = MyPawn ? Cast<ALatticeAIController>(MyPawn->GetController()) : NULL;	
	// ALatticePlayerController* PlayerController = MyPawn ? Cast<ALatticePlayerController>(MyPawn->GetController()) : NULL;
	// if (BotAI)
	// {
	// 	BotAI->CheckAmmo(this);
	// }
	// else if(PlayerController)
	// {
	// 	ALatticePlayerState* PlayerState = Cast<ALatticePlayerState>(PlayerController->PlayerState);
	// 	switch (GetAmmoType())
	// 	{
	// 		case EAmmoType::ERocket:
	// 			PlayerState->AddRocketsFired(1);
	// 			break;
	// 		case EAmmoType::EBullet:
	// 		default:
	// 			PlayerState->AddBulletsFired(1);
	// 			break;			
	// 	}
	// }
}

void ALatticeWeapon::HandleFiring()
{
	if ((CurrentAmmoInClip > 0 || HasInfiniteClip() || HasInfiniteAmmo()) && CanFire())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			// SimulateWeaponFire();
		}

		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			FireWeapon();

			UseAmmo();
			
			// // update firing FX on remote clients if function was called on server
			// BurstCounter++;
		}
	}
	else if (CanReload())
	{
		StartReload();
	}
	else if (MyPawn && MyPawn->IsLocallyControlled())
	{
		if (GetCurrentAmmo() == 0 && !bRefiring)
		{
			// PlayWeaponSound(OutOfAmmoSound);
			// ALatticePlayerController* MyPC = Cast<ALatticePlayerController>(MyPawn->Controller);
			// ALatticeHUD* MyHUD = MyPC ? Cast<ALatticeHUD>(MyPC->GetHUD()) : NULL;
			// if (MyHUD)
			// {
			// 	MyHUD->NotifyOutOfAmmo();
			// }
		}
		
		// stop weapon fire FX, but stay in Firing state
		if (BurstCounter > 0)
		{
			OnBurstFinished();
		}
	}

	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		// local client will notify server
		if (Role < ROLE_Authority)
		{
			ServerHandleFiring();
		}

		// reload after firing last round
		if (CurrentAmmoInClip <= 0 && CanReload())
		{
			StartReload();
		}

		// setup refire timer
		bRefiring = (CurrentState == EWeaponState::Firing && WeaponConfig.TimeBetweenShots > 0.0f);
		if (bRefiring)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ALatticeWeapon::HandleFiring, WeaponConfig.TimeBetweenShots, false);
		}
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

// bool ALatticeWeapon::ServerHandleFiring_Validate()
// {
// 	return true;
// }

// void ALatticeWeapon::ServerHandleFiring_Implementation()
// {
// 	const bool bShouldUpdateAmmo = (CurrentAmmoInClip > 0 && CanFire());

// 	HandleFiring();

// 	if (bShouldUpdateAmmo)
// 	{
// 		// update ammo
// 		UseAmmo();

// 		// update firing FX on remote clients
// 		BurstCounter++;
// 	}
// }

// void ALatticeWeapon::ReloadWeapon()
// {
// 	int32 ClipDelta = FMath::Min(WeaponConfig.AmmoPerClip - CurrentAmmoInClip, CurrentAmmo - CurrentAmmoInClip);

// 	if (HasInfiniteClip())
// 	{
// 		ClipDelta = WeaponConfig.AmmoPerClip - CurrentAmmoInClip;
// 	}

// 	if (ClipDelta > 0)
// 	{
// 		CurrentAmmoInClip += ClipDelta;
// 	}

// 	if (HasInfiniteClip())
// 	{
// 		CurrentAmmo = FMath::Max(CurrentAmmoInClip, CurrentAmmo);
// 	}
// }

void ALatticeWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	const EWeaponState::Type PrevState = CurrentState;

	if (PrevState == EWeaponState::Firing && NewState != EWeaponState::Firing)
	{
		OnBurstFinished();
	}

	CurrentState = NewState;

	if (PrevState != EWeaponState::Firing && NewState == EWeaponState::Firing)
	{
		OnBurstStarted();
	}
}

void ALatticeWeapon::DetermineWeaponState()
{
	EWeaponState::Type NewState = EWeaponState::Idle;

	if (bIsEquipped)
	{
		if( bPendingReload  )
		{
			if( CanReload() == false )
			{
				NewState = CurrentState;
			}
			else
			{
				NewState = EWeaponState::Reloading;
			}
		}		
		else if ( (bPendingReload == false ) && ( bWantsToFire == true ) && ( CanFire() == true ))
		{
			NewState = EWeaponState::Firing;
		}
	}
	else if (bPendingEquip)
	{
		NewState = EWeaponState::Equipping;
	}

	SetWeaponState(NewState);
}

void ALatticeWeapon::OnBurstStarted()
{
	// start firing, can be delayed to satisfy TimeBetweenShots
	const float GameTime = GetWorld()->GetTimeSeconds();
	if (LastFireTime > 0 && WeaponConfig.TimeBetweenShots > 0.0f &&
		LastFireTime + WeaponConfig.TimeBetweenShots > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ALatticeWeapon::HandleFiring, LastFireTime + WeaponConfig.TimeBetweenShots - GameTime, false);
	}
	else
	{
		HandleFiring();
	}
}

void ALatticeWeapon::OnBurstFinished()
{
	// stop firing FX on remote clients
	BurstCounter = 0;

	// stop firing FX locally, unless it's a dedicated server
	if (GetNetMode() != NM_DedicatedServer)
	{
		// StopSimulatingWeaponFire();
	}
	
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
	bRefiring = false;
}


// //////////////////////////////////////////////////////////////////////////
// // Weapon usage helpers

// UAudioComponent* ALatticeWeapon::PlayWeaponSound(USoundCue* Sound)
// {
// 	UAudioComponent* AC = NULL;
// 	if (Sound && MyPawn)
// 	{
// 		AC = UGameplayStatics::SpawnSoundAttached(Sound, MyPawn->GetRootComponent());
// 	}

// 	return AC;
// }

// float ALatticeWeapon::PlayWeaponAnimation(const FWeaponAnim& Animation)
// {
// 	float Duration = 0.0f;
// 	if (MyPawn)
// 	{
// 		UAnimMontage* UseAnim = MyPawn->IsFirstPerson() ? Animation.Pawn1P : Animation.Pawn3P;
// 		if (UseAnim)
// 		{
// 			Duration = MyPawn->PlayAnimMontage(UseAnim);
// 		}
// 	}

// 	return Duration;
// }

// void ALatticeWeapon::StopWeaponAnimation(const FWeaponAnim& Animation)
// {
// 	if (MyPawn)
// 	{
// 		UAnimMontage* UseAnim = MyPawn->IsFirstPerson() ? Animation.Pawn1P : Animation.Pawn3P;
// 		if (UseAnim)
// 		{
// 			MyPawn->StopAnimMontage(UseAnim);
// 		}
// 	}
// }

// FVector ALatticeWeapon::GetCameraAim() const
// {
// 	ALatticePlayerController* const PlayerController = Instigator ? Cast<ALatticePlayerController>(Instigator->Controller) : NULL;
// 	FVector FinalAim = FVector::ZeroVector;

// 	if (PlayerController)
// 	{
// 		FVector CamLoc;
// 		FRotator CamRot;
// 		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
// 		FinalAim = CamRot.Vector();
// 	}
// 	else if (Instigator)
// 	{
// 		FinalAim = Instigator->GetBaseAimRotation().Vector();		
// 	}

// 	return FinalAim;
// }

FVector ALatticeWeapon::GetAdjustedAim() const
{
	ALatticePlayerController* const PlayerController = Instigator ? Cast<ALatticePlayerController>(Instigator->Controller) : NULL;
	FVector FinalAim = FVector::ZeroVector;
	// If we have a player controller use it for the aim
	// if (PlayerController)
	// {
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	// }
	// else if (Instigator)
	// {
	// 	// Now see if we have an AI controller - we will want to get the aim from there if we do
	// 	ALatticeAIController* AIController = MyPawn ? Cast<ALatticeAIController>(MyPawn->Controller) : NULL;
	// 	if(AIController != NULL )
	// 	{
	// 		FinalAim = AIController->GetControlRotation().Vector();
	// 	}
	// 	else
	// 	{			
	// 		FinalAim = Instigator->GetBaseAimRotation().Vector();
	// 	}
	// }

	return FinalAim;
}

// FVector ALatticeWeapon::GetCameraDamageStartLocation(const FVector& AimDir) const
// {
// 	ALatticePlayerController* PC = MyPawn ? Cast<ALatticePlayerController>(MyPawn->Controller) : NULL;
// 	ALatticeAIController* AIPC = MyPawn ? Cast<ALatticeAIController>(MyPawn->Controller) : NULL;
// 	FVector OutStartTrace = FVector::ZeroVector;

// 	if (PC)
// 	{
// 		// use player's camera
// 		FRotator UnusedRot;
// 		PC->GetPlayerViewPoint(OutStartTrace, UnusedRot);

// 		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
// 		OutStartTrace = OutStartTrace + AimDir * ((Instigator->GetActorLocation() - OutStartTrace) | AimDir);
// 	}
// 	else if (AIPC)
// 	{
// 		OutStartTrace = GetMuzzleLocation();
// 	}

// 	return OutStartTrace;
// }

// FVector ALatticeWeapon::GetMuzzleLocation() const
// {
// 	USkeletalMeshComponent* UseMesh = GetWeaponMesh();
// 	return UseMesh->GetSocketLocation(MuzzleAttachPoint);
// }

// FVector ALatticeWeapon::GetMuzzleDirection() const
// {
// 	USkeletalMeshComponent* UseMesh = GetWeaponMesh();
// 	return UseMesh->GetSocketRotation(MuzzleAttachPoint).Vector();
// }

// FHitResult ALatticeWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
// {
// 	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

// 	// Perform trace to retrieve hit info
// 	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
// 	TraceParams.bTraceAsyncScene = true;
// 	TraceParams.bReturnPhysicalMaterial = true;

// 	FHitResult Hit(ForceInit);
// 	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams);

// 	return Hit;
// }

void ALatticeWeapon::SetOwningPawn(ALatticeGameCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
		// net owner for RPC calls
		SetOwner(NewOwner);
	}	
}

//////////////////////////////////////////////////////////////////////////
// Replication & effects

void ALatticeWeapon::OnRep_MyPawn()
{
	if (MyPawn)
	{
		OnEnterInventory(MyPawn);
	}
	else
	{
		OnLeaveInventory();
	}
}

void ALatticeWeapon::OnRep_BurstCounter()
{
	if (BurstCounter > 0)
	{
		// SimulateWeaponFire();
	}
	else
	{
		// StopSimulatingWeaponFire();
	}
}

void ALatticeWeapon::OnRep_Reload()
{
	if (bPendingReload)
	{
		StartReload(true);
	}
	else
	{
		StopReload();
	}
}

// void ALatticeWeapon::SimulateWeaponFire()
// {
// 	if (Role == ROLE_Authority && CurrentState != EWeaponState::Firing)
// 	{
// 		return;
// 	}

// 	if (MuzzleFX)
// 	{
// 		USkeletalMeshComponent* UseWeaponMesh = GetWeaponMesh();
// 		if (!bLoopedMuzzleFX || MuzzlePSC == NULL)
// 		{
// 			// Split screen requires we create 2 effects. One that we see and one that the other player sees.
// 			if( (MyPawn != NULL ) && ( MyPawn->IsLocallyControlled() == true ) )
// 			{
// 				AController* PlayerCon = MyPawn->GetController();				
// 				if( PlayerCon != NULL )
// 				{
// 					Mesh1P->GetSocketLocation(MuzzleAttachPoint);
// 					MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh1P, MuzzleAttachPoint);
// 					MuzzlePSC->bOwnerNoSee = false;
// 					MuzzlePSC->bOnlyOwnerSee = true;

// 					Mesh3P->GetSocketLocation(MuzzleAttachPoint);
// 					MuzzlePSCSecondary = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh3P, MuzzleAttachPoint);
// 					MuzzlePSCSecondary->bOwnerNoSee = true;
// 					MuzzlePSCSecondary->bOnlyOwnerSee = false;				
// 				}				
// 			}
// 			else
// 			{
// 				MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, UseWeaponMesh, MuzzleAttachPoint);
// 			}
// 		}
// 	}

// 	if (!bLoopedFireAnim || !bPlayingFireAnim)
// 	{
// 		PlayWeaponAnimation(FireAnim);
// 		bPlayingFireAnim = true;
// 	}

// 	if (bLoopedFireSound)
// 	{
// 		if (FireAC == NULL)
// 		{
// 			FireAC = PlayWeaponSound(FireLoopSound);
// 		}
// 	}
// 	else
// 	{
// 		PlayWeaponSound(FireSound);
// 	}

// 	ALatticePlayerController* PC = (MyPawn != NULL) ? Cast<ALatticePlayerController>(MyPawn->Controller) : NULL;
// 	if (PC != NULL && PC->IsLocalController())
// 	{
// 		if (FireCameraShake != NULL)
// 		{
// 			PC->ClientPlayCameraShake(FireCameraShake, 1);
// 		}
// 		if (FireForceFeedback != NULL)
// 		{
// 			PC->ClientPlayForceFeedback(FireForceFeedback, false, "Weapon");
// 		}
// 	}
// }

// void ALatticeWeapon::StopSimulatingWeaponFire()
// {
// 	if (bLoopedMuzzleFX )
// 	{
// 		if( MuzzlePSC != NULL )
// 		{
// 			MuzzlePSC->DeactivateSystem();
// 			MuzzlePSC = NULL;
// 		}
// 		if( MuzzlePSCSecondary != NULL )
// 		{
// 			MuzzlePSCSecondary->DeactivateSystem();
// 			MuzzlePSCSecondary = NULL;
// 		}
// 	}

// 	if (bLoopedFireAnim && bPlayingFireAnim)
// 	{
// 		StopWeaponAnimation(FireAnim);
// 		bPlayingFireAnim = false;
// 	}

// 	if (FireAC)
// 	{
// 		FireAC->FadeOut(0.1f, 0.0f);
// 		FireAC = NULL;

// 		PlayWeaponSound(FireFinishSound);
// 	}
// }

// void ALatticeWeapon::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
// {
// 	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

// 	DOREPLIFETIME( ALatticeWeapon, MyPawn );

// 	DOREPLIFETIME_CONDITION( ALatticeWeapon, CurrentAmmo,		COND_OwnerOnly );
// 	DOREPLIFETIME_CONDITION( ALatticeWeapon, CurrentAmmoInClip, COND_OwnerOnly );

// 	DOREPLIFETIME_CONDITION( ALatticeWeapon, BurstCounter,		COND_SkipOwner );
// 	DOREPLIFETIME_CONDITION( ALatticeWeapon, bPendingReload,	COND_SkipOwner );
// }

USkeletalMeshComponent* ALatticeWeapon::GetWeaponMesh() const
{
	return (MyPawn != NULL && MyPawn->IsFirstPerson()) ? Mesh1P : Mesh3P;
}

// class ALatticeGameCharacter* ALatticeWeapon::GetPawnOwner() const
// {
// 	return MyPawn;
// }

// bool ALatticeWeapon::IsEquipped() const
// {
// 	return bIsEquipped;
// }

bool ALatticeWeapon::IsAttachedToPawn() const
{
	return bIsEquipped || bPendingEquip;
}

// EWeaponState::Type ALatticeWeapon::GetCurrentState() const
// {
// 	return CurrentState;
// }

int32 ALatticeWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

// int32 ALatticeWeapon::GetCurrentAmmoInClip() const
// {
// 	return CurrentAmmoInClip;
// }

// int32 ALatticeWeapon::GetAmmoPerClip() const
// {
// 	return WeaponConfig.AmmoPerClip;
// }

// int32 ALatticeWeapon::GetMaxAmmo() const
// {
// 	return WeaponConfig.MaxAmmo;
// }

bool ALatticeWeapon::HasInfiniteAmmo() const
{
	// const ALatticePlayerController* MyPC = (MyPawn != NULL) ? Cast<const ALatticePlayerController>(MyPawn->Controller) : NULL;
	return WeaponConfig.bInfiniteAmmo; // || (MyPC && MyPC->HasInfiniteAmmo());
}

bool ALatticeWeapon::HasInfiniteClip() const
{
	// const ALatticePlayerController* MyPC = (MyPawn != NULL) ? Cast<const ALatticePlayerController>(MyPawn->Controller) : NULL;
	return WeaponConfig.bInfiniteClip; // || (MyPC && MyPC->HasInfiniteClip());
}

// float ALatticeWeapon::GetEquipStartedTime() const
// {
// 	return EquipStartedTime;
// }

// float ALatticeWeapon::GetEquipDuration() const
// {
// 	return EquipDuration;
// }
