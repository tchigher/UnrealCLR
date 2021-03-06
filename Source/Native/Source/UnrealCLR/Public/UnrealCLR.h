/*
 * Copyright (c) 2020 Stanislav Denisov (nxrighthere@gmail.com)
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 with a static linking exception which accompanies this
 * distribution.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

#pragma once

// @third party code - BEGIN CoreCLR
#include "../../Dependencies/CoreCLR/includes/coreclr_delegates.h"
#include "../../Dependencies/CoreCLR/includes/hostfxr.h"
// @third party code - END CoreCLR

#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/InputComponent.h"
#include "Components/LightComponent.h"
#include "Components/LightComponentBase.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DirectionalLight.h"
#include "Engine/GameEngine.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/Light.h"
#include "Engine/Player.h"
#include "Engine/PointLight.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/RectLight.h"
#include "Engine/SpotLight.h"
#include "Engine/TriggerBox.h"
#include "Engine/TriggerCapsule.h"
#include "Engine/TriggerSphere.h"
#include "Engine/TriggerVolume.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/DefaultValueHelper.h"
#include "Modules/ModuleManager.h"
#include "MotionControllerComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Sound/AmbientSound.h"
#include "UnrealEngine.h"

#include "UnrealCLRFramework.h"
#include "UnrealCLRLibrary.h"
#include "UnrealCLRManager.h"

#if WITH_EDITOR
	#include "Editor.h"
	#include "Framework/Notifications/NotificationManager.h"
	#include "Widgets/Notifications/SNotificationList.h"
#endif

#define UNREALCLR_NONE
#define UNREALCLR_BRACKET_LEFT (
#define UNREALCLR_BRACKET_RIGHT )

UNREALCLR_API DECLARE_LOG_CATEGORY_EXTERN(LogUnrealCLR, Log, All);

namespace UnrealCLR {
	enum struct StatusType : int32 {
		Stopped,
		Idle,
		Running
	};

	enum struct LogLevel : int32 {
		Display,
		Warning,
		Error,
		Fatal
	};

	enum struct ObjectType : int32 {
		ActorOverlapDelegate,
		ActorHitDelegate,
		ComponentOverlapDelegate,
		ComponentHitDelegate
	};

	enum struct ArgumentType : int32 {
		None,
		Single,
		Integer,
		Pointer,
		Object
	};

	enum {
		OnWorldBegin,
		OnWorldPrePhysicsTick,
		OnWorldDuringPhysicsTick,
		OnWorldPostPhysicsTick,
		OnWorldPostUpdateTick,
		OnWorldEnd,
		OnActorBeginOverlap,
		OnActorEndOverlap,
		OnActorHit,
		OnComponentBeginOverlap,
		OnComponentEndOverlap,
		OnComponentHit
	};

	struct Object {
		void** Parameters;
		ObjectType Type;

		FORCEINLINE Object(void** Parameters, ObjectType Type) {
			this->Parameters = Parameters;
			this->Type = Type;
		}
	};

	struct Argument {
		union {
			float Single;
			uint32_t Integer;
			void* Pointer;
			Object Object;
		};
		ArgumentType Type;

		FORCEINLINE Argument(float Value) {
			this->Single = Value;
			this->Type = ArgumentType::Single;
		}

		FORCEINLINE Argument(uint32_t Value) {
			this->Integer = Value;
			this->Type = ArgumentType::Integer;
		}

		FORCEINLINE Argument(void* Value) {
			this->Pointer = Value;
			this->Type = !Value ? ArgumentType::None : ArgumentType::Pointer;
		}

		FORCEINLINE Argument(UnrealCLR::Object Value) {
			this->Object = Value;
			this->Type = ArgumentType::Object;
		}
	};

	typedef void (*ExecuteManagedFunctionDelegate)(void*, Argument);
	typedef void* (*FindManagedFunctionDelegate)(const char_t*, int32_t);
	typedef void (*LoadAssembliesDelegate)();
	typedef void (*UnloadAssembliesDelegate)();

	static ExecuteManagedFunctionDelegate ExecuteManagedFunction;
	static FindManagedFunctionDelegate FindManagedFunction;
	static LoadAssembliesDelegate LoadAssemblies;
	static UnloadAssembliesDelegate UnloadAssemblies;

	static FString ProjectPath;
	static FString UserAssembliesPath;

	static StatusType Status = StatusType::Stopped;

	struct PrePhysicsTickFunction : public FTickFunction {
		virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
		virtual FString DiagnosticMessage() override;
	};

	struct DuringPhysicsTickFunction : public FTickFunction {
		virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
		virtual FString DiagnosticMessage() override;
	};

	struct PostPhysicsTickFunction : public FTickFunction {
		virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
		virtual FString DiagnosticMessage() override;
	};

	struct PostUpdateTickFunction : public FTickFunction {
		virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
		virtual FString DiagnosticMessage() override;
	};

	class Module : public IModuleInterface {
		protected:

		virtual void StartupModule() override;
		virtual void ShutdownModule() override;

		private:

		void OnWorldInitializedActors(const UWorld::FActorsInitializedParams& ActorsInitializedParams);
		void OnWorldCleanup(UWorld* World, bool SessionEnded, bool CleanupResources);

		static void RegisterTickFunction(FTickFunction& TickFunction, ETickingGroup TickGroup);
		static void HostError(const char_t* Message);
		static void Invoke(void(*)(), Argument Value);
		static void Exception(const char* Message);
		static void Log(UnrealCLR::LogLevel Level, const char* Message);

		FDelegateHandle OnWorldInitializedActorsHandle;
		FDelegateHandle OnWorldCleanupHandle;

		PrePhysicsTickFunction OnPrePhysicsTickFunction;
		DuringPhysicsTickFunction OnDuringPhysicsTickFunction;
		PostPhysicsTickFunction OnPostPhysicsTickFunction;
		PostUpdateTickFunction OnPostUpdateTickFunction;

		void* HostfxrLibrary;
	};

	namespace Engine {
		static UUnrealCLRManager* Manager;
		static UWorld* World;
		static bool TickStarted;
	}

	namespace Shared {
		constexpr static int32 storageSize = 64;

		// Non-instantiable

		static void* AssertFunctions[storageSize];
		static void* CommandLineFunctions[storageSize];
		static void* DebugFunctions[storageSize];
		static void* ObjectFunctions[storageSize];
		static void* ApplicationFunctions[storageSize];
		static void* ConsoleManagerFunctions[storageSize];
		static void* EngineFunctions[storageSize];
		static void* WorldFunctions[storageSize];

		// Instantiable

		static void* BlueprintFunctions[storageSize];
		static void* ConsoleObjectFunctions[storageSize];
		static void* ConsoleVariableFunctions[storageSize];
		static void* ActorFunctions[storageSize];
		static void* PawnFunctions[storageSize];
		static void* ControllerFunctions[storageSize];
		static void* AIControllerFunctions[storageSize];
		static void* PlayerControllerFunctions[storageSize];
		static void* VolumeFunctions[storageSize];
		static void* PostProcessVolumeFunctions[storageSize];
		static void* SoundBaseFunctions[storageSize];
		static void* SoundWaveFunctions[storageSize];
		static void* AnimationInstanceFunctions[storageSize];
		static void* PlayerFunctions[storageSize];
		static void* PlayerInputFunctions[storageSize];
		static void* Texture2DFunctions[storageSize];
		static void* ActorComponentFunctions[storageSize];
		static void* InputComponentFunctions[storageSize];
		static void* SceneComponentFunctions[storageSize];
		static void* AudioComponentFunctions[storageSize];
		static void* CameraComponentFunctions[storageSize];
		static void* ChildActorComponentFunctions[storageSize];
		static void* PrimitiveComponentFunctions[storageSize];
		static void* ShapeComponentFunctions[storageSize];
		static void* BoxComponentFunctions[storageSize];
		static void* SphereComponentFunctions[storageSize];
		static void* CapsuleComponentFunctions[storageSize];
		static void* MeshComponentFunctions[storageSize];
		static void* LightComponentBaseFunctions[storageSize];
		static void* LightComponentFunctions[storageSize];
		static void* MotionControllerComponentFunctions[storageSize];
		static void* StaticMeshComponentFunctions[storageSize];
		static void* InstancedStaticMeshComponentFunctions[storageSize];
		static void* SkinnedMeshComponentFunctions[storageSize];
		static void* SkeletalMeshComponentFunctions[storageSize];
		static void* RadialForceComponentFunctions[storageSize];
		static void* MaterialInterfaceFunctions[storageSize];
		static void* MaterialFunctions[storageSize];
		static void* MaterialInstanceFunctions[storageSize];
		static void* MaterialInstanceDynamicFunctions[storageSize];
		static void* HeadMountedDisplayFunctions[storageSize];

		static void* ManagedFunctions[4];
		static void* NativeFunctions[5];
		static void* Events[128];
		static void* Functions[128];
	}

	namespace Utility {
		FORCEINLINE static size_t Strcpy(char* Destination, const char* Source, size_t Length);
		FORCEINLINE static size_t Strlen(const char* Source);
	}
}