---
name: brand-guidelines
description: Applies Anthropic's official brand colors and typography to any sort of artifact that may benefit from having Anthropic's look-and-feel. Use it when brand colors or style guidelines, visual formatting, or company design standards apply.
license: Complete terms in LICENSE.txt
triggers:
  - Anthropic brand
  - brand color
  - brand guidelines
  - brand style
  - Anthropic design
  - corporate identity
  - brand typography
---

# Anthropic Brand Styling

## Overview

Anthropic's official brand identity and style resources for consistent application across all artifacts — web, presentation, and print.

## Brand Guidelines

### Colors

**Main Colors:**

| Name | HEX | Usage |
|------|-----|-------|
| Dark | `#141413` | Primary text, dark backgrounds |
| Light | `#faf9f5` | Light backgrounds, text on dark |
| Mid Gray | `#b0aea5` | Secondary elements |
| Light Gray | `#e8e6dc` | Subtle backgrounds, borders |

**Accent Colors:**

| Name | HEX | Usage |
|------|-----|-------|
| Orange | `#d97757` | Primary accent, CTAs |
| Blue | `#6a9bcc` | Secondary accent, links |
| Green | `#788c5d` | Tertiary accent, success states |

### Typography

| Context | Font | Fallback |
|---------|------|----------|
| Headings | Poppins | Arial, sans-serif |
| Body Text | Lora | Georgia, serif |

---

## Application Patterns (3 Code Examples)

### Pattern 1: CSS Custom Properties

Apply brand tokens via CSS variables for web projects.

```css
:root {
  /* Brand Colors */
  --brand-bg: #faf9f5;
  --brand-text: #141413;
  --brand-gray: #b0aea5;
  --brand-gray-light: #e8e6dc;
  --brand-accent: #d97757;
  --brand-accent-blue: #6a9bcc;
  --brand-accent-green: #788c5d;

  /* Typography */
  --font-heading: 'Poppins', Arial, sans-serif;
  --font-body: 'Lora', Georgia, serif;
}

body {
  background-color: var(--brand-bg);
  color: var(--brand-text);
  font-family: var(--font-body);
  line-height: 1.6;
}

h1, h2, h3, h4, h5, h6 {
  font-family: var(--font-heading);
  color: var(--brand-text);
}

.brand-cta {
  background-color: var(--brand-accent);
  color: var(--brand-bg);
  border: none;
  padding: 0.75rem 1.5rem;
  font-family: var(--font-heading);
  font-weight: 600;
  border-radius: 4px;
  cursor: pointer;
  transition: opacity 0.2s ease;
}
.brand-cta:hover {
  opacity: 0.9;
}

.brand-link {
  color: var(--brand-accent-blue);
  text-decoration: underline;
  text-underline-offset: 2px;
}
```

### Pattern 2: Python python-pptx Integration

Apply brand colors and fonts in PowerPoint generation.

```python
from pptx.util import Pt, Inches, Emu
from pptx.dml.color import RGBColor

# Brand color constants
BRAND_COLORS = {
    "bg": RGBColor(0xFA, 0xF9, 0xF5),
    "text": RGBColor(0x14, 0x14, 0x13),
    "gray": RGBColor(0xB0, 0xAE, 0xA5),
    "gray_light": RGBColor(0xE8, 0xE6, 0xDC),
    "accent": RGBColor(0xD9, 0x77, 0x57),
    "accent_blue": RGBColor(0x6A, 0x9B, 0xCC),
    "accent_green": RGBColor(0x78, 0x8C, 0x5D),
}

def apply_brand_heading(run, size=24):
    """Apply Poppins heading style to a text run."""
    run.font.name = "Poppins"
    run.font.size = Pt(size)
    run.font.color.rgb = BRAND_COLORS["text"]
    run.font.bold = True

def apply_brand_body(run, size=12):
    """Apply Lora body style to a text run."""
    run.font.name = "Lora"
    run.font.size = Pt(size)
    run.font.color.rgb = BRAND_COLORS["text"]

def set_slide_background(slide, color_key="bg"):
    """Set slide background to brand color."""
    background = slide.background
    fill = background.fill
    fill.solid()
    fill.fore_color.rgb = BRAND_COLORS[color_key]
```

### Pattern 3: Tailwind CSS Configuration

Integrate brand tokens into Tailwind for utility-class usage.

```js
// tailwind.config.js
module.exports = {
  theme: {
    extend: {
      colors: {
        brand: {
          bg: '#faf9f5',
          text: '#141413',
          gray: '#b0aea5',
          'gray-light': '#e8e6dc',
          accent: '#d97757',
          'accent-blue': '#6a9bcc',
          'accent-green': '#788c5d',
        }
      },
      fontFamily: {
        heading: ['"Poppins"', 'Arial', 'sans-serif'],
        body: ['"Lora"', 'Georgia', 'serif'],
      }
    }
  }
}
```

**Usage in JSX:**

```tsx
<div className="bg-brand-bg text-brand-text font-body">
  <h1 className="font-heading text-3xl font-bold">Brand Heading</h1>
  <p className="text-base leading-relaxed">Body text in Lora.</p>
  <button className="bg-brand-accent text-brand-bg px-6 py-3 font-heading font-semibold rounded">
    Call to Action
  </button>
  <a href="#" className="text-brand-accent-blue underline underline-offset-2">
    Brand Link
  </a>
</div>
```

---

## Anti-Patterns

### 1. Brand Color Arbitrary Variation

Using "close enough" colors instead of exact brand HEX values.

- **BAD:** `#E87B35` or `#FF8C00` instead of `#d97757` (similar but not brand)
- **GOOD:** Always reference CSS variables or constants: `var(--brand-accent)` / `BRAND_COLORS["accent"]`

### 2. Logo Readability Ignored

Placing logo on backgrounds without sufficient contrast.

- **BAD:** Light logo on `#faf9f5` background (near-zero contrast)
- **GOOD:** Dark logo on light backgrounds, light/inverted logo on dark backgrounds

### 3. Non-brand Font Mixing

Mixing brand fonts with incompatible typefaces in the same artifact.

- **BAD:** Poppins headings + Arial body text in the same document
- **GOOD:** Poppins headings + Lora body (official pairing), or system font fallback only

### 4. Hardcoded Color Values

Repeating HEX values inline instead of using variables/constants.

- **BAD:** `style="color: #141413; background: #faf9f5"` scattered throughout
- **GOOD:** CSS variables (`var(--brand-text)`), Python constants (`BRAND_COLORS["text"]`), Tailwind classes (`text-brand-text`)

### 5. Accent Color Overuse

Using accent orange on every element, reducing its impact.

- **BAD:** Orange headings + orange buttons + orange borders + orange backgrounds
- **GOOD:** Orange reserved for primary CTAs and key highlights only. Use Dark/Gray for structure.

---

## Quick Reference

| Element | Font | Color | Size |
|---------|------|-------|------|
| Page heading | Poppins Bold | `#141413` | 24-32pt |
| Section heading | Poppins SemiBold | `#141413` | 18-24pt |
| Body text | Lora Regular | `#141413` | 12-16pt |
| Secondary text | Lora Regular | `#b0aea5` | 11-14pt |
| CTA button | Poppins SemiBold | `#faf9f5` on `#d97757` | 14-16pt |
| Link | Lora Regular | `#6a9bcc` | inherit |
| Background | — | `#faf9f5` | — |
| Dark background | — | `#141413` | — |
