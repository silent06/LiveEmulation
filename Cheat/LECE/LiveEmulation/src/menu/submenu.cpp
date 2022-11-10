#include "stdafx.h"

void Submenu::MenuUpdate() {
	Update();
	GetRenderer()->DrawTitle(m_name);
	GetSubmenuHandler()->m_currentOptionsWithoutBreaks = 0;
	GetSubmenuHandler()->m_totalOptionsWithoutBreaks = 0;
	GetSubmenuHandler()->m_totalOptions = 0;

	vector<shared_ptr<Option>> tempOptions;
	tempOptions.reserve(m_options.size());

	if (m_options.size() > 0) {
		int relativeOptionPosition = 0, visibilityOffset = 0;
		for (size_t i = 0; i < m_options.size(); i++) {
			if (m_options.at(i)->IsVisible()) {
				tempOptions.push_back(m_options.at(i));
				visibilityOffset++;
				GetSubmenuHandler()->m_totalOptions++;
				if (visibilityOffset <= GetMenu()->m_currentOption) GetSubmenuHandler()->m_currentOptionsWithoutBreaks++;
				GetSubmenuHandler()->m_totalOptionsWithoutBreaks++;
			}
		}

		for (int i = GetMenu()->m_scrollOffset; i < GetMenu()->m_scrollOffset + GetMenu()->m_maxOptions; i++) {
			if (i >= 0 && i < tempOptions.size()) {
				if (relativeOptionPosition >= GetMenu()->m_maxOptions) break;
				tempOptions.at(i).get()->Render(relativeOptionPosition);
				if (GetMenu()->IsOpen()) tempOptions.at(i).get()->Input(relativeOptionPosition);
				if (relativeOptionPosition == GetMenu()->m_currentOption - GetMenu()->m_scrollOffset) {
					Option* option = tempOptions.at(i).get();
					GetRenderer()->SetToolip(option->GetTooltip());
					if (GetMenu()->IsOpen()) option->RenderSelected(relativeOptionPosition);
				}
				relativeOptionPosition++;
			}
		}
	}
}

void Submenu::Init() {}
void Submenu::InitVars() {}
void Submenu::UpdateOnce() {}
void Submenu::Update() {}
void Submenu::FeatureUpdate() {}