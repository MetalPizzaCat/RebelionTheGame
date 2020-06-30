local Player ={}
--for now it is advised to not have any variables that are important for work of the script as they will be reset due to issues with save system
Player.DebugInt=0;

function Player:SpawnDebugString()
    Player.DebugInt=Player.DebugInt+1;
    print("Lua code is exucuted DebugInt is ".. Player.DebugInt)
    
end

function Player:OnWeaponChanged()
  --react to changing to new weapon
end

function Player:OnLoaded()
--called when actor was loaded from save
end

function Player:OnNum9Pressed()
    Player.SpawnDebugString();    
end

function Player:BeginPlay()
    --Add here code
end

return Player;