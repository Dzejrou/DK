#pragma once

#include <set>

#include "System.hpp"
#include "EntitySystem.hpp"
#include "GridSystem.hpp"

class TaskSystem : public System
{
	public:
		TaskSystem(EntitySystem&, GridSystem&);
		~TaskSystem() {}

		void update(Ogre::Real);
		void add_task(std::size_t, std::size_t);
		void cancel_task(std::size_t);
		std::size_t create_task(std::size_t, TASK_TYPE);
		std::deque<std::size_t> get_task_queue(std::size_t);
		bool task_possible(std::size_t, std::size_t) const;
		void clear_task_queue(std::size_t);
	private:
		void next_task_(TaskHandlerComponent&);
		void handle_task_(std::size_t, TaskComponent&);
		bool current_task_completed_(TaskHandlerComponent&);
		EntitySystem& entities_;
		GridSystem& grid_;
};