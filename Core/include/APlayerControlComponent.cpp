#include "APlayerControlComponent.h"

typedef AInteractionSequence* (__thiscall* _CallInteractionSequence)(APlayerControlComponent* pThis);
_CallInteractionSequence N_CallInteractionSequence = (_CallInteractionSequence)0x0058f530;

AInteractionSequence* APlayerControlComponent::CallInteractionSequence()
{
    return N_CallInteractionSequence(this);
}

typedef bool (__thiscall* _HandleCurrentInteractionSequence)(APlayerControlComponent* pThis);
_HandleCurrentInteractionSequence N_HandleCurrentInteractionSequence = (_HandleCurrentInteractionSequence)0x0058eb10;

bool APlayerControlComponent::HandleCurrentInteractionSequence()
{
    return N_HandleCurrentInteractionSequence(this);
}
