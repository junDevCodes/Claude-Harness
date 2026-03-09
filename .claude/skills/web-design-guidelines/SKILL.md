---
name: web-design-guidelines
description: Review UI code for Web Interface Guidelines compliance. Use when asked to "review my UI", "check accessibility", "audit design", "review UX", or "check my site against best practices".
metadata:
  author: vercel
  version: "1.0.0"
  argument-hint: <file-or-pattern>
triggers:
  - accessibility
  - a11y
  - UX review
  - UI review
  - design audit
  - WCAG
  - aria
  - web interface guidelines
  - semantic HTML
  - responsive design
---

# Web Interface Guidelines

Review files for compliance with Web Interface Guidelines. Provides inline fallback patterns for offline use and fetches the latest rules from the source URL when available.

## Purpose

Ensure web interfaces meet accessibility, usability, and design quality standards. Covers semantic HTML, ARIA, responsive layout, color contrast, and keyboard navigation.

## When to Apply

- User asks to "review my UI", "check accessibility", "audit design"
- Building new pages or components that need accessibility compliance
- Checking WCAG AA conformance
- Reviewing responsive layout and interaction patterns

---

## Key Patterns (3 Inline Fallback Patterns)

### Pattern 1: Semantic HTML + ARIA

Use semantic elements instead of generic `<div>` for accessibility and SEO. Add ARIA attributes only when native semantics are insufficient.

**BAD — div-soup with click handler:**

```html
<div class="nav">
  <div class="nav-item" onclick="navigate('/home')">Home</div>
  <div class="nav-item" onclick="navigate('/about')">About</div>
</div>
<div class="content">
  <div class="title">Page Title</div>
  <div class="text">Content here...</div>
</div>
```

**GOOD — semantic structure with proper elements:**

```html
<nav aria-label="Main navigation">
  <ul role="list">
    <li><a href="/home">Home</a></li>
    <li><a href="/about">About</a></li>
  </ul>
</nav>
<main>
  <article>
    <h1>Page Title</h1>
    <p>Content here...</p>
  </article>
</main>
```

**Page Structure Template:**

```html
<body>
  <header>
    <nav aria-label="Primary">...</nav>
  </header>
  <main id="main-content">
    <article>
      <h1>...</h1>
      <section aria-labelledby="section-heading">
        <h2 id="section-heading">...</h2>
      </section>
    </article>
    <aside aria-label="Related content">...</aside>
  </main>
  <footer>...</footer>
</body>
```

### Pattern 2: Responsive Layout

Use CSS Grid + `clamp()` for fluid layouts that adapt without excessive media queries.

**BAD — fixed pixel widths:**

```css
.container {
  width: 1200px;
  margin: 0 auto;
}
.card {
  width: 380px;
  height: 250px;
  font-size: 16px;
}
@media (max-width: 768px) {
  .container { width: 100%; }
  .card { width: 100%; }
}
@media (max-width: 480px) {
  .card { height: 200px; font-size: 14px; }
}
```

**GOOD — fluid layout with CSS Grid + clamp():**

```css
.container {
  width: min(90%, 1200px);
  margin-inline: auto;
  padding-inline: clamp(1rem, 3vw, 3rem);
}
.card-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(min(100%, 320px), 1fr));
  gap: clamp(1rem, 2vw, 2rem);
}
.card {
  font-size: clamp(0.875rem, 0.8rem + 0.4vw, 1.125rem);
}
```

**Responsive Typography Scale:**

```css
:root {
  --text-xs: clamp(0.75rem, 0.7rem + 0.25vw, 0.875rem);
  --text-sm: clamp(0.875rem, 0.8rem + 0.35vw, 1rem);
  --text-base: clamp(1rem, 0.9rem + 0.5vw, 1.125rem);
  --text-lg: clamp(1.25rem, 1.1rem + 0.75vw, 1.5rem);
  --text-xl: clamp(1.5rem, 1.2rem + 1.5vw, 2.25rem);
  --text-2xl: clamp(2rem, 1.5rem + 2.5vw, 3rem);
}
```

### Pattern 3: Color Contrast + Focus Indicators

Ensure WCAG AA compliance: 4.5:1 contrast ratio for normal text, 3:1 for large text. Never remove focus indicators.

**BAD — invisible focus, low contrast:**

```css
*:focus {
  outline: none; /* Removes all focus indicators */
}
.subtle-text {
  color: #ccc; /* ~1.5:1 contrast on white — fails AA */
  background: #fff;
}
a {
  color: inherit;
  text-decoration: none; /* Links indistinguishable from text */
}
```

**GOOD — visible focus, proper contrast:**

```css
:focus-visible {
  outline: 2px solid var(--color-focus, #2563eb);
  outline-offset: 2px;
}
/* Skip-link for keyboard navigation */
.skip-link {
  position: absolute;
  top: -100%;
  left: 1rem;
  z-index: 100;
  padding: 0.5rem 1rem;
  background: var(--color-bg);
  color: var(--color-text);
}
.skip-link:focus {
  top: 1rem;
}

.body-text {
  color: #374151; /* ~7.5:1 contrast on white — passes AAA */
  background: #fff;
}
.secondary-text {
  color: #6b7280; /* ~4.6:1 contrast on white — passes AA */
}

a {
  color: #2563eb;
  text-decoration: underline;
  text-underline-offset: 2px;
}
a:hover {
  text-decoration-thickness: 2px;
}
```

**Contrast Quick Reference (WCAG AA):**

| Element | Min Ratio | Example |
|---------|-----------|---------|
| Normal text (< 18pt) | 4.5:1 | `#6b7280` on `#ffffff` = 4.6:1 |
| Large text (>= 18pt bold / 24pt) | 3:1 | `#9ca3af` on `#ffffff` = 3.0:1 |
| UI components & icons | 3:1 | Border, icon against background |
| Focus indicator | 3:1 | Against adjacent colors |

---

## Anti-Patterns

### 1. div Soup (Non-semantic Markup)

`<div>` and `<span>` everywhere instead of semantic elements. Screen readers cannot navigate the page structure.

- **BAD:** `<div class="button" onclick="submit()">Submit</div>`
- **GOOD:** `<button type="submit">Submit</button>`

### 2. outline: none (Focus Removal)

Removing all focus indicators breaks keyboard navigation. 2.4 million US keyboard-only users affected.

- **BAD:** `*:focus { outline: none; }` or `button { outline: 0; }`
- **GOOD:** `*:focus-visible { outline: 2px solid #2563eb; outline-offset: 2px; }`

### 3. Fixed font-size in px (Accessibility Violation)

Users who set browser font size larger cannot override `px` values. Use `rem` for text sizing.

- **BAD:** `body { font-size: 16px; }` → user settings ignored
- **GOOD:** `body { font-size: 1rem; }` → respects user preference

### 4. Color-only Information (Color Blindness)

Relying solely on color to convey meaning (error states, status, required fields). ~8% of males have color vision deficiency.

- **BAD:** `<input style="border-color: red">` (error indicator)
- **GOOD:** `<input aria-invalid="true" style="border-color: red">` + error icon + text message

### 5. Autoplay Media (Motion Sensitivity)

Autoplaying video/animation without respecting `prefers-reduced-motion`. Can trigger vestibular disorders.

- **BAD:** `<video autoplay loop>` + CSS animations always running
- **GOOD:**
  ```css
  @media (prefers-reduced-motion: reduce) {
    *, *::before, *::after {
      animation-duration: 0.01ms !important;
      transition-duration: 0.01ms !important;
    }
  }
  ```

---

## Full Guidelines (External Reference)

Fetch the latest complete guidelines before each review:

```
https://raw.githubusercontent.com/vercel-labs/web-interface-guidelines/main/command.md
```

Use WebFetch to retrieve the latest rules. The fetched content contains all rules and output format instructions.

## Review Workflow

1. Fetch fresh guidelines from the source URL above (if available)
2. Read the specified files (or prompt user for files/pattern)
3. Check against all rules — inline patterns above as fallback
4. Output findings in the terse `file:line` format

## WCAG AA Quick Checklist

- [ ] All interactive elements keyboard-accessible (Tab, Enter, Escape)
- [ ] Focus indicators visible on all focusable elements
- [ ] Color contrast >= 4.5:1 for normal text, >= 3:1 for large text
- [ ] Images have `alt` text (decorative images: `alt=""`)
- [ ] Form inputs have associated `<label>` elements
- [ ] Page has a single `<h1>`, headings in logical order
- [ ] Skip-to-content link present
- [ ] `lang` attribute on `<html>` element
- [ ] No content conveyed by color alone
- [ ] `prefers-reduced-motion` respected for animations
