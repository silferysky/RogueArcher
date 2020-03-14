#include "Precompiled.h"
#include "SoulComponent.h"

namespace Rogue
{
	std::string SoulComponent::Serialize()
	{
		return std::string();
	}

	void SoulComponent::Deserialize(std::string_view toDeserialize)
	{
	}

	void SoulComponent::DisplayOnInspector()
	{
		ImGui::TextColored({ 0.23f, 0.75f, 0.92f, 1.0f }, "Number of souls in level: %d", PLAYER_STATUS.GetTotalSoulsInLevel());
		ImGui::NewLine();
		ImGui::TextColored({ 0.23f, 0.75f, 0.92f, 1.0f }, "Number of collected souls in level: %d", PLAYER_STATUS.GetCollectedSoulsInLevel());
		ImGui::NewLine();
		ImGui::TextColored({ 0.23f, 0.75f, 0.92f, 1.0f }, "Completion in level: %.1f%%", PLAYER_STATUS.GetCollectedSoulsInLevel() / static_cast<float>(PLAYER_STATUS.GetTotalSoulsInLevel()) * 100.0f);
		ImGui::NewLine();

		ImGui::TextColored({ 0.93f, 0.07f, 0.37f, 1.0f }, "Number of souls in game: %d", PLAYER_STATUS.GetTotalSoulsInGame());
		ImGui::NewLine();
		ImGui::TextColored({ 0.93f, 0.07f, 0.37f, 1.0f }, "Number of collected souls in game: %d", PLAYER_STATUS.GetCollectedSoulsInGame());
		ImGui::NewLine();
		ImGui::TextColored({ 0.93f, 0.07f, 0.37f, 1.0f }, "Completion in game: %.1f%%", PLAYER_STATUS.GetCollectedSoulsInGame() / static_cast<float>(PLAYER_STATUS.GetTotalSoulsInGame()) * 100.0f);
	}
}