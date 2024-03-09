/**
 * @file CSharpObject.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-08
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <Coral/ManagedObject.hpp>

namespace SW {

	/**
	 * @class CSharpObject
	 * @brief Represents a C# object that can be invoked from C++ code.
	 */
	class CSharpObject {
	public:
		/**
		 * @brief Invokes a method on the C# object.
		 * @tparam TArgs The types of the arguments passed to the method.
		 * @param methodName The name of the method to invoke.
		 * @param args The arguments to pass to the method.
		 */
		template<typename... TArgs>
		void Invoke(std::string_view methodName, TArgs&&... args)
		{
			if (!m_Handle)
				return;

			m_Handle->InvokeMethod(methodName, std::forward<TArgs>(args)...);
		}

		/**
		 * @brief Checks if the C# object is valid.
		 * @return True if the object is valid, false otherwise.
		 */
		bool IsValid() const { return m_Handle != nullptr; }

		/**
		 * @brief Gets the handle to the managed object.
		 * @return A pointer to the managed object.
		 */
		Coral::ManagedObject* GetHandle() const { return m_Handle; }

	private:
		Coral::ManagedObject* m_Handle = nullptr; /** @brief Handle to the managed object. */

		friend class ScriptingCore;
	};

}
