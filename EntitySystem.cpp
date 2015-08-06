#include "EntitySystem.hpp"

EntitySystem::EntitySystem(Ogre::SceneManager& mgr)
	: scene_{mgr}, entities_{}, physics_{}, health_{}, ai_{},
	  graphics_{}, movement_{}, combat_{}, event_{}, to_be_destroyed_{},
	  components_to_be_removed_{}, entity_register_{}
{ /* DUMMY BODY */ }

void EntitySystem::update(Ogre::Real)
{
	cleanup();
}

std::size_t EntitySystem::get_new_id() const
{
	std::size_t id{0};

	for(auto it = entities_.begin(); it != entities_.end(); ++it)
	{
		if(it->first != id) // First unused id.
			break;
		else
			++id;
	}

	return id;
}

void EntitySystem::cleanup()
{
	// Remove components.
	for(auto& ent : components_to_be_removed_)
	{
		entities_.find(ent.first)->second.set(ent.second, false);
		delete_component_now(ent.first, ent.second);
		if(!entities_.find(ent.first)->second.any()) // No components remaining -> destroy it.
			to_be_destroyed_.push_back(ent.first);
	}
	components_to_be_removed_.clear();

	// Remove entire entities.
	for(auto id : to_be_destroyed_)
	{
		auto entity = entities_.find(id);
		if(entity == entities_.end())
			continue;

		for(std::size_t i = 0; i < entity->second.size(); ++i)
		{
			if(!entity->second.test(i))
				continue;
			delete_component_now(id, i);
		}
		entities_.erase(id);
	}
	to_be_destroyed_.clear();
}

std::size_t EntitySystem::create_entity(std::string table_name)
{
	std::size_t id = get_new_id();
	entities_.emplace(std::make_pair(id, std::bitset<Component::count>{}));

	if(table_name == "") // Allows to create empty entities that are setup manually.
		return id;
	else
		register_entity(table_name);

	auto& bits = entities_.find(id)->second;

	lpp::Script& script = lpp::Script::get_singleton();
	std::vector<int> comps = script.get_vector<int>(table_name + ".components");

	for(auto x : comps)
	{
		bits.set(x); // Duplicate components will just overwrite, no need for error checking.
		switch(x)
		{
			case PhysicsComponent::type:
				load_component<PhysicsComponent>(id, table_name);
				break;
			case HealthComponent::type:
				load_component<HealthComponent>(id, table_name);
				break;
			case AIComponent::type:
				load_component<AIComponent>(id, table_name);
				break;
			case GraphicsComponent::type:
				load_component<GraphicsComponent>(id, table_name);
				break;
			case MovementComponent::type:
				load_component<MovementComponent>(id, table_name);
				break;
			case CombatComponent::type:
				load_component<CombatComponent>(id, table_name);
				break;
			case EventComponent::type:
				load_component<EventComponent>(id, table_name);
				break;
			case InputComponent::type:
				load_component<InputComponent>(id, table_name);
				break;
			case TimeComponent::type:
			case ManaComponent::type:
			case SpellComponent::type:
			case ProductionComponent::type:
				// TODO:
				break;
			case GridNodeComponent::type:
			case GridLineComponent::type:
				// Cannot be loaded automatically, will be handled by GridSystem.
				// TODO: More research on this.
				break;
			case PathfindingComponent::type:
				load_component<PathfindingComponent>(id, table_name);
				break;
			case TaskComponent::type:
				// Cannot be loaded automatically, will be handled by TaskSystem.
				break;
			case TaskHandlerComponent::type:
				load_component<TaskHandlerComponent>(id, table_name);
				break;
		}
	}

	return id;
}

void EntitySystem::destroy_entity(std::size_t id)
{
	auto ai_comp = ai_.find(id);
	if(ai_comp != ai_.end())
	{
		std::string blueprint = ai_comp->second.blueprint;
		lpp::Script::get_singleton().call<void, std::size_t>(blueprint + ".finnish", id); // Calls the "destructor".
	}

	to_be_destroyed_.push_back(id);
}

const std::map<std::size_t, std::bitset<Component::count>>& EntitySystem::get_component_list() const
{
	return entities_;
}

void EntitySystem::add_component(std::size_t ent_id, int comp_id)
{
	switch(comp_id)
	{
		case PhysicsComponent::type:
			add_component<PhysicsComponent>(ent_id);
			break;
		case HealthComponent::type:
			add_component<HealthComponent>(ent_id);
			break;
		case AIComponent::type:
			add_component<AIComponent>(ent_id);
			break;
		case GraphicsComponent::type:
			add_component<GraphicsComponent>(ent_id);
			break;
		case MovementComponent::type:
			add_component<MovementComponent>(ent_id);
			break;
		case CombatComponent::type:
			add_component<CombatComponent>(ent_id);
			break;
		case EventComponent::type:
			add_component<EventComponent>(ent_id);
			break;
		case InputComponent::type:
			add_component<InputComponent>(ent_id);
			break;
		case TimeComponent::type:
			add_component<TimeComponent>(ent_id);
			break;
		case ManaComponent::type:
			add_component<ManaComponent>(ent_id);
			break;
		case SpellComponent::type:
			add_component<SpellComponent>(ent_id);
			break;
		case ProductionComponent::type:
			add_component<ProductionComponent>(ent_id);
			break;
		case GridNodeComponent::type:
			add_component<GridNodeComponent>(ent_id);
			break;
		case GridLineComponent::type:
			add_component<GridLineComponent>(ent_id);
			break;
		case PathfindingComponent::type:
			add_component<PathfindingComponent>(ent_id);
			break;
		case TaskComponent::type:
			add_component<TaskComponent>(ent_id);
			break;
		case TaskHandlerComponent::type:
			add_component<TaskHandlerComponent>(ent_id);
			break;
	}
}

void EntitySystem::delete_component(std::size_t ent_id, int comp_id)
{
	switch(comp_id)
	{
		case PhysicsComponent::type:
			delete_component<PhysicsComponent>(ent_id);
			break;
		case HealthComponent::type:
			delete_component<HealthComponent>(ent_id);
			break;
		case AIComponent::type:
			delete_component<AIComponent>(ent_id);
			break;
		case GraphicsComponent::type:
			delete_component<GraphicsComponent>(ent_id);
			break;
		case MovementComponent::type:
			delete_component<MovementComponent>(ent_id);
			break;
		case CombatComponent::type:
			delete_component<CombatComponent>(ent_id);
			break;
		case EventComponent::type:
			delete_component<EventComponent>(ent_id);
			break;
		case InputComponent::type:
			delete_component<InputComponent>(ent_id);
			break;
		case TimeComponent::type:
			delete_component<TimeComponent>(ent_id);
			break;
		case ManaComponent::type:
			delete_component<ManaComponent>(ent_id);
			break;
		case SpellComponent::type:
			delete_component<SpellComponent>(ent_id);
			break;
		case ProductionComponent::type:
			delete_component<ProductionComponent>(ent_id);
			break;
		case GridNodeComponent::type:
			delete_component<GridNodeComponent>(ent_id);
			break;
		case GridLineComponent::type:
			delete_component<GridLineComponent>(ent_id);
			break;
		case PathfindingComponent::type:
			delete_component<PathfindingComponent>(ent_id);
			break;
		case TaskComponent::type:
			delete_component<TaskComponent>(ent_id);
			break;
		case TaskHandlerComponent::type:
			delete_component<TaskHandlerComponent>(ent_id);
			break;
	}
}

void EntitySystem::delete_component_now(std::size_t ent_id, int comp_id)
{
	switch(comp_id)
	{
		case PhysicsComponent::type:
			physics_.erase(ent_id);
			break;
		case HealthComponent::type:
			health_.erase(ent_id);
			break;
		case AIComponent::type:
			ai_.erase(ent_id);
			break;
		case GraphicsComponent::type:
		{
			auto& graph_comp = get_component<GraphicsComponent>(ent_id);
			if(graph_comp.node && graph_comp.entity)
			{
				graph_comp.node->detachObject(graph_comp.entity);
				if(graph_comp.node->numChildren() == 0)
					scene_.destroySceneNode(graph_comp.node);
				scene_.destroyEntity(graph_comp.entity);
			}
			graphics_.erase(ent_id);
			break;
		}
		case MovementComponent::type:
			movement_.erase(ent_id);
			break;
		case CombatComponent::type:
			combat_.erase(ent_id);
			break;
		case EventComponent::type:
			event_.erase(ent_id);
			break;
		case InputComponent::type:
		case TimeComponent::type:
		case ManaComponent::type:
		case SpellComponent::type:
		case ProductionComponent::type:
			break; // TODO: Finnish remaining components.
		case GridNodeComponent::type:
			grid_node_.erase(ent_id);
			break;
		case GridLineComponent::type:
			grid_line_.erase(ent_id);
			break;
		case PathfindingComponent::type:
			pathfinding_.erase(ent_id);
			break;
		case TaskComponent::type:
			task_.erase(ent_id);
			break;
		case TaskHandlerComponent::type:
			task_handler_.erase(ent_id);
			break;
	}

}

void EntitySystem::init_graphics_component(std::size_t id)
{
	auto& comp = graphics_.find(id)->second;
	comp.entity = scene_.createEntity(comp.mesh);
	comp.node = scene_.getRootSceneNode()->createChildSceneNode();
	comp.node->attachObject(comp.entity);
	comp.node->setVisible(comp.visible);

	auto half_height = comp.entity->getWorldBoundingBox(true).getHalfSize().y;
	if(has_component<PhysicsComponent>(id))
	{
		auto& phys_comp = get_component<PhysicsComponent>(id);
		phys_comp.half_height = half_height;
		phys_comp.position = Ogre::Vector3{phys_comp.position.x, half_height, phys_comp.position.y};
		comp.node->setPosition(phys_comp.position);
	}
}

void EntitySystem::register_entity(const std::string& table_name)
{
	entity_register_.emplace(table_name);
}

std::set<std::string>& EntitySystem::get_registered_entities()
{
	return entity_register_;
}
