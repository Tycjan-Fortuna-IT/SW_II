/**
 * @file TypeInfo.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-10
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <type_traits>
#include <string>

#include "Core/Hash.hpp"

namespace SW {

	using TypeNameString = std::string_view;

	template<typename T, bool ExcludeNamespace>
	struct TypeInfoBase
	{
	protected:
		TypeNameString DemangleTypeName(std::string_view typeName) const
		{
			u64 spacePos = typeName.find(' ');

			if (spacePos != std::string_view::npos)
				typeName.remove_prefix(spacePos + 1);

			if constexpr (ExcludeNamespace)
			{
				u64 namespacePos = typeName.find("::");
				if (namespacePos != std::string_view::npos)
					typeName.remove_prefix(namespacePos + 2);
			}

			return typeName;
		}
	};

	template<typename T, bool ExcludeNamespace = false>
	struct TypeInfo : TypeInfoBase<T, ExcludeNamespace>
	{
	public:
		using Base = TypeInfoBase<T, ExcludeNamespace>;

		TypeInfo()
			: m_DemangledName(Base::DemangleTypeName(typeid(T).name()))
		{

		}

		TypeNameString Name() { return m_DemangledName; }

		const TypeNameString& Name() const { return m_DemangledName; }

		u32 HashCode() const { return Hash::GenerateFNVHash(m_DemangledName.data()); }

	private:
		TypeNameString m_DemangledName;
	};
}
