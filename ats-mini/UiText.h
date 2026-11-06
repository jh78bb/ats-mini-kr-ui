#ifndef UI_TEXT_H
#define UI_TEXT_H

#include "Common.h"

enum UiLanguage : uint8_t { UI_ENGLISH, UI_KOREAN };
static constexpr UiLanguage DEFAULT_UI_LANGUAGE = UI_KOREAN;
extern uint8_t uiLanguage;

const char *uiText(const char *text);
const lgfx::IFont *uiFont(const char *text, const lgfx::IFont *font);
int32_t uiTextWidth(const char *text, const lgfx::IFont *font);
size_t drawUiString(const char *text, int32_t x, int32_t y,
                   const lgfx::IFont *font = nullptr, int32_t maxWidth = 0);

#endif
