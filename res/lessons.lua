Lessons = {
	EventResult = { Message = 1, Stop = 2, Skip = 3 }

	, LastEvntIdWithMsg = -1
	, Events = {}
	, LastMsgTime = 0
}


function Lessons:Init(evnts)
	self.Events = evnts
	self.LastEvntIdWithMsg = -1
	self.LastMsgTime = 0
end


function Lessons:ProcessEvent(ts, evntId)
	local ev = self.Events[ evntId ]

    if ( ev.sTime == nil ) then
        ev.sTime = ts
	end

    if ev.tp == 1 then
        if ev.once ~= nil then
			return self.EventResult.Skip
		end
		
        ev.once = true

        if ev.func and (ev.func() == true) then
			return self.EventResult.Skip
		end

        return self.EventResult.Message
		
	elseif ev.tp == 2 then
        if (ts - ev.sTime) <= ev.time then
            return self.EventResult.Skip
		end

        ev.sTime = ts

        if ev.func and (ev.func() == true) then
			return self.EventResult.Skip
		end
		
        return self.EventResult.Message
	
    elseif ev.tp == 3 then
		if ev.func and (ev.func() ~= true) then
			return self.EventResult.Message
		end
		return self.EventResult.Skip
		
	elseif ev.tp == 4 then
        if ev.once == nil then
            ev.once = true
		elseif (ts - ev.sTime) <= ev.time then
			return self.EventResult.Skip
		else
			ev.sTime = ts
		end
		
		if ev.func and (ev.func() == true) then
			return self.EventResult.Skip
		end
		
		return self.EventResult.Message
		
    elseif ev.tp == 5 then
        if (ts - ev.sTime) <= ev.time then
			return self.EventResult.Stop
		end
		
        if ev.once ~= nil then
            return self.EventResult.Skip
		end

        ev.once = true

        if ev.func and (ev.func() == true) then
			return self.EventResult.Skip
		end

        return self.EventResult.Message
		  
    elseif ev.tp == 6 then
        if (ts - ev.sTime) <= ev.time then
			return self.EventResult.Stop
		end

        if ev.once ~= nil then
            return self.EventResult.Skip
		end

        ev.once = true
        
        local res = self.EventResult.Message

		if ev.func and (ev.func() == true) then
			res = self.EventResult.Skip
		end

        World.ExitTutorialWindow(World.Handle)
        return res
    end

    return self.EventResult.Skip
end


function Lessons:ShowMsg(ts, evntId)
	local ev = self.Events[evntId]
	
	self.LastEvntIdWithMsg = evntId
    self.LastMsgTime = ts
	
	if ev.msgs then
		if ev.CurMsg == nil or ev.msgs[ev.CurMsg] == nil then
			ev.CurMsg = 1
		else
			ev.CurMsg = ev.CurMsg + 1
		end
	
		if ev.msgs[ev.CurMsg] then
			World.ShowMessageID(World.Handle, ev.msgs[ev.CurMsg])
		end
	end 
end

function Lessons:Update(ts)
	local res = 0
	local i = 1
	
	while self.Events[i] do
		res = self:ProcessEvent(ts, i)
				
		if res ~= self.EventResult.Skip then
			break
		end
		
		i = i + 1
	end
	
	if res == self.EventResult.Message and self.Events[i] then
		if i == self.LastEvntIdWithMsg then
			if (ts - self.LastMsgTime) > self.Events[i].time then
				self:ShowMsg(ts, i)
			end
		else
			self:ShowMsg(ts, i)
		end
	end
end

