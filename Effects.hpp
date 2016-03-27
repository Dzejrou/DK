#pragma once

#include "Typedefs.hpp"
class EntitySystem;

/**
 * The util namespace contains functors used as conditions in searches and other
 * helper structures/functions used throughout the code.
 */
namespace util
{

/**
 * Contains effect functors that perform an action on
 * the entity they are called on.
 */
namespace effect
{
	/**
	 * Deals a random damage in a given range to the entity it's
	 * called on.
	 */
	struct DAMAGE_EFFECT
	{
		/**
		 * Constructor.
		 * Param: Entity system containing the entities this effect
		 *        will be used on.
		 * Param: Lower bound of the damage range.
		 * Param: Upper bound of the damage range.
		 */
		DAMAGE_EFFECT(EntitySystem&, tdt::uint, tdt::uint);

		/**
		 * Destructor.
		 */
		~DAMAGE_EFFECT() = default;

		/**
		 * Brief: Applies the damage effect to a given entity.
		 * Param: ID of the entity.
		 */
		void operator()(tdt::uint);

		private:
			/**
			 * The damage range.
			 */
			tdt::uint min_, max_;

			/**
			 * Entity system containing the entities this effect
			 * will be used on.
			 */
			EntitySystem& entities_;
	};

	/**
	 * Fully heals the entity it's called on.
	 */
	struct HEAL_EFFECT
	{
		/**
		 * Constructor.
		 * Param: Entity system containing the entities this
		 *        effect will be called on.
		 */
		HEAL_EFFECT(EntitySystem&);

		/**
		 * Destructor.
		 */
		~HEAL_EFFECT() = default;

		/**
		 * Brief: Fully heals a given entity.
		 * Param: ID of the entity.
		 */
		void operator()(tdt::uint);

		private:
			/**
			 * Entity system containing the entities this
			 * effect will be called on.
			 */
			EntitySystem& entities_;
	};

	/**
	 * Halves the speed of the entity it's called on
	 * for a given time period.
	 */
	struct LOWER_SPEED_EFFECT
	{
		/**
		 * Constructor.
		 * Param: Entity system containing the entities this
		 *        effect will be called on.
		 * Param: The time period before the speed is restored.
		 */
		LOWER_SPEED_EFFECT(EntitySystem&, tdt::real);

		/**
		 * Destructor.
		 */
		~LOWER_SPEED_EFFECT() = default;

		/**
		 * Brief: Halves the speed of a given entity.
		 * Param: ID of the entity.
		 */
		void operator()(tdt::uint);

		private:
			/**
			 * Entity system containing the entities this
			 * effect will be called on.
			 */
			EntitySystem& entities_;

			/**
			 * The time period that has to pass before the
			 * speed of the affected entities gets restored.
			 */
			tdt::real time_;
	};

	/**
	 * Freezes a given entity in place for a given time
	 * period. (This effect stops movement but not any
	 * other action.)
	 */
	struct FREEZE_EFFECT
	{
		/**
		 * Constructor.
		 * Param: Entity system containing the entities this
		 *        effect will be called on.
		 * Param: Duration of the freeze.
		 */
		FREEZE_EFFECT(EntitySystem&, tdt::real);

		/**
		 * Destructor.
		 */
		~FREEZE_EFFECT() = default;

		/**
		 * Brief: Freezes a given entity in place.
		 * Param: ID of the entity.
		 */
		void operator()(tdt::uint);

		private:
			/**
			 * Entity system containing the entities this
			 * effect will be called on.
			 */
			EntitySystem& entities_;

			/**
			 * The duration of the freeze.
			 */
			tdt::real time_;
	};
}
}