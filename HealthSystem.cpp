#include "HealthSystem.hpp"

HealthSystem::HealthSystem(EntitySystem& ent)
	: entities_{ent}, regen_timer_{0}, regen_period_{1000} // TODO: Experiment with regen period!
{ /* DUMMY BODY */ }

void HealthSystem::update(Ogre::Real)
{
	update_regen();
	for(auto& ent : entities_.get_component_container<HealthComponent>())
	{
		if(!entities_.get_component<HealthComponent>(ent.first).alive)
		{ // Entity died.
			entities_.destroy_entity(ent.first);
		}
		else if(regen_)
			add_health(ent.first, ent.second.regen);
	}
}

bool HealthSystem::is_valid(std::size_t id) const
{
	return entities_.has_component<HealthComponent>(id);
}

void HealthSystem::update_regen()
{
	if(regen_)
	{
		regen_ = false;
		return;
	}

	if(regen_timer_ > regen_period_)
	{
		regen_ = true;
		regen_timer_ = 0;
	}
	else
		++regen_timer_;
}

std::size_t HealthSystem::get_health(std::size_t id) const
{
	if(is_valid(id))
		return entities_.get_component<HealthComponent>(id).curr_hp;
	else
		return 0;
}

void HealthSystem::add_health(std::size_t id, std::size_t val)
{
	if(is_valid(id))
	{
		auto& comp = entities_.get_component<HealthComponent>(id);
		std::size_t curr = comp.curr_hp;
		std::size_t max = comp.max_hp;

		if(curr + val > max)
			val = max - curr;
		comp.curr_hp += val;
	}
}

void HealthSystem::sub_health(std::size_t id, std::size_t val, bool ignore_defense)
{
	if(is_valid(id))
	{
		auto& comp = entities_.get_component<HealthComponent>(id);

		if(!ignore_defense)
			val -= comp.defense;

		if(comp.curr_hp <= val)
		{
			comp.curr_hp = 0;
			comp.alive = false;
		}
		else
			comp.curr_hp -= val;

	}
}

void HealthSystem::heal(std::size_t id)
{
	if(is_valid(id))
	{
		auto& comp = entities_.get_component<HealthComponent>(id);
		comp.curr_hp = comp.max_hp;
	}
}

void HealthSystem::buff(std::size_t id, std::size_t val)
{
	if(is_valid(id))
	{
		auto& comp = entities_.get_component<HealthComponent>(id);
		comp.curr_hp += val;
		comp.max_hp += val;
	}
}

void HealthSystem::set_regen(std::size_t id, std::size_t regen)
{
	if(is_valid(id))
		entities_.get_component<HealthComponent>(id).regen = regen;
}

void HealthSystem::set_alive(std::size_t id, bool alive)
{
	if(is_valid(id))
		entities_.get_component<HealthComponent>(id).alive = alive;
}

std::size_t HealthSystem::get_defense(std::size_t id) const
{
	if(is_valid(id))
		return entities_.get_component<HealthComponent>(id).defense;
	else
		return 0;
}

void HealthSystem::add_defense(std::size_t id, std::size_t val)
{
	if(is_valid(id))
		entities_.get_component<HealthComponent>(id).defense += val;
}

void HealthSystem::sub_defense(std::size_t id, std::size_t val)
{
	if(is_valid(id))
	{
		auto& comp = entities_.get_component<HealthComponent>(id);
		if(comp.defense - val < 0)
			comp.defense = 0;
		else
			comp.defense -= val;
	}
}
