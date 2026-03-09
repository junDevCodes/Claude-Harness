---
name: frontend-design
description: Create distinctive, production-grade frontend interfaces with high design quality. Use this skill when the user asks to build web components, pages, artifacts, posters, or applications (examples include websites, landing pages, dashboards, React components, HTML/CSS layouts, or when styling/beautifying any web UI). Generates creative, polished code and UI design that avoids generic AI aesthetics.
license: Complete terms in LICENSE.txt
triggers:
  - UI design
  - component design
  - landing page
  - dashboard design
  - frontend design
  - beautiful UI
  - styled component
  - responsive design
---

# Frontend Design Guidelines

Create distinctive, production-grade frontend interfaces that avoid generic "AI slop" aesthetics. Implement real working code with exceptional attention to aesthetic details and creative choices.

## Design Philosophy

The user provides frontend requirements: a component, page, application, or interface to build. They may include context about the purpose, audience, or technical constraints.

### Design Thinking

Before coding, understand the context and commit to a BOLD aesthetic direction:
- **Purpose**: What problem does this interface solve? Who uses it?
- **Tone**: Pick an extreme: brutally minimal, maximalist chaos, retro-futuristic, organic/natural, luxury/refined, playful/toy-like, editorial/magazine, brutalist/raw, art deco/geometric, soft/pastel, industrial/utilitarian, etc. There are so many flavors to choose from. Use these for inspiration but design one that is true to the aesthetic direction.
- **Constraints**: Technical requirements (framework, performance, accessibility).
- **Differentiation**: What makes this UNFORGETTABLE? What's the one thing someone will remember?

**CRITICAL**: Choose a clear conceptual direction and execute it with precision. Bold maximalism and refined minimalism both work - the key is intentionality, not intensity.

Then implement working code (HTML/CSS/JS, React, Vue, etc.) that is:
- Production-grade and functional
- Visually striking and memorable
- Cohesive with a clear aesthetic point-of-view
- Meticulously refined in every detail

### Aesthetics Focus Areas

- **Typography**: Choose fonts that are beautiful, unique, and interesting. Avoid generic fonts like Arial and Inter; opt for distinctive choices that elevate the aesthetics. Pair a distinctive display font with a refined body font.
- **Color & Theme**: Commit to a cohesive aesthetic. Use CSS variables for consistency. Dominant colors with sharp accents outperform timid, evenly-distributed palettes.
- **Motion**: Use animations for effects and micro-interactions. Prioritize CSS-only solutions for HTML. Use Motion library for React when available. Focus on high-impact moments: one well-orchestrated page load with staggered reveals creates more delight than scattered micro-interactions. Use scroll-triggering and hover states that surprise.
- **Spatial Composition**: Unexpected layouts. Asymmetry. Overlap. Diagonal flow. Grid-breaking elements. Generous negative space OR controlled density.
- **Backgrounds & Visual Details**: Create atmosphere and depth rather than defaulting to solid colors. Add contextual effects and textures that match the overall aesthetic.

**IMPORTANT**: Match implementation complexity to the aesthetic vision. Maximalist designs need elaborate code with extensive animations and effects. Minimalist or refined designs need restraint, precision, and careful attention to spacing, typography, and subtle details. Elegance comes from executing the vision well.

---

## Key Patterns (3 Code Patterns)

### Pattern 1: Typography Scale System

Define a fluid type scale with CSS custom properties for consistent, responsive typography.

```css
/* Fluid Type Scale — no media queries needed */
:root {
  /* Scale ratio: 1.25 (Major Third) */
  --font-xs: clamp(0.75rem, 0.7rem + 0.25vw, 0.875rem);
  --font-sm: clamp(0.875rem, 0.8rem + 0.35vw, 1rem);
  --font-base: clamp(1rem, 0.9rem + 0.5vw, 1.125rem);
  --font-lg: clamp(1.25rem, 1.1rem + 0.75vw, 1.5rem);
  --font-xl: clamp(1.5rem, 1.2rem + 1.5vw, 2.25rem);
  --font-2xl: clamp(2rem, 1.5rem + 2.5vw, 3.5rem);
  --font-3xl: clamp(2.5rem, 1.8rem + 3.5vw, 5rem);

  /* Font stacks — ALWAYS pick distinctive fonts */
  --font-display: 'Playfair Display', 'DM Serif Display', Georgia, serif;
  --font-body: 'Satoshi', 'General Sans', system-ui, sans-serif;
  --font-mono: 'JetBrains Mono', 'Fira Code', monospace;

  /* Line heights by context */
  --leading-tight: 1.1;
  --leading-normal: 1.5;
  --leading-relaxed: 1.75;
}

.heading-hero {
  font-family: var(--font-display);
  font-size: var(--font-3xl);
  line-height: var(--leading-tight);
  letter-spacing: -0.02em;
  font-weight: 700;
}

.body-text {
  font-family: var(--font-body);
  font-size: var(--font-base);
  line-height: var(--leading-normal);
  letter-spacing: 0.01em;
}
```

### Pattern 2: Color Palette System

Build a semantic color system with CSS variables that supports theming.

```css
/* Semantic Color Tokens */
:root {
  /* Surfaces */
  --color-surface: #fafaf9;
  --color-surface-elevated: #ffffff;
  --color-surface-sunken: #f0efeb;

  /* Text */
  --color-text: #1a1a2e;
  --color-text-secondary: #64748b;
  --color-text-muted: #94a3b8;

  /* Brand */
  --color-primary: #e94560;
  --color-primary-hover: #d13a54;
  --color-accent: #0f3460;

  /* Feedback */
  --color-success: #22c55e;
  --color-warning: #f59e0b;
  --color-error: #ef4444;

  /* Shadows — layered for depth */
  --shadow-sm: 0 1px 2px rgba(0,0,0,0.05);
  --shadow-md: 0 4px 6px rgba(0,0,0,0.04), 0 2px 4px rgba(0,0,0,0.06);
  --shadow-lg: 0 10px 25px rgba(0,0,0,0.06), 0 4px 10px rgba(0,0,0,0.04);
  --shadow-xl: 0 20px 50px rgba(0,0,0,0.08), 0 8px 20px rgba(0,0,0,0.04);
}

/* Dark mode — single toggle */
[data-theme="dark"] {
  --color-surface: #0f0f1a;
  --color-surface-elevated: #1a1a2e;
  --color-surface-sunken: #0a0a14;
  --color-text: #e2e8f0;
  --color-text-secondary: #94a3b8;
}

/* Tailwind integration */
/* tailwind.config.ts */
/*
theme: {
  extend: {
    colors: {
      surface: 'var(--color-surface)',
      primary: 'var(--color-primary)',
      accent: 'var(--color-accent)',
    }
  }
}
*/
```

### Pattern 3: Motion + Transition System

Apply motion with `prefers-reduced-motion` respect. Stagger reveals for page-load delight.

```css
/* Motion tokens */
:root {
  --ease-out: cubic-bezier(0.16, 1, 0.3, 1);
  --ease-bounce: cubic-bezier(0.34, 1.56, 0.64, 1);
  --duration-fast: 150ms;
  --duration-normal: 300ms;
  --duration-slow: 500ms;
}

/* Base card transition */
.card {
  transition: transform var(--duration-normal) var(--ease-out),
              box-shadow var(--duration-normal) var(--ease-out);
}
.card:hover {
  transform: translateY(-4px);
  box-shadow: var(--shadow-xl);
}

/* Staggered reveal on page load */
.reveal {
  opacity: 0;
  transform: translateY(20px);
  animation: reveal-up var(--duration-slow) var(--ease-out) forwards;
}
.reveal:nth-child(1) { animation-delay: 0ms; }
.reveal:nth-child(2) { animation-delay: 100ms; }
.reveal:nth-child(3) { animation-delay: 200ms; }
.reveal:nth-child(4) { animation-delay: 300ms; }

@keyframes reveal-up {
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

/* Respect user preference */
@media (prefers-reduced-motion: reduce) {
  *, *::before, *::after {
    animation-duration: 0.01ms !important;
    animation-iteration-count: 1 !important;
    transition-duration: 0.01ms !important;
  }
  .reveal {
    opacity: 1;
    transform: none;
  }
}
```

**React (Motion library):**

```tsx
import { motion } from 'motion/react'

function CardGrid({ items }: { items: Item[] }) {
  return (
    <div className="grid grid-cols-3 gap-6">
      {items.map((item, i) => (
        <motion.div
          key={item.id}
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ delay: i * 0.1, ease: [0.16, 1, 0.3, 1] }}
          whileHover={{ y: -4, boxShadow: '0 20px 50px rgba(0,0,0,0.08)' }}
        >
          <Card {...item} />
        </motion.div>
      ))}
    </div>
  )
}
```

---

## Anti-Patterns

### 1. Generic AI Aesthetics

Default to gradient cards + rounded corners + purple-blue themes. Every output looks identical.

- **BAD:** `bg-gradient-to-r from-purple-500 to-blue-500 rounded-xl shadow-lg` on every card
- **GOOD:** Project-specific colors + intentional typography + distinctive layout choices

### 2. Common Choice Convergence

Every component uses the same `rounded-xl shadow-lg p-6` pattern. No visual hierarchy.

- **BAD:** All cards identical: same border radius, same shadow, same padding
- **GOOD:** Differentiate by content type — hero uses full-bleed, sidebar uses compact, feature uses oversized

### 3. prefers-reduced-motion Ignored

Forcing animations on all users, including those with vestibular disorders.

- **BAD:** Complex animations always active, no motion query check
- **GOOD:** `@media (prefers-reduced-motion: reduce)` disables non-essential motion

### 4. Excessive Decoration

Unnecessary gradients, shadows, borders stacked together. Visual noise over content.

- **BAD:** Gradient background + drop shadow + border + glow effect + inner shadow on one element
- **GOOD:** Content-first design. Use whitespace for structure. One accent effect, not five.

### 5. Generic Font Stack

Using Inter, Roboto, Arial, or system-ui as the only font. No character.

- **BAD:** `font-family: Inter, system-ui, sans-serif` everywhere
- **GOOD:** Distinctive display font + refined body font. `'Playfair Display'` for headings, `'Satoshi'` for body.

### 6. Fixed Spacing / No Fluid Design

Hardcoded pixel values that don't adapt to viewport or content density.

- **BAD:** `padding: 24px; gap: 16px; margin: 48px;` (static everywhere)
- **GOOD:** `padding: clamp(1rem, 3vw, 3rem); gap: clamp(0.75rem, 2vw, 2rem);` (fluid)

---

## Design Quality Checklist

- [ ] Distinctive font pairing chosen (not Inter/Arial/Roboto)
- [ ] Color palette defined with CSS variables (not hardcoded per element)
- [ ] Visual hierarchy clear: hero > section headings > body > captions
- [ ] Motion intentional: page load reveal, hover feedback, state transitions
- [ ] `prefers-reduced-motion` respected
- [ ] Dark mode considered (CSS variable-based toggle)
- [ ] Responsive: fluid typography + grid, no fixed px widths
- [ ] No generic "AI look" — design is context-specific and memorable
