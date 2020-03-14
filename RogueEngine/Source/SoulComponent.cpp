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
		ImGui::Text("Collected souls in level: ");
		ImGui::TextColored({ 0.23f, 0.75f, 0.92f, 1.0f }, "%d", PLAYER_STATUS.GetCollectedSoulsInLevel());
		ImGui::Text("Souls in level: "); 
		ImGui::TextColored({ 0.23f, 0.75f, 0.92f, 1.0f }, "%d", PLAYER_STATUS.GetTotalSoulsInLevel());
		ImGui::Text("Completion in level: ");
		ImGui::TextColored({ 0.23f, 0.75f, 0.92f, 1.0f }, "%.1f%%", PLAYER_STATUS.GetCollectedSoulsInLevel() / static_cast<float>(PLAYER_STATUS.GetTotalSoulsInLevel()) * 100.0f);
		ImGui::NewLine();

		ImGui::Text("Collected souls in game: ");
		ImGui::TextColored({ 1.0f, 0.47f, 0.65f, 1.0f }, "%d", PLAYER_STATUS.GetCollectedSoulsInGame());
		ImGui::Text("Souls in game:  ");
		ImGui::TextColored({ 1.0f, 0.47f, 0.65f, 1.0f }, "%d", PLAYER_STATUS.GetTotalSoulsInGame());
		ImGui::Text("Completion in game : ");
		ImGui::TextColored({ 1.0f, 0.47f, 0.65f, 1.0f }, "%.1f%%", PLAYER_STATUS.GetCollectedSoulsInGame() / static_cast<float>(PLAYER_STATUS.GetTotalSoulsInGame()) * 100.0f);
	}
}