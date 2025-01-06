// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorsCheatManager.h"

#include "Materials/MaterialParameterCollectionInstance.h"

void UDoorsCheatManager::ShowNavModifierOverrideVolumeDebug()
{
    if (NavModifierOverrideVolumeDebugMPC)
    {
		if (UMaterialParameterCollectionInstance* MPCInstance = GetWorld()->GetParameterCollectionInstance(NavModifierOverrideVolumeDebugMPC))
        {
			MPCInstance->SetScalarParameterValue("ShowNavModifierVolumeDebug", 1);
        }
    }
}

void UDoorsCheatManager::HideNavModifierOverrideVolumeDebug()
{
    if (NavModifierOverrideVolumeDebugMPC)
    {
		if (UMaterialParameterCollectionInstance* MPCInstance = GetWorld()->GetParameterCollectionInstance(NavModifierOverrideVolumeDebugMPC))
        {
			MPCInstance->SetScalarParameterValue("ShowNavModifierVolumeDebug", 0);
        }
    }
}