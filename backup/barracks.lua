barracks = {
	components = {
		game.enum.component.physics,
		game.enum.component.graphics,
		game.enum.component.production,
		game.enum.component.structure,
		game.enum.component.faction,
		game.enum.component.price,
		game.enum.component.name
	},

	PhysicsComponent = {
		solid = false
	},

	GraphicsComponent = {
		mesh = "cube.mesh",
		material = "colour/blue",
		visible = true,
		manual_scaling = true,
		scale_x = 50,
		scale_y = 1,
		scale_z = 50,
		query_flags = game.enum.entity_type.building
	},

	ProductionComponent = {
		blueprint = "ogre_warrior",
		limit = 1,
		cooldown = 5
	},

	StructureComponent = {
		radius = 0,
		walk_through = true
	},

	FactionComponent = {
		faction = game.enum.faction.friendly
	},

	PriceComponent = {
		price = 1000
	},

	NameComponent = {
		name = "BARRACKS"
	}
}
if game then
	game.entity.register("barracks")
end