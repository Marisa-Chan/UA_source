System.RunScript("lessons.lua")

function CheckForEnemyHS()
	local unitList = World.GetUnits(World.Handle)
	local userRobo = World.GetUserRobo(World.Handle)
	
	local i, unit = next(unitList)
	while i do
		-- Check for enemy units present
		if  unit ~= userRobo and Bact.GetType(unit) == Bact.TYPE_ROBO and Bact.GetStatus(unit) ~= Bact.STATUS_DEAD then
			return true
		end
		
		i, unit = next(unitList, i)
	end
	return false
end

function CheckCreateVehicle()
	local userRobo = World.GetUserRobo(World.Handle)
	local userUnit = World.GetUserUnit(World.Handle)
	
	if CheckForEnemyHS() and
	   ((userUnit == userRobo or World.IsInRoboGun(World.Handle)) and World.GetUserSquadsCount(World.Handle) == 0) then
	   return false
	end
	
	return true
end


function CheckIfVehiclePresent()
	local userRobo = World.GetUserRobo(World.Handle)
	local userUnit = World.GetUserUnit(World.Handle)

	if CheckForEnemyHS() and
	  ((userUnit == userRobo or World.IsInRoboGun(World.Handle)) and World.GetUserSquadsCount(World.Handle) > 0) then
		return false
	end

	return true
end


function CheckForEnemyHS2()
	return not CheckForEnemyHS()
end


Events = 
{
	{ tp = 5
	, time = 5000
	, msgs = { 0 } } 
  , { tp = 3
    , time = 20000
	, func = CheckCreateVehicle
	, msgs = { 47 } }
  , { tp = 3
    , time = 20000
	, func = CheckIfVehiclePresent
	, msgs = { 48 } }
  , { tp = 3
    , time = 20000
	, func = CheckForEnemyHS2
	, msgs = { 49 } }
  , { tp = 6
    , time = 15000
	, msgs = { 60 } }
}



function init(ts)
	Lessons:Init(Events)
end

function update(ts, dtime)
	Lessons:Update(ts)
end
