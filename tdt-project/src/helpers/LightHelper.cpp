#include <Components.hpp>
#include <Cache.hpp>
#include <systems/EntitySystem.hpp>
#include "LightHelper.hpp"

#if CACHE_ALLOWED == 1
static tdt::cache::LightCache cache{Component::NO_ENTITY, nullptr};
#endif

void LightHelper::set_visible(EntitySystem& ents, tdt::uint id, bool val)
{
	LightComponent* comp{nullptr};
	GET_COMPONENT(id, ents, comp, LightComponent);
	if(comp && comp->light)
		comp->light->setVisible(val);
}

void LightHelper::toggle_visible(EntitySystem& ents, tdt::uint id)
{
	LightComponent* comp{nullptr};
	GET_COMPONENT(id, ents, comp, LightComponent);
	if(comp && comp->light)
		comp->light->setVisible(!comp->light->isVisible());
}

bool LightHelper::is_visible(EntitySystem& ents, tdt::uint id)
{
	LightComponent* comp{nullptr};
	GET_COMPONENT(id, ents, comp, LightComponent);
	if(comp && comp->light)
		return comp->light->isVisible();
	else
		return false;
}

void LightHelper::init(EntitySystem& ents, tdt::uint id)
{
	LightComponent* comp{nullptr};
	GET_COMPONENT(id, ents, comp, LightComponent);
	if(comp && !comp->light) // comp->light == true -> already initialized.
	{
		comp->light = ents.get_scene_manager().createLight("entity_" + std::to_string(id) + "_light");
		comp->light->setType(Ogre::Light::LT_POINT);
		comp->light->setVisible(true);
		comp->light->setPosition(comp->light->getPosition() + Ogre::Vector3{0.f, 45.f, 0.f});
		comp->light->setDiffuseColour(1.f, .5f, 0.f);
		comp->light->setSpecularColour(1.f, 1.f, 0.f);
		comp->light->setAttenuation(3250.f, 1.f, .0014f, .000007f);

		auto graph = ents.get_component<GraphicsComponent>(id);
		if(graph)
		{
			comp->node = graph->node;
			comp->node->attachObject(comp->light);
		}
	}
}
