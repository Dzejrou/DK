gold_deposit = {
	components = {
		game.enum.component.physics,
		game.enum.component.health,
		game.enum.component.graphics,
		game.enum.component.structure,
		game.enum.component.destructor,
		game.enum.component.gold,
		game.enum.component.faction
	},

	PhysicsComponent = {
		solid = true
	},

	HealthComponent = {
		max_hp = 2000,
		regen = 0,
		defense = 13
	},

	GraphicsComponent = {
		mesh = "cube.mesh",
		material = "colour/gold",
		visible = true,
		manual_scaling = true,
		scale_x = 50,
		scale_y = 50,
		scale_z = 50,
		query_flags = game.enum.entity_type.wall
	},

	StructureComponent = {
		radius = 0
	},

	DestructorComponent = {
		blueprint = "drop_gold_destructor"
	},

	GoldComponent = {
		curr = 1500,
		max = 1500
	},

	FactionComponent = {
		faction = game.enum.faction.neutral
	}
}

if game then
	game.entity.register("gold_deposit")
end
