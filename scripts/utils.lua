-- Contains auxiliary functions used for debugging and testing.
utils = {
	-- Returns an array of environment variables and their types.
	get_env = function()
		res = {}
		for k, v in pairs(_G) do
			res[#res + 1] = tostring(k) .. " : " .. type(v)
		end
		return res
	end,

	-- Lists all environment variables to the game console.
	print_env = function()
		if game then
			for k, v in pairs(_G) do
				game.print(k .. " = " .. tostring(v))
			end
		end
	end,

	-- Returns a blueprint table of a given entity.
	get_blueprint_table = function(id)
		table_name = game.get_blueprint(id)
		return _G[table_name] -- _G contains all global variables.
	end,

	-- Returns true if a given value v has a type matching
	-- one of the type names passed as the remaining arguments.
	assert_types = function(v, ...)
		res = false

		for _, t in ipairs{...} do
			if type(t) == "string" and type(v) == t then
				res = true
			end
		end

		return res
	end
}

-- INIT:
game.utils = utils

-- HELPER FUNCTIONS:
if show_msg then
	game.show_msg = show_msg
elseif game.print then
	game.show_msg = game.print
end

game.quit = function()
	game.set_game_state(game.enum.game_state.ended)
end

game.pause = function()
	game.set_game_state(game.enum.game_state.paused)
end

game.unpause = function()
	game.set_game_state(game.enum.game_state.running)
end

game.gold.go_deposit = function(id, prio)
	prio = prio or false
	vault = game.gold.closest_free_gold_vault(id)
	game.print("Found free gold vault: " .. vault)

	if vault ~= game.const.no_ent then
		task = game.task.create(vault, game.enum.task.go_deposit_gold)
		if prio then
			game.gold.add_priority(id, task)
		else
			game.gold.add(id, task)
		end
	else
		-- TODO: Notify player that he has no free space in gold vaults.
	end
end

game.attack = function(id, target)
	task = game.task.create(target, game.enum.task.go_kill)
	game.task.add(id, task)
end
