#pragma once

struct FVector {
	float X;
	float Y;
	float Z;
};

struct FRotator {
	float Pitch;
	float Yaw;
	float Roll;
};

struct FMinimalViewInfo
{
	FVector Location;
	FRotator Rotation;
	float FOV;
	// etc
};

enum EStereoscopicPass
{
	eSSP_FULL,
	eSSP_LEFT_EYE,
	eSSP_RIGHT_EYE,
	eSSP_LEFT_EYE_SIDE,
	eSSP_RIGHT_EYE_SIDE,
};

// Class Engine.SceneComponent
struct USceneComponent {
	char imgay[0x11C];
	FVector RelativeLocation; // 0x11C(0x0C)
	FRotator RelativeRotation; // 0x128(0x0C)
};

// Class Engine.Actor
struct AActor {
	char imgay[0x138];
	USceneComponent* RootComponent; // 0x138 (0x08)
};

// Class Engine.Pawn 
struct APawn : AActor {
};

// Class Engine.PlayerController
struct APlayerController {
	char imgay[0x2a8];
	APawn* AcknowledgedPawn; // 0x2a8 (0x08)
};

// Class Engine.Player
struct UPlayer {
	char imgay[0x30];
	APlayerController* PlayerController; // 0x30(0x08)
};

// Class Engine.LocalPlayer
struct ULocalPlayer : UPlayer {
};