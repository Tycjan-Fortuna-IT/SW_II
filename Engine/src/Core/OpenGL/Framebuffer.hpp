/**
 * @file Framebuffer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	/**
	 * @brief Structure containing specification of a framebuffer. 
	 */
	struct FramebufferSpecification final
	{
		u32 Width;		/**< Width of the framebuffer. */
		u32 Height; 	/**< Height of the framebuffer. */
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
		 * @return u32 Renderer ID of the framebuffer.
		 */
		u32 GetColorAttachmentRendererID() const { return m_ColorAttachment; }

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
		 * @brief Unbind the framebuffer. Meaning that all the following draw calls will be rendered to the default framebuffer.
		 */
		void Unbind() const;

		/**
		 * @brief Clear the framebuffer.
		 * @warning Must be called after binding the framebuffer.
		 */
		void Clear() const;

	private:
		/**
		 * @brief Invalidate the framebuffer.
		 * @warning Must be called internally after resizing the framebuffer.
		 */
		void Invalidate();

		u32 m_RendererID = 0;       /**< Renderer ID of the framebuffer. */
		u32 m_ColorAttachment = 0;  /**< Color attachment of the framebuffer. */
		u32 m_DepthAttachment = 0;  /**< Depth attachment of the framebuffer. */

		FramebufferSpecification m_Specification; /**< Specification of the framebuffer. */
	};

}
