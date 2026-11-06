#include "UiText.h"
#include <string.h>

uint8_t uiLanguage = DEFAULT_UI_LANGUAGE;

struct UiTranslation
{
  const char *english;
  const char *korean;
};

// Constant strings and fonts stay in flash; no translation cache in DRAM.
static const UiTranslation translations[] = {
  { "Seoul Area", "수도권" },
  { "Chuncheon", "춘천" },
  { "Wonju", "원주" },
  { "Gangneung", "강릉" },
  { "Donghae+", "동해·삼척·태백" },
  { "Daejeon", "대전" },
  { "Cheongju", "청주" },
  { "Chungju", "충주" },
  { "Jeonju", "전주" },
  { "Namwon", "남원" },
  { "Gwangju", "광주" },
  { "Mokpo", "목포" },
  { "Yeosu+", "여수·순천·광양" },
  { "Daegu", "대구" },
  { "Andong", "안동" },
  { "Pohang", "포항" },
  { "Busan", "부산" },
  { "Ulsan", "울산" },
  { "Changwon", "창원" },
  { "Jinju+", "진주·거창" },
  { "Jeju", "제주" },
  { "Seogwipo", "서귀포" },
  { "Menu", "메뉴" },
  { "Band", "밴드" },
  { "Volume", "음량" },
  { "TuneMode", "조정 방식" },
  { "Step", "간격" },
  { "Seek", "자동 탐색" },
  { "Scan", "신호 스캔" },
  { "Memory", "방송 목록" },
  { "ETM Scan", "ETM 스캔" },
  { "Favorite", "즐겨찾기" },
  { "Settings", "설정" },
  { "---More---", "추가 설정" },
  { "Squelch", "잡음 차단" },
  { "Bandwidth", "대역폭" },
  { "SoftMute", "약신호 감쇠" },
  { "Mode", "수신 모드" },
  { "NTP Now", "시간 동기화" },
  { "Brightness", "밝기" },
  { "Calibration", "주파수 보정" },
  { "UTC Offset", "시차" },
  { "Date/Time", "날짜·시간" },
  { "FM Region", "FM 지역" },
  { "KR Area", "수신 지역" },
  { "FM Stereo", "FM 스테레오" },
  { "Theme", "색상 테마" },
  { "UI Layout", "화면 구성" },
  { "Zoom Menu", "메뉴 확대" },
  { "Scroll Dir.", "회전 방향" },
  { "Sleep", "절전 시간" },
  { "Sleep Mode", "절전 방식" },
  { "Load EiBi", "EiBi 읽기" },
  { "USB Port", "USB 연결" },
  { "TCP Port", "TCP 연결" },
  { "Bluetooth", "블루투스" },
  { "Wi-Fi", "와이파이" },
  { "About", "기기 정보" },
  { "Language", "언어" },
  { "Off", "끄기" },
  { "On", "켜기" },
  { "Auto", "자동" },
  { "Mono", "모노" },
  { "Stereo", "스테레오" },
  { "Default", "기본" },
  { "Locked", "잠금" },
  { "Unlocked", "조작 허용" },
  { "CPU Sleep", "CPU 절전" },
  { "AP Only", "AP 전용" },
  { "AP+Connect", "AP+접속" },
  { "Connect", "접속" },
  { "Sync Only", "시간만 동기화" },
  { "Unpair All", "페어링 해제" },
  { "Ad hoc", "원격 제어" },
  { "Add Current", "현재 저장" },
  { "Clear", "비우기" },
  { "Hold again", "다시 길게" },
  { "Clear all?", "전체 삭제?" },
  { "Delete?", "삭제할까요?" },
  { "ATS scan?", "ATS 실행?" },
  { "Searching...", "탐색 중…" },
  { "Scanning...", "스캔 중…" },
  { "Scanning band...", "밴드 스캔 중…" },
  { "Frequency added", "주파수 저장됨" },
  { "Already saved", "이미 저장됨" },
  { "List full", "목록이 가득 참" },
  { "Save failed", "저장 실패" },
  { "AM/FM only", "AM/FM 전용" },
  { "Scan cancelled", "스캔 취소됨" },
  { "ETM scan saved", "ETM 저장됨" },
  { "Not enough PSRAM", "PSRAM 부족" },
  { "Select KR first", "FM 지역을 KR로" },
  { "Max Attn", "최대 감쇠" },
  { "Max Gain", "최대 이득" },
  { "Muted", "음소거" },
  { "Update FW", "펌웨어 갱신" },
  { "Check", "확인" },
  { "Update", "갱신" },
  { "Memory %u", "방송 %u개" },
  { "Found %u", "발견 %u개" },
  { "ETM Found %u", "ETM %u개" },
};

const char *uiText(const char *text)
{
  if(uiLanguage == UI_KOREAN)
    for(const UiTranslation &entry : translations)
      if(strcmp(text, entry.english) == 0) return entry.korean;
  return text;
}

const lgfx::IFont *uiFont(const char *text, const lgfx::IFont *font)
{
  // Korean region/station names must also work with English menus.
  for(const unsigned char *p = (const unsigned char *)text; *p; ++p)
    if((*p & 0xF0) == 0xE0 && p[1] && p[2])
    {
      uint16_t code = ((*p & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F);
      if(code >= 0xAC00 && code <= 0xD7A3)
        return font == FONT_LARGE ? &lgfx::fonts::efontKR_24 : &lgfx::fonts::efontKR_14;
    }
  return font;
}

int32_t uiTextWidth(const char *text, const lgfx::IFont *font)
{
  text = uiText(text);
  return spr.textWidth(text, uiFont(text, font));
}

size_t drawUiString(const char *text, int32_t x, int32_t y,
                   const lgfx::IFont *font, int32_t maxWidth)
{
  text = uiText(text);
  if(!font) font = spr.getFont();
  font = uiFont(text, font);
  float sizeX = spr.getTextSizeX(), sizeY = spr.getTextSizeY();
  int32_t width = spr.textWidth(text, font);
  if((font == &lgfx::fonts::efontKR_14 || font == &lgfx::fonts::efontKR_24) &&
     maxWidth > 0 && width > maxWidth)
    spr.setTextSize(sizeX * maxWidth / width, sizeY);
  size_t result = spr.drawString(text, x, y, font);
  spr.setTextSize(sizeX, sizeY);
  return result;
}
