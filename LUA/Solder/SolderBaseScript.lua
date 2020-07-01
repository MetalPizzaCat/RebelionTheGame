local Solder = {}

function Solder:BeginPlay()
   
end

function Solder:Die()
end

 function Solder:CalculateHitVelocity(HitFromDirection,HitInfo,DamageCauser)
    --this code is currently not used because of vector issue(result is always in one direction for grenades)
    ImpulseModifier=0.7;--default is 0.07. Increase it to create ragdoll chaos
    Impulse=nil;
    if(DamageCauser~=nil)
    then
        if(IsBullet(DamageCauser)==true)
        then
        Impulse= Vector:MultiplyByFloat(Actor:GetVelocity(DamageCauser),ImpulseModifier)
        elseif(IsGrenade(DamageCauser)==true)     
        then
            Rotation= Rotator:FindLookAtRotation(Actor:GetLocation(DamageCauser),HitInfo["Location"]);
            Distance = Grenade:GetDamageRadius(DamageCauser) - HitInfo["Distance"];
            Impulse = Vector:MultiplyByFloat(Rotator:GetRotationXVector(Rotation),Distance*0.01);
        else
            Impulse=Vector:Subtract(HitInfo["TraceStart"],HitInfo["TraceEnd"])
        end
    else
        ImpulseVector:Subtract(HitInfo["TraceStart"],HitInfo["TraceEnd"])
    end
    Result={}
    Result["Impulse"]=Impulse
    Result["BoneName"]=HitInfo["HitBoneName"]
    Result["ImpulseLocation"]=HitInfo["Location"]
    return Result;
    
end


return Solder;