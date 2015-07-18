#include "MovementSystem.hpp"

MovementSystem::MovementSystem(EntitySystem& ents)
	: entities_{ents}
{ /* DUMMY BODY */ }

void MovementSystem::update(Ogre::Real delta)
{
	auto& ents = entities_.get_component_list();
	for(auto it = ents.begin(); it != ents.end(); ++it)
	{ // TODO: Account for delta.
		if(is_valid(it->first) && is_moving(it->first))
		{
			auto& phys_comp = entities_.get_component<PhysicsComponent>(it->first);
			auto& mov_comp = entities_.get_component<MovementComponent>(it->first);
			phys_comp.position += mov_comp.movement_vector * mov_comp.speed_modifier;

			if(entities_.has_component<GraphicsComponent>(it->first))
				entities_.get_component<GraphicsComponent>(it->first).node->setPosition(phys_comp.position);

			mov_comp.moving = false;
		}
	}
}

bool MovementSystem::is_valid(std::size_t id) const
{
	return entities_.has_component<PhysicsComponent>(id)
		   && entities_.has_component<MovementComponent>(id);
}

bool MovementSystem::is_moving(std::size_t id) const
{
	if(is_valid(id))
		return entities_.get_component<MovementComponent>(id).moving;
	else
		return false;
}

bool MovementSystem::is_solid(std::size_t id) const
{
	if(is_valid(id))
		return entities_.get_component<PhysicsComponent>(id).solid;
	else
		return false;
}

bool MovementSystem::can_move_to(std::size_t id, Ogre::Vector3 pos)
{
	if(!entities_.has_component<GraphicsComponent>(id))
		return true; // Invisible objects will be able to move anywhere.

	if(is_valid(id))
	{
		auto& phys_comp = entities_.get_component<PhysicsComponent>(id);
		auto& graph_comp = entities_.get_component<GraphicsComponent>(id);

		if(!phys_comp.solid)
			return true;

		graph_comp.node->setPosition(pos); // Old position backed up in phys_comp.position.
		auto& ents = entities_.get_component_container<PhysicsComponent>(); // Only need to check entities with PhysicsComponent.
		for(const auto& ent : ents)
		{
			if(id == ent.first)
				continue;

			if(is_valid(ent.first) && is_solid(ent.first) &&
			   collide(id, ent.first))
			{
				graph_comp.node->setPosition(phys_comp.position);
				return false;
			}
		}

		// Reverts the change, because of possible checks without actual movement.
		graph_comp.node->setPosition(phys_comp.position);
		return true;
	}
	else
		return false;
}

bool MovementSystem::move(std::size_t id, Ogre::Vector3 dir_vector)
{
	if(is_valid(id))
	{
		auto& phys_comp = entities_.get_component<PhysicsComponent>(id);
		auto& mov_comp = entities_.get_component<MovementComponent>(id);

		auto new_pos = phys_comp.position;
		auto dir = dir_vector * mov_comp.speed_modifier; 
		new_pos += dir;

		if(can_move_to(id, new_pos))
		{
			mov_comp.movement_vector = dir_vector;
			mov_comp.moving = true;

			if(entities_.has_component<GraphicsComponent>(id))
				entities_.get_component<GraphicsComponent>(id).node->setPosition(phys_comp.position);

			return true;
		}
	}

	return false;
}

void MovementSystem::move_to(std::size_t id, Ogre::Vector3 pos)
{
	if(is_valid(id))
	{
		auto& phys_comp = entities_.get_component<PhysicsComponent>(id);
		phys_comp.position = pos;

		if(entities_.has_component<GraphicsComponent>(id))
			entities_.get_component<GraphicsComponent>(id).node->setPosition(pos);
	}
}

void MovementSystem::rotate(std::size_t id, Ogre::Real delta)
{
	if(entities_.has_component<GraphicsComponent>(id))
		entities_.get_component<GraphicsComponent>(id).node->rotate(Ogre::Vector3{0, 1, 0}, Ogre::Radian{delta});
}

const Ogre::AxisAlignedBox& MovementSystem::get_bounds(std::size_t id) const
{
	if(entities_.has_component<GraphicsComponent>(id))
		return entities_.get_component<GraphicsComponent>(id).entity->getWorldBoundingBox();
	else
		throw std::runtime_error("[Error][MovementSystem] Trying to get bounding box of entity #"
								 + std::to_string(id) + " which does not have GraphicsComponent.");
}

bool MovementSystem::collide(std::size_t id1, std::size_t id2) const
{
	if(entities_.has_component<GraphicsComponent>(id1) &&
	   entities_.has_component<GraphicsComponent>(id2)) // Collision checking is done through Ogre.
		return get_bounds(id1).intersects(get_bounds(id2));
	else
		return false;
}

Ogre::Real MovementSystem::get_distance(std::size_t id1, std::size_t id2) const
{
	if(is_valid(id1) && is_valid(id2))
		return entities_.get_component<PhysicsComponent>(id1)
			.position.distance(entities_.get_component<PhysicsComponent>(id2).position);
	else
		return std::numeric_limits<Ogre::Real>::max();
}

Ogre::Vector3 MovementSystem::get_position(std::size_t id) const
{
	if(is_valid(id))
		return entities_.get_component<PhysicsComponent>(id).position;
	else
		return Ogre::Vector3{0, 0, 0};
}

Ogre::Real MovementSystem::get_speed_modifier(std::size_t id) const
{
	if(is_valid(id))
		return entities_.get_component<MovementComponent>(id).speed_modifier;
	else
		return Ogre::Real{};
}

void MovementSystem::set_speed_modifier(std::size_t id, Ogre::Real speed)
{
	if(is_valid(id))
		entities_.get_component<MovementComponent>(id).speed_modifier = speed;
}
