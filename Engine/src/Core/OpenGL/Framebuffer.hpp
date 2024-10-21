/**
 * @file Framebuffer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-25
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	/**
	 * @brief Enum class containing all possible texture formats.
	 */
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,       // 8 bits per channel = 32 bits
		RED_INTEGER, // 32 bits

		// Depth/Stencil
		DEPTH24STENCIL8, // 24 bits depth, 8 bits stencil = 32 bits

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	/**
	 * @brief Structure containing specification of a texture.
	 */
	struct FramebufferTextureSpecification final
	{
		/**
		 * @brief Default onstructor
		 */
		FramebufferTextureSpecification() = default;

		/**
		 * @brief Construct a new Framebuffer Texture Specification object
		 *
		 * @param format Format of the texture.
		 */
		FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None; /**< Format of the texture. */
	};

	/**
	 * @brief Structure containing specification of a framebuffer texture.
	 */
	struct FramebufferAttachmentSpecification final
	{
		/**
		 * @brief Default constructor
		 */
		FramebufferAttachmentSpecification() = default;

		/**
		 * @brief Construct a new Framebuffer Attachment Specification object
		 *
		 * @param attachments List of texture specifications.
		 */
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		    : Attachments(attachments)
		{
		}

		std::vector<FramebufferTextureSpecification> Attachments; /**< List of texture specifications. */
	};

	/**
	 * @brief Structure containing specification of a framebuffer.
	 */
	struct FramebufferSpecification final
	{
		u32 Width   = 0; /**< Width of the framebuffer. */
		u32 Height  = 0; /**< Height of the framebuffer. */
		u32 Samples = 1;
		FramebufferAttachmentSpecification Attachments;
	};

	/**
	 * @brief The Framebuffer class is responsible for managing framebuffers.
	 */
	class Framebuffer final
	{
	public:
		/**
		 * @brief Construct a new Framebuffer object
		 *
		 * @param specification Specification of the framebuffer.
		 */
		Framebuffer(const FramebufferSpecification& specification);

		/**
		 * @brief Destroy the Framebuffer object
		 */
		~Framebuffer();

		/**
		 * @brief Get the Specification object
		 *
		 * @return const FramebufferSpecification& Current specification of the framebuffer.
		 */
		const FramebufferSpecification& GetSpecification() const { return m_Specification; }

		/**
		 * @brief Get the Renderer ID object
		 *
		 * @param index Index of the color attachment.
		 * @return u32 Renderer ID of the framebuffer.
		 */
		u32 GetColorAttachmentRendererID(u32 index = 0) const
		{
			ASSERT(index < m_ColorAttachments.size(), "Wrong attachement index!");
			return m_ColorAttachments[index];
		}

		/**
		 * @brief Clear the chosen color attachment with the chosen value.
		 *
		 * @param index Index of the color attachment.
		 * @param value Value to clear the attachment with.
		 */
		void ClearAttachment(u32 index, int value);

		/**
		 * @brief Get the Depth Attachment Renderer ID object
		 *
		 * @return u32 Depth attachment of the framebuffer.
		 */
		void Resize(u32 width, u32 height);

		/**
		 * @brief Bind the framebuffer. Meaning that all the following draw calls will be rendered to the framebuffer.
		 */
		void Bind() const;

		/**
		 * @brief Unbind the framebuffer. Meaning that all the following draw calls will be rendered to the default
		 * framebuffer.
		 */
		void Unbind() const;

		/**
		 * @brief Clear the framebuffer.
		 * @warning Must be called after binding the framebuffer.
		 */
		void Clear() const;

		/**
		 * Reads the pixel value at the specified coordinates from the framebuffer attachment.
		 *
		 * @param attachmentIndex The index of the attachment to read from.
		 * @param x The x-coordinate of the pixel.
		 * @param y The y-coordinate of the pixel.
		 * @return The pixel value at the specified coordinates.
		 */
		int ReadPixel(u32 attachmentIndex, int x, int y);

	private:
		/**
		 * @brief Invalidate the framebuffer.
		 * @warning Must be called internally after resizing the framebuffer.
		 */
		void Invalidate();

		u32 m_RendererID = 0;                     /**< Renderer ID of the framebuffer. */
		FramebufferSpecification m_Specification; /**< Specification of the framebuffer. */

		std::vector<FramebufferTextureSpecification>
		    m_ColorAttachmentSpecifications; /**< List of color attachment specifications. */
		FramebufferTextureSpecification m_DepthAttachmentSpecification =
		    FramebufferTextureFormat::None; /**< Depth attachment specification. */

		std::vector<u32> m_ColorAttachments; /**< List of color attachments. */
		u32 m_DepthAttachment = 0;           /**< Depth attachment. */
	};

} // namespace SW
