/**
 * @file LogCustomFormatters.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.2.0
 * @date 2024-05-03
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <glm/glm.hpp>

template<>
struct std::formatter<glm::vec2, char> {
	constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx)
	{
		std::format_parse_context::iterator it = ctx.begin(), end = ctx.end();

		if (it != end && *it != '}')
			throw std::format_error("invalid format");

		return it;
	}

	std::format_context::iterator format(glm::vec2 s, std::format_context& ctx) const
	{
		std::format_context::iterator it = ctx.out();

		*it++ = '(';
		it = std::formatter<float, char>().format(s.x, ctx);
		*it++ = ',';
		*it++ = ' ';
		it = std::formatter<float, char>().format(s.y, ctx);
		*it++ = ')';

		return it;
	}
};

template<>
struct std::formatter<glm::vec3, char> {
	constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx)
	{
		std::format_parse_context::iterator it = ctx.begin(), end = ctx.end();

		if (it != end && *it != '}')
			throw std::format_error("invalid format");

		return it;
	}

	std::format_context::iterator format(glm::vec3 s, std::format_context& ctx) const
	{
		std::format_context::iterator it = ctx.out();

		*it++ = '(';
		it = std::formatter<float, char>().format(s.x, ctx);
		*it++ = ',';
		*it++ = ' ';
		it = std::formatter<float, char>().format(s.y, ctx);
		*it++ = ',';
		*it++ = ' ';
		it = std::formatter<float, char>().format(s.z, ctx);
		*it++ = ')';

		return it;
	}
};

template<>
struct std::formatter<glm::vec4, char> {
	constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx)
	{
		std::format_parse_context::iterator it = ctx.begin(), end = ctx.end();

		if (it != end && *it != '}')
			throw std::format_error("invalid format");

		return it;
	}

	std::format_context::iterator format(glm::vec4 s, std::format_context& ctx) const
	{
		std::format_context::iterator it = ctx.out();

		*it++ = '(';
		it = std::formatter<float, char>().format(s.x, ctx);
		*it++ = ',';
		*it++ = ' ';
		it = std::formatter<float, char>().format(s.y, ctx);
		*it++ = ',';
		*it++ = ' ';
		it = std::formatter<float, char>().format(s.z, ctx);
		*it++ = ',';
		*it++ = ' ';
		it = std::formatter<float, char>().format(s.w, ctx);
		*it++ = ')';

		return it;
	}
};

template<>
struct std::formatter<std::filesystem::path, char> {
	constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx)
	{
		std::format_parse_context::iterator it = ctx.begin(), end = ctx.end();

		if (it != end && *it != '}')
			throw std::format_error("invalid format");

		return it;
	}

	std::format_context::iterator format(std::filesystem::path s, std::format_context& ctx) const
	{
		std::format_context::iterator it = ctx.out();

		std::string path = s.string();
		std::copy(path.begin(), path.end(), it);

		return it;
	}
};
