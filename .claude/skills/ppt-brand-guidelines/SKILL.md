---
name: ppt-brand-guidelines
description: PPT brand guidelines for VRL presentations. Use when creating slides, decks, or any presentation materials. Provides color palette, logo usage, and minimal design principles.
triggers:
  - VRL
  - VRL presentation
  - VRL brand
  - VRL slides
  - VRL deck
  - VRL design
---

# VRL PPT Brand Guidelines

Modern, clean brand guidelines for VRL presentations featuring the lime green logo.

## Brand Identity

**Logo**: Lime green rounded square with dark lightning bolt icon
**Style**: Modern, tech-forward, bold yet clean

## Brand Colors

### Primary Palette

| Color | Hex | Role |
|-------|-----|------|
| **Lime Green** | `#BDFF00` | Brand color (from logo) |
| **Dark Navy** | `#1E293B` | Primary dark, backgrounds, text |
| **White** | `#FFFFFF` | Light backgrounds, text on dark |

### Supporting Colors

| Color | Hex | Role |
|-------|-----|------|
| **Slate Gray** | `#64748B` | Secondary text, muted elements |
| **Light Slate** | `#94A3B8` | Tertiary text, captions |
| **Off White** | `#F8FAFC` | Card backgrounds on white |

## Logo Usage

### CRITICAL RULES
- **NEVER apply CSS filters** to the logo (no brightness, invert, etc.)
- Logo is self-contained with lime green background - use AS-IS
- Logo works on both dark and light backgrounds without modification

### Placement
- **Title slide**: Centered, 70-80pt, above title
- **Closing slide**: Centered, 80pt
- **Content slides**: Optional, bottom-right, 24pt

## Typography

### Font
- **Primary**: Arial, Helvetica, sans-serif
- **Weights**: Bold for headers, Regular for body

### Scale
| Element | Size | Weight | Color |
|---------|------|--------|-------|
| Hero Title | 42-48pt | Bold | White |
| Section Title | 24-28pt | Bold | Dark Navy |
| Subtitle | 14-16pt | Regular | Light Slate |
| Body | 11-13pt | Regular | Dark Navy or Slate |
| Caption | 9-10pt | Regular | Slate Gray |

## Slide Layouts

### 1. Title Slide (Hero)
**Background**: Dark Navy (#1E293B)

```
┌─────────────────────────────────────┐
│                                     │
│         [LOGO - as-is]              │
│                                     │
│     MAIN TITLE (white, 42pt)        │
│                                     │
│   subtitle (light slate, 14pt)      │
│                                     │
│         date (slate, 10pt)          │
└─────────────────────────────────────┘
```

**IMPORTANT**:
- NO CSS filters on logo
- NO accent lines between title elements
- Simple, clean vertical stack
- Generous spacing between elements

### 2. Content Slide
**Background**: White (#FFFFFF)

```
┌─────────────────────────────────────┐
│ HEADER (dark navy, 24pt, bold)      │
│                                     │
│  Content with clean layout          │
│  Cards use #F8FAFC background       │
│                                     │
└─────────────────────────────────────┘
```

- Header: Dark Navy, bold, left-aligned
- NO underlines or accent lines under headers
- Cards: Off White (#F8FAFC) with 8pt radius

### 3. Data/Stats Slide
- Large numbers: Lime Green (#BDFF00) or Dark Navy
- Labels: Slate Gray (#64748B)
- Clean grid layout

### 4. Table Slide
- Header: Dark Navy background, white text
- Body: Alternating white/#F8FAFC rows
- Highlight: Lime Green background for emphasis column

### 5. Closing Slide
**Background**: Dark Navy (#1E293B)
- Logo centered (as-is, no filters)
- Tagline in white
- Summary stats in Lime Green

## Design Rules

### DO
- Use logo AS-IS without any CSS filters
- Dark Navy (#1E293B) for hero/closing slides
- White for content slides
- Lime Green for key stats and highlights only
- Clean, simple layouts
- Generous whitespace (40%+)

### DON'T
- Apply brightness/invert filters to logo
- Use accent lines that overlap text
- Add decorative elements
- Use circular badges or icons
- Mix too many colors
- Add borders or frames

## CSS Guidelines

### Logo in HTML (Title/Closing slides)
```css
.logo {
  width: 70pt;
  height: 70pt;
  /* NO FILTER - use logo as-is */
}
.logo img {
  width: 100%;
  height: 100%;
  object-fit: contain;
}
```

### Title Slide Body
```css
body {
  background: #1E293B;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}
```

### Card Styling
```css
.card {
  background: #F8FAFC;
  padding: 18pt;
  border-radius: 8pt;
}
```

### Highlighted Card
```css
.card.highlight {
  background: #BDFF00;
  color: #1E293B;
}
```

## Example Slide Specs

### Title Slide
- Background: #1E293B
- Logo: 70pt, centered, NO FILTER
- Title: white, 42pt, bold
- Subtitle: #94A3B8, 14pt
- Date: #64748B, 10pt
- Spacing: 25pt between elements

### Content Slide
- Background: #FFFFFF
- Header: #1E293B, 24pt, bold
- Cards: #F8FAFC, 8pt radius, 18pt padding
- Body text: #1E293B, 11pt

## 안티패턴

### ❌ 로고에 CSS 필터 적용

로고는 라임 그린 배경이 내장되어 있어 필터 적용 시 색상이 왜곡된다.

```css
/* BAD — 로고 색상 파괴 */
.logo img { filter: brightness(0) invert(1); }
.logo img { filter: grayscale(1); }

/* GOOD — 필터 없이 그대로 사용 */
.logo img { width: 100%; height: 100%; object-fit: contain; /* NO FILTER */ }
```

### ❌ 브랜드 외 색상 사용

VRL 3색(Lime Green `#BDFF00` / Dark Navy `#1E293B` / White `#FFFFFF`) 외 임의 색상을 사용하면 브랜드 일관성이 깨진다.

```
BAD: 파란색 헤더, 빨간색 강조, 회색 배경 — 브랜드 색상 미준수
GOOD: 강조 → #BDFF00, 배경 → #1E293B 또는 #FFFFFF, 보조 → #64748B / #94A3B8
```

### ❌ 과도한 텍스트 밀도

슬라이드 당 한 가지 메시지 원칙을 어기고 긴 문장을 가득 채우는 것은 VRL 미니멀 스타일 위반이다.

```
BAD: 슬라이드 1장에 10개 이상 불릿 포인트, 9pt 이하 소형 폰트
GOOD: 핵심 메시지 3개 이하, Body 최소 11pt, 여백 40% 이상 유지
```

### ❌ 타이틀/클로징 슬라이드에 흰 배경 사용

Hero/Closing 슬라이드는 반드시 Dark Navy 배경이어야 한다. 흰 배경은 브랜드 임팩트를 잃는다.

```
BAD: Title 슬라이드 background: #FFFFFF
GOOD: Title/Closing 슬라이드 background: #1E293B (Dark Navy)
     Content 슬라이드 background: #FFFFFF
```

### ❌ 헤더 아래 장식선 / 뱃지 추가

언더라인, 구분선, 원형 아이콘 뱃지는 VRL 클린 디자인에 반한다.

```
BAD: 헤더 하단 accent line, 섹션 제목 underline, 아이콘 circular badge
GOOD: 계층은 폰트 크기·굵기·색상(Navy vs Slate)으로만 표현
```

### ❌ Lime Green을 배경 전체에 사용

Lime Green(`#BDFF00`)은 통계 수치, 하이라이트 카드 등 핵심 강조 요소에만 제한적으로 사용한다. 배경 전체 적용 시 가독성 저하.

```
BAD: 슬라이드 배경 전체를 #BDFF00으로 채움
GOOD: 핵심 통계 숫자, highlight card(.card.highlight), summary stats에만 사용
```

---

## Assets

Logo file: `assets/logo.png` (lime green with lightning bolt - use as-is)
