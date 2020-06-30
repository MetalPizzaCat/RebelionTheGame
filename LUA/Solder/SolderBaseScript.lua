local Solder = {}

function Solder:BeginPlay()
   
end

function Solder:Die()
end

 function Solder:CalculateHitVelocity(HitFromDirection,HitInfo,DamageCauser)
   ImpulseModifier=0.07;--default is 0.07. Increase it to create ragdoll chaos
    Impulse=nil;
    if(DamageCauser~=nil)
    then
        if(IsBullet(DamageCauser)==true)
        then
        Impulse= Vector:new(Actor:GetVelocity(DamageCauser)["X"]* ImpulseModifier,Actor:GetVelocity(DamageCauser)["Y"]* ImpulseModifier,Actor:GetVelocity(DamageCauser)["Z"]* ImpulseModifier)
        elseif(IsGrenade(DamageCauser)==true)     
        then
            Rotation= Rotator:FindLookAtRotation(Actor:GetLocation(DamageCauser),HitInfo["Location"]);
            Distance = Grenade:GetDamageRadius(DamageCauser)-HitInfo["Distance"];
            Impulse = Vector:MultiplyByFloat(Rotator:GetRotationXVector(Rotation),Distance);
        else
            Impulse=HitInfo["TraceStart"]-HitInfo["TraceEnd"]
        end
    else
        Impulse=HitInfo["TraceStart"]-HitInfo["TraceEnd"]
    end
    Result={}
    Result["Impulse"]=Impulse
    Result["BoneName"]=HitInfo["HitBoneName"]
    Result["ImpulseLocation"]=HitInfo["Location"]
    return Result;
    
end


return Solder;