/**
 * @file ScriptStorage.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-09
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <Coral/Array.hpp>
#include <Coral/ManagedObject.hpp>
#include <Coral/Type.hpp>

#include "Core/Buffer.hpp"

namespace SW
{

	enum class DataType
	{
		Byte,
		Short,
		UShort,
		Int,
		UInt,
		Long,
		ULong,

		Float,
		Double,

		Bool,

		Entity,
		Prefab,
	};

	inline u64 DataTypeSize(DataType type)
	{
		switch (type)
		{
		case DataType::Byte:
			return sizeof(u8);
		case DataType::Short:
			return sizeof(i16);
		case DataType::UShort:
			return sizeof(u16);
		case DataType::Int:
			return sizeof(i32);
		case DataType::UInt:
			return sizeof(u32);
		case DataType::Long:
			return sizeof(i64);
		case DataType::ULong:
			return sizeof(u64);
		case DataType::Float:
			return sizeof(f32);
		case DataType::Double:
			return sizeof(f64);
		case DataType::Bool:
			return sizeof(Coral::Bool32);
		case DataType::Entity:
			return sizeof(u64);
		case DataType::Prefab:
			return sizeof(u64);
		}

		return 0;
	}

	inline std::string DataTypeToString(DataType type)
	{
		switch (type)
		{
		case DataType::Byte:
			return "Byte";
		case DataType::Short:
			return "Short";
		case DataType::UShort:
			return "UShort";
		case DataType::Int:
			return "Int";
		case DataType::UInt:
			return "UInt";
		case DataType::Long:
			return "Long";
		case DataType::ULong:
			return "ULong";
		case DataType::Float:
			return "Float";
		case DataType::Double:
			return "Double";
		case DataType::Bool:
			return "Bool";
		case DataType::Entity:
			return "Entity";
		case DataType::Prefab:
			return "Prefab";
		}

		return "Unknown";
	}

	inline DataType DataTypeFromString(const std::string& type)
	{
		if (type == "Byte")
			return DataType::Byte;
		if (type == "Short")
			return DataType::Short;
		if (type == "UShort")
			return DataType::UShort;
		if (type == "Int")
			return DataType::Int;
		if (type == "UInt")
			return DataType::UInt;
		if (type == "Long")
			return DataType::Long;
		if (type == "ULong")
			return DataType::ULong;
		if (type == "Float")
			return DataType::Float;
		if (type == "Double")
			return DataType::Double;
		if (type == "Bool")
			return DataType::Bool;
		if (type == "Entity")
			return DataType::Entity;
		if (type == "Prefab")
			return DataType::Prefab;

		return DataType::Byte;
	}

	struct FieldMetadata;

	class FieldStorage
	{
	public:
		std::string_view GetName() const { return m_Name; }

		DataType GetType() const { return m_DataType; }

		bool IsArray() const { return m_Type->IsSZArray(); }

		u64 GetLength() const
		{
			ASSERT(m_Type->IsSZArray(), "Not an array!");

			/*if (m_Instance)
			{
			    auto arr = m_Instance->GetFieldValue<Coral::Array<T>>(m_Name);
			    u32 length = arr.Length();
			    Coral::Array<T>::Free(arr);
			    return length;
			}*/

			return m_ValueBuffer.Size / DataTypeSize(m_DataType);
		}

		template <typename T>
		T GetValue() const
		{
			return m_Instance ? m_Instance->GetFieldValue<T>(m_Name) : m_ValueBuffer.Read<T>();
		}

		template <typename T>
		T GetValue(u32 index) const
		{
			if (m_Instance)
			{
				auto arr = m_Instance->GetFieldValue<Coral::Array<T>>(m_Name);
				T value  = arr[index];
				Coral::Array<T>::Free(arr);
				return value;
			}

			return m_ValueBuffer.Read<T>(index * sizeof(T));
		}

		template <typename T>
		void SetValue(const T& value)
		{
			if (m_Instance)
			{
				m_Instance->SetFieldValue(m_Name, value);
			}
			else
			{
				m_ValueBuffer.Write(&value, sizeof(T));
			}
		}

		template <typename T>
		void SetValue(const T& value, u64 index)
		{
			ASSERT(m_Type->IsSZArray(), "Not an array!");

			if (m_Instance)
			{
				auto arr   = m_Instance->GetFieldValue<Coral::Array<T>>(m_Name);
				arr[index] = value;
				m_Instance->SetFieldValue(m_Name, arr);
				Coral::Array<T>::Free(arr);
			}
			else
			{
				u64 offset = index * sizeof(T);
				m_ValueBuffer.Write(&value, sizeof(T), offset);
			}
		}

		void Resize(u32 newLength)
		{
			u64 size = newLength * DataTypeSize(m_DataType);

			if (m_Instance)
			{
				/*auto arr = m_Instance->GetFieldValue<Coral::Array<T>>(m_Name);
				auto newArr = Coral::Array<T>::New(arr.Length() + 1);
				newArr.Assign(arr);
				Coral::Array<T>::Free(arr);
				m_Instance->SetFieldValue(m_Name, newArr);
				Coral::Array<T>::Free(newArr);*/
			}
			else
			{
				u64 copySize     = std::min<u64>(size, m_ValueBuffer.Size);
				Buffer oldBuffer = Buffer::Copy(m_ValueBuffer.Data, (u32)copySize);
				m_ValueBuffer.Allocate((u32)size);
				m_ValueBuffer.ZeroInitialize();
				memcpy(m_ValueBuffer.Data, oldBuffer.Data, copySize);
				oldBuffer.Release();
			}
		}

		void RemoveAt(u32 index)
		{
			u32 newLength = (u32)GetLength() - 1;

			Buffer oldBuffer = Buffer::Copy(m_ValueBuffer);
			m_ValueBuffer.Release();
			m_ValueBuffer.Allocate((u32)(newLength * DataTypeSize(m_DataType)));

			if (index != 0)
			{
				u64 indexOffset = index * DataTypeSize(m_DataType);
				memcpy(m_ValueBuffer.Data, oldBuffer.Data, indexOffset);
				memcpy(reinterpret_cast<std::byte*>(m_ValueBuffer.Data) + indexOffset,
				       reinterpret_cast<std::byte*>(oldBuffer.Data) + indexOffset + DataTypeSize(m_DataType),
				       (newLength - index) * DataTypeSize(m_DataType));
			}
			else
			{
				memcpy(m_ValueBuffer.Data, reinterpret_cast<std::byte*>(oldBuffer.Data) + DataTypeSize(m_DataType),
				       newLength * DataTypeSize(m_DataType));
			}

			oldBuffer.Release();
		}

	private:
		std::string m_Name;
		Coral::Type* m_Type;
		DataType m_DataType;
		Buffer m_ValueBuffer;

		Coral::ManagedObject* m_Instance;

		friend struct ScriptStorage;
		friend class ScriptingCore;
	};

	struct EntityScriptStorage
	{
		u64 ScriptID;
		std::unordered_map<u32, FieldStorage> Fields;
		Coral::ManagedObject* Instance = nullptr;
	};

	struct ScriptStorage
	{
		std::unordered_map<u64, EntityScriptStorage> EntityStorage;

		void InitializeEntityStorage(u64 scriptID, u64 entityID);
		void ShutdownEntityStorage(u64 scriptID, u64 entityID);

		void SynchronizeStorage();

		void CopyTo(ScriptStorage& other) const;
		void CopyEntityStorage(u64 entityID, u64 targetEntityID, ScriptStorage& targetStorage) const;
		void Clear();

	private:
		void InitializeFieldStorage(EntityScriptStorage& storage, u32 fieldID, const FieldMetadata& fieldMetadata);
	};

} // namespace SW
