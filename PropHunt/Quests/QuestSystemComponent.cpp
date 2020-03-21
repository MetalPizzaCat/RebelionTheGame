// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "QuestSystemComponent.h"
#include "QuestRequirementTalkTo.h"
#include "Engine.h"

// Sets default values for this component's properties
UQuestSystemComponent::UQuestSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuestSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UQuestSystemComponent::CompleteAction(FName actionName)
{
	if (CurrentQuest != nullptr) 
	{
		if (CurrentQuest->Requirementes.Num() > 0)
		{
			
			for (int i = 0; i < CurrentQuest->Requirementes.Num(); i++)
			{
				
				if (CurrentQuest->Requirementes[i]->Name == actionName&& !CurrentQuest->Requirementes[i]->bFulfilled)
				{
					CurrentQuest->Requirementes[i]->bFulfilled = true;
					
					return true;
				}
			}
		}
	}
	return false;
}

bool UQuestSystemComponent::CompleteTalkToSomeone(AActor* someone)
{
	if (CurrentQuest != nullptr)
	{
		if (CurrentQuest->Requirementes.Num() > 0)
		{
			for (int i = 0; i < CurrentQuest->Requirementes.Num(); i++)
			{
				auto Req = Cast< UQuestRequirementTalkTo>(CurrentQuest->Requirementes[i]);
				if(Req !=nullptr&& Req->IsValidLowLevel())
				if (Req->Someone == someone)
				{
					CurrentQuest->Requirementes[i]->bFulfilled = true;
					return true;
				}
			}
		}
	}
	return false;
}

void UQuestSystemComponent::ForceCompleteQuest()
{
	CurrentQuest->FinishQuest();
	CompletedQuestes.Add(CurrentQuest);
	CurrentQuest = nullptr;
}

bool UQuestSystemComponent::ReceiveQuest(TSubclassOf<UQuestBase>questClass, TArray< UQuestRequirementBase*>Requirements)
{
	if (CurrentQuest != nullptr && CurrentQuest->IsValidLowLevel())
	{
		return false;
	}
	else
	{
		if (Requirements.Num() > 0)
		{
			CurrentQuest = Cast< UQuestBase>(StaticConstructObject_Internal(questClass));
			for (int i = 0; i < Requirements.Num(); i++)
			{
				CurrentQuest->Requirementes.Add(Requirements[i]);
			}
			return true;		
		}		
	}	
	return false;
 }

bool UQuestSystemComponent::FinishQuest_Implementation()
{
	if (CurrentQuest != nullptr)
	{
		if (CurrentQuest->Requirementes.Num() > 0)
		{
			for (int i = 0; i < CurrentQuest->Requirementes.Num(); i++)
			{
				if (CurrentQuest->Requirementes[i] == nullptr || CurrentQuest->Requirementes[i]->IsValidLowLevel())
				{
					if (!CurrentQuest->Requirementes[i]->GetIsFulfilled() && !CurrentQuest->Requirementes[i]->bOptional)
					{
						return false;
					}
				}
			}
			
			CurrentQuest->FinishQuest();
			CompletedQuestes.Add(CurrentQuest);
			CurrentQuest = nullptr;
			return true;
		}
	}
	

	return true;
}

