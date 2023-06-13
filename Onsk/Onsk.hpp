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

#ifndef ONSK_ONSK_HPP
#define ONSK_ONSK_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <vector>
#include <memory>

// Onsk v1.0.0
class Onsk : public sf::Drawable
{
public:
	enum class AlphaScalingMode
	{
		None,
		Linear,
		Log,
	};

	Onsk(std::size_t maxNumberOfLayers = 1u);
	void setNumberOfLayers(std::size_t numberOfLayers);
	void setSize(sf::Vector2u size);
	void setAlphaScalingMode(AlphaScalingMode alphaScalingMode);
	void setLogBase(float logBase);
	void setContextSettings(const sf::ContextSettings& contextSettings);

	std::size_t getMaxNumberOfLayers() const { return m_maxNumberOfLayers; }
	std::size_t getNumberOfLayers() const { return m_numberOfLayers; }
	sf::Vector2u getSize() const { return m_size; }
	AlphaScalingMode getAlphaScalingMode() const { return m_alphaScalingMode; }
	float getLogBase() const { return static_cast<float>(m_logBase); }
	sf::ContextSettings getContextSettings() const { return m_contextSettings; }



	Onsk& operator++() { nextLayer(); return *this; }
	Onsk& operator++(int) { nextLayer(); return *this; }
	void nextLayer();
	sf::RenderTexture& getCurrentLayer();







private:
	const std::size_t m_maxNumberOfLayers;
	std::size_t m_numberOfLayers;
	std::size_t m_currentLayer;
	sf::Vector2u m_size;
	AlphaScalingMode m_alphaScalingMode;
	double m_logBase;
	sf::ContextSettings m_contextSettings;

	std::vector<std::unique_ptr<sf::RenderTexture>> m_renderTextures;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::size_t getLayerFromOffset(const std::size_t offset) const;

	void resetLayers();
};

#endif // ONSK_ONSK_HPP
