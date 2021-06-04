function main()
	
	print("[LUA] Running...")

	--
	-- First Working Tree
	--
	--tree = ai.BehaviorTree("LuaTree")

	--sequence = ai.BTSequence("Starting Sequence")
	--fallback = ai.BTFallback("Fallback")
	--timer = ai.BTTimer("Timer", 0, 1, 3.0)

	--sequence:addChild(timer)
	--sequence:addChild(fallback)

	--tree:setRoot(sequence)

	tree = ai.BehaviorTree("LuaTree")
	
	sequence = ai.BTSequence("Starting Sequence")
	fallback = ai.BTFallback("Fallback")
	timer = ai.BTTimer("Timer", 0, 1, 3.0)
	
	sequence:addChild(timer)
	sequence:addChild(fallback)
	
	tree:setRoot(sequence)

	return tree
end