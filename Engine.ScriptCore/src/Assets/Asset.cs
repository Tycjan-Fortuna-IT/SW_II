using System;

namespace SW
{
	/// <summary>
	/// 	Represents an asset.
	/// 	Assets are objects that are used to store data that can be loaded and used at runtime.
	/// 	For example: textures, models, sounds, etc.
	/// 	Assets are created in the editor and then loaded at runtime.
	/// </summary>
	public partial class Asset<T> : IEquatable<T> where T : Asset<T>
	{
		/// <summary>
		/// 	The invalid asset handle.
		/// </summary>
		public static readonly ulong InvalidHandle = 0;

		/// <summary>
		/// 	The handle of the asset.
		/// </summary>
		public ulong Handle;

		internal Asset()
		{
			Handle = InvalidHandle;
		}

		internal Asset(ulong handle)
		{
			Handle = handle;
		}

		/// <summary>
		/// 	Check whether asset handle is still valid and asset is available.
		/// </summary>
		/// <returns>Whether asset is valid</returns>
		public bool IsValid()
		{
			unsafe { return InternalCalls.AssetHandle_IsValid(Handle); }
		}

		public override bool Equals(object? obj) => obj is T asset && Equals(asset);

		public bool Equals(T? other)
		{
			if (other is null)
				return false;

			if (ReferenceEquals(this, other))
				return true;

			return Handle == other.Handle;
		}

		public override int GetHashCode() => Handle.GetHashCode();

		public static bool IsValid(Asset<T>? asset) => asset != null && asset.IsValid();
		public static implicit operator bool(Asset<T>? asset) => IsValid(asset);
	}
}
