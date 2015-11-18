-- Creates a pile of gold after an entity is killed, contains
-- all of the entity's gold. Also creates an event announcing
-- the appearance of a new gold pile on the ground so that minions
-- in range can pick it up.
drop_gold_destructor = {
	dtor = function(id, killer)
		-- TODO: Add experience to the killer?
		local pile = game.entity.create("gold_pile")
		game.gold.set_current(pile, game.gold.get_current(id))
		game.gold.set_max(pile, game.gold.get_max(id))
		local x1, y1, z1 = game.physics.get_position(pile)
		local x2, y2, z2 = game.physics.get_position(id)
		game.movement.move_to(pile, x2, y1, z2)

		local evt = game.entity.create("")
		game.entity.add_component(evt, game.enum.component.event)
		game.event.set_target(evt, pile)
		game.event.set_type(evt, game.enum.event.gold_dropped)
		game.event.set_radius(evt, 300.0)

		if killer and killer ~= game.const.no_ent then
			game.event_handling.set_handler(evt, killer)
		end
	end
}
