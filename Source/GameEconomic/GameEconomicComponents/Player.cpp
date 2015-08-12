#include <Urho3D/Urho3D.h>

#include "../../../Urho3D/Core/CoreEvents.h"
#include "../../../Urho3D/Engine/Engine.h"
#include "../../../Urho3D/Core/ProcessUtils.h"
#include "../../../Urho3D/Resource/ResourceCache.h"
#include "../../../Urho3D/Core/Context.h"
#include "../../../Urho3D/Container/Vector.h"
#include "../../../Urho3D/Container/Str.h"
#include "../../../Urho3D/Scene/LogicComponent.h"

/// header
#include "Player.h"


using namespace std;
using namespace Urho3D;

/** @brief Player
  *
  * @todo: document this function
  */
Player::Player(Context *context):
    LogicComponent(context)
    ,ThisPlayer(NULL)
{
    /// Allocate a new player
    ThisPlayer = new PlayerObject();
}

Player::~Player()
{
    /// Remove player from memory
    delete ThisPlayer;

    //dtor
}

/// Entity Register Object
void Player::RegisterObject(Context* context)
{
    context->RegisterFactory<Player>();

    return;
}


void Player::Start(void)
{
    UniqueID.Append("PlayerOmegaTester");

    return;
}

bool Player::AddCredits(unsigned long int AddingCredits)
{
    Credits+=AddingCredits;

    return true;
}

bool Player::SubtractCredits(unsigned long int SubtractingCredits)
{
    Credits-=SubtractingCredits;

    return true;
}
