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

function CheckForEnemyHS2()
	return not CheckForEnemyHS()
end


function CheckOwnPowerStation()
	local psList = World.GetPowerStations(World.Handle)
	local userRobo = World.GetUserRobo(World.Handle)
	
	local i, ps = next(psList)
	while i do
		inf = World.GetSectorInfo(ps.sector)
		
		if inf.owner == Bact.GetOwner(userRobo) then
			return true
		end
		
		i, ps = next(psList, i)
	end
	
	return false
end


function CheckRoboOnPower()
	local userRobo = World.GetUserRobo(World.Handle)
	local pSector = Bact.GetPSector(userRobo)
	local inf = World.GetSectorInfo(pSector)
	return inf.w_type == 2
end


function CheckGates()
	local gateList = World.GetGates(World.Handle)
	local i, gate = next(gateList)
	while i do
		inf = World.GetSectorInfo(gate.sector)
		
		if inf.w_type == 6 then
			return true
		end
		
		i, gate = next(gateList, i)
	end
	
	return false
end



Events = 
{
	{ tp = 5
	, time = 3000
	, msgs = { 0 } } 
  , { tp = 5
    , time = 8000
	, msgs = { 59 } }
  , { tp = 3
    , time = 60000
	, func = CheckOwnPowerStation
	, msgs = { 54, 94 } }
  , { tp = 5
    , time = 3000
	, msgs = { 95 } }
  , { tp = 3
    , time = 60000
	, func = CheckRoboOnPower
	, msgs = { 97, 96 } }
  , { tp = 5
    , time = 3000
	, msgs = { 98 } }
  , { tp = 5
    , time = 9000
	, msgs = { 99 } }
  , { tp = 5
    , time = 5000
	, msgs = { 0 } }
  , { tp = 3
    , time = 60000
	, func = CheckForEnemyHS2
	, msgs = { 101, 100 } }
  , { tp = 5
    , time = 4000
	, msgs = { 102 } }
  , { tp = 3
    , time = 60000
	, func = CheckGates
	, msgs = { 55 } }
  , { tp = 5
    , time = 3000
	, msgs = { 103 } }
  , { tp = 5
    , time = 45000
	, msgs = { 0 } }
  , { tp = 4
    , time = 60000
	, msgs = { 104, 56, 65 } }
}


function init(ts)
	Lessons:Init(Events)
end

function update(ts, dtime)
	Lessons:Update(ts)
end
