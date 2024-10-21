/**
 * @file Buffer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-09
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	struct Buffer
	{
		void* Data;
		u32 Size;

		Buffer() : Data(nullptr), Size(0) {}

		Buffer(const void* data, u32 size = 0) : Data((void*)data), Size(size) {}

		static Buffer Copy(const Buffer& other)
		{
			Buffer buffer;

			buffer.Allocate(other.Size);

			memcpy(buffer.Data, other.Data, other.Size);

			return buffer;
		}

		static Buffer Copy(const void* data, u32 size)
		{
			Buffer buffer;

			buffer.Allocate(size);

			memcpy(buffer.Data, data, size);

			return buffer;
		}

		void Allocate(u32 size)
		{
			delete[] (u8*)Data;

			Data = nullptr;

			if (size == 0)
				return;

			Data = new u8[size];

			Size = size;
		}

		void Release()
		{
			delete[] (u8*)Data;

			Data = nullptr;

			Size = 0;
		}

		void ZeroInitialize()
		{
			if (Data)
				memset(Data, 0, Size);
		}

		template <typename T>
		T& Read(u32 offset = 0)
		{
			return *(T*)((u8*)Data + offset);
		}

		template <typename T>
		const T& Read(u32 offset = 0) const
		{
			return *(T*)((u8*)Data + offset);
		}

		u8* ReadBytes(u32 size, u32 offset) const
		{
			ASSERT(offset + size <= Size, "Buffer overflow!");

			u8* buffer = new u8[size];

			memcpy(buffer, (u8*)Data + offset, size);

			return buffer;
		}

		void Write(const void* data, u32 size, u32 offset = 0)
		{
			ASSERT(offset + size <= Size, "Buffer overflow!");

			memcpy((u8*)Data + offset, data, size);
		}

		u8& operator[](int index) { return ((u8*)Data)[index]; }

		u8 operator[](int index) const { return ((u8*)Data)[index]; }

		template <typename T>
		T* As() const
		{
			return (T*)Data;
		}

		inline u32 GetSize() const { return Size; }
	};

} // namespace SW
