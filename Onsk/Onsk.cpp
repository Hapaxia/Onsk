//////////////////////////////////////////////////////////////////////////////
//
// Onsk (https://github.com/Hapaxia/Onsk)
//
// Copyright(c) 2023 M.J.Silk
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
// M.J.Silk
// MJSilk2@gmail.com
//
//////////////////////////////////////////////////////////////////////////////

#include "Onsk.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>
#include <stdexcept>

Onsk::Onsk(const std::size_t maxNumberOfLayers)
	: m_maxNumberOfLayers{ maxNumberOfLayers }
	, m_numberOfLayers { 4u }
	, m_currentLayer{ 0u }
	, m_size{ 1u, 1u }
	, m_alphaScalingMode{ AlphaScalingMode::Log }
	, m_logBase{ 1.4 }
	, m_renderTextures(maxNumberOfLayers)
{
	resetLayers();
}

void Onsk::setNumberOfLayers(const std::size_t numberOfLayers)
{
	m_numberOfLayers = (numberOfLayers > m_maxNumberOfLayers) ? m_maxNumberOfLayers : numberOfLayers;
	resetLayers();
}

void Onsk::setSize(const sf::Vector2u size)
{
	m_size = size;
	resetLayers();
}

void Onsk::setAlphaScalingMode(const AlphaScalingMode alphaScalingMode)
{
	m_alphaScalingMode = alphaScalingMode;
}

void Onsk::setLogBase(const float logBase)
{
	m_logBase = static_cast<double>(logBase);
}

void Onsk::setContextSettings(const sf::ContextSettings& contextSettings)
{
	m_contextSettings = contextSettings;
	resetLayers();
}

sf::RenderTexture& Onsk::getCurrentLayer()
{
	return *(m_renderTextures[m_currentLayer]);
}

void Onsk::nextLayer()
{
	++m_currentLayer;
	if (m_currentLayer >= m_maxNumberOfLayers)
		m_currentLayer = 0u;
}








// PRIVATE

void Onsk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_renderTextures.empty())
		return;

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(m_renderTextures.front()->getSize()));
	const double maxLog{ std::pow(m_logBase, m_numberOfLayers) - 1.0 };
	for (std::size_t i{ 0u }; i < m_numberOfLayers; ++i)
	{
		double ratio{ 1.0 };
		switch (m_alphaScalingMode)
		{
		case AlphaScalingMode::Log:
		{
			const double log{ std::pow(m_logBase, i + 1u) - 1.0 };
			ratio = (log / maxLog);
		}
			break;
		case AlphaScalingMode::Linear:
			ratio = static_cast<double>(i + 1u) / m_numberOfLayers;
			break;
		case AlphaScalingMode::None:
		default:
			;
		}
		sf::Uint8 colorValue{ static_cast<sf::Uint8>(ratio * 255.0) };
		rectangle.setFillColor(sf::Color(255u, 255u, 255u, colorValue));

		rectangle.setTexture(&m_renderTextures[getLayerFromOffset(m_numberOfLayers - i - 1u)]->getTexture(), true);
		target.draw(rectangle);
	}
}

std::size_t Onsk::getLayerFromOffset(const std::size_t offset) const
{
	if (offset > m_currentLayer)
		return m_maxNumberOfLayers - (offset - m_currentLayer);
	else
		return m_currentLayer - offset;
}

void Onsk::resetLayers()
{
	m_currentLayer = 0u;
	if (m_renderTextures.empty())
		return;

	for (auto& renderTexture : m_renderTextures)
	{
		if (renderTexture == nullptr)
			renderTexture = std::make_unique<sf::RenderTexture>();
		if (!renderTexture->create(m_size.x, m_size.y, m_contextSettings))
			throw std::runtime_error("[ONSK] Unable to create render texture!");
		renderTexture->clear(sf::Color::Transparent);
	}
}
