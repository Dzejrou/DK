-- Default task handler used by most friendly entities in the game.
default_task_handler = {
	-- Called from the C++ engine to handle a task, it returns true
	-- if the task has made the handling entity busy, false otherwise.
	handle_task = function(id, task)
		local task_type = game.task.get_type(task)
		local target = game.task.get_target(task)

		if not game.entity.exists(target) then
			return false
		end

		local res = false
		if task_type == game.enum.task.none or
		   not game.task.possible(id, task) then
			res = false
		elseif task_type == game.enum.task.go_to or
		       task_type == game.enum.task.go_near or
		       task_type == game.enum.task.get_in_range then
			res = game.path.pathfind(id, target)

			if task_type == game.enum.task.go_near then
				game.path.pop_last(id)
			end
	   	elseif task_type == game.enum.task.go_kill then
			local t1 = game.task.create(target, game.enum.task.get_in_range)
			local t2 = game.task.create(target, game.enum.task.kill)

			game.task.add_priority(id, t2)
			game.task.add_priority(id, t1)

			game.task.cancel(task)
			res = false
		elseif task_type == game.enum.task.kill then
			game.combat.set_target(id, target)
			res = true
		elseif task_type == game.enum.task.go_pick_up_gold then
			local t1 = game.task.create(target, game.enum.task.go_near)
			local t2 = game.task.create(target, game.enum.task.pick_up_gold)

			game.task.add_priority(id, t2)
			game.task.add_priority(id, t1)

			if game.gold.full(id) then
				game.gold.go_deposit(id, true)
			end

			game.task.cancel(task)
			res = false
		elseif task_type == game.enum.task.pick_up_gold then
			game.gold.transfer_all(target, id)
			if game.gold.get_current(target) > 0 then
				evt = game.entity.create("")
				game.entity.add_component(evt, game.enum.component.event)
				game.event.set_target(evt, target)
				game.event.set_handler(evt, id)
				game.event.set_type(evt, game.enum.event.gold_dropped)
			else
				game.entity.destroy(target)
			end

			if game.gold.full(id) then
				game.gold.go_deposit(id, true)
			end

			game.task.cancel(task)
			res = false
		elseif task_type == game.enum.task.go_deposit_gold then
			local t1 = game.task.create(target, game.enum.task.go_near)
			local t2 = game.task.create(target, game.enum.task.deposit_gold)

			game.task.add_priority(id, t2)
			game.task.add_priority(id, t1)

			game.task.cancel(task)
			res = false
		elseif task_type == game.enum.task.deposit_gold then
			game.gold.transfer_all(id, target)

			if game.gold.get_current(id) > 0 then
				game.gold.go_deposit(id, true)
			end

			game.task.cancel(task)
			res = false
	   	end

		return res
	end,

	-- Called from the C++ engine to check if a task has been
	-- completed. The TaskComponent has an auxiliary boolean field
	-- TaskComponent::complete that can be used for this.
	task_complete = function(id, task)
		local task_type = game.task.get_type(task)
		if task_type == game.enum.task.none or
		   not game.task.type_possible(id, task_type) or
		   not game.entity.exists(game.task.get_target(task)) then
			return true
		end

		local res = true

		if task_type == game.enum.task.go or
		   task_type == game.enum.task.go_near then
			res = game.path.empty(id)
		elseif task_type == game.enum.task.get_in_range then
			local target = game.task.get_target(task)
			if game.path.empty(id) then
				return true
			end
			local range = game.combat.get_range(id)
			range = range - (range / 10)
			range = range * range

			local dist = game.physics.get_distance(id, target)
			res = dist < range and game.combat.in_sight(id, target)
			if res then
				game.path.clear(id)
				game.animation.stop(id)
			end
		elseif task_type == game.enum.task.kill then
			res = game.combat.get_target(id) == game.const.no_ent
		end

		return res
	end
}
