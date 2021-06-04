function main()
	
	print("[LUA] Running...")

	--tree = ai.BehaviorTree("LuaTree")

	--root = ai.BTFallback("Root Fallback")

	--tree:setRoot(root)	


	tree = ai.BehaviorTree("LuaTree")

	sequence = ai.BTSequence("Starting Sequence")
	fallback = ai.BTFallback("Fallback")
	timer = ai.BTTimer("Timer", 0, 1, 3.0)

	sequence:addChild(timer)
	sequence:addChild(fallback)

	tree:setRoot(sequence)

	return tree
end